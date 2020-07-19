
#include <iostream>
#include <filesystem>

#include "img/image.h"
#include "filter/blur.h"
#include "filter/channelRotate.h"
#include "filter/flat.h"
#include "filter/merge.h"
#include "filter/poster.h"
#include "img/formatter.h"

auto main(int argc, char** argv) -> int {
	try {
		auto path = std::filesystem::path(R"(C:\Users\Emmet\PQ\man-up.png)");
		auto extension = path.extension().string();

		/* to lowercase */
		for (auto it = extension.begin(); it < extension.end(); ++it) {
			*it |= 0x20;
		}
		
		auto img = std::unique_ptr<FPP::Image>();
		
		if (extension == ".png") {
			img = FPP::Image::fromPNG(path.string().c_str());
		} else if (extension == ".jpg") {
			img = FPP::Image::fromJPG(path.string().c_str());
		} else {
			throw std::exception(FPP::Formatter::format("extension ", extension.c_str(), " is not supported"));
		}
		
		auto sheet = FPP::Image::makeSheet(img->getWidth(), img->getHeight());

		auto [sheet0, sheet1] = FPP::Merge::filter(img.get(), sheet.get(), 8, 100);
		
		sheet0->write(R"(C:\Users\Emmet\PP\PP4\backgroundCORR.png)");
		
	} catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
}
