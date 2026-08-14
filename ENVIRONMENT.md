# 项目环境与复现说明

本文档统一记录本项目最终版本所使用的硬件环境、软件依赖、构建方式、正确性验证方法和性能实验方法。

最终测试与性能实验日期：2026-08-14。

## 1. 硬件与操作系统

| 项目 | 配置 |
|---|---|
| CPU | AMD Ryzen 7 7435H |
| 物理核心 | 8 |
| 逻辑处理器 | 16 |
| 操作系统 | Microsoft Windows 11 家庭版中文版 |
| 系统版本 | 10.0.26200 |
| 系统架构 | 64 位 |
| 计算设备 | CPU |
| GPU | 未使用 |

本项目中的正确性结果和性能结果均在上述机器上产生。

## 2. C++ 工具链

| 项目 | 版本或配置 |
|---|---|
| C++ 标准 | C++17 |
| 编译器 | g++.exe 16.1.0，MSYS2 Rev5 |
| 编译环境 | MSYS2 UCRT64 |
| CMake | 4.4.0 |
| 构建系统 | Ninja |
| 并行运行时 | OpenMP |
| 计时工具 | `std::chrono::steady_clock` |

项目通过 `find_package(OpenMP REQUIRED)` 查找 OpenMP，并通过 `OpenMP::OpenMP_CXX` 链接 OpenMP 运行时。

## 3. Python 环境

| 项目 | 版本 |
|---|---|
| Python | 3.12.10 |
| NumPy | 2.4.6 |
| SciPy | 1.17.1 |
| Matplotlib | 3.10.9 |
| 虚拟环境 | `keras_env` |

Python 依赖的精确版本同时记录在项目根目录的 `requirements.txt` 中。

安装依赖：

    python -m pip install -r requirements.txt

其中：

- NumPy 用于输入和数组处理；
- SciPy 用作 Baseline 正确性验证的参考实现；
- Matplotlib 用于生成性能图表。

## 4. 构建配置

项目使用两个相互独立的 CMake 构建目录。

### 4.1 功能测试构建

`build` 目录用于示例程序和正确性测试。

    cmake -S . -B build -G Ninja
    cmake --build build
    ctest --test-dir build -V

该目录包含：

- `softmax_demo.exe`
- `test_driver.exe`
- `test_optimization.exe`
- CTest 测试配置

### 4.2 Release 性能构建

`build-release` 目录使用 Release 配置，仅用于正式性能测试。

    cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build-release
    .\build-release\softmax_benchmark.exe

正式 benchmark 数据均来自 `build-release` 中的可执行文件，不使用普通 `build` 目录中的 benchmark 程序。

## 5. 正确性验证

### 5.1 比较方式

- 1D 和 2D Baseline 实现逐元素与 SciPy 参考结果比较；
- Opt1、Opt2 和 Opt3 与相应的 Baseline 实现比较；
- 误差指标为最大绝对误差；
- 统一通过阈值为 `1e-5`。

### 5.2 测试覆盖

最终正确性验证包含以下14项测试：

- 1D 随机输入；
- 1D 大数值稳定性输入；
- 1D 单元素输入；
- 1D 全相同元素输入；
- 2D 按行 Softmax；
- 2D 通过转置实现的按列 Softmax；
- 2D 的 `1x1` 矩阵；
- 2D 全相同元素矩阵；
- 2D 单列矩阵；
- 1D Opt1 输出缓冲复用；
- 1D Opt2 倒数乘法；
- 2D Opt1 输出缓冲复用；
- 2D Opt2 倒数乘法；
- 2D Opt3 OpenMP 行级并行。

Python 正确性测试允许使用随机输入，并在每次运行时打印实际 seed。这样可以扩大测试输入覆盖范围，同时为失败排查保留对应的输入标识。

最终正确性测试记录的 seed 为：

| 测试 | Seed |
|---|---:|
| 1D 正确性测试 | 1786702224800992000 |
| 2D 正确性测试 | 1786702225475422300 |

CTest 将三个测试设置为串行执行，避免使用 TXT 文件完成 Python-C++ 数据交换时发生文件访问冲突。

### 5.3 最终正确性结果

| 指标 | 结果 |
|---|---:|
| CTest | 3/3 通过 |
| 独立验证场景 | 14/14 通过 |
| 最大绝对误差 | 2.9802322387695312e-08 |
| 允许误差 | 1e-5 |
| CTest 退出码 | 0 |

