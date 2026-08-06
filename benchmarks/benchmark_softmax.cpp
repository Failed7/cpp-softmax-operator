#include "softmax.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

int main(){
    size_t size = 1000000;
    vector<float> input(size);

    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    cout << "1D seed = " << seed << endl;
    mt19937 rng(seed);

    uniform_real_distribution<float> dist(-10.0f, 10.0f);

    for (int i = 0; i < input.size(); ++i) {
        input[i] = dist(rng);
    }

    vector<float> output(size);

    for (int i = 0; i < 5; ++i){
        output = MySoftmax_1D(input);
    }

    vector<double> times;
    
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();

        output = MySoftmax_1D(input);

        auto end = chrono::steady_clock::now();

        double elapsed_ms = chrono::duration<double, milli>(end - start).count();

        times.push_back(elapsed_ms);
    }

    sort(times.begin(), times.end());

    double median_ms = (times[9] + times[10]) / 2.0;
    cout << "Median time = " << median_ms << " ms" << endl;
    cout << "Input size = " << size << endl;
    
    float sum = accumulate(output.begin(), output.end(), 0.0f);
    cout << "Sum of the last output = " << sum << endl;

    return 0;
}