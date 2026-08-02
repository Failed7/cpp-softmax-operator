#include "softmax.h"

#include <cmath>
#include <algorithm>

using namespace std; 

vector<float>  MySoftmax_1D(const vector<float>& nums){
    if (nums.empty()){
        return {};
    }

    float sum = 0.0;
    vector<float> e(nums.size());
    float max = *max_element(nums.begin(), nums.end());

    for(int i = 0; i < nums.size(); ++i){
        e[i] = exp(nums[i] - max);
        sum = sum + e[i];
    }

    for(int i = 0; i < nums.size(); ++i){
        e[i] = e[i] / sum;
    }

    return e;
    
};