最大观测误差约为允许误差的0.3%，比正确性阈值小约336倍。

完整记录位于：

- `tests/results/correctness_final.txt`
- `tests/results/correctness_summary.md`

## 6. 性能实验方法

### 6.1 输入生成

| 项目 | 配置 |
|---|---|
| 最终固定 seed | 20260814 |
| 随机数生成器 | `std::mt19937` |
| 输入分布 | `[-10, 10]` 上的均匀浮点数分布 |

性能实验固定随机种子，以保证不同实现和三次完整运行使用等价的输入数据。

### 6.2 输入规模

1D 输入规模：

- 1,000；
- 10,000；
- 100,000；
- 1,000,000；
- 5,000,000 个元素。

2D 矩阵规模：

- `10 x 100`；
- `100 x 100`；
- `100 x 1000`；
- `1000 x 1000`；
- `1000 x 5000`。

### 6.3 单次运行的测量方法

每个实现和输入规模均执行以下步骤：

1. 先执行5次不计时的预热；
2. 再执行20次正式计时；
3. 对20个计时结果排序；
4. 使用排序后第10和第11个结果的平均值作为中位数。

每个主实验输出以下指标：

- 中位运行时间，单位为毫秒；
- 吞吐量，单位为百万元素每秒；
- 单元素时间，单位为纳秒；
- 最后一次输出的总和。

1D Softmax 的输出总和应约为1。

按行计算的 2D Softmax，其全部输出总和应约等于矩阵行数。

输出总和仅作为 benchmark 中的快速完整性检查，正式正确性结论仍以独立的正确性测试为准。

### 6.4 三次正式运行

最终 Release benchmark 共完整运行三次：

- `final_run_01.txt`
- `final_run_02.txt`
- `final_run_03.txt`

三次运行使用相同 seed 和相同输入，但保留操作系统调度、CPU状态和后台进程造成的真实时间波动。

每次完整运行包含55行结构化测量结果：

- 35行主实验结果；
- 20行 OpenMP 线程缩放结果。

因此：

- `benchmark_results.csv` 包含165行三次运行的明细数据；
- `benchmark_summary.csv` 包含55行最终统计结果。

最终统计包括：

- 三次运行中位数；
- 最小值；
- 最大值；
- 波动范围百分比；
- 吞吐量；
- 单元素时间；
- 相对 Baseline 加速比；
- 相对单线程 OpenMP 加速比。

## 7. OpenMP 线程缩放实验

OpenMP Extra 实验显式测试以下线程数：

- 1线程；
- 2线程；
- 4线程；
- 8线程。

主实验中 Opt3 使用系统默认的16线程。绘制线程缩放图时，将主实验中的16线程结果作为线程缩放曲线的最后一个数据点。

本机具有8个物理核心和16个逻辑处理器。因此，16线程结果使用了同时多线程，而不是16个独立物理核心。

小规模矩阵可能因为线程创建、调度和同步开销而变慢；较大矩阵才更可能从行级并行中获得加速。

## 8. 结果处理流程

### 8.1 正确性结果

处理流程为：

1. 使用 `ctest --test-dir build -V` 运行全部测试；
2. 将完整输出保存为 `correctness_final.txt`；
3. 将关键误差和通过情况整理到 `correctness_summary.md`。

原始日志用于保留证据，Markdown 文件用于阅读和 README 引用。

### 8.2 性能结果

处理流程为：

1. 保存三次完整运行产生的原始 TXT；
2. 使用 `parse_results.py` 生成 `benchmark_results.csv`；
3. 汇总三次结果，生成 `benchmark_summary.csv`；
4. 使用汇总数据生成最终性能图表。

最终图表包括：

- `serial_optimization.png`
- `openmp_crossover.png`
- `openmp_scaling.png`

所有汇总结果均可追溯到三份原始 benchmark TXT 文件。

## 9. 结果解释限制

性能结果反映的是当前机器和当前软件环境中的相对表现。绝对运行时间可能受到以下因素影响：

- CPU动态频率和加速策略；
- 系统电源模式；
- CPU温度；
- 后台程序；
- 操作系统线程调度；
- 编译器版本和优化参数；
- 内存系统状态。

因此，本项目主要比较同一最终实验环境中不同实现之间的相对差异，不将绝对时间解释为适用于所有硬件平台的通用结果。
