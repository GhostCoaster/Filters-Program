//
// Created by Emmet on 12/19/2022.
//

#include <random>

#include "filters.h"
#include "../util.h"

namespace FPP {
    auto splatterFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 8, 1, 64, "radius", "splatter radius");

        return { "splatter", std::move(parameters), [](Buffers &buffers, std::vector <Parameter::Value> &values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto radius = Parameter::fromValue<int>(values[0]);

            auto engine = std::default_random_engine(std::random_device()());
            auto splatterRange = std::uniform_int_distribution<int>(-radius, radius);

            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto sampleX = Util::bounds(x + splatterRange(engine), width);
                    auto sampleY = Util::bounds(y + splatterRange(engine), height);

                    pixels1[Util::pos(x, y, width)] = pixels0[Util::pos(sampleX, sampleY, width)];
                }
            }

            buffers.swap();
        }};
    }
}