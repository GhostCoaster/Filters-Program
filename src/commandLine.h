
#ifndef FILTERS_PROGRAM_COMMANDLINE_H
#define FILTERS_PROGRAM_COMMANDLINE_H

#include "filter/filter.h"

namespace FPP {
	class CommandLine {
    public:
        struct Mode {
            bool showInfo = false;
            Filter * showFilter = nullptr;
        };

	private:
		using FlagParser = void(CommandLine::*)(int, char**, int&, std::string&);

		struct flagInfo {
			const char* flag;
			FlagParser parser;
		};

		static flagInfo flags[];

        Mode mode;

		std::vector<Image> images;
		std::vector<std::filesystem::path> imagePaths;

		std::vector<Filter*> filters;
		std::vector<std::vector<Parameter::Value>> params;
		std::filesystem::path output;
		std::string suffix;

		/* top level flag parsers */
		auto parseImages(int, char**, int&, std::string&) -> void;
		auto parseFilters(int, char**, int&, std::string&) -> void;
		auto parseSuffix(int, char**, int&, std::string&) -> void;
		auto parseOutput(int, char**, int&, std::string&) -> void;
        auto parseList(int, char**, int&, std::string&) -> void;

		/* sub level functions */
		auto getParameters(int, char **, int &, Filter &, std::string &) -> std::vector<Parameter::Value>;

		constexpr static int NOT_A_FLAG = -1;

		auto getFlag(const char*) -> int;

	public:
		CommandLine(int, char**, std::string&);

        auto getMode() -> Mode;

		auto getNumImages() -> int;
		auto getImage(int) -> Image&;
		auto getImagePath(int) -> std::filesystem::path&;

		auto getNumFilters() -> int;
		auto getFilter(int) -> Filter*;
		auto getParams(int) -> std::vector<Parameter::Value>&;

		auto getOuput() -> std::filesystem::path;
		auto getSuffix() -> std::string&;
	};
}

#endif //FILTERS_PROGRAM_COMMANDLINE_H
