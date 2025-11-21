#include "recommend.h"
#include <cstring>

extern "C" {
    // scores: input array, n: length, out_indices: preallocated int array, out_len: capacity
    int rec_rank_topk(const double* scores, int n, int* out_indices, int out_len) {
        if (!scores || n<=0 || !out_indices || out_len<=0) return 0;
        std::vector<double> v(scores, scores+n);
        auto idx = rec::rank_by_score(v, out_len);
        int written = 0;
        for (int i=0;i<(int)idx.size() && i<out_len;++i) {
            out_indices[i] = idx[i];
            ++written;
        }
        return written; // number of indices written
    }
}
