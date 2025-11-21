#include "dataprocess.h"
#include <algorithm>

namespace dp {
    std::vector<double> normalize(const std::vector<double>& v) {
        if (v.empty()) return {};
        double lo = *std::min_element(v.begin(), v.end());
        double hi = *std::max_element(v.begin(), v.end());
        std::vector<double> out; out.reserve(v.size());
        for (double x: v) {
            if (hi==lo) out.push_back(1.0);
            else out.push_back((x-lo)/(hi-lo));
        }
        return out;
    }
}
