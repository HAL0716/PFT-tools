#pragma once

#include <memory>
#include <string>

#include "io/Config.hpp"
#include "GraphGenerator.hpp"

class GeneratorFactory {
   public:
    static std::unique_ptr<GraphGenerator> create(const io::type::Config& config);
};
