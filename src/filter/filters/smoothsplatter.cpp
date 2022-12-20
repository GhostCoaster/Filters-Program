//
// Created by Emmet on 12/19/2022.
//

#include <random>

#include "filters.h"
#include "../util.h"

namespace FPP {
    auto pack(int x, int y) -> u32 {
        return ((u32)(x + (1 << 15)) << 16) | ((u32)(y + (1 << 15)));
    }

    struct coord { int x; int y; };

    auto unpack(u32 packed) -> coord {
        return {
            (int)(packed >> 16) - (1 << 15),
            (int)(packed & 0x0000FFFF) - (1 << 15),
        };
    }

    auto smoothSplatterFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 8, 1, 128, "radius", "splatter radius");
        parameters.emplace_back(Parameter::TYPE_INT, 1, 1, 64, "smooth", "smooth radius");

        return { "smoothSplatter", std::move(parameters), [](Buffers &buffers, std::vector <Parameter::Value> &values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto radius = Parameter::fromValue<int>(values[0]);
            auto blurRadius = Parameter::fromValue<int>(values[1]);

            auto engine = std::default_random_engine(std::random_device()());
            auto splatterRange = std::uniform_int_distribution<int>(-radius, radius);

            auto offsetSheet = Image::makeSheet(width, height);
            auto * pixels2 = offsetSheet.getPixels();

            /* initial offset population */
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    pixels2[Util::pos(x, y, width)] = pack(splatterRange(engine), splatterRange(engine));
                }
            }

            /* blur offset into pixels1 then back to pixels2 */
            /* horizontal blur */
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto left = Util::smallBound(x - blurRadius);
                    auto right = Util::largeBound(x + blurRadius + 1, width);

                    auto totalX = 0;
                    auto totalY = 0;

                    for (auto i = left; i < right; ++i) {
                        auto [offsetX, offsetY] = unpack(pixels2[Util::pos(i, y, width)]);
                        totalX += offsetX;
                        totalY += offsetY;
                    }

                    totalX /= (right - left);
                    totalY /= (right - left);

                    pixels1[Util::pos(x, y, width)] = pack(totalX, totalY);
                }
            }

            /* vertical blur */
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto up = Util::smallBound(y - blurRadius);
                    auto down = Util::largeBound(y + blurRadius + 1, height);

                    auto totalX = 0;
                    auto totalY = 0;

                    for (auto i = up; i < down; ++i) {
                        auto [offsetX, offsetY] = unpack(pixels1[Util::pos(x, i, width)]);
                        totalX += offsetX;
                        totalY += offsetY;
                    }

                    totalX /= (down - up);
                    totalY /= (down - up);

                    pixels2[Util::pos(x, y, width)] = pack(totalX, totalY);
                }
            }

            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto [offsetX, offsetY] = unpack(pixels2[Util::pos(x, y, width)]);

                    auto sampleX = Util::bounds(x + offsetX, width);
                    auto sampleY = Util::bounds(y + offsetY, height);

                    pixels1[Util::pos(x, y, width)] = pixels0[Util::pos(sampleX, sampleY, width)];
                }
            }

            buffers.swap();
        }};
    }
}