
#include <iostream>

#include "img/image.h"
#include "commandLine.h"
#include "filter/filterList.h"
#include "help.h"

auto main(int argc, char** argv) -> int {
    FPP::FilterList::init();

	auto errMessage = std::string("");

	auto commandLine = FPP::CommandLine(argc, argv, errMessage);

	if (!errMessage.empty()) {
		std::cout << errMessage << std::endl;
		return -1;
	}

    auto mode = commandLine.getMode();
    if (mode.showInfo) {
        if (mode.showFilter == nullptr) {
            std::cout << FPP::Help::filtersList() << std::endl;
        } else {
            std::cout << FPP::Help::prettyFilter(*mode.showFilter) << std::endl;
        }
        return 0;
    }

	for (auto i = 0; i < commandLine.getNumImages(); ++i) {
		auto& image = commandLine.getImage(i);

		auto sheet = FPP::Image::makeSheet(image.getWidth(), image.getHeight());

		auto imgPtr = &image;
		auto sheetPtr = &sheet;

        auto buffers = FPP::Buffers(imgPtr, sheetPtr);

		for (auto j = 0; j < commandLine.getNumFilters(); ++j) {
            commandLine.getFilter(j)->filter(buffers, commandLine.getParams(j));
		}

		auto outputPath = commandLine.getOuput();
		outputPath += "\\";
		outputPath.replace_filename(commandLine.getImagePath(i).stem().string() + commandLine.getSuffix());
		outputPath.replace_extension(".png");

		imgPtr->write(outputPath);
	}

	return 0;
}
