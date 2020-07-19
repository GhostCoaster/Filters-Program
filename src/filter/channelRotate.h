
#ifndef FPP_CHANNEL_ROTATE
#define FPP_CHANNEL_ROTATE

#include "filter.h"
#include "../img/image.h"

namespace FPP {
	class ChannelRotate {
	public:
		static auto filter(Image*, Image*)->Filter::sheetReturn;
	};
}

#endif