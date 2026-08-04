from scipy.special import softmax
from pathlib import Path
import numpy as np
import subprocess

rng = np.random.default_rng(42)

input_1d = rng.uniform(low = -10.0, high = 10.0, size = 5).astype(np.float32)

np.savetxt("tests/input.txt", input_1d, fmt = "%.8f")

subprocess.run(["build/test_driver.exe"], check = True)
output_1d_cpp = np.loadtxt("tests/output.txt", dtype = np.float32)

output_1d_ref = softmax(input_1d).astype(np.float32)

max_error = np.max(np.abs(output_1d_cpp - output_1d_ref))

print(max_error)

assert max_error < 1e-5, f"Test failed: max_error = {max_error}"
print("1D Softmax test passed")

Path("tests/input.txt").unlink(missing_ok = True)
Path("tests/output.txt").unlink(missing_ok = True)