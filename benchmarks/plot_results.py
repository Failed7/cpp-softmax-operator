from pathlib import Path
import csv

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


BASE_DIR = Path(__file__).resolve().parent
SUMMARY_CSV = BASE_DIR / "results" / "benchmark_summary.csv"
PLOTS_DIR = BASE_DIR / "plots"

COLORS = {
    "Baseline": "#4C78A8",
    "Opt1": "#F58518",
    "Opt2": "#54A24B",
    "Opt3": "#E45756",
}


def load_summary():
    with SUMMARY_CSV.open(newline="", encoding="utf-8-sig") as input_file:
        rows = list(csv.DictReader(input_file))

    if len(rows) != 55:
        raise RuntimeError(f"Expected 55 summary rows, found {len(rows)}")

    for row in rows:
        row["elements"] = int(row["elements"])
        row["median_ms"] = float(row["median_ms"])
        row["ns_per_element"] = float(row["ns_per_element"])
        if row["rows"]:
            row["rows"] = int(row["rows"])
        if row["cols"]:
            row["cols"] = int(row["cols"])
        if row["threads"]:
            row["threads"] = int(row["threads"])

    return rows


def element_label(value, _position=None):
    labels = {
        1_000: "1K",
        10_000: "10K",
        100_000: "100K",
        1_000_000: "1M",
        5_000_000: "5M",
    }
    return labels.get(int(value), f"{value:g}")


def plot_serial_optimization(rows):
    fig, axes = plt.subplots(1, 2, figsize=(12, 4.8), sharey=True)

    for axis, dimension in zip(axes, ("1D", "2D")):
        for implementation in ("Baseline", "Opt1", "Opt2"):
            selected = sorted(
                (
                    row for row in rows
                    if row["experiment"] == "main"
                    and row["dimension"] == dimension
                    and row["implementation"] == implementation
                ),
                key=lambda row: row["elements"],
            )
            axis.plot(
                [row["elements"] for row in selected],
                [row["ns_per_element"] for row in selected],
                marker="o",
                linewidth=2,
                markersize=6,
                label=implementation,
                color=COLORS[implementation],
            )

        axis.set_xscale("log")
        axis.set_xticks([1_000, 10_000, 100_000, 1_000_000, 5_000_000])
        axis.xaxis.set_major_formatter(FuncFormatter(element_label))
        axis.set_xlabel("Number of elements")
        axis.set_title(f"{dimension} serial implementations")
        axis.grid(True, which="major", alpha=0.25)
        axis.legend(frameon=False)

    axes[0].set_ylabel("Time per element (ns)")
    fig.suptitle("Serial Softmax Optimization Across Input Sizes", fontsize=14)
    fig.text(
        0.5,
        0.01,
        "Each point is the median of three benchmark runs; each run reports the median of 20 measurements.",
        ha="center",
        fontsize=9,
        color="#555555",
    )
    fig.tight_layout(rect=(0, 0.05, 1, 0.93))
    fig.savefig(PLOTS_DIR / "serial_optimization.png", dpi=200, bbox_inches="tight")
    plt.close(fig)


def plot_openmp_crossover(rows):
    main_2d = [
        row for row in rows
        if row["experiment"] == "main" and row["dimension"] == "2D"
    ]
    shape_data = {}

    for row in main_2d:
        shape = (row["rows"], row["cols"], row["elements"])
        shape_data.setdefault(shape, {})[row["implementation"]] = row["median_ms"]

    ordered_shapes = sorted(shape_data, key=lambda shape: shape[2])
    labels = [f"{row_count}×{col_count}" for row_count, col_count, _ in ordered_shapes]
    speedups = [
        shape_data[shape]["Opt2"] / shape_data[shape]["Opt3"]
        for shape in ordered_shapes
    ]
    bar_colors = ["#D95F5F" if speedup < 1.0 else "#4C78A8" for speedup in speedups]

    fig, axis = plt.subplots(figsize=(9, 5))
    bars = axis.bar(labels, speedups, color=bar_colors, width=0.68)
    axis.set_yscale("log")
    axis.axhline(1.0, color="#333333", linestyle="--", linewidth=1.3)
    axis.set_xlabel("Matrix shape")
    axis.set_ylabel("Speedup over Opt2 (×, log scale)")
    axis.set_title("When Row-Level OpenMP Parallelization Becomes Beneficial")
    axis.grid(True, axis="y", which="both", alpha=0.25)

    for bar, speedup in zip(bars, speedups):
        axis.annotate(
            f"{speedup:.2f}×",
            xy=(bar.get_x() + bar.get_width() / 2, speedup),
            xytext=(0, 5),
            textcoords="offset points",
            ha="center",
            va="bottom",
            fontsize=9,
        )

    fig.text(
        0.5,
        0.01,
        "Values below 1× indicate that OpenMP overhead exceeds the parallel speedup.",
        ha="center",
        fontsize=9,
        color="#555555",
    )
    fig.tight_layout(rect=(0, 0.05, 1, 1))
    fig.savefig(PLOTS_DIR / "openmp_crossover.png", dpi=200, bbox_inches="tight")
    plt.close(fig)


def plot_openmp_scaling(rows):
    target_shapes = ((1000, 1000), (1000, 5000))
    thread_counts = [1, 2, 4, 8, 16]

    fig, axis = plt.subplots(figsize=(9, 5.2))

    for index, (row_count, col_count) in enumerate(target_shapes):
        scaling_rows = {
            row["threads"]: row["median_ms"]
            for row in rows
            if row["experiment"] == "thread_scaling"
            and row["rows"] == row_count
            and row["cols"] == col_count
        }
        main_opt3 = next(
            row for row in rows
            if row["experiment"] == "main"
            and row["implementation"] == "Opt3"
            and row["rows"] == row_count
            and row["cols"] == col_count
        )
        scaling_rows[16] = main_opt3["median_ms"]
        one_thread_time = scaling_rows[1]
        speedups = [one_thread_time / scaling_rows[threads] for threads in thread_counts]

        axis.plot(
            thread_counts,
            speedups,
            marker="o",
            linewidth=2.2,
            markersize=6,
            label=f"{row_count}×{col_count}",
            color=("#4C78A8", "#E45756")[index],
        )

    axis.plot(
        thread_counts,
        thread_counts,
        linestyle="--",
        linewidth=1.5,
        color="#777777",
        label="Ideal linear scaling",
    )
    axis.set_xticks(thread_counts)
    axis.set_xlim(0.5, 16.5)
    axis.set_ylim(0, 16.8)
    axis.set_xlabel("OpenMP threads")
    axis.set_ylabel("Speedup over 1 thread (×)")
    axis.set_title("OpenMP Thread Scaling on Large 2D Inputs")
    axis.grid(True, alpha=0.25)
    axis.legend(frameon=False)
    fig.text(
        0.5,
        0.01,
        "The 16-thread point uses the default-thread Opt3 measurement from the main benchmark.",
        ha="center",
        fontsize=9,
        color="#555555",
    )
    fig.tight_layout(rect=(0, 0.05, 1, 1))
    fig.savefig(PLOTS_DIR / "openmp_scaling.png", dpi=200, bbox_inches="tight")
    plt.close(fig)


PLOTS_DIR.mkdir(parents=True, exist_ok=True)
summary_rows = load_summary()
plot_serial_optimization(summary_rows)
plot_openmp_crossover(summary_rows)
plot_openmp_scaling(summary_rows)

print("Created 3 plots:")
for filename in (
    "serial_optimization.png",
    "openmp_crossover.png",
    "openmp_scaling.png",
):
    print(f"- {PLOTS_DIR / filename}")
