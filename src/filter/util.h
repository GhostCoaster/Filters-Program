
#ifndef FPP_UTIL
#define FPP_UTIL

#include <cstdint>
#include <exception>
#include "../type.h"
#include "../img/image.h"

namespace FPP {
	class Util {
	public:
		struct channelReturn {
			u8 red;
			u8 gre;
			u8 blu;
			u8 alp;
		};
		
		static auto red(u32) -> u8;
		static auto gre(u32) -> u8;
		static auto blu(u32) -> u8;
		static auto alp(u32) -> u8;
		
		static auto pix(u8, u8, u8, u8) -> u32;
		static auto pix(u32) -> channelReturn;
		
		static auto pos(u32, u32, u32) -> u32;

		static auto difference(u32, u32) -> u32;

		static auto swapBuffers(Image**, Image**) -> void;

		static auto mod(int, unsigned) -> int;
	};
}

#endif
