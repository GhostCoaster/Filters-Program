
#include "poster.h"

#include <iostream>

#include "util.h"

namespace FPP {
	auto Poster::filter(Image* sheet0, Image* sheet1, int numColors, int radius) -> Filter::sheetReturn {
		const auto width = sheet0->getWidth();
		const auto height = sheet0->getHeight();
		auto* pixels0 = sheet0->getPixels();
		auto* pixels1 = sheet1->getPixels();

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

		/* now place back in the colors based on a mode check */
		for (auto i = 0u; i < width; ++i) {
			for (auto j = 0u; j < height; ++j) {
				/* reuse counts for mode check */
				for (auto i = 0; i < numColors; ++i)
					counts[i] = 0;

				auto left = i - radius;
				if (left < 0) left = 0;

				auto right = i + radius + 1;
				if (right >= width) right = width;
				
				auto up = j - radius;
				if (up < 0) up = 0;

				auto down = j + radius + 1;
				if (down >= height) down = height;

				/* count how many of each color is around this pixel */
				for (auto k = left; k < right; ++k)
					for (auto l = up; l < down; ++l)
						++counts[pixels1[Util::pos(k, l, width)]];

				auto index = 0;
				auto highestCount = 0;
				
				/* find the mode of the colors around this pixel */
				for (auto k = 0; k < numColors; ++k) {
					if (counts[k] > highestCount) {
						highestCount = counts[k];
						index = k;
					}
				}

				auto pos = Util::pos(i, j, width);
				pixels0[pos] = colors[index] | Util::alp(pixels0[pos]);
			}
		}

		delete[] counts;
		delete[] reds;
		delete[] gres;
		delete[] blus;
		delete[] colors;
		
		return Filter::sheetReturn{ sheet0, sheet1 };
	}
}
