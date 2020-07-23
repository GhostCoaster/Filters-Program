
#include <string>
#include <filesystem>
#include "commandLine.h"
#include "filter/filters/filters.h"
#include "filter/util.h"

namespace FPP {
	CommandLine::flagInfo CommandLine::flags[] {
		{"-i", &CommandLine::parseImages},
		{"-f", &CommandLine::parseFilters},
		{"-s", &CommandLine::parseSuffix},
		{"-o", &CommandLine::parseOutput}
	};

	Filter CommandLine::filterList[] {
		blurFilter(),
		channelRotateFilter(),
		mergeFilter(),
		posterFilter(),
		shredFilter(),
		shockwaveFilter(),
		noiseFilter(),
		noiseScaleFilter(),
		badShlek(),
		sortFilter(),
		shlek()
	};

	CommandLine::CommandLine(int numArguments, char** arguments, std::string& errMessage)
	: images(), filters(), parameters(), output(), suffix() {
		for (auto i = 1; i < numArguments; ++i) {
			/* top level arguments must be a flag */
			auto argument = arguments[i];

			auto flagIndex = getFlag(argument);

			if (flagIndex == NOT_A_FLAG) {
				errMessage = std::string("expected flag, found ") + argument + " instead";
				return;
			}

			/* parse arguments for this flag */
			(this->*flags[flagIndex].parser)(numArguments, arguments, ++i, errMessage);

			/* handle errors from the parser */
			if (errMessage != "") {
				return;
			}
		}

		/* images flag and filters flag must be provided */
		if (images.size() == 0) {
			errMessage = "no images provided";
			return;
		}

		if (filters.size() == 0) {
			errMessage = "no filters provided";
			return;
		}

		/* default values for output dir and suffix */
		if (output == "") {
			output = imagePaths[0].parent_path();
		}

		if (suffix == "") {
			suffix = "_CORR";
		}
	}

	auto CommandLine::parseImages(int numArguments, char** arguments, int& index, std::string& errMessage) -> void {
		auto imagesCount = 0;

		for (; index < numArguments; ++index) {
			auto argument = arguments[index];

			/* exit if we found next flag */
			auto flagIndex = getFlag(argument);
			if (flagIndex != NOT_A_FLAG) {
				if (imagesCount == 0)
					errMessage = "no arguments provided for -i";
				else
					--index;

				return;
			}

			/* check if path exists */
			auto path = std::filesystem::path(argument);

			if (!std::filesystem::exists(path)) {
				errMessage = std::string("image ") + argument + " does not exist";
				return;
			}

			/* find extension of file */
			auto extension = path.extension().string();

			/* make lowercase extension */
			for (auto it = extension.begin(); it < extension.end(); ++it) *it |= 0x20;

			/* load the image */
			auto image = (extension == ".png") ? Image::fromPNG(path) :
			             (extension == ".jpg") ? Image::fromJPG(path) :
			             Image::makeEmpty();

			/* check if extension check worked */
			if (!image.isValid()) {
				errMessage =  std::string("extension ") + extension + " is not supported";
				return;
			}

			/* image is valid so store its data */
			imagePaths.push_back(std::move(path));
			images.push_back(std::move(image));
			++imagesCount;
		}

		/* if the -i flag is the last argument */
		if (imagesCount == 0) {
			errMessage = "no arguments provided for -i";
			return;
		}
	}

	auto CommandLine::parseFilters(int numArguments, char** arguments, int& index, std::string& errMessage) -> void {
		/* if -f flag is last argument */
		if (index == numArguments) {
			errMessage = "no arguments provided for -f";
			return;
		}

		auto argument = arguments[index];
		auto filter = static_cast<Filter*>(nullptr);

		/* find which filter argument is */
		for (auto i = 0; i < Util::size(filterList); ++i) {
			if (strcmp(filterList[i].getName(), argument) == 0) {
				filter = filterList + i;
				break;
			}
		}

		if (filter == nullptr) {
			errMessage = std::string("no filter found with name ") + argument;
			return;
		}

		auto tempParameters = getParameters(numArguments, arguments, ++index, filter, errMessage);

		/* check any errors caused by getParameters */
		if (errMessage != "") {
			return;
		}

		/* add filter and parameters to list */
		filters.push_back(filter);
		parameters.push_back(std::move(tempParameters));
	}

