#include "softmax.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <omp.h>

using namespace std;

int run_1d_benchmark(const vector<float>& input){
    cout << "[1D Baseline]" << endl;

    vector<float> output(input.size());
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

    double elements_per_second = output.size() / (median_ms / 1000.0);
    cout << "Throughput = " << elements_per_second / 1e6 << " M elements/s" << endl;
    
    double ns_per_element = median_ms * 1e6 / output.size();
    cout << "Time per element = " << ns_per_element << " ns" << endl;
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << endl << endl;

    return 0;
}

int run_2d_benchmark(size_t row, size_t col, const vector<float>& input){
    cout << "[2D Baseline]" << endl;

    size_t size = row * col;
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
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << " (expected about " << row << ")" << endl << endl;

    return 0;
}

int run_1d_opt1_benchmark(const vector<float>& input){
    cout << "[1D Opt1 - Output Buffer Reuse]" << endl;

    vector<float> output(input.size());
    for (int i = 0; i < 5; ++i){
        MySoftmax_1D_Opt1(input, output);
    }

    vector<double> times;
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();
        MySoftmax_1D_Opt1(input, output);
        auto end = chrono::steady_clock::now();

        double elapsed_ms = chrono::duration<double, milli>(end - start).count();
        times.push_back(elapsed_ms);
    }

    sort(times.begin(), times.end());

    double median_ms = (times[9] + times[10]) / 2.0;
    cout << "Median time = " << median_ms << " ms" << endl;

    double elements_per_second = output.size() / (median_ms / 1000.0);
    cout << "Throughput = " << elements_per_second / 1e6 << " M elements/s" << endl;
    
    double ns_per_element = median_ms * 1e6 / output.size();
    cout << "Time per element = " << ns_per_element << " ns" << endl;
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << endl << endl;

    return 0;
}

int run_2d_opt1_benchmark(size_t row, size_t col, const vector<float>& input){
    cout << "[2D Opt1 - Output Buffer Reuse]" << endl;
    
    size_t size = row * col;
    vector<float> output(size);
    for (int i = 0; i < 5; ++i){
        MySoftmax_2D_Opt1(input, row, col, output);
    }

    vector<double> times;
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();
        MySoftmax_2D_Opt1(input, row, col, output);
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
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << " (expected about " << row << ")" << endl << endl;

    return 0;
}

int run_1d_opt2_benchmark(const vector<float>& input){
    cout << "[1D Opt2 - Reciprocal Multiplication]" << endl;

    vector<float> output(input.size());
    for (int i = 0; i < 5; ++i){
        MySoftmax_1D_Opt2(input, output);
    }

    vector<double> times;
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();
        MySoftmax_1D_Opt2(input, output);
        auto end = chrono::steady_clock::now();

        double elapsed_ms = chrono::duration<double, milli>(end - start).count();
        times.push_back(elapsed_ms);
    }

    sort(times.begin(), times.end());

    double median_ms = (times[9] + times[10]) / 2.0;
    cout << "Median time = " << median_ms << " ms" << endl;

    double elements_per_second = output.size() / (median_ms / 1000.0);
    cout << "Throughput = " << elements_per_second / 1e6 << " M elements/s" << endl;
    
    double ns_per_element = median_ms * 1e6 / output.size();
    cout << "Time per element = " << ns_per_element << " ns" << endl;
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << endl << endl;

    return 0;
}

int run_2d_opt2_benchmark(size_t row, size_t col, const vector<float>& input){
    cout << "[2D Opt2 - Reciprocal Multiplication]" << endl;
    
    size_t size = row * col;
    vector<float> output(size);
    for (int i = 0; i < 5; ++i){
        MySoftmax_2D_Opt2(input, row, col, output);
    }

    vector<double> times;
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();
        MySoftmax_2D_Opt2(input, row, col, output);
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
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << " (expected about " << row << ")" << endl << endl;

    return 0;
}

