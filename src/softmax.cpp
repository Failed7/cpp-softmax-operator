#include "softmax.h"

#include <cmath>
#include <algorithm>

using namespace std; 

vector<float> MySoftmax_1D(const vector<float>& nums){
    if(nums.empty()){
        return {};
    }

    float sum = 0.0f;
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

vector<float> MySoftmax_2D(const vector<float>& nums, int row, int col){
    if(nums.empty() || col <= 0 || row <= 0 || col * row != nums.size()){
        return {};
    }

    float row_sum;
    vector<float> e(nums.size());
    float row_max;

    for(int i = 0; i < row; ++i){
        row_sum = 0.0f;
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

void MySoftmax_1D_Opt1(const vector<float>& nums, vector<float>& output){
    if(nums.empty()){
        output.clear();
        return;
    }

    if(output.size() != nums.size()){
        output.resize(nums.size());
    }
    float sum = 0.0f;
    float max = *max_element(nums.begin(), nums.end());

    for(int i = 0; i < nums.size(); ++i){
        output[i] = exp(nums[i] - max);
        sum = sum + output[i];
    }

    for(int i = 0; i < nums.size(); ++i){
        output[i] = output[i] / sum;
    }
}

void MySoftmax_2D_Opt1(const vector<float>& nums, int row, int col, vector<float>& output){
    if(nums.empty() || col <= 0 || row <= 0 || col * row != nums.size()){
        output.clear();
        return;
    }

    if(output.size() != nums.size()){
        output.resize(nums.size());
    }
    float row_sum;
    float row_max;

    for(int i = 0; i < row; ++i){
        row_sum = 0.0f;
        row_max = *max_element(nums.begin() + i * col, nums.begin() + (i + 1) * col);

        for(int j = 0; j < col; ++j){
            output[i * col + j] = exp(nums[i * col + j] - row_max);
            row_sum = row_sum + output[i * col + j];
        }

        for(int j = 0; j < col; ++j){
            output[i * col + j] = output[i * col + j] / row_sum;
        }
    }
}

void MySoftmax_1D_Opt2(const vector<float>& nums, vector<float>& output){
    if(nums.empty()){
        output.clear();
        return;
    }

    if(output.size() != nums.size()){
        output.resize(nums.size());
    }
    float sum = 0.0f;
    float max = *max_element(nums.begin(), nums.end());

    for(int i = 0; i < nums.size(); ++i){
        output[i] = exp(nums[i] - max);
        sum = sum + output[i];
    }

    float rev_sum = 1 / sum;

    for(int i = 0; i < nums.size(); ++i){
        output[i] = output[i] * rev_sum;
    }
}

void MySoftmax_2D_Opt2(const vector<float>& nums, int row, int col, vector<float>& output){
    if(nums.empty() || col <= 0 || row <= 0 || col * row != nums.size()){
        output.clear();
        return;
    }

    if(output.size() != nums.size()){
        output.resize(nums.size());
    }
    float row_sum;
    float row_max;

    for(int i = 0; i < row; ++i){
        row_sum = 0.0f;
        row_max = *max_element(nums.begin() + i * col, nums.begin() + (i + 1) * col);

        for(int j = 0; j < col; ++j){
            output[i * col + j] = exp(nums[i * col + j] - row_max);
            row_sum = row_sum + output[i * col + j];
        }

        float rev_row_sum = 1 / row_sum;

        for(int j = 0; j < col; ++j){
            output[i * col + j] = output[i * col + j] * rev_row_sum;
        }
    }
}

void MySoftmax_2D_Opt3(const vector<float>& nums, int row, int col, vector<float>& output){
    if(nums.empty() || col <= 0 || row <= 0 || col * row != nums.size()){
        output.clear();
        return;
    }

    if(output.size() != nums.size()){
        output.resize(nums.size());
    }

    #pragma omp parallel for
    for(int i = 0; i < row; ++i){
        float row_sum = 0.0f;
        float row_max = *max_element(nums.begin() + i * col, nums.begin() + (i + 1) * col);

        for(int j = 0; j < col; ++j){
            output[i * col + j] = exp(nums[i * col + j] - row_max);
            row_sum = row_sum + output[i * col + j];
        }

        float rev_row_sum = 1 / row_sum;

        for(int j = 0; j < col; ++j){
            output[i * col + j] = output[i * col + j] * rev_row_sum;
        }
    }
}