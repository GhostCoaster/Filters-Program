//
// Created by Emmet on 7/21/2020.
//

#ifndef FILTERS_PROGRAM_FILTERS_H
#define FILTERS_PROGRAM_FILTERS_H

#include "../filter.h"

namespace FPP {
	extern auto shredFilter() -> Filter;

	extern auto shockwaveFilter() -> Filter;

	extern auto mergeFilter() -> Filter;

	extern auto channelRotateFilter() -> Filter;

	extern auto blurFilter() -> Filter;

	extern auto posterFilter() -> Filter;
}

#endif //FILTERS_PROGRAM_FILTERS_H
