from pathlib import Path
import csv
import re


BASE_DIR = Path(__file__).resolve().parent
RAW_DIR = BASE_DIR / "results" / "raw"
OUTPUT_CSV = BASE_DIR / "results" / "benchmark_results.csv"

INPUT_PATTERN = re.compile(r"Input size =\s*(\d+)(?:\s*\*\s*(\d+))?")
MAIN_PATTERN = re.compile(
    r"\[(1D|2D) (Baseline|Opt1|Opt2|Opt3)[^\]]*\]\s*"
    r"Median time = ([\d.]+) ms(?: \(default threads = (\d+)\))?\s*"
    r"Throughput = ([\d.]+) M elements/s\s*"
    r"Time per element = ([\d.]+) ns\s*"
    r"Sum of the last output = ([\d.eE+-]+)"
)
THREAD_PATTERN = re.compile(r"(\d+) thread\(s\): ([\d.]+) ms")


def parse_file(path):
    text = path.read_text(encoding="utf-8-sig")
    seed = int(re.search(r"Seed = (\d+)", text).group(1))
    run_id = int(re.search(r"final_run_(\d+)", path.stem).group(1))
    input_matches = list(INPUT_PATTERN.finditer(text))
    result_rows = []

    for index, input_match in enumerate(input_matches):
        first = int(input_match.group(1))
        second = input_match.group(2)
        section_end = (
            input_matches[index + 1].start()
            if index + 1 < len(input_matches)
            else len(text)
        )
        section = text[input_match.end():section_end]

        if second is None:
            dimension, row_count, col_count, elements = "1D", "", "", first
        else:
            dimension = "2D"
            row_count, col_count = first, int(second)
            elements = row_count * col_count

        expected_sum = 1 if dimension == "1D" else row_count

        for match in MAIN_PATTERN.finditer(section):
            result_rows.append({
                "run_id": run_id,
                "seed": seed,
                "source_log": path.name,
                "experiment": "main",
                "dimension": dimension,
                "rows": row_count,
                "cols": col_count,
                "elements": elements,
                "implementation": match.group(2),
                "threads": match.group(4) or "",
                "median_ms": match.group(3),
                "throughput_mel_s": match.group(5),
                "ns_per_element": match.group(6),
                "output_sum": match.group(7),
                "expected_sum": expected_sum,
            })

        if dimension == "2D":
            extra_start = section.find("[2D Opt3 Extra - Thread Scaling]")
            if extra_start != -1:
                extra_section = section[extra_start:]
                for threads, median_ms in THREAD_PATTERN.findall(extra_section):
                    median_ms = float(median_ms)
                    result_rows.append({
                        "run_id": run_id,
                        "seed": seed,
                        "source_log": path.name,
                        "experiment": "thread_scaling",
                        "dimension": dimension,
                        "rows": row_count,
                        "cols": col_count,
                        "elements": elements,
                        "implementation": "Opt3",
                        "threads": threads,
                        "median_ms": median_ms,
                        "throughput_mel_s": elements / (median_ms * 1000.0),
                        "ns_per_element": median_ms * 1e6 / elements,
                        "output_sum": "",
                        "expected_sum": expected_sum,
                    })

    if len(result_rows) != 55:
        raise RuntimeError(
            f"{path.name}: expected 55 rows, parsed {len(result_rows)}"
        )

    return result_rows


input_files = sorted(RAW_DIR.glob("final_run_*.txt"))
if len(input_files) != 3:
    raise RuntimeError(f"Expected 3 raw files, found {len(input_files)}")

all_rows = []
for input_file in input_files:
    all_rows.extend(parse_file(input_file))

fieldnames = list(all_rows[0].keys())
with OUTPUT_CSV.open("w", newline="", encoding="utf-8") as output_file:
    writer = csv.DictWriter(output_file, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(all_rows)

print(f"Parsed {len(input_files)} files and {len(all_rows)} rows")
print(f"Created: {OUTPUT_CSV}")
