
#ifndef FPP_POSTER
#define FPP_POSTER

#include "filter.h"
#include "../img/image.h"

namespace FPP {
	class Poster {
	public:
		static auto filter(Image*, Image*, int, int)->Filter::sheetReturn;
	};
}

#endif
