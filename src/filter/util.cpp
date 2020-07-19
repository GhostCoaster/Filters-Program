
#include "util.h"

#include <cmath>

namespace FPP {
	auto Util::red(u32 pixel) -> u8 {
		return u8((pixel >> 24) & 0xff);
	}

	auto Util::gre(u32 pixel) -> u8 {
		return u8((pixel >> 16) & 0xff);
	}

	auto Util::blu(u32 pixel) -> u8 {
		return u8((pixel >> 8) & 0xff);
	}

	auto Util::alp(u32 pixel) -> u8 {
		return u8((pixel) & 0xff);
	}
	
	auto Util::pix(const u8 red, const u8 gre, const u8 blu, const u8 alp) -> u32 {
		return u32((red << 24) | (gre << 16) | (blu << 8) | alp);
	}

	auto Util::pix(const u32 pixel) -> channelReturn {
		return channelReturn{
			red(pixel),
			gre(pixel),
			blu(pixel),
			alp(pixel)
		};
	}

	auto Util::pos(u32 x, u32 y, u32 width) -> u32 {
		return u32(y * width + x);
	}

	auto Util::difference(u32 pixel0, u32 pixel1) -> u32 {
		auto [red0, gre0, blu0, alp0] = Util::pix(pixel0);
		auto [red1, gre1, blu1, alp1] = Util::pix(pixel1);

		return u32(abs(red1 - red0) + abs(gre1 - gre0) + abs(blu1 - blu0) + abs(alp1 - alp0));
	}
}
