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

			auto conformToRange = [](int val, int low, int high) {
				if (val < low)
					val = low;
				else if (val > high)
					val = high;

				return val;
			};

			for (auto i = 0; i < width * height; ++i) {
				if (monochrome) {
					auto amount = distribution(engine);

					auto pixel = pixels0[i];

					auto red = conformToRange(Util::red(pixel) + amount, 0x00, 0xff);
					auto gre = conformToRange(Util::gre(pixel) + amount, 0x00, 0xff);
					auto blu = conformToRange(Util::blu(pixel) + amount, 0x00, 0xff);

					pixels0[i] = Util::pix(red, gre, blu, Util::alp(pixel));

				} else {
					auto pixel = pixels0[i];

					auto red = conformToRange(Util::red(pixel) + distribution(engine), 0x00, 0xff);
					auto gre = conformToRange(Util::gre(pixel) + distribution(engine), 0x00, 0xff);
					auto blu = conformToRange(Util::blu(pixel) + distribution(engine), 0x00, 0xff);

					pixels0[i] = Util::pix(red, gre, blu, Util::alp(pixel));
				}
			}
		});
	}
}