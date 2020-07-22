
#include <iostream>
#include <filesystem>

#include "img/image.h"
#include "img/formatter.h"
#include "commandLine.h"

auto main(int argc, char** argv) -> int {
	auto errMessage = std::string("");

	auto commandLine = FPP::CommandLine(argc, argv, errMessage);

	if (errMessage != "") {
		std::cout << errMessage << std::endl;
		return -1;
	}

	for (auto i = 0; i < commandLine.getNumImages(); ++i) {
		auto& image = commandLine.getImage(i);

		auto sheet = FPP::Image::makeSheet(image.getWidth(), image.getHeight());

		auto imgPtr = &image;
		auto sheetPtr = &sheet;

		for (auto j = 0; j < commandLine.getNumFilters(); ++j) {
			auto* filter = commandLine.getFilter(j);
			auto& parameters = commandLine.getParameters(j);

			filter->filter(&imgPtr, &sheetPtr, parameters.data());
		}

		auto outputPath = commandLine.getOuput();
		outputPath += "\\";
		outputPath.replace_filename(commandLine.getImagePath(i).stem().string() + commandLine.getSuffix());
		outputPath.replace_extension(".png");

		imgPtr->write(outputPath);
	}

	return 0;
}
