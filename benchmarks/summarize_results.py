from collections import defaultdict
from pathlib import Path
from statistics import median
import csv


BASE_DIR = Path(__file__).resolve().parent
RESULTS_DIR = BASE_DIR / "results"
INPUT_CSV = RESULTS_DIR / "benchmark_results.csv"
OUTPUT_CSV = RESULTS_DIR / "benchmark_summary.csv"

GROUP_FIELDS = (
    "experiment",
    "dimension",
    "rows",
    "cols",
    "elements",
    "implementation",
    "threads",
)

IMPLEMENTATION_ORDER = {
    "Baseline": 0,
    "Opt1": 1,
    "Opt2": 2,
    "Opt3": 3,
}


def clean_number(value, digits=8):
    return f"{value:.{digits}g}"


with INPUT_CSV.open(newline="", encoding="utf-8-sig") as input_file:
    raw_rows = list(csv.DictReader(input_file))

if len(raw_rows) != 165:
    raise RuntimeError(f"Expected 165 raw rows, found {len(raw_rows)}")

groups = defaultdict(list)
for row in raw_rows:
    key = tuple(row[field] for field in GROUP_FIELDS)
    groups[key].append(row)

if len(groups) != 55:
    raise RuntimeError(f"Expected 55 result groups, found {len(groups)}")

summary_rows = []
for key, group_rows in groups.items():
    if len(group_rows) != 3:
        raise RuntimeError(f"Expected 3 runs for {key}, found {len(group_rows)}")

    times = [float(row["median_ms"]) for row in group_rows]
    median_ms = median(times)
    elements = int(group_rows[0]["elements"])
    seeds = sorted({row["seed"] for row in group_rows})
    output_sums = sorted({row["output_sum"] for row in group_rows if row["output_sum"]})

    if len(seeds) != 1:
        raise RuntimeError(f"Multiple seeds found for {key}: {seeds}")
    if len(output_sums) > 1:
        raise RuntimeError(f"Different output sums found for {key}: {output_sums}")

    summary_rows.append({
        "experiment": key[0],
        "dimension": key[1],
        "rows": key[2],
        "cols": key[3],
        "elements": elements,
        "implementation": key[5],
        "threads": key[6],
        "seed": seeds[0],
        "run_count": len(group_rows),
        "median_ms": median_ms,
        "min_ms": min(times),
        "max_ms": max(times),
        "range_percent": (max(times) - min(times)) / median_ms * 100.0,
        "throughput_mel_s": elements / (median_ms * 1000.0),
        "ns_per_element": median_ms * 1e6 / elements,
        "speedup_vs_baseline": "",
        "speedup_vs_one_thread": "",
        "output_sum": output_sums[0] if output_sums else "",
        "expected_sum": group_rows[0]["expected_sum"],
    })

main_baselines = {}
thread_baselines = {}

for row in summary_rows:
    shape_key = (row["dimension"], row["rows"], row["cols"], row["elements"])
    if row["experiment"] == "main" and row["implementation"] == "Baseline":
        main_baselines[shape_key] = row["median_ms"]
    if row["experiment"] == "thread_scaling" and row["threads"] == "1":
        thread_baselines[shape_key] = row["median_ms"]

for row in summary_rows:
    shape_key = (row["dimension"], row["rows"], row["cols"], row["elements"])
    if row["experiment"] == "main":
        row["speedup_vs_baseline"] = main_baselines[shape_key] / row["median_ms"]
    else:
        row["speedup_vs_one_thread"] = thread_baselines[shape_key] / row["median_ms"]


def sort_key(row):
    return (
        0 if row["experiment"] == "main" else 1,
        0 if row["dimension"] == "1D" else 1,
        row["elements"],
        int(row["rows"] or 0),
        int(row["cols"] or 0),
        IMPLEMENTATION_ORDER[row["implementation"]],
        int(row["threads"] or 0),
    )


summary_rows.sort(key=sort_key)

numeric_fields = (
    "median_ms",
    "min_ms",
    "max_ms",
    "throughput_mel_s",
    "ns_per_element",
    "speedup_vs_baseline",
    "speedup_vs_one_thread",
)

for row in summary_rows:
    row["range_percent"] = f"{row['range_percent']:.2f}"
    for field in numeric_fields:
        if row[field] != "":
            row[field] = clean_number(row[field])

fieldnames = list(summary_rows[0].keys())
with OUTPUT_CSV.open("w", newline="", encoding="utf-8") as output_file:
    writer = csv.DictWriter(output_file, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(summary_rows)

print(f"Read {len(raw_rows)} raw rows and created {len(summary_rows)} summary rows")
print(f"Created: {OUTPUT_CSV}")
