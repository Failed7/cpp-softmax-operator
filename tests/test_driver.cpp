#include "softmax.h"

#include <iostream> 
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std; 

int main(){
    vector<float> input;
    ifstream input_file("tests/input.txt");

    if(!input_file.is_open()) {
        return 1;
    }

    float value;

    while(input_file >> value){
        input.push_back(value);
    }

    vector<float> output = MySoftmax_1D(input);

    ofstream output_file("tests/output.txt");
    output_file << scientific << setprecision(16);
    for(int i = 0; i < output.size(); ++i){
        output_file << output[i] << '\n';
    }

    return 0;
}