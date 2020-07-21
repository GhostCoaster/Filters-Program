
#ifndef FPP_PNG
#define FPP_PNG

#include "../type.h"
#include <memory>

namespace FPP {
	class Image {
	private:
		u32 width;
		u32 height;
		
		u32* pixels;
		
	public:
		static auto fromPNG(const char*) -> Image;
		static auto fromJPG(const char*) -> Image;
		static auto makeSheet(u32, u32) -> Image;
		static auto makeEmpty() -> Image;

		Image(u32, u32, u32*);
		Image(Image&&);
		~Image();
		
		auto getWidth() const -> u32;
		auto getHeight() const -> u32;
		
		auto getPixels() const -> u32*;

		auto write(const char*) const -> void;

		auto isValid() -> bool;
	};
}

#endif
