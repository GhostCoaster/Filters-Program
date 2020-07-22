//
// Created by Emmet on 7/21/2020.
//

#include <random>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto noiseScaleFilter() -> Filter {
		return Filter("noiseScale", {Parameter::TYPE_INT, Parameter::TYPE_BOOL, Parameter::TYPE_FLOAT, Parameter::TYPE_FLOAT}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			const auto strength = parameters[0].as<int>(75);
			const auto monochrome = parameters[1].as<bool>(false);
			const auto minScale = parameters[2].as<float>(1.0f);
			const auto maxScale = parameters[3].as<float>(16.0f);

			auto engine = std::default_random_engine(std::random_device()());
			auto distribution = std::uniform_int_distribution<int>(-strength, strength);

			constexpr static auto MID_PIXEL = 0x7f7f7fff;

			/* make noise in the sheet */
			/* stored as an offset from 127 (treat it like signed) */
			for (auto i = 0; i < width * height; ++i) {
				pixels1[i] = monochrome
		            ? Util::addNoise(MID_PIXEL, distribution(engine))
		            : Util::addNoise(MID_PIXEL, distribution(engine), distribution(engine), distribution(engine));
			}

			for (auto i = 0; i < width; ++i) {
				for (auto j = 0; j < height; ++j) {
					auto pos = Util::pos(i, j, width);
					auto pixel = pixels0[pos];

					auto along = float(Util::luminance(pixel)) / Util::MAX_LUMINANCE;

					auto inverseScale = Util::interp(minScale, maxScale, along);
					auto scale = 1 / inverseScale;

					auto noisePixel = Util::sample::nearest(pixels1, ((i - width / 2.0f) * scale) + (width / 2.0f), ((j - height / 2.0f) * scale) + (height / 2.0f), width, height, MID_PIXEL);

					auto noiseRed = 0x7f - Util::red(noisePixel);
					auto noiseGre = 0x7f - Util::gre(noisePixel);
					auto noiseBlu = 0x7f - Util::blu(noisePixel);

					pixels0[pos] = Util::addNoise(pixel, noiseRed, noiseGre, noiseBlu);
				}
			}
		});
	}
}
