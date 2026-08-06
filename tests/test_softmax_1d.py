from scipy.special import softmax
from pathlib import Path
import numpy as np
import subprocess
import time

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
    print()

    Path("tests/input.txt").unlink(missing_ok = True)
    Path("tests/output.txt").unlink(missing_ok = True)

seed = time.time_ns()
print(f"1D seed = {seed}")
print()
rng = np.random.default_rng(seed)

input_random_1d = rng.uniform(low = -10.0, high = 10.0, size = 5).astype(np.float32)
run_1d_case(input_random_1d, "random")

input_stability_1d = np.array([80000.0, 80001.0, 80002.0], dtype = np.float32)
run_1d_case(input_stability_1d, "numerical stability")

run_1d_case(np.array([5.0], dtype=np.float32), "single element")
run_1d_case(np.full(5, 3.0, dtype=np.float32), "all equal")