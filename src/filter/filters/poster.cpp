
#include "../util.h"
#include "filters.h"

namespace FPP {
	auto posterFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 16, 0, 256, "levels", "how many levels of color");
        parameters.emplace_back(Parameter::TYPE_INT, 3, 0, 40, "radius", "how far away to check for merging");

        return { "poster", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto numColors = Parameter::fromValue<int>(values[0]);
            auto radius = Parameter::fromValue<int>(values[1]);

			auto counts = new u32[numColors]();
			auto reds = new u32[numColors]();
			auto gres = new u32[numColors]();
			auto blus = new u32[numColors]();

			auto colorspace = UINT32_MAX >> 8;

			/* create the base mask of color indices */
			for (auto i = 0u; i < width; ++i) {
				for (auto j = 0u; j < height; ++j) {
					auto pos = Util::pos(i, j, width);
					auto pixel = pixels0[pos];

					auto index = (pixel >> 8) / (colorspace / numColors);
					if (index == numColors) --index;

					reds[index] += Util::red(pixel);
					gres[index] += Util::gre(pixel);
					blus[index] += Util::blu(pixel);
					++counts[index];

					pixels1[pos] = index;
				}
			}

			/* get the average colors */
			auto colors = new u32[numColors]();
			for (auto i = 0; i < numColors; ++i) {
				if (counts[i] > 0)
					colors[i] = Util::pix(reds[i] / counts[i], gres[i] / counts[i], blus[i] / counts[i], 0x00);
			}

			Util::mode(buffers.back(), buffers.front(), colors, numColors, radius);

			delete[] counts;
			delete[] reds;
			delete[] gres;
			delete[] blus;
			delete[] colors;
		}};
	}
}
