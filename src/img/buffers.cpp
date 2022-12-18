//
// Created by Emmet on 12/17/2022.
//

#include "buffers.h"

namespace FPP {
    Buffers::Buffers(Image * buffer0, Image * buffer1) : buffer0(buffer0), buffer1(buffer1) {}

    auto Buffers::front() -> Image & {
        return *buffer0;
    }

    auto Buffers::back() -> Image & {
        return *buffer1;
    }

    auto Buffers::swap() -> void {
        auto temp = buffer0;
        buffer0 = buffer1;
        buffer1 = temp;
    }

    auto Buffers::dimensions() -> Dimensions {
        return {
        (int)buffer0->getWidth(),
        (int)buffer0->getHeight(),
        };
    }

    auto Buffers::matchFrontToBack() -> void {
        buffer0->resize(buffer1->getWidth(), buffer1->getWidth());
    }
}
