#ifndef RECOMMEND_H
#define RECOMMEND_H

#include <vector>
#include <string>

namespace rec {
    // accept K rows of (id, score) and return top-k indices - here simplified
    std::vector<int> rank_by_score(const std::vector<double>& scores, int topk);
}

#endif // RECOMMEND_H