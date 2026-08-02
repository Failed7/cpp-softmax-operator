#include <iostream> 
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std; 

vector<float>  MySoftmax_1D(vector<float> nums){
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

int main(){
    vector<float> in = {1000.0, 1001.0, 1002.0};
    vector<float> result = MySoftmax_1D(in);
    for(int i = 0; i < result.size(); ++i){
        cout << result[i] << " ";
    }
    return 0; 
}