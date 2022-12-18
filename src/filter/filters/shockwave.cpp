//
// Created by Gnarwhal on 7/21/2020.
//

#include <cmath>
#include <algorithm>

#include "../util.h"
#include "filters.h"

namespace FPP {
	auto shockwaveFilter() -> Filter {
		auto parameters = std::vector<Parameter>();
		parameters.emplace_back(Parameter::TYPE_FLOAT, 7, "count", "Ripple count");
		parameters.emplace_back(Parameter::TYPE_FLOAT, 35.0f, "mag", "Ripple magnitude");

		return Filter("shockwave", std::move(parameters), [](
            Buffers & buffers,
            std::vector<Parameter::Value> & params
        ) {
			auto [width, height] = buffers.dimensions();
			auto * src = buffers.front().getPixels();
			auto * target = buffers.back().getPixels();

			auto rippleCount = Parameter::fromValue<float>(params[0]);
			auto magnitude   = Parameter::fromValue<float>(params[1]);

			struct {
				double x;
				double y;
			} center = { double(width) / 2.0, double(height) / 2.0 };
			auto perPix = (rippleCount * Util::PI<double> * 2) / std::max(center.x, center.y);

			for (auto i = 0; i < width; ++i) {
				for (auto j = 0; j < height; ++j) {
					auto x = i - center.x;
					auto y = j - center.y;
					auto distance = sqrt(x * x + y * y);
					distance += sin(distance * perPix) * magnitude;
					auto rotation = atan2(y, x);
					x = std::clamp(cos(rotation) * distance + center.x, 0.0, double(width - 1));
					y = std::clamp(sin(rotation) * distance + center.y, 0.0, double(height - 1));
					target[Util::pos(i, j, width)] = Util::sample::bilinear(src, x, y, width, height, 0x000000FF);
				}
			}

			buffers.swap();
		});
	}
}
