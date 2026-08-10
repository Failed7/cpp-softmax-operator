#pragma once

#include <vector>

std::vector<float> MySoftmax_1D(const std::vector<float>& nums);
std::vector<float> MySoftmax_2D(const std::vector<float>& nums, int row, int col);

void MySoftmax_1D_Opt1(const std::vector<float>& nums, std::vector<float>& output);
void MySoftmax_2D_Opt1(const std::vector<float>& nums, int row, int col, std::vector<float>& output);