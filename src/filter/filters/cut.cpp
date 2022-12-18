//
// Created by Emmet on 7/21/2020.
//

#include <random>
#include <thread>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto cutFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_FLOAT, 0.5f, 0.0f, 1.0f, "width", "percentage of resulting width");
        parameters.emplace_back(Parameter::TYPE_FLOAT, 0.5f, 0.0f, 1.0f, "height", "percentage of resulting height");
        parameters.emplace_back(Parameter::TYPE_INT, 1, 5, 32, "radius", "radius of energy function");

		return { "cut", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
			auto [width, height] = buffers.dimensions();
			auto * pixels0 = buffers.front().getPixels();
			auto * pixels1 = buffers.back().getPixels();

			auto newWidth = (int)round((float)width * Parameter::fromValue<float>(values[0]));
			auto newHeight = (int)round((float)height * Parameter::fromValue<float>(values[1]));
			auto radius = Parameter::fromValue<int>(values[2]);

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

			struct power {
				u32 index;
				u64 power;
			};

			auto* verticals = new power[width]();
			auto* horizontals = new power[height]();
			auto* verticalAllowed = new bool[width]();
			auto* horizontalAllowed = new bool[height]();

			auto verticalThread = std::thread([=]() {
				for (auto i = 0; i < width; ++i) {
					auto count = 0llu;

					for (auto j = 0; j < height; ++j)
						count += pixels1[Util::pos(i, j, width)];

					verticals[i].index = i;
					verticals[i].power = count;
				}

				std::sort(verticals, verticals + width, [](power& left, power& right) {
					return left.power > right.power;
				});

				for (auto i = 0; i < newWidth; ++i) {
					verticalAllowed[verticals[i].index] = true;
				}
			});

			auto horizontalThread = std::thread([=]() {
				for (auto j = 0; j < height; ++j) {
					auto count = 0llu;

					for (auto i = 1; i < width; ++i)
						count += pixels1[Util::pos(i, j, width)];

					horizontals[j].index = j;
					horizontals[j].power = count;
				}

				std::sort(horizontals, horizontals + height, [](power& left, power& right) {
					return left.power > right.power;
				});

				for (auto j = 0; j < newHeight; ++j) {
					horizontalAllowed[horizontals[j].index] = true;
				}
			});

			verticalThread.join();
			horizontalThread.join();

            /* prepare target for new size */
            pixels1 = buffers.back().resize(newWidth, newHeight);

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

			buffers.matchFrontToBack();
			buffers.swap();
		}};
	}
}
