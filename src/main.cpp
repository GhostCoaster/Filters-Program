
#include <iostream>
#include <filesystem>

#include "img/image.h"
#include "img/formatter.h"
#include "commandLine.h"

auto main(int argc, char** argv) -> int {
	auto errMessage = std::string("");

	auto image = FPP::CommandLine::getImage(argc, argv, errMessage);

	if (errMessage != "") {
		std::cout << errMessage << std::endl;
		return -1;
	}

	auto filter = FPP::CommandLine::getFilter(argc, argv, errMessage);

	if (errMessage != "") {
		std::cout << errMessage << std::endl;
		return -1;
	}

	auto parameters = FPP::CommandLine::getParameters(argc, argv, filter, errMessage);

	if (errMessage != "") {
		std::cout << errMessage << std::endl;
		return -1;
	}

	auto sheet = FPP::Image::makeSheet(image.getWidth(), image.getHeight());

	auto imgPtr = &image;
	auto sheetPtr = &sheet;

	filter->filter(&imgPtr, &sheetPtr, parameters.data());

	imgPtr->write(R"(../imgs/output.png)");

	return 0;
}
