
#include <string>
#include <filesystem>
#include "commandLine.h"
#include "filter/filters/shred.h"

namespace FPP {
	Filter CommandLine::filterList[] {
		blurFilter(),
		channelRotateFilter(),
		mergeFilter(),
		posterFilter(),
		shredFilter()
	};

	auto CommandLine::getImage(int numArguments, char** arguments) -> imageReturn {
		if (numArguments < 2)
			return {Image::makeEmpty(), "no filepath provided"};

		auto path = std::filesystem::path(arguments[1]);

		if (!std::filesystem::exists(path))
			return {Image::makeEmpty(), "image does not exist"};

		auto pathStr = path.string();

		/* make lowercase extension */
		auto extension = path.extension().string();
		for (auto it = extension.begin(); it < extension.end(); ++it) {
			*it |= 0x20;
		}

		auto image = (extension == ".png") ? Image::fromPNG(pathStr.c_str()) :
		             (extension == ".jpg") ? Image::fromJPG(pathStr.c_str()) :
		             Image::makeEmpty();

		/* check if extension check worked */
		if (!image.isValid())
			return {std::move(image), std::string("extension ") + extension + " is not supported"};

		return {std::move(image), ""};
	}

	auto CommandLine::getFilter(int numArguments, char** arguments) -> filterReturn {
		if (numArguments < 3)
			return {nullptr, "no filter name provided"};

		auto lookFor = arguments[2];

		auto numFilters = sizeof(filterList) / sizeof(Filter);

		for (auto i = 0; i < numFilters; ++i) {
			auto name = filterList[i].getName();

			if (strcmp(name, lookFor) == 0) {
				return {&filterList[i], ""};
			}
		}

		/* after loop if we have not returned */
		/* no filter matches name */
		return {nullptr, std::string("no filter of name ") + lookFor + " found"};
	}

	auto CommandLine::getParameters(int numArguments, char** arguments, Filter* filter) -> parametersReturn {
		auto numParameters = filter->getNumParams();

		auto ret = std::vector<Parameter>(numParameters);

		/* for each parameter */
		for (auto i = 0; i < numParameters && i < numArguments - 3; ++i) {
			/* create the parameter of the right type */
			auto type = filter->getParamType(i);
			ret.at(i) = Parameter(type);

			/* parse the value from command line arguments */
			if (!(ret[i].*ret[i].parse)(arguments[i + 3])) {
				return {std::move(ret), std::string("parameter number ") + std::to_string(i + 1) + " is malformed"};
			}
		}

		return {std::move(ret), ""};
	}
}