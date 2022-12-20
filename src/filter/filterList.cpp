//
// Created by Emmet on 12/17/2022.
//

#include "filterList.h"
#include "filters/filters.h"
#include "util.h"

namespace FPP {
    std::vector<Filter> FilterList::list = {};

    auto FilterList::init() -> void {
        list.push_back(std::move(blurFilter()));
        list.push_back(std::move(channelRotateFilter()));
        list.push_back(std::move(mergeFilter()));
        list.push_back(std::move(posterFilter()));
        list.push_back(std::move(shredFilter()));
        list.push_back(std::move(shockwaveFilter()));
        list.push_back(std::move(noiseFilter()));
        list.push_back(std::move(noiseScaleFilter()));
        list.push_back(std::move(cutFilter()));
        list.push_back(std::move(sortFilter()));
        list.push_back(std::move(puzzleFilter()));
        list.push_back(std::move(splatterFilter()));
        list.push_back(std::move(smoothSplatterFilter()));
        list.push_back(std::move(glowFilter()));
    }

    auto FilterList::getFilter(std::string & name) -> Filter * {
        for (auto & filter : list) {
            if (Util::stringEqual(filter.getName(), name)) {
                return &filter;
            }
        }
        return nullptr;
    }
}