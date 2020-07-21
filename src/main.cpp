
#include <iostream>
#include <filesystem>

#include "img/image.h"
#include "img/formatter.h"
#include "commandLine.h"

auto main(int argc, char** argv) -> int {
	auto [image, errMessage] = FPP::CommandLine::getImage(argc, argv);

	if (errMessage != "") {
		std::cout << errMessage << std::endl;
		return -1;
	}

	auto [filter, errMessage2] = FPP::CommandLine::getFilter(argc, argv);

	if (errMessage2 != "") {
		std::cout << errMessage2 << std::endl;
		return -1;
	}

	auto [parameters, errMessage3] = FPP::CommandLine::getParameters(argc, argv, filter);

	if (errMessage3 != "") {
		std::cout << errMessage3 << std::endl;
		return -1;
	}

	auto sheet = FPP::Image::makeSheet(image.getWidth(), image.getHeight());

	auto imgPtr = &image;
	auto sheetPtr = &sheet;

	filter->filter(&imgPtr, &sheetPtr, parameters.data());

	imgPtr->write(R"(../imgs/output.png)");

	return 0;
}
