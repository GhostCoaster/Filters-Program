//
// Created by Emmet on 7/22/2020.
//

#ifndef FILTERS_PROGRAM_SHLEK_H
#define FILTERS_PROGRAM_SHLEK_H

#include "../../type.h"

namespace FPP {
	struct entropy {
		int entropy;
		int position;
	};

	extern auto findLeastEntropy(u32*, int, int, u32, u32) -> entropy;

	extern auto recalculate(u32*, u32*, int, int, u32, u32, u32) -> void;
}

#endif //FILTERS_PROGRAM_SHLEK_H
