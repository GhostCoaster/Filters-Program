//
// Created by Emmet on 7/21/2020.
//

#include <random>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto noiseScaleFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 75, "strength", "how much deviation in color");
        parameters.emplace_back(Parameter::TYPE_BOOL, false, "monochrome", "prevent noise from changing color of pixel");
        parameters.emplace_back(Parameter::TYPE_FLOAT, 1.0f, "minScale", "minimum block size at low brightness");
        parameters.emplace_back(Parameter::TYPE_FLOAT, 16.0f, "maxScale", "maxmimum block size at high brightness");

        return { "noiseScale", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto strength = Parameter::fromValue<int>(values[0]);
            auto monochrome = Parameter::fromValue<bool>(values[1]);
            auto minScale = Parameter::fromValue<float>(values[2]);
            auto maxScale = Parameter::fromValue<float>(values[3]);

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
		}};
	}
}
