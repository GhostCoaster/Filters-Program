//
// Created by Emmet on 7/21/2020.
//

#include <random>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto noiseFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 75, "strength", "how much deviation in color");
        parameters.emplace_back(Parameter::TYPE_BOOL, false, "monochrome", "prevent noise from changing color of pixel");

        return { "noise", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();

			auto strength = Parameter::fromValue<int>(values[0]);
			auto monochrome =Parameter::fromValue<bool>(values[1]);

			auto engine = std::default_random_engine(std::random_device()());
			auto distribution = std::uniform_int_distribution<int>(-strength, strength);

			for (auto i = 0; i < width * height; ++i) {
				pixels0[i] = monochrome
					? Util::addNoise(pixels0[i], distribution(engine))
                    : Util::addNoise(pixels0[i], distribution(engine), distribution(engine), distribution(engine));
			}
		}};
	}
}
