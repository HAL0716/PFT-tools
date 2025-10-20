#include "GeneratorFactory.hpp"

#include <functional>
#include <map>

#include "io/utils.hpp"
#include "Beal.hpp"
#include "DeBruijn.hpp"

std::unique_ptr<GraphGenerator> GeneratorFactory::create(const io::type::Config& config) {
    static const std::map<std::string,
                          std::function<std::unique_ptr<GraphGenerator>(const io::type::Config&)>>
        generatorMap = {{"Beal",
                         [](const io::type::Config& config) {
                             return std::make_unique<Beal>(config.generation.alphabet,
                                                           config.generation.period);
                         }},
                        {"DeBruijn", [](const io::type::Config& config) {
                             return std::make_unique<DeBruijn>(config.generation.alphabet,
                                                               config.generation.period,
                                                               config.generation.forbidden.length);
                         }}};

    auto it = generatorMap.find(config.generation.algorithm);
    if (it == generatorMap.end()) {
        io::utils::printErrorAndExit("Unknown algorithm: " + config.generation.algorithm);
    }

    return it->second(config);
}
