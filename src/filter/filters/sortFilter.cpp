//
// Created by Emmet on 7/22/2020.
//

#include "filters.h"
#include "../util.h"

namespace FPP {
	auto sortFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 80, 0, Util::MAX_LUMINANCE, "threshold", "threshold to combine pixels");
        parameters.emplace_back(Parameter::TYPE_INT, 1, 0, 32, "radius", "how far away to check for combining");

		return { "sort", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto threshold = Parameter::fromValue<int>(values[0]);
            auto radius = Parameter::fromValue<int>(values[1]);

			/* copy pixels into pixels1 */
			for (auto i = 0; i < width * height; ++i) {
				pixels1[i] = pixels0[i];
			}

			/* sort pixels 1 */
			std::sort(pixels1, pixels1 + width * height, [](u32 leftPixel, u32 rightPixel) {
				return leftPixel < rightPixel;
			});

			for (auto j = 0; j < height; ++j) {
				auto red = 0llu;
				auto gre = 0llu;
				auto blu = 0llu;

				for (auto i = 0; i < width; ++i) {
					auto pixel = pixels1[Util::pos(i, j, width)];

					red += Util::red(pixel);
					gre += Util::gre(pixel);
					blu += Util::blu(pixel);
				}

				auto average = Util::pix(red / width, gre / width, blu / width);
				pixels1[Util::pos(0, j, width)] = average;
			}

			auto colors = std::vector<u32>();

			auto red = 0llu;
			auto gre = 0llu;
			auto blu = 0llu;
			auto count = 0;

			/* add colors to color list */
			for (auto j = 0; j < height; ++j) {
				auto thisColor = pixels1[Util::pos(0, j, width)];

				red += Util::red(thisColor);
				gre += Util::gre(thisColor);
				blu += Util::blu(thisColor);
				++count;

				if (j == height - 1) {
					colors.push_back(Util::pix(red / count, gre / count, blu / count, 0x00));

				} else {
					auto difference = Util::difference(red / count, gre / count, blu / count, pixels1[Util::pos(0, j + 1, width)]);

					if (difference > threshold) {
						colors.push_back(Util::pix(red / count, gre / count, blu / count, 0x00));

						red = 0;
						gre = 0;
						blu = 0;
						count = 0;
					}
				}
			}

			/* replace colors in the original image */
			for (auto j = 0; j < height; ++j) {
				for (auto i = 0; i < width; ++i) {
					auto pos = Util::pos(i, j, width);
					auto pixel = pixels0[pos];

					auto bestColor = 0;
					auto bestDifference = Util::MAX_LUMINANCE;

					for (auto k = 0; k < colors.size(); ++k) {
						auto difference = Util::difference(colors[k], pixel);

						if (difference < bestDifference) {
							bestDifference = difference;
							bestColor = k;
						}
					}

					pixels0[pos] = ((bestColor << 8) & 0xffffff00) | Util::alp(pixel);
				}
			}

			Util::mode(buffers.front(), buffers.back(), colors.data(), colors.size(), radius);

			buffers.swap();
		}};
	}
}
