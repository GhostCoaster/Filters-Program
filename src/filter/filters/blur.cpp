
#include "blur.h"
#include "../util.h"

namespace FPP {
	auto blurFilter() -> Filter {
		return Filter("blur", {Parameter::TYPE_INT}, [](Image** image0, Image** image1, Parameter* parameters) {
			const auto width = (*image0)->getWidth();
			const auto height = (*image0)->getHeight();
			auto* pixels0 = (*image0)->getPixels();
			auto* pixels1 = (*image1)->getPixels();

			auto radius = parameters[0].as<int>(0, 1000, 10);

			/* do horizontal blur
			 * result is put into sheet1
			 */
			for (auto j = 0; j < height; ++j) {
				for (auto i = 0; i < width; ++i) {
					auto red = 0;
					auto gre = 0;
					auto blu = 0;
					auto alp = 0;

					auto left = i - radius;
					if (left < 0) left = 0;

					auto right = i + radius + 1;
					if (right > width) right = width;

					for (auto k = left; k < right; ++k) {
						auto pixel = pixels0[Util::pos(k, j, width)];

						red += Util::red(pixel);
						gre += Util::gre(pixel);
						blu += Util::blu(pixel);
						alp += Util::alp(pixel);
					}

					const auto size = right - left;

					pixels1[Util::pos(i, j, width)] = Util::pix(red / size, gre / size, blu / size, alp / size);
				}
			}

			/* do vertical blur
			 * result is put back into sheet0
			 */
			for (auto j = 0; j < height; ++j) {
				for (auto i = 0; i < width; ++i) {
					auto red = 0;
					auto gre = 0;
					auto blu = 0;
					auto alp = 0;

					auto up = j - radius;
					if (up < 0) up = 0;

					auto down = j + radius + 1;
					if (down > height) down = height;

					for (auto k = up; k < down; ++k) {
						auto pixel = pixels1[Util::pos(i, k, width)];

						red += Util::red(pixel);
						gre += Util::gre(pixel);
						blu += Util::blu(pixel);
						alp += Util::alp(pixel);
					}

					auto size = down - up;

					pixels0[Util::pos(i, j, width)] = Util::pix(red / size, gre / size, blu / size, alp / size);
				}
			}
		});
	}
}