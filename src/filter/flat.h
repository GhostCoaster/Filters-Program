
#ifndef FPP_FLAT
#define FPP_FLAT

#include "../img/image.h"
#include "filter.h"

namespace FPP {
	class Flat {
	public:
		static int globalCount;
		
		static auto filter(Image*, Image*, int)->Filter::sheetReturn;
	};
}

#endif
