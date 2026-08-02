# C++ Softmax 算子实现

这是一个面向 AI Infra 学习的小型实践项目，目标是使用 C++ 在 CPU 上实现一个具有数值稳定性的 Softmax 算子，并完成正确性验证与基础性能测试。

**项目开始时间：** 2026 年 8 月 1 日  
**第一版目标日期：** 2026 年 8 月 14 日

## 项目目标

本项目不会尝试在短时间内覆盖完整的 AI Infra 技术栈，而是从一个规模较小、计算过程明确的算子开始，练习以下内容：

- 使用 C++ 实现基础张量计算
- 理解 Softmax 的计算过程与数值稳定性
- 使用 NumPy 或 PyTorch 验证计算结果
- 测试不同输入规模下的运行时间
- 尝试进行简单的 CPU 性能优化
- 熟悉 CMake 与基础 C++ 工程结构

## 第一阶段范围

第一版计划实现：

- CPU 版本 Softmax
- FP32 浮点数输入
- 一维及二维输入
- 普通版本与数值稳定版本
- 与 NumPy 或 PyTorch 的结果对比
- 多种输入规模下的运行时间测试
- 基础性能结果记录

## Softmax

Softmax 的基本计算公式为：

\[
y_i = \frac{e^{x_i}}{\sum_j e^{x_j}}
\]

直接计算指数可能出现数值溢出，因此本项目将使用减去输入最大值的稳定形式：

\[
y_i = \frac{e^{x_i-\max(x)}}{\sum_j e^{x_j-\max(x)}}
\]

## 计划中的项目结构

```text
cpp-softmax-operator/
├── README.md
├── CMakeLists.txt
├── include/
│   └── softmax.h
├── src/
│   ├── softmax.cpp
│   └── main.cpp
├── tests/
│   └── test_softmax.py
├── benchmarks/
│   └── benchmark.py
└── results/
    └── benchmark_results.csv