int run_2d_opt3_benchmark(size_t row, size_t col, const vector<float>& input){
    cout << "[2D Opt3 - OpenMP Row-Level Parallelization]" << endl;
    
    size_t size = row * col;
    vector<float> output(size);
    for (int i = 0; i < 5; ++i){
        MySoftmax_2D_Opt3(input, row, col, output);
    }

    vector<double> times;
    for (int i = 0; i < 20; ++i){
        auto start = chrono::steady_clock::now();
        MySoftmax_2D_Opt3(input, row, col, output);
        auto end = chrono::steady_clock::now();

        double elapsed_ms = chrono::duration<double, milli>(end - start).count();
        times.push_back(elapsed_ms);
    }

    sort(times.begin(), times.end());

    double median_ms = (times[9] + times[10]) / 2.0;
    cout << "Median time = " << median_ms << " ms (default threads = " << omp_get_max_threads() << ")" << endl;

    double elements_per_second = size / (median_ms / 1000.0);
    cout << "Throughput = " << elements_per_second / 1e6 << " M elements/s" << endl;
    
    double ns_per_element = median_ms * 1e6 / size;
    cout << "Time per element = " << ns_per_element << " ns" << endl;
    
    double sum = accumulate(output.begin(), output.end(), 0.0);
    cout << "Sum of the last output = " << sum << " (expected about " << row << ")" << endl << endl;

    return 0;
}

int run_2d_opt3_extra_benchmark(size_t row, size_t col, const vector<float>& input){
    cout << "[2D Opt3 Extra - Thread Scaling]" << endl;

    int original_threads = omp_get_max_threads();
    vector<float> output(row * col);

    vector<int> thread_counts = {1, 2, 4, 8};

    for(int num_threads : thread_counts){
        omp_set_num_threads(num_threads);

        for(int i = 0; i < 5; ++i){
            MySoftmax_2D_Opt3(input, row, col, output);
        }

        vector<double> times;

        for(int i = 0; i < 20; ++i){
            auto start = chrono::steady_clock::now();
            MySoftmax_2D_Opt3(input, row, col, output);
            auto end = chrono::steady_clock::now();

            double elapsed_ms = chrono::duration<double, milli > (end - start).count();
            times.push_back(elapsed_ms);
        }

        sort(times.begin(), times.end());

        double median_ms = (times[9] + times[10]) / 2.0;
        cout << num_threads << " thread(s): " << median_ms << " ms" << endl;
    }

    omp_set_num_threads(original_threads);
    return 0;
}

int main(){
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    
    mt19937 rng(seed);

    cout << "Seed = " << seed << endl << endl;
    vector<size_t> sizes = {1000, 10000, 100000, 1000000, 5000000};

    uniform_real_distribution<float> dist(-10.0f, 10.0f);

    for(size_t i = 0; i < sizes.size(); ++i){
        cout << "========================================" << endl;
        cout << "Input size = " << sizes[i] << endl;
        cout << "========================================" << endl << endl;

        vector<float> input_1d(sizes[i]);
        for (size_t i = 0; i < input_1d.size(); ++i) {
            input_1d[i] = dist(rng);
        }

        run_1d_benchmark(input_1d);
        run_1d_opt1_benchmark(input_1d);
        run_1d_opt2_benchmark(input_1d);
    }

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

        cout << "========================================" << endl;
        cout << "Input size = " << row << " * " << col << endl;
        cout << "========================================" << endl << endl;

        vector<float> input_2d(row * col);

        uniform_real_distribution<float> dist(-10.0f, 10.0f);
        for (size_t i = 0; i < input_2d.size(); ++i) {
            input_2d[i] = dist(rng);
        }

        run_2d_benchmark(row, col, input_2d);
        run_2d_opt1_benchmark(row, col, input_2d);
        run_2d_opt2_benchmark(row, col, input_2d);
        run_2d_opt3_benchmark(row, col, input_2d);
        run_2d_opt3_extra_benchmark(row, col, input_2d);
    }

    return 0;
}