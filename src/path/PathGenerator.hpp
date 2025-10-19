#pragma once

#include <string>
#include <vector>

#include "core/Node.hpp"
#include "io/Config.hpp"

namespace path {

using Config = io::type::Config;

class Generator {
   public:
    Generator(const Config& config, const std::vector<Node>& nodes);
    std::string genFilePath(const std::string& subDir = "", const std::string& ext = "csv");

   private:
    std::string baseDir;
    std::string baseName;
};

}  // namespace path
