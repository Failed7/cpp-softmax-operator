from scipy.special import softmax
from pathlib import Path
import numpy as np
import subprocess

def run_1d_case(input_1d, case_name):
    assert input_1d.ndim == 1
    length = input_1d.size

    with open("tests/input.txt", "w") as f:
        f.write("1D\n")
        f.write(f"{length}\n")
        np.savetxt(f, input_1d, fmt = "%.8f")

    subprocess.run(["build/test_driver.exe"], check = True)
    output_1d_cpp = np.loadtxt("tests/output.txt", dtype = np.float32)

    output_1d_ref = softmax(input_1d).astype(np.float32)

    max_error = np.max(np.abs(output_1d_cpp - output_1d_ref))

    print(f"{case_name}: max_error = {max_error}")
    assert max_error < 1e-5, f"{case_name} failed: max_error = {max_error}"
    print(f"{case_name} passed")

    Path("tests/input.txt").unlink(missing_ok = True)
    Path("tests/output.txt").unlink(missing_ok = True)

def run_2d_case(input_2d, case_name):
    assert input_2d.ndim == 2
    row, col = input_2d.shape

    with open("tests/input.txt", "w") as f:
        f.write("2D\n")
        f.write(f"{row} {col}\n")
        np.savetxt(f, input_2d, fmt = "%.8f")

    subprocess.run(["build/test_driver.exe"], check = True)
    output_2d_cpp = np.loadtxt("tests/output.txt", dtype = np.float32)
    assert output_2d_cpp.size == row * col
    output_2d_cpp = output_2d_cpp.reshape(row, col)

    output_2d_ref = softmax(input_2d, axis = 1).astype(np.float32)

    max_error = np.max(np.abs(output_2d_cpp - output_2d_ref))

    print(f"{case_name}: max_error = {max_error}")
    assert max_error < 1e-5, f"{case_name} failed: max_error = {max_error}"
    print(f"{case_name} passed")

    Path("tests/input.txt").unlink(missing_ok = True)
    Path("tests/output.txt").unlink(missing_ok = True)

rng = np.random.default_rng(42)
input_random_1d = rng.uniform(low = -10.0, high = 10.0, size = 5).astype(np.float32)
run_1d_case(input_random_1d, "random")

input_stability_1d = np.array([80000.0, 80001.0, 80002.0], dtype = np.float32)
run_1d_case(input_stability_1d, "numerical stability")

input_random_2d = rng.uniform(low = -10.0, high = 10.0, size=(3, 4)).astype(np.float32)
run_2d_case(input_random_2d, "row")

run_2d_case(input_random_2d.T, "column")
