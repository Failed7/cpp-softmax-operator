#include "softmax.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

int run_1d_benchmark(size_t size, mt19937& rng){
    cout << "Input size = " << size << endl;

    vector<float> input(size);

    uniform_real_distribution<float> dist(-10.0f, 10.0f);
    for (size_t i = 0; i < input.size(); ++i) {
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

    double elements_per_second = size / (median_ms / 1000.0);
    cout << "Throughput = " << elements_per_second / 1e6 << " M elements/s" << endl;
    
    double ns_per_element = median_ms * 1e6 / size;
    cout << "Time per element = " << ns_per_element << " ns" << endl;
    
    float sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << endl << endl;

    return 0;
}

int run_2d_benchmark(size_t row, size_t col, mt19937& rng){
    cout << "Input size = " << row << " * " << col << endl;

    size_t size = row * col;
    vector<float> input(size);

    uniform_real_distribution<float> dist(-10.0f, 10.0f);
    for (size_t i = 0; i < input.size(); ++i) {
        input[i] = dist(rng);
    }

    vector<float> output(size);
    for (int i = 0; i < 5; ++i){
        output = MySoftmax_2D(input, row, col);
    }

    vector<double> times;
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();
        output = MySoftmax_2D(input, row, col);
        auto end = chrono::steady_clock::now();

        double elapsed_ms = chrono::duration<double, milli>(end - start).count();
        times.push_back(elapsed_ms);
    }

    sort(times.begin(), times.end());

    double median_ms = (times[9] + times[10]) / 2.0;
    cout << "Median time = " << median_ms << " ms" << endl;

    double elements_per_second = size / (median_ms / 1000.0);
    cout << "Throughput = " << elements_per_second / 1e6 << " M elements/s" << endl;
    
    double ns_per_element = median_ms * 1e6 / size;
    cout << "Time per element = " << ns_per_element << " ns" << endl;
    
    float sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << " (expected about " << row << ")" << endl << endl;

    return 0;
}

int main(){
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    
    mt19937 rng(seed);

    cout << "1D seed = " << seed << endl << endl;
    vector<size_t> sizes = {1000, 10000, 100000, 1000000, 5000000};

    for(size_t i = 0; i < sizes.size(); ++i){
        run_1d_benchmark(sizes[i], rng);
    }

    cout << "2D seed = " << seed << endl << endl;
    vector<pair<size_t, size_t>> shapes = {
        {10, 100},
        {100, 100},
        {100, 1000},
        {1000, 1000},
        {1000, 5000}
    };

    for(size_t i = 0; i < shapes.size(); ++i){
        size_t row = shapes[i].first;
        size_t col = shapes[i].second;

        run_2d_benchmark(row, col, rng);
    }

    return 0;
}