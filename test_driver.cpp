#include "softmax.h"

#include <iostream> 
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std; 

int run_1d_test(ifstream& input_stream){
    vector<float> input;
    int len;

    if(!(input_stream >> len) || len < 0){
        return 3;
    }

    float value;
    input.reserve(len);

    for(int i = 0; i < len; ++i){
        if(!(input_stream >> value)){
            return 4;
        }
        input.push_back(value);
    }

    vector<float> output = MySoftmax_1D(input);

    ofstream output_file("tests/output.txt");

    if(!output_file.is_open()){
        return 5;
    }

    output_file << scientific << setprecision(16);
    for(int i = 0; i < output.size(); ++i){
        output_file << output[i] << '\n';
    }

    return 0;
}

int run_2d_test(ifstream& input_stream){
    vector<float> input;
    int row, col;

    if(!(input_stream >> row >> col) || row <= 0 || col <= 0){
        return 3;
    }

    float value;
    input.reserve(row * col);

    for(int i = 0; i < row * col; ++i){
        if(!(input_stream >> value)){
            return 4;
        }
        input.push_back(value);
    }

    vector<float> output = MySoftmax_2D(input, row, col);

    ofstream output_file("tests/output.txt");

    if(!output_file.is_open()){
        return 5;
    }

    output_file << scientific << setprecision(16);
    for(int i = 0; i < output.size(); ++i){
        output_file << output[i] << '\n';
    }

    return 0;
}

int main(){
    string md;

    ifstream input_file("tests/input.txt");

    if(!input_file.is_open()){
        return 1;
    }

    if(!(input_file >> md) || (md != "1D" && md != "2D")){ //后续改不是1D与2D
        return 2;
    }

    if(md == "1D"){
        return run_1d_test(input_file);
    }
    else if(md == "2D"){
        return run_2d_test(input_file);
    }
    
    
}