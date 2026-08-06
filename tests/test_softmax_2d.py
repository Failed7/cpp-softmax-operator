from scipy.special import softmax
from pathlib import Path
import numpy as np
import subprocess
import time

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
    print()

    Path("tests/input.txt").unlink(missing_ok = True)
    Path("tests/output.txt").unlink(missing_ok = True)

seed = time.time_ns()
print(f"2D seed = {seed}")
print()
rng = np.random.default_rng(seed)

input_random_2d = rng.uniform(low = -10.0, high = 10.0, size=(3, 4)).astype(np.float32)
run_2d_case(input_random_2d, "row-wise")

run_2d_case(input_random_2d.T, "column-wise through transpose")
