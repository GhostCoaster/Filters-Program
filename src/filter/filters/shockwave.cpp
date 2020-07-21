//
// Created by Gnarwhal on 7/21/2020.
//

#include <cmath>
#include <algorithm>
#include "../util.h"
#include "shockwave.h"

namespace FPP {
	auto shockwaveFilter() -> Filter {
		return Filter("shockwave", { Parameter::TYPE_FLOAT, Parameter::TYPE_FLOAT }, [](Image** srcImage, Image** targetImage, Parameter* parameters) {
			const auto width  = (*srcImage)->getWidth();
			const auto height = (*srcImage)->getHeight();
			const auto src    = (*srcImage)->getPixels();
			const auto target = (*targetImage)->getPixels();

			/* get parameters */
			auto rippleCount = parameters[0].as<float>(7);
			auto magnitude   = parameters[1].as<float>(std::max(width, height) / 35.0);

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

			Util::swapBuffers(srcImage, targetImage);
		});
	}
}