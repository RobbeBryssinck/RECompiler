# TODO:
# * IAT functions(?)
# * Don't stub functions for which there is a definition already.
# * Don't stub standard functions(?)
# * Define types before function/data. Maybe just dump idb types every time?
# * Handle complex symbols.
# * Handle seemingly duplicate (function) names (probably mangled).
# * Handle "this" arg name.
# * Can't compile noreturn.
# * Normalize functions with spaces and '::'.

import ida_hexrays
import ida_kernwin
import ida_auto
import idaapi
import ida_nalt
import ida_typeinf
import ida_funcs
import ida_ua
import idautils
import idc
import ida_name

import re
import os
from typing import List
import configparser
import tkinter as tk
from tkinter import filedialog
import pathlib

def info_ex(message: str):
    print(message)
    ida_kernwin.info(message)

def get_rva(address: int) -> int:
    return address - idaapi.get_imagebase()

def get_function_details(address: int):
    function_tinfo = idaapi.tinfo_t()
    ida_nalt.get_tinfo(function_tinfo, address)
    
    function_details = idaapi.func_type_data_t()
    function_tinfo.get_func_details(function_details)

    return function_details

def get_function_start_address(ea: int) -> int:
    for function_address in idautils.Functions():
        if ida_funcs.is_same_func(function_address, ea):
            return function_address
    
    return 0

def get_name_at_address(address: int) -> str:
    name = ida_funcs.get_func_name(address)
    
    # The flags for demangle_name are undocumented. Through bruteforce, I discovered
    # that 8 gives a shortened demangled name, without params or return types.
    demangled_name: str = idc.demangle_name(name, 8)
    if demangled_name != None:
        name = demangled_name
    
    return name

def get_instructions_from_function(function_address: int) -> List[int]:
    instructions: List[int] = []
    current_instruction: int = function_address

    function_end_address: int = idc.find_func_end(function_address)

    while current_instruction < function_end_address:
        instructions.append(current_instruction)
        current_instruction = idc.find_code(current_instruction, idc.SEARCH_DOWN)
    
    return instructions

def get_all_function_calls_in_function(function_address: int) -> List[int]:
    functions: List[int] = []

    for instruction in get_instructions_from_function(function_address):
        instruction_mnemonic: str = idc.print_insn_mnem(instruction)
        if instruction_mnemonic != "call" and instruction_mnemonic != "jmp":
            continue

        call_address: int = idc.get_operand_value(instruction, 0)
        segment = idaapi.get_visible_segm_name(idaapi.getseg(call_address))
        if "idata" in segment:
            # TODO: figure out a way of handling IAT functions (if at all).
            # This should work for now, as standard functions should be in Windows.h,
            # which is included with the sdk automatically.
            continue
        elif get_function_start_address(call_address) != call_address:
            continue

        functions.append(call_address)

    return functions

def get_all_global_data_in_function(function_address: int):
    global_data: List[int] = []

    for instruction in get_instructions_from_function(function_address):
        instruction_mnemonic: str = idc.print_insn_mnem(instruction)
        if not "mov" in instruction_mnemonic and instruction_mnemonic != "lea":
            continue

        if idc.get_operand_type(instruction, 0) == ida_ua.o_mem:
            global_data.append(idc.get_operand_value(instruction, 0))
        if idc.get_operand_type(instruction, 1) == ida_ua.o_mem:
            global_data.append(idc.get_operand_value(instruction, 1))

    return global_data

def create_signature_type(function_address: int) -> str:
    function_details = get_function_details(function_address)

    arguments: str = ""
    for i in range(function_details.size()):
        if i != 0:
            arguments = arguments + ", "
        arguments = arguments + "{}".format(ida_typeinf.print_tinfo('', 0, 0, idc.PRTYPE_1LINE, function_details[i].type, '', ''))

    return "{}({})".format(ida_typeinf.print_tinfo('', 0, 0, idc.PRTYPE_1LINE, function_details.rettype, '', ''), arguments)

