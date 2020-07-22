//
// Created by Emmet on 7/21/2020.
//

#include <random>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto badShlek() -> Filter {
		return Filter("badShlek", {Parameter::TYPE_INT, Parameter::TYPE_INT}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			auto newWidth = parameters[0].as<int>(1, width, width / 2);
			auto newHeight = parameters[1].as<int>(1, height, height / 2);

			struct power {
				u32 index;
				u64 power;
			};

			auto radius = 10;

			auto energySheet = Image::makeSheet(width, height);
			auto energyPixels = energySheet.getPixels();

			/* find horizontal energy */
			for (auto i = 0; i < width; ++i) {
				auto left = Util::smallBound(i - radius);
				auto right = Util::largeBound(i + radius + 1, width);

				for (auto j = 0; j < height; ++j) {
					auto pos = Util::pos(i, j, width);
					auto pixel = pixels0[pos];

					auto energy = 0llu;
					auto count = 0;

					for (auto k = left; k < right; ++k) {
						auto dist = (radius + 1) - abs(k - i);

						auto current = pixels0[Util::pos(k, j, width)];
						energy += Util::difference(current, pixel) * dist;
						count += dist;
					}

					energyPixels[pos] = energy / count;
					//auto energyPixel = u8((energy / count) / 3);
					//pixels1[pos] = Util::pix(energyPixel, energyPixel, energyPixel, 0xff);
				}
			}

			///* make sheet1 the energy map
			for (auto j = 0; j < height; ++j) {
				auto up = Util::smallBound(j - radius);
				auto down = Util::largeBound(j + radius + 1, height);

				for (auto i = 0; i < width; ++i) {
					auto pos = Util::pos(i, j, width);
					auto pixel = pixels0[pos];

					auto energy = 0llu;
					auto count = 0;

					for (auto l = up; l < down; ++l) {
						auto dist = (radius + 1) - abs(l - j);

						auto current = pixels0[Util::pos(i, l, width)];
						energy += Util::difference(current, pixel) * dist;
						count += dist;
					}

					//auto energyPixel = u8(((energy / count) + energyPixels[pos]) / 6);
					//pixels1[pos] = Util::pix(energyPixel, energyPixel, energyPixel, 0xff);

					pixels1[pos] = energy / count;
				}
			}

			auto* verticals = new power[width]();
			auto* horizontals = new power[height]();

			// make vertical strips
			for (auto i = 0; i < width; ++i) {
				auto count = 0llu;

				for (auto j = 1; j < height; ++j) {
					auto last = pixels1[Util::pos(i, j - 1, width)];
					auto now = pixels1[Util::pos(i, j, width)];

					count += Util::difference(last, now);
				}

				verticals[i].index = i;
				verticals[i].power = count;
			}

			///* make horizontal strips
			for (auto j = 0; j < height; ++j) {
				auto count = 0llu;

				for (auto i = 1; i < width; ++i) {
					auto last = pixels1[Util::pos(i - 1, j, width)];
					auto now = pixels1[Util::pos(i, j, width)];

					count += Util::difference(last, now);
				}

				horizontals[j].index = j;
				horizontals[j].power = count;
			}

			///* resize the target image
			(*image1)->resize(newWidth, newHeight);
			pixels1 = (*image1)->getPixels();

			std::sort(verticals, verticals + width, [](power& left, power& right) {
				return left.power > right.power;
			});

			std::sort(horizontals, horizontals + height, [](power& left, power& right) {
				return left.power > right.power;
			});

			auto* verticalAllowed = new bool[width]();
			auto* horizontalAllowed = new bool[height]();

			for (auto i = 0; i < newWidth; ++i) {
				verticalAllowed[verticals[i].index] = true;
			}

			for (auto j = 0; j < newHeight; ++j) {
				horizontalAllowed[horizontals[j].index] = true;
			}

			auto xIndex = 0;

			for (auto i = 0; i < newWidth; ++i, ++xIndex) {
				while (!verticalAllowed[xIndex]) {
					++xIndex;
				}

				auto yIndex = 0;

				for (auto j = 0; j < newHeight; ++j, ++yIndex) {
					while (!horizontalAllowed[yIndex]) {
						++yIndex;
					}

					pixels1[Util::pos(i, j, newWidth)] = pixels0[Util::pos(xIndex, yIndex, width)];
				}
			}

			delete[] verticalAllowed;
			delete[] horizontalAllowed;

			delete[] verticals;
			delete[] horizontals;

			Util::matchSize(image1, image0);
			Util::swapBuffers(image0, image1);
		});
	}
}
