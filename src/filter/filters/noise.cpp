//
// Created by Emmet on 7/21/2020.
//

#include <random>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto noiseFilter() -> Filter {
		return Filter("noise", {Parameter::TYPE_INT, Parameter::TYPE_BOOL}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			const auto strength = parameters[0].as<int>(75);
			const auto monochrome = parameters[1].as<bool>(true);

			auto engine = std::default_random_engine(std::random_device()());
			auto distribution = std::uniform_int_distribution<int>(-strength, strength);

			for (auto i = 0; i < width * height; ++i) {
				pixels0[i] = monochrome
					? Util::addNoise(pixels0[i], distribution(engine))
                    : Util::addNoise(pixels0[i], distribution(engine), distribution(engine), distribution(engine));
			}
		});
	}
}
