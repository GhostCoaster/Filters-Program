//
// Created by Emmet on 12/19/2022.
//

#include <random>

#include "filters.h"
#include "../util.h"

namespace FPP {
    auto glowFilter() -> Filter {
        auto parameters = std::vector<Parameter>();
        parameters.emplace_back(Parameter::TYPE_INT, 8, 1, 64, "radius", "splatter radius");
        parameters.emplace_back(Parameter::TYPE_INT, 127, 1, 255, "max", "pixel difference resulting in the max glow");

        return { "glow", std::move(parameters), [](Buffers &buffers, std::vector <Parameter::Value> &values) {
            auto [width, height] = buffers.dimensions();
            auto * pixels0 = buffers.front().getPixels();
            auto * pixels1 = buffers.back().getPixels();

            auto radius = Parameter::fromValue<int>(values[0]);
            auto brightLimit = Parameter::fromValue<int>(values[1]);

            auto sheet = Image::makeSheet(width, height);
            auto * pixels2 = sheet.getPixels();

            /* horizontal difference into buffer 1 */
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto left = Util::smallBound(x - radius);
                    auto right = Util::largeBound(x + radius + 1, width);

                    auto pixel = pixels0[Util::pos(x, y, width)];

                    auto difference = 0;

                    for (auto i = left; i < right; ++i) {
                        difference += (int)Util::difference(pixel, pixels0[Util::pos(i, y, width)]);
                    }

                    pixels1[Util::pos(x, y, width)] = difference / (right - left);
                }
            }

            /* vertical difference into buffer 2 */
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto up = Util::smallBound(y - radius);
                    auto down = Util::largeBound(y + radius + 1, height);

                    auto pixel = pixels0[Util::pos(x, y, width)];

                    auto difference = 0;

                    for (auto i = up; i < down; ++i) {
                        difference += (int)Util::difference(pixel, pixels0[Util::pos(x, i, width)]);
                    }

                    pixels2[Util::pos(x, y, width)] = (pixels1[Util::pos(x, y, width)] + (difference / (down - up))) / 2;
                }
            }

            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    auto value = (float)std::min(brightLimit, (int)pixels2[Util::pos(x, y, width)]) / (float)brightLimit;

                    auto along = (value * value);

                    //auto redAmp = std::min(1.0f, std::max(0.0f, along < 0.5f ? 2.0f : 2.0f - 8.0f * std::abs(along - 0.50f)));
                    //auto greAmp = std::min(1.0f, std::max(0.0f, 2.0f - 8.0f * std::abs(along - 0.75f)));
                    //auto bluAmp = std::min(1.0f, std::max(0.0f, 2.0f - 8.0f * std::abs(along - 1.00f)));

                    auto greAmp = std::min(1.0f, std::max(0.0f, 2.0f - 4.0f * std::abs(along - 0.00f)));
                    auto bluAmp = std::min(1.0f, std::max(0.0f, 2.0f - 4.0f * std::abs(along - 0.50f)));
                    auto redAmp = std::min(1.0f, std::max(0.0f, 2.0f - 4.0f * std::abs(along - 1.00f)));

                    auto [r, g, b, a] = Util::pix(pixels0[Util::pos(x, y, width)]);

                    r = std::min(r + (int)(along * redAmp * 0xff), 0xff);
                    g = std::min(g + (int)(along * greAmp * 0xff), 0xff);
                    b = std::min(b + (int)(along * bluAmp * 0xff), 0xff);

                    pixels1[Util::pos(x, y, width)] = Util::pix(r, g, b, a);
                }
            }

            buffers.swap();
        }};
    }
}