def generate_function(sdk_header_file_name: str, sdk_impl_file_name: str, function_address: int, is_stub: bool, global_data: List[int]):
    delete_function_if_exists(sdk_header_file_name, sdk_impl_file_name, function_address)

    target_function = ida_funcs.get_func(function_address)
    failure = ida_hexrays.hexrays_failure_t()

    function_rva: int = get_rva(function_address)

    decompiled_output = str(ida_hexrays.decompile_func(target_function, failure, ida_hexrays.DECOMP_NO_CACHE))

    function_name = get_name_at_address(function_address)

    type_name: str = "T{}".format(function_name)
    signature_type: str = create_signature_type(function_address)
    real_function_variable = "Real_{}".format(function_name)
    metadata = "\n// Symbol defined, type: {} function, address: {}".format("stub" if is_stub else "full", hex(function_address))

    is_variadic: bool = False

    with open(sdk_header_file_name, 'a') as f:
        f.write(metadata + "\n")

        decompiled_lines = decompiled_output.split("\n")
        
        found_signature = False
        signature: str = ""
        for i in range(len(decompiled_lines)):
            signature = decompiled_lines[i]
            if signature[0:2] != "//":
                found_signature = True
                break
        
        if not found_signature:
            raise RuntimeError("No signature found in function {}".format(function_name))
        
        signature = signature.rstrip("\n") + ";\n"
        f.write(signature)

        is_variadic = ", ...)" in signature

        f.write("static IdaFunction<decltype({function_name})> {real_function_variable} {{ {function_rva}, &{function_name} }};\n".format(function_name=function_name, real_function_variable=real_function_variable, function_rva=function_rva))

    with open(sdk_impl_file_name, 'r+') as f:
        if is_stub:
            decompiled_output = decompiled_output.split("\n")
            open_bracket_index = decompiled_output.index("{")
            decompiled_output = decompiled_output[0:open_bracket_index+1]

            function_details = get_function_details(function_address)
            arguments: str = ""
            for i in range(function_details.size()):
                if i != 0:
                    arguments = arguments + ", "

                argument: str = function_details[i].name
                if argument == "":
                    argument = "a{}".format(i + 1)

                arguments = arguments + argument

            if is_variadic:
                decompiled_output.append("  return {}.GetRawFunctionPointer()({});".format(real_function_variable, arguments))
            else:
                decompiled_output.append("  return {}({});".format(real_function_variable, arguments))

            decompiled_output.append("}")

            decompiled_output = "\n".join(decompiled_output)
        else:
            for global_variable in global_data:
                variable_name = idc.get_name(global_variable)
                
                # TODO: Python re doesn't like complex strings, needs to be escaped.
                if idc.demangle_name(variable_name, 0) != None:
                    continue

                # 'shift' variable is needed for the insertions.
                shift = 0
                # Match the whole word
                regex_string: str = "\\b{}\\b".format(variable_name)
                occurences = [m.start() for m in re.finditer(regex_string, decompiled_output)]
                
                for occurence in occurences:
                    i = occurence + len(variable_name) + shift
                    decompiled_output = decompiled_output[:i] + ".Get()" + decompiled_output[i:]
                    shift = shift + len(".Get()")
        
        decompiled_output = decompiled_output.rstrip("\n")

        hook = """{metadata}
{decompiled_output}
// End of function symbol, address: {function_address_hex}

""".format(metadata=metadata, type_name=type_name, signature_type=signature_type, real_function_variable=real_function_variable, function_address_hex=hex(function_address), decompiled_output=decompiled_output)

        lines = f.readlines()
        f.seek(0)
        hook_position = lines.index("// PLACE HOOKS HERE\n") + 1
        lines.insert(hook_position, hook)
        f.writelines(lines)
        f.truncate()
    
    with open(sdk_impl_file_name, 'r+') as f:
        hook = """
  // Hook installation, address: {function_address_hex}
  {real_function_variable}.InstallHook();
""".format(function_address_hex=hex(function_address), real_function_variable=real_function_variable)
        
        lines = f.readlines()
        f.seek(0)
        hook_position = lines.index("  // INSTALL HOOKS HERE\n") + 1
        lines.insert(hook_position, hook)
        f.writelines(lines)
        f.truncate()

def generate_global_variable(sdk_header_file_name: str, variable_address: int):
    # TODO: handle complex types?
    variable_name: str = idc.get_name(variable_address)
    demangled_name: str = idc.demangle_name(variable_name, 0)
    if demangled_name != None:
        return
    
    # Use "8" as flag here for valid variable names.
    variable_name = ida_name.validate_name(variable_name, 8)
    
    delete_declaration_if_exists(sdk_header_file_name, variable_address)

    variable_rva: int = get_rva(variable_address)

    with open(sdk_header_file_name, 'a') as f:
        f.write("\n// Symbol defined, type: global variable, address: {}\n".format(hex(variable_address)))
        
        type_name: str = idc.get_type(variable_address)
        if type_name is None:
            type_name = idc.guess_type(variable_address)
            if type_name is None:
                type_name = "_UNKNOWN"

        f.write("static IdaVariable<{type_name}> {variable_name} {{ {variable_rva} }};\n".format(type_name=type_name, variable_name=variable_name, variable_rva=variable_rva))

def delete_function_if_exists(sdk_header_file_name: str, sdk_impl_file_name: str, function_address: int):
    delete_declaration_if_exists(sdk_header_file_name, function_address)
    delete_function_hook_if_exists(sdk_impl_file_name, function_address)

