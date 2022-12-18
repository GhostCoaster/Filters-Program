
#include <random>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto channelRotateFilter() -> Filter {
		return { "channelRotate", std::vector<Parameter>(), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels = buffers.front().getPixels();

			auto engine = std::default_random_engine(std::random_device()());
			auto distribution = std::uniform_int_distribution<int>(0, 5);

			for (auto j = 0; j < height; ++j) {
				for (auto i = 0; i < width; ++i) {
					const auto pos = Util::pos(i, j, width);

					const auto pixel = pixels[pos];

					const auto red = Util::red(pixel);
					const auto gre = Util::gre(pixel);
					const auto blu = Util::blu(pixel);
					const auto alp = Util::alp(pixel);

					switch (distribution(engine)) {
						case 1: {
                            pixels[pos] = Util::pix(gre, red, blu, alp);
						} break;
						case 2: {
                            pixels[pos] = Util::pix(gre, blu, red, alp);
						} break;
						case 3: {
                            pixels[pos] = Util::pix(blu, red, gre, alp);
						} break;
						case 4: {
                            pixels[pos] = Util::pix(blu, gre, red, alp);
						} break;
						case 5: {
                            pixels[pos] = Util::pix(red, blu, gre, alp);
						} break;
					}
				}
			}
		}};
	}
}
