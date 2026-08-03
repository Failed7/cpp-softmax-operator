#include "softmax.h"

#include <iostream> 
#include <vector>

using namespace std; 

int main(){
    vector<float> in_1 = {1000.0, 1001.0, 1002.0};
    vector<float> result_1 = MySoftmax_1D(in_1);

    for(int i = 0; i < result_1.size(); ++i){
        cout << result_1[i] << " ";
    }

    cout << endl;
    cout << endl;

    vector<float> in_2 = {
        1001.0, 1003.0, 1005.0, 
          -5.0,   -4.0,   -3.0
    };
    int row = 2; int col = 3;
    vector<float> result_2 = MySoftmax_2D(in_2, row, col);

    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            cout << result_2[i * col + j] << " ";
        }
        cout << endl;
    }

    return 0; 
}