	auto CommandLine::getParameters(int numArguments, char** arguments, int &index, Filter* filter, std::string& errMessage) -> std::vector<Parameter> {
		auto numParameters = filter->getNumParams();

		auto parameterCount = 0;

		auto ret = std::vector<Parameter>(numParameters);

		/* for each parameter */
		for (; index < numArguments; ++index, ++parameterCount) {
			auto argument = arguments[index];

			if (getFlag(argument) != NOT_A_FLAG) {
				--index;
				return ret;
			}

			if (parameterCount == numParameters) {
				errMessage = std::string("more parameters than expected in filter ") + filter->getName() + ", expected " + std::to_string(numParameters);
				return ret;
			}

			/* create the parameter of the right type */
			auto type = filter->getParamType(parameterCount);

			auto& parameter = ret.at(parameterCount);
			parameter = Parameter(type);

			/* parse the value from command line arguments */
			if (!(parameter.*parameter.parse)(argument)) {
				errMessage = std::string("parameter number ") + std::to_string(parameterCount) + " in filter " + filter->getName() + " is malformed";
				return ret;
			}
		}

		return ret;
	}

	auto CommandLine::parseSuffix(int numArguments, char** arguments, int& index, std::string& errMessage) -> void {
		if (suffix != "") {
			errMessage = "suffix already set";
			return;
		}

		auto argument = arguments[index];

		suffix = argument;

		if (suffix.length() > 200) {
			errMessage = "suffix too long, must be at most 200 characters";
			return;
		}

		static const char bad[] = R"(\/:*?"<>|)";

		/* make sure suffix contains only filesystem safe characters */
		for (auto it = suffix.begin(); it < suffix.end(); ++it) {
			auto current = *it;

			for (auto k = 0; k < Util::size(bad); ++k) {
				if (current == bad[k]) {
					errMessage = std::string("unexpected character ") + current + " at position";
					return;
				}
			}
		}
	}

	auto CommandLine::parseOutput(int numArguments, char** arguments, int& index, std::string& errMessage) -> void {
		auto argument = arguments[index];

		output = argument;

		/* output directory must be a directory */
		if (std::filesystem::exists(output)) {
			if (!std::filesystem::is_directory(output)) {
				errMessage = std::string(argument) + "is not a directory";
			}

		/* make directory if it doesn't exist */
		} else {
			if (!std::filesystem::create_directory(output)) {
				errMessage = std::string("failed to create directory ") + argument;
			}
		}
	}

	auto CommandLine::getNumImages() -> int {
		return images.size();
	}

	auto CommandLine::getImage(int index) -> Image& {
		return images.at(index);
	}

	auto CommandLine::getImagePath(int index) -> std::filesystem::path& {
		return imagePaths.at(index);
	}

	auto CommandLine::getNumFilters() -> int {
		return filters.size();
	}

	auto CommandLine::getFilter(int index) -> Filter* {
		return filters[index];
	}

	auto CommandLine::getParameters(int index) -> std::vector<Parameter>& {
		return parameters.at(index);
	}

	auto CommandLine::getOuput() -> std::filesystem::path {
		return output;
	}

	auto CommandLine::getSuffix() -> std::string& {
		return suffix;
	}

	auto CommandLine::getFlag(const char* argument) -> int {
		for (auto j = 0; j < Util::size(flags); ++j) {
			if (strcmp(flags[j].flag, argument) == 0) {
				return j;
				break;
			}
		}

		return NOT_A_FLAG;
	}
}
