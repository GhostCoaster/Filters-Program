//
// Created by Emmet on 12/18/2022.
//

#include <random>

#include "filters.h"
#include "../util.h"

namespace FPP {
    auto puzzleFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 8, 1, 64, "wide", "number of pieces wide");
        parameters.emplace_back(Parameter::TYPE_INT, 8, 1, 64, "tall", "number of pieces tall");

        return { "puzzle", std::move(parameters), [](Buffers & buffers, std::vector<Parameter::Value> & values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto wide = Parameter::fromValue<int>(values[0]);
            auto tall = Parameter::fromValue<int>(values[1]);

            auto boxWidth = (float)width / (float)wide;
            auto boxHeight = (float)height / (float)tall;

            auto pieceMap = std::vector<int>(wide * tall);
            for (auto i = 0; i < wide * tall; ++i) {
                pieceMap[i] = i;
            }

            auto engine = std::default_random_engine(std::random_device()());
            auto orderRange = std::uniform_int_distribution<int>(0, wide * tall - 1);

            /* shuffle */
            for (auto i = 0; i < wide * tall; ++i) {
                auto toIndex = orderRange(engine);
                auto temp = pieceMap[i];
                pieceMap[i] = pieceMap[toIndex];
                pieceMap[toIndex] = temp;
            }

            /* precompute box bounds */
            /* each box bound also contains a final which is the image's edge */
            auto lefts = std::vector<int>(wide + 1);
            auto ups = std::vector<int>(tall + 1);

            auto runner = 0.0f;
            for (auto i = 0; i < wide; ++i) {
                lefts[i] = (int)runner;
                runner += boxWidth;
            }
            lefts[wide] = width;

            runner = 0.0f;
            for (auto i = 0; i < tall; ++i) {
                ups[i] = (int)runner;
                runner += boxHeight;
            }
            ups[tall] = height;

            for (auto y = 0; y < tall; ++y) {
                for (auto x = 0; x < wide; ++x) {
                    auto boxIndex = y * wide + x;
                    auto sourceBoxIndex = pieceMap[boxIndex];

                    auto left = lefts[x];
                    auto up = ups[y];

                    auto right = lefts[x + 1];
                    auto down = ups[y + 1];

                    auto maxOffsetX = right - left;
                    auto maxOffsetY = down - up;

                    auto sourceX = sourceBoxIndex % wide;
                    auto sourceY = sourceBoxIndex / wide;

                    auto sourceLeft = lefts[sourceX];
                    auto sourceUp = ups[sourceY];

                    /* somehow not needed */
                    //auto sourceRight = sourceLeft + maxOffsetX;
                    // auto sourceDown = sourceUp + maxOffsetY;

                    //if (sourceRight > width + 1) --sourceLeft;
                    //if (sourceDown > height + 1) --sourceUp;

                    for (auto offsetY = 0; offsetY < maxOffsetY; ++offsetY) {
                        for (auto offsetX = 0; offsetX < maxOffsetX; ++offsetX) {
                            pixels1[Util::pos(left + offsetX, up + offsetY, width)] = pixels0[Util::pos(sourceLeft + offsetX, sourceUp + offsetY, width)];
                        }
                    }
                }
            }

            buffers.swap();
        }};
    }
}
