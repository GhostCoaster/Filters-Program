
#include <random>

#include "filters.h"
#include "../util.h"

namespace FPP {
	auto shredFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 8, 1, 128, "width", "width of each strip");
        parameters.emplace_back(Parameter::TYPE_INT, 16, 0, 256, "displace", "maximum offset of a strip");
        parameters.emplace_back(Parameter::TYPE_BOOL, true, "wrap", "strips wrap around the top and bottom");

		return { "shred", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

			/* get params */
            auto stripWidth = Parameter::fromValue<int>(values[0]);
            auto maxDisplace = Parameter::fromValue<int>(values[1]);
            auto wrap = Parameter::fromValue<bool>(values[2]);

			auto engine = std::default_random_engine(std::random_device()());
			auto distribution = std::uniform_int_distribution<int>(-maxDisplace, maxDisplace);

			/* starting x value for stripWidth */
			for (auto s = 0; s < width; s += stripWidth) {
				auto yOffset = distribution(engine);

				/* copy strip from sheet0 into sheet1 with y offset */
				for (auto i = s; i < s + stripWidth && i < width; ++i) {
					for (auto j = 0; j < height; ++j) {
						int yPos;

						if (wrap) {
							yPos = Util::mod(j + yOffset, height);
						} else {
							yPos = j + yOffset;
							if (yPos < 0) yPos = 0;
							if (yPos >= height) yPos = height - 1;
						}

						pixels1[Util::pos(i, j, width)] = pixels0[Util::pos(i, yPos, width)];
					}
				}
			}

			buffers.swap();
		}};
	}
}
