
#include "util.h"

#include <cmath>

namespace FPP::Util {
	auto red(u32 pixel) -> u8 {
		return u8((pixel >> 24) & 0xff);
	}

	auto gre(u32 pixel) -> u8 {
		return u8((pixel >> 16) & 0xff);
	}

	auto blu(u32 pixel) -> u8 {
		return u8((pixel >> 8) & 0xff);
	}

	auto alp(u32 pixel) -> u8 {
		return u8((pixel) & 0xff);
	}

	auto pix(const u8 red, const u8 gre, const u8 blu, const u8 alp) -> u32 {
		return u32((red << 24) | (gre << 16) | (blu << 8) | alp);
	}

	auto pix(const u32 pixel) -> channelReturn {
		return channelReturn{
			red(pixel),
			gre(pixel),
			blu(pixel),
			alp(pixel)
		};
	}

	auto pos(u32 x, u32 y, u32 width) -> u32 {
		return u32(y * width + x);
	}

	auto difference(u32 pixel0, u32 pixel1) -> u32 {
		auto [red0, gre0, blu0, alp0] = pix(pixel0);
		auto [red1, gre1, blu1, alp1] = pix(pixel1);

		return u32(abs(red1 - red0) + abs(gre1 - gre0) + abs(blu1 - blu0) + abs(alp1 - alp0));
	}

	auto swapBuffers(Image** image0, Image** image1) -> void {
		auto temp = *image0;
		*image0 = *image1;
		*image1 = temp;
	}

	auto mod(int a, unsigned b) -> int {
		int mod = a % (int)b;

		if (a < 0) mod += b;

		return mod;
	}

	auto mix(u32 color0, u32 color1, float distribution) -> u32 {
		return pix(
			u8(red(color0) * distribution + red(color1) * (1.0 - distribution)),
			u8(gre(color0) * distribution + gre(color1) * (1.0 - distribution)),
			u8(blu(color0) * distribution + blu(color1) * (1.0 - distribution)),
			u8(alp(color0) * distribution + alp(color1) * (1.0 - distribution))
		);
	}

	namespace sample {
		auto at(u32 * src, u32 x, u32 y, u32 width, u32 height, u32 fallbackColor) -> u32 {
			if (x >= width || y >= height) {
				return fallbackColor;
			} else {
				return src[Util::pos(x, y, width)];
			}
		}

		auto nearest(u32 * src, float x, float y, u32 width, u32 height, u32 fallbackColor) -> u32 {
			return at(src, u32(round(x)), u32(round(y)), width, height, fallbackColor);
		}

		auto bilinear(u32 * src, float x, float y, u32 width, u32 height, u32 fallbackColor) -> u32 {
			auto color0 = at(src, u32(floor(x)), u32(floor(y)), width, height, fallbackColor);
			auto color1 = at(src, u32(ceil(x)),  u32(floor(y)), width, height, fallbackColor);
			auto color2 = at(src, u32(floor(x)), u32(ceil(y)),  width, height, fallbackColor);
			auto color3 = at(src, u32(ceil(x)),  u32(ceil(y)),  width, height, fallbackColor);

			auto horizontalDistribution = x - floor(x);
			auto verticalDistribution   = y - floor(y);

			return mix(
				mix(color0, color1, horizontalDistribution),
				mix(color2, color3, horizontalDistribution),
				verticalDistribution
			);
		}
	}
}
