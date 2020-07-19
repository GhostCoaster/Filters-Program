
#include "flat.h"

#include "util.h"

namespace FPP {
	int Flat::globalCount = 0;
	
	auto floodFill(u32* const pixels0, u32* const pixels1, u32* const pixelsM, u32 const original, u32 const threshold, u32 const x, u32 const y, u32 const width, u32 const height, u32 const generation) -> void {
		auto doSide = [&](int xOff, int yOff) {
			if(x - xOff < 0 || x + xOff >= width || y - yOff < 0 || y + yOff >= height)
				return;

			const auto pos = Util::pos(x + xOff, y + yOff, width);

			if (pixels1[pos] != original) {
				const auto next = pixels0[pos];
				const auto dif = Util::difference(next, original);

				if (dif <= threshold) {
					if (generation > pixelsM[pos]) {
						pixels1[pos] = original;
						pixelsM[pos] = generation;
					}

					++Flat::globalCount;

					if (Flat::globalCount < 1000)
						floodFill(pixels0, pixels1, pixelsM, original, threshold, x + xOff, y + yOff, width, height, generation + 1);
				}
			}
		};
		
		doSide(-1, 0);
		doSide(0, -1);
		doSide(1, 0);
		doSide(0, 1);
	}
	
	auto Flat::filter(Image* sheet0, Image* sheet1, int threshold) -> Filter::sheetReturn {
		const auto width = sheet0->getWidth();
		const auto height = sheet0->getHeight();
		const auto mask = Image::makeSheet(width, height);
		auto* pixels0 = sheet0->getPixels();
		auto* pixels1 = sheet1->getPixels();
		auto* pixelsM = mask->getPixels();
		
		for (auto i = 0u; i < width * height; ++i) {
			pixels1[i] = 0;
		}

		Flat::globalCount = 0;
		
		for (auto j = 0u; j < height; ++j) {
			for (auto i = 0u; i < width; ++i) {
				const auto pos = Util::pos(i, j, width);	
				const auto original = pixels0[pos];
			
				if (pixelsM[pos] == 0) {
					pixels1[pos] = original;
				}
				
				if (original != 0)
					floodFill(pixels0, pixels1, pixelsM, original, threshold, i, j, width, height, 1);
			}
		}
		
		return Filter::sheetReturn{ sheet1, sheet0 };
	}
}
