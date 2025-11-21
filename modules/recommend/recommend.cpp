#include "recommend.h"
#include <algorithm>

namespace rec {
    std::vector<int> rank_by_score(const std::vector<double>& scores, int topk) {
        std::vector<int> idx(scores.size());
        for (int i=0;i<(int)scores.size();++i) idx[i]=i;
        std::stable_sort(idx.begin(), idx.end(), [&](int a,int b){ return scores[a]>scores[b]; });
        if ((int)idx.size()>topk) idx.resize(topk);
        return idx;
    }
}
