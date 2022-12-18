//
// Created by Emmet on 12/17/2022.
//

#ifndef FILTERS_PROGRAM_BUFFERS_H
#define FILTERS_PROGRAM_BUFFERS_H

#include "image.h"

namespace FPP {
    class Buffers {
    private:
        Image * buffer0;
        Image * buffer1;

    public:
        Buffers(Image *, Image *);

        auto front() -> Image &;
        auto back() -> Image &;

        struct Dimensions { int width; int height; };
        auto dimensions() -> Dimensions;

        auto swap() -> void;
        auto matchFrontToBack() -> void;
    };
}

#endif //FILTERS_PROGRAM_BUFFERS_H
