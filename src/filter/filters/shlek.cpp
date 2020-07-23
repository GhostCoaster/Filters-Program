//
// Created by Emmet on 7/21/2020.
//

#include <random>
#include <thread>

#include "shlek.h"
#include "../util.h"
#include "filters.h"

namespace FPP {
	auto shlek() -> Filter {
		return Filter("shlek", {Parameter::TYPE_INT, Parameter::TYPE_INT, Parameter::TYPE_INT}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			auto newWidth = parameters[0].as<int>(1, width, width / 2);
			auto newHeight = parameters[1].as<int>(1, height, height / 2);
			auto radius = parameters[2].as<int>(1, 32, 5);

			auto energySheet = Image::makeSheet(width, height);
			auto energyPixels = energySheet.getPixels();

			auto numThreads = std::thread::hardware_concurrency();
			auto threads = new std::thread[numThreads];

			for (auto threadIndex = 0; threadIndex < numThreads; ++threadIndex) {
				threads[threadIndex] = std::thread([=]() {
					/* find horizontal energy */
					for (auto i = threadIndex; i < width; i += numThreads) {
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
						}
					}
				});
			}

			for (auto threadIndex = 0; threadIndex < numThreads; ++threadIndex)
				threads[threadIndex].join();

			for (auto threadIndex = 0; threadIndex < numThreads; ++threadIndex) {
				threads[threadIndex] = std::thread([=]() {
					/* find vertical energy */
					for (auto j = threadIndex; j < height; j += numThreads) {
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

							pixels1[pos] = energyPixels[pos] + energy / count;
						}
					}
				});
			}

			for (auto threadIndex = 0; threadIndex < numThreads; ++threadIndex)
				threads[threadIndex].join();

			/* copy sheet 1 back into energyMap */
			Util::copy(*image1, &energySheet);

			/* make vertical entropy map */
			for (auto j = 1; j < height; ++j) {
				for (auto i = 0; i < width; ++i) {
					pixels1[Util::pos(i, j, width)] += findLeastEntropy(pixels1, i, j, width, width).entropy;
				}
			}

			auto seam = new int[height]();

			for (auto usingWidth = width; usingWidth > width / 2; --usingWidth) {
				std::cout << "iteration " <<  (width - usingWidth) << std::endl;

				auto leastEntropy = INT_MAX;
				auto seamPosition = 0;

				/* find seam start */
				for (auto i = 0; i < usingWidth; ++i) {
					auto power = pixels1[Util::pos(i, height - 1, width)];

					if (power < leastEntropy) {
						leastEntropy = power;
						seamPosition = i;
					}
				}

				/* find all seam positions */
				for (auto j = int(height) - 1; j >= 0; --j) {
					seam[j] = seamPosition;

					if (j != 0) {
						seamPosition = findLeastEntropy(pixels1, seamPosition, j, width, usingWidth).position;
					}
				}

				/* remove the seam from the bottom up */
				for (auto j = int(height) - 1; j >= 0; --j) {
					auto seamPosition = seam[j];

					//pixels0[Util::pos(seamPosition, j, width)] = 0xff0000ff;

					for (auto i = seamPosition; i < usingWidth - 1; ++i) {
						pixels0[Util::pos(i, j, width)] = pixels0[Util::pos(i + 1, j, width)];
						pixels1[Util::pos(i, j, width)] = pixels1[Util::pos(i + 1, j, width)];
					}
				}

				/* recalculate pixels caused by seam */
				for (auto j = int(height) - 2; j > 0; --j) {
					recalculate(pixels1, energyPixels, seam[j], j, width, height, usingWidth);
				}
			}

			delete[] seam;

			//Util::swapBuffers(image0, image1);
		});
	}

	auto findLeastEntropy(u32* pixels, int i, int j, u32 width, u32 usingWidth) -> entropy {
		auto left = Util::smallBound(i - 1);
		auto right = Util::largeBound(i + 2, usingWidth);

		auto leastEntropy = INT_MAX;
		auto leastPosition = 0;

		for (auto k = left; k < right; ++k) {
			auto power = pixels[Util::pos(k, j - 1, width)];

			if (power < leastEntropy) {
				leastEntropy = power;
				leastPosition = k;
			}
		}

		return entropy {
			leastEntropy,
			leastPosition
		};
	};

	auto recalculate(u32* pixels, u32* base, int x, int y, u32 width, u32 height, u32 usingWidth) -> void {
		if (y == height - 1) return;

		auto left = Util::smallBound(x - 1);
		auto right = Util::largeBound(x + 2, usingWidth);

		for (auto i = left; i < right; ++i) {
			auto [cumulativeEntropy, position] = findLeastEntropy(pixels, i, y + 1, width, usingWidth);

			auto pos = Util::pos(i, y + 1, width);

			auto baseEntropy = base[pos];

			if(baseEntropy + cumulativeEntropy != pixels[pos]) {
				pixels[pos] = baseEntropy + cumulativeEntropy;

				recalculate(pixels, base, i, y + 1, width, height, usingWidth);
			}
		}
	}
}
