
#include <string>
#include <string.h>
#include <filesystem>
#include "commandLine.h"

#include "filter/util.h"
#include "filter/filterList.h"

namespace FPP {
	CommandLine::flagInfo CommandLine::flags[] {
		{"-i", &CommandLine::parseImages},
		{"-f", &CommandLine::parseFilters},
		{"-s", &CommandLine::parseSuffix},
		{"-o", &CommandLine::parseOutput},
        {"-l", &CommandLine::parseList}
	};

	CommandLine::CommandLine(int numArguments, char** arguments, std::string& errMessage)
	: mode(), images(), filters(), params(), output(), suffix() {
		for (auto i = 1; i < numArguments; ++i) {
			/* get the top level argument, usually a flag */
			auto argument = arguments[i];
			auto flagIndex = getFlag(argument);

			if (flagIndex == NOT_A_FLAG) {
				/* special rule for first argument, doesn't have to be the -i flag */
				if (i == 1) {
					(this->*flags[0].parser)(numArguments, arguments, i, errMessage);

				/* otherwise syntax error, needs a flag */
				} else {
					errMessage = std::string("expected flag, found ") + argument + " instead";
					return;
				}

			/* parse the NEXT element after the flag based on the flag's parser */
			} else {
				(this->*flags[flagIndex].parser)(numArguments, arguments, ++i, errMessage);

				if (errMessage != "") return;
			}
		}

		/* images flag and filters flag must be provided if not in list mode */
        if (!mode.showInfo) {
            if (images.size() == 0) {
                errMessage = "no images provided";
                return;
            }

            if (filters.size() == 0) {
                errMessage = "no filters provided";
                return;
            }

            /* default values for output dir and suffix */
            if (output.empty()) {
                output = imagePaths[0].parent_path();
            }

            if (suffix.empty()) {
                suffix = "_CORR";
            }
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
		/* if -f flag is last filterName */
		if (index == numArguments) {
			errMessage = "no arguments provided for -f";
			return;
		}

		auto filterName = std::string(arguments[index]);
		auto filter = FilterList::getFilter(filterName);

		if (filter == nullptr) {
			errMessage = std::string("no filter found with name ") + filterName;
			return;
		}

		auto tempParameters = getParameters(numArguments, arguments, ++index, *filter, errMessage);

		/* check any errors caused by getParameters */
		if (!errMessage.empty()) return;

		/* add filter and params to list */
		filters.push_back(filter);
		params.push_back(std::move(tempParameters));
	}

    struct ParsedParam {
        std::string name;
        std::string value;

        auto hasName() {
            return !name.empty();
        }
    };

    auto parseParam(char * argument) -> ParsedParam {
        auto equalsIndex = -1;
        auto iter = argument;
        while (*iter != '\0') {
            if (*iter == '=') {
                equalsIndex = (int)(iter - argument);
                break;
            }
        }

        if (equalsIndex == -1) {
            return { "", argument };
        } else {
            argument[equalsIndex] = '\0';
            return { argument, argument + equalsIndex + 1 };
        }
    }

    /*
     * unnamed arguments:
     * -f myFilter 0 1.3 true
     *
     * named arguments:
     * -f myFilter param0=0 param1=1.3 param2=true
     *
     * don't mix, that's just stupid
     */

	auto CommandLine::getParameters(int numArguments, char ** arguments, int & index, Filter & filter, std::string & errMessage) -> std::vector<Parameter::Value> {
		auto paramIndex = 0;

		auto valuesList = std::vector<Parameter::Value>();
        valuesList.reserve(filter.numParams());
        for (auto i = 0; i < filter.numParams(); ++i) {
            valuesList.push_back(filter.getParam(i).defaultValue);
        }

        bool namedMode;

		/* for each parameter */
		for (; index < numArguments; ++index, ++paramIndex) {
			auto argument = arguments[index];

			if (getFlag(argument) != NOT_A_FLAG) {
				--index;
				return valuesList;
			}

			if (paramIndex == filter.numParams()) {
				errMessage = std::string("more params than expected in filter ") + filter.getName() + ", expected " + std::to_string(filter.numParams());
				return valuesList;
			}

            auto param = parseParam(argument);
            if (paramIndex == 0) {
                namedMode = param.hasName();
            } else if (namedMode != param.hasName()) {
                errMessage = "mix of named and unnamed parameters";
                return valuesList;
            }

            Parameter * parameter;
            if (param.hasName()) {
                parameter = filter.getParam(param.name);
                if (parameter == nullptr) {
                    errMessage = std::string("unknown parameter \"") + param.name + "\"";
                    return valuesList;
                }
            } else {
                parameter = &filter.getParam(paramIndex);
            }

            try {
                valuesList.emplace_back((parameter->*parameter->parse)(argument));
            } catch (std::exception & ex) {
                errMessage = std::string("bad argument for parameter ") + parameter->name + " (" + std::to_string(paramIndex) + ") in filter " + filter.getName() + ": " + ex.what();
                return valuesList;
            }
		}

		return valuesList;
	}

	auto CommandLine::parseSuffix(int numArguments, char** arguments, int& index, std::string& errMessage) -> void {
		if (!suffix.empty()) {
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

    auto CommandLine::parseList(int numArguments, char** arguments, int& index, std::string& errMessage) -> void {
        auto subIndex = 0;

        mode.showInfo = true;

        for (; index < numArguments; ++index, ++subIndex) {
            auto argument = arguments[index];

            if (getFlag(argument) != NOT_A_FLAG) {
                --index;
                return;
            }

            if (subIndex == 0) {
                auto filterName = std::string(argument);
                auto * filter = FilterList::getFilter(filterName);

                if (filter == nullptr) {
                    errMessage = "no filter by name \"" + filterName + "\"";
                    return;
                }

                mode.showFilter = filter;

            } else {
                errMessage = "unexpected extra arguments for list, please only specify a filter";
                return;
            }
        }
    }

    auto CommandLine::getMode() -> Mode {
        return mode;
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

	auto CommandLine::getParams(int index) -> std::vector<Parameter::Value>& {
		return params.at(index);
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
			}
		}

		return NOT_A_FLAG;
	}
}
