
#include "../util.h"
#include "filters.h"

namespace FPP {
	auto posterFilter() -> Filter {
		return Filter("poster", {Parameter::TYPE_INT, Parameter::TYPE_INT}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			auto numColors = parameters[0].as<int>(0, 256, 16);
			auto radius = parameters[1].as<int>(0, 40, 3);

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

			Util::mode(image1, image0, colors, numColors, radius);

			delete[] counts;
			delete[] reds;
			delete[] gres;
			delete[] blus;
			delete[] colors;
		});
	}
}
