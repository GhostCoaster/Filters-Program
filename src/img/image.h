
#ifndef FPP_PNG
#define FPP_PNG

#include <filesystem>
#include <memory>

#include "../type.h"

namespace FPP {
	class Image {
	private:
		u32 width;
		u32 height;
		
		u32* pixels;
		
	public:
		static auto fromPNG(std::filesystem::path&) -> Image;
		static auto fromJPG(std::filesystem::path&) -> Image;

		static auto makeSheet(u32, u32) -> Image;
		static auto makeEmpty() -> Image;

		Image(u32, u32, u32*);
		Image(Image&&);
		~Image();
		
		auto getWidth() const -> u32;
		auto getHeight() const -> u32;
		
		auto getPixels() const -> u32*;

		auto write(std::filesystem::path&) const -> void;

		auto isValid() -> bool;
	};
}

#endif
