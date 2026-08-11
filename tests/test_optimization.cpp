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

    mt19937 rng(808);
    uniform_real_distribution<float> dist(-10.0f, 10.0f);

    vector<float> input(row * col);

    for(int i = 0; i < input.size(); ++i){
        input[i] = dist(rng);
    }


    // 1D
    vector<float> baseline_1d = MySoftmax_1D(input);

    vector<float> optimized1_1d(input.size());
    vector<float> optimized2_1d(input.size());

    MySoftmax_1D_Opt1(input, optimized1_1d);
    MySoftmax_1D_Opt2(input, optimized2_1d);

    vector<float> max_error(3, 0.0f);

    for(int i = 0; i < input.size(); ++i){
        max_error[0] = max(max_error[0], abs(baseline_1d[i] - optimized1_1d[i]));
        max_error[1] = max(max_error[1], abs(baseline_1d[i] - optimized2_1d[i]));
    }

    cout << "1D Opt1 max_error = " << max_error[0] << endl;
    if(max_error[0] > 1e-5f){
        return 1;
    }
    cout << "1D Opt1 passed" << endl;

    cout << "1D Opt2 max_error = " << max_error[1] << endl;
    if(max_error[1] > 1e-5f){
        return 2;
    }
    cout << "1D Opt2 passed" << endl;


    // 2D
    vector<float> baseline_2d = MySoftmax_2D(input, row, col);

    vector<float> optimized1_2d(input.size());
    vector<float> optimized2_2d(input.size());
    vector<float> optimized3_2d(input.size());
    
    MySoftmax_2D_Opt1(input, row, col, optimized1_2d);
    MySoftmax_2D_Opt2(input, row, col, optimized2_2d);
    MySoftmax_2D_Opt3(input, row, col, optimized3_2d);

    max_error[0] = 0.0f;
    max_error[1] = 0.0f;
    max_error[2] = 0.0f;

    for(int i = 0; i < input.size(); ++i){
        max_error[0] = max(max_error[0], abs(baseline_2d[i] - optimized1_2d[i]));
        max_error[1] = max(max_error[1], abs(baseline_2d[i] - optimized2_2d[i]));
        max_error[2] = max(max_error[2], abs(baseline_2d[i] - optimized3_2d[i]));
    }

    cout << "2D Opt1 max_error = " << max_error[0] << endl;
    if(max_error[0] > 1e-5f){
        return 1;
    }
    cout << "2D Opt1 passed" << endl;

    cout << "2D Opt2 max_error = " << max_error[1] << endl;
    if(max_error[1] > 1e-5f){
        return 2;
    }
    cout << "2D Opt2 passed" << endl;

    cout << "2D Opt3 max_error = " << max_error[2] << endl;
    if(max_error[2] > 1e-5f){
        return 3;
    }
    cout << "2D Opt3 passed" << endl;

    return 0;
}