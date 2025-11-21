#include "dataprocess.h"
#include <cstring>

extern "C" {
    // simple C wrapper: normalize input double array in-place
    void dp_normalize(double* arr, int len) {
        if (!arr || len<=0) return;
        std::vector<double> v(arr, arr+len);
        auto res = dp::normalize(v);
        for (int i=0;i<len && i<(int)res.size();++i) arr[i] = res[i];
    }
}