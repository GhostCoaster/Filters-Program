
#ifndef FPP_BLUR
#define FPP_BLUR

#include "../img/image.h"
#include "filter.h"

namespace FPP {
	class Blur {
	public:
		static auto filter(Image*, Image*, int) -> Filter::sheetReturn;
	};	
}

#endif
