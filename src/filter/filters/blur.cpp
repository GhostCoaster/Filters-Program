
#include "../util.h"
#include "filters.h"

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
			for (auto i = 0; i < width; ++i) {
				auto left = Util::smallBound(i - radius);
				auto right = Util::largeBound(i + radius + 1, width);

				for (auto j = 0; j < height; ++j) {
					auto red = 0;
					auto gre = 0;
					auto blu = 0;
					auto alp = 0;

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
				auto up = Util::smallBound(j - radius);
				auto down = Util::largeBound(j + radius + 1, height);

				for (auto i = 0; i < width; ++i) {
					auto red = 0;
					auto gre = 0;
					auto blu = 0;
					auto alp = 0;

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