def delete_declaration_if_exists(sdk_header_file_name: str, address: int):
    with open(sdk_header_file_name, 'r+') as f:
        lines = f.readlines()
        f.seek(0)
        for line in lines:
            if not "// Symbol defined, type:" in line:
                continue

            defined_address = int(line.rstrip("\n").split(" ")[-1], 16)
            if defined_address != address:
                continue

            begin = lines.index(line) - 1
            end = begin+3 if "variable" in line else begin+4
            del lines[begin:end]

            break
        
        f.writelines(lines)
        f.truncate()

def delete_function_hook_if_exists(sdk_impl_file_name: str, function_address: int):
    with open(sdk_impl_file_name, 'r+') as f:
        lines = f.readlines()
        f.seek(0)

        # Delete the hook definition.
        for line in lines:
            if not "// Symbol defined, type:" in line:
                continue

            defined_address = int(line.rstrip("\n").split(" ")[-1], 16)
            if defined_address != function_address:
                continue

            begin = lines.index(line)
            end = 0
            for i in range(begin, len(lines)):
                if "// End of function symbol" in lines[i]:
                    end = i + 3 # + 3 to delete newlines.
                    break

            del lines[begin:end]

            break

        # Delete the hook installation.
        for line in lines:
            if not "// Hook installation" in line:
                continue

            defined_address = int(line.rstrip("\n").split(" ")[-1], 16)
            if defined_address != function_address:
                continue

            # - 1 and + 3 to delete blank line
            i = lines.index(line) - 1
            del lines[i:i+3]

            break
        
        f.writelines(lines)
        f.truncate()

def ask_directory(title: str) -> str:
    root = tk.Tk()
    root.withdraw()

    return filedialog.askdirectory(title=title)

def initialize_settings_file(ini_path: str):
    ida_kernwin.info("Make sure you have generated an RECompiler project with RECompilerGenerator.exe.")

    project_path: str = ask_directory("Select the project directory generated by RECompilerGenerator.")

    config = configparser.ConfigParser()
    config["global"] = {}
    config["global"]["project_path"] = project_path

    with open(ini_path, 'w') as f:
        config.write(f)

def initialize_project() -> str:
    ini_path: str = "./{}.ini".format(ida_nalt.get_root_filename())

    if not os.path.exists(ini_path):
        dialogue_result = ida_kernwin.ask_yn(ida_kernwin.ASKBTN_CANCEL, "Config file for RECompiler project not found. Would you like to generate it?")
        if dialogue_result != ida_kernwin.ASKBTN_YES:
            return None
        
        initialize_settings_file(ini_path)
    
    config = configparser.ConfigParser()
    config.read(ini_path)

    if not "global" in config.sections() or not "project_path" in config["global"]:
        ida_kernwin.warning("Project ini seems to be corrupt. It will be regenerated.")
        initialize_settings_file(ini_path)
    
    project_path: str = config["global"]["project_path"]
    while not os.path.exists(project_path):
        ida_kernwin.warning("Project path '{}' does not exist. Please try again.".format(project_path))
        initialize_settings_file(ini_path)

        config = configparser.ConfigParser()
        config.read(ini_path)
        project_path = config["global"]["project_path"]

    return project_path

if __name__ == "__main__":
    ida_auto.auto_wait()

    project_path: str = initialize_project()
    if not project_path:
        exit()
    
    project_name: str = pathlib.PurePath(project_path).name

    sdk_header_file_name = project_path + "/" + project_name + "/RESDK.h"
    if not os.path.exists(sdk_header_file_name):
        ida_kernwin.warning("SDK header file '{}' not found. Confirm the path in the ini setting file.".format(sdk_header_file_name))
        exit()
    
    sdk_impl_file_name = project_path + "/" + project_name + "/RESDK.cpp"
    if not os.path.exists(sdk_impl_file_name):
        ida_kernwin.warning("SDK implementation file '{}' not found. Confirm the path in the ini setting file.".format(sdk_impl_file_name))
        exit()

    ea: int = ida_kernwin.get_screen_ea()
    function_address: int = get_function_start_address(ea)

    delete_function_if_exists(sdk_header_file_name, sdk_impl_file_name, function_address)

    function_calls: List[int] = get_all_function_calls_in_function(function_address)
    global_data: List[int] = get_all_global_data_in_function(function_address)

    for function in function_calls:
        generate_function(sdk_header_file_name, sdk_impl_file_name, function, True, global_data)

    for global_variable in global_data:
        generate_global_variable(sdk_header_file_name, global_variable)

    generate_function(sdk_header_file_name, sdk_impl_file_name, function_address, False, global_data)

    print("RECompiler generator has finished running, data was dumped in project '{}'.".format(project_path))

