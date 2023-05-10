#pragma once

#include <string>

namespace REGenerator
{

  enum class GenerateResult
  {
    kOk = 0,
    kInvalidTargetPath,
    kMissingTemplateDir,
  };

  GenerateResult Generate(const std::string& aPath);

}
