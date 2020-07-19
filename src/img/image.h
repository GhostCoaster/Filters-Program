
#ifndef FPP_PNG
#define FPP_PNG

#include "../type.h"
#include <memory>
#include <libpng16/png.h>

namespace FPP {
	class Image {
	private:
		u32 width;
		u32 height;
		
		u32* pixels;
		
	public:
		static auto fromPNG(const char*) -> std::unique_ptr<Image>;
		static auto fromJPG(const char*) -> std::unique_ptr<Image>;
		static auto makeSheet(u32, u32) -> std::unique_ptr<Image>;
		
		Image(u32, u32, u32*);
		~Image();
		
		auto getWidth() const -> u32;
		auto getHeight() const -> u32;
		
		auto getPixels() const -> u32*;

		auto write(const char*) const -> void;
	};
}

#endif
