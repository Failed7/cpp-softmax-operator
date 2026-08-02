# C++ Softmax 算子实现

这是一个面向 AI Infra 入门的小型实践项目。

本项目计划使用 C++ 在 CPU 上实现 Softmax 算子，重点练习基础 C++ 工程开发、数值稳定性处理、正确性验证和性能测试。

- **项目开始时间：** 2026 年 8 月 1 日
- **第一版目标日期：** 2026 年 8 月 14 日
- **当前状态：** 开发中

## 项目目标

第一阶段计划完成：

- 使用 C++ 实现基础 Softmax
- 实现具有数值稳定性的 Softmax
- 支持一维和二维 FP32 输入
- 使用 NumPy 或 PyTorch 验证计算结果
- 测试不同输入规模下的运行时间
- 尝试一种简单的 CPU 性能优化
- 使用 CMake 管理项目构建

## Softmax 计算

Softmax 的基本公式为：

$$
y_i = \frac{e^{x_i}}{\sum_j e^{x_j}}
$$

直接计算指数时可能出现数值溢出，因此实际实现将使用以下形式：

$$
y_i = \frac{e^{x_i-\max(x)}}{\sum_j e^{x_j-\max(x)}}
$$

## 计划中的项目结构

```text
cpp-softmax-operator/
├── README.md
├── CMakeLists.txt
├── include/
│   └── softmax.h
├── src/
│   ├── main.cpp
│   └── softmax.cpp
├── tests/
│   └── test_softmax.py
└── benchmarks/
    └── benchmark.py
```

项目目录会随着开发进度逐步建立。

## 开发进度

- [x] 创建 GitHub 仓库
- [x] 确定第一阶段项目范围
- [ ] 建立基础 C++ 与 CMake 工程
- [ ] 实现普通 Softmax
- [ ] 实现数值稳定版 Softmax
- [ ] 编写正确性测试
- [ ] 编写性能测试
- [ ] 尝试基础 CPU 优化
- [ ] 整理测试结果与项目文档

## 第一阶段暂不涉及

为了控制项目规模，第一版暂不包括：

- CUDA
- Ascend C
- GPU 或 NPU 算子开发
- 分布式计算
- 深度学习框架底层集成
- 其他复杂 AI 算子

后续是否扩展，将根据第一阶段的完成情况决定。

## 许可证

本项目使用 MIT License。
