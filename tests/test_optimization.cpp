#include "softmax.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

int main(){
    int row = 100;
    int col = 100;

    mt19937 rng(42);
    uniform_real_distribution<float> dist(-10.0f, 10.0f);

    vector<float> input(row * col);

    for(int i = 0; i < input.size(); ++i){
        input[i] = dist(rng);
    }


    // 1D
    vector<float> baseline_1d = MySoftmax_1D(input);

    vector<float> optimized_1d(input.size());
    MySoftmax_1D_Opt1(input, optimized_1d);

    float max_error = 0.0f;

    for(int i = 0; i < input.size(); ++i){
        max_error = max(max_error, abs(baseline_1d[i] - optimized_1d[i]));
    }

    cout << "1D Opt1 max_error = " << max_error << endl;

    if(max_error > 1e-5f){
        return 1;
    }

    cout << "1D Opt1 passed" << endl;


    // 2D
    vector<float> baseline_2d = MySoftmax_2D(input, row, col);

    vector<float> optimized_2d(input.size());
    MySoftmax_2D_Opt1(input, row, col, optimized_2d);

    max_error = 0.0f;

    for(int i = 0; i < input.size(); ++i){
        max_error = max(max_error, abs(baseline_2d[i] - optimized_2d[i]));
    }

    cout << "2D Opt1 max_error = " << max_error << endl;

    if(max_error > 1e-5f){
        return 1;
    }

    cout << "2D Opt1 passed" << endl;

    return 0;
}