
#ifndef FPP_MERGE
#define FPP_MERGE

#include "../img/image.h"
#include "filter.h"

namespace FPP {
	class Merge {
	public:
		static auto filter(Image*, Image*, int, int)->Filter::sheetReturn;
	};
}

#endif
#pragma once
