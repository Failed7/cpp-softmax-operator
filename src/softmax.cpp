#include "softmax.h"

#include <cmath>
#include <algorithm>

using namespace std; 

vector<float>  MySoftmax_1D(const vector<float>& nums){
    if(nums.empty()){
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
    
}

vector<float>  MySoftmax_2D(const vector<float>& nums, int row, int col){
    if(nums.empty() || col <= 0 || row <= 0 || col * row != nums.size()){
        return {};
    }

    float row_sum;
    vector<float> e(nums.size());
    float row_max;

    for(int i = 0; i < row; ++i){
        row_sum = 0.0;
        row_max = *max_element(nums.begin() + i * col, nums.begin() + (i + 1) * col);

        for(int j = 0; j < col; ++j){
            e[i * col + j] = exp(nums[i * col + j] - row_max);
            row_sum = row_sum + e[i * col + j];
        }

        for(int j = 0; j < col; ++j){
            e[i * col + j] = e[i * col + j] / row_sum;
        }
    }

    return e;

}