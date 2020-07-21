//
// Created by Emmet on 7/20/2020.
//

#include <random>
#include "shred.h"
#include "../util.h"

namespace FPP {
	auto shredFilter() -> Filter {
		return Filter("shred", {Parameter::TYPE_INT, Parameter::TYPE_INT, Parameter::TYPE_BOOL}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			/* get parameters */
			auto stripWidth = parameters[0].as<int>(1, width, 8);
			auto maxDisplace = parameters[1].as<int>(0, height * 2, height / 8);
			auto wrap = parameters[2].as<bool>(true);

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

			Util::swapBuffers(image0, image1);
		});
	}
}
