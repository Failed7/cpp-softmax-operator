#include "softmax.h"

#include <iostream> 
#include <vector>

using namespace std; 

int main(){
    vector<float> in = {1000.0, 1001.0, 1002.0};
    vector<float> result = MySoftmax_1D(in);
    for(int i = 0; i < result.size(); ++i){
        cout << result[i] << " ";
    }
    return 0; 
}