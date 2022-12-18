
#include <random>

#include "filters.h"
#include "../util.h"

namespace FPP {
	auto mergeFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 10, 0, 256, "radius", "radius that pixels can merge from");
        parameters.emplace_back(Parameter::TYPE_INT, 80, 0, 768, "threshold", "difference between pixels required to merge");

		return Filter("merge", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

			auto radius = Parameter::fromValue<int>(values[0]);
			auto threshold = Parameter::fromValue<int>(values[1]);

			for (auto i = 0; i < width; ++i) {
				for (auto j = 0; j < height; ++j) {
					auto originPos = Util::pos(i, j, width);
					auto original = pixels0[originPos];

					auto left = i - radius;
					if (left < 0) left = 0;

					auto right = i + radius + 1;
					if (right >= width) right = width;

					auto red = 0llu;
					auto gre = 0llu;
					auto blu = 0llu;
					auto alp = 0llu;
					auto count = 0llu;

					for (auto k = left; k < right; ++k) {
						auto pixel = pixels0[Util::pos(k, j, width)];
						auto dif = Util::difference(pixel, original);

						if(dif <= threshold) {
							red += Util::red(pixel);
							gre += Util::gre(pixel);
							blu += Util::blu(pixel);
							alp += Util::alp(pixel);
							++count;
						}
					}

					pixels1[Util::pos(i, j, width)] = Util::pix(red / count, gre / count, blu / count, alp / count);
				}
			}

			for (auto i = 0; i < width; ++i) {
				for (auto j = 0; j < height; ++j) {
					auto originPos = Util::pos(i, j, width);
					auto original = pixels0[originPos];

					auto up = j - radius;
					if (up < 0) up = 0;

					auto down = j + radius + 1;
					if (down >= height) down = height;

					auto red = 0llu;
					auto gre = 0llu;
					auto blu = 0llu;
					auto alp = 0llu;
					auto count = 0llu;

					for (auto l = up; l < down; ++l) {
						auto pixel = pixels1[Util::pos(i, l, width)];
						auto dif = Util::difference(pixel, original);

						if (dif <= threshold) {
							red += Util::red(pixel);
							gre += Util::gre(pixel);
							blu += Util::blu(pixel);
							alp += Util::alp(pixel);
							++count;
						}
					}

					pixels0[Util::pos(i, j, width)] = Util::pix(red / count, gre / count, blu / count, alp / count);
				}
			}
		});
	}
}
