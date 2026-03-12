"""
graph.py - Visualize Big O benchmark results from the linked list demo.

Usage:
  1. Build and run the C++ program first (it writes results.csv)
  2. Run: python graph.py

Requires: matplotlib
  pip install matplotlib
"""

import csv
import sys
from collections import defaultdict

try:
    import matplotlib.pyplot as plt
    import matplotlib.ticker as ticker
except ImportError:
    print("matplotlib not found. Install it with: pip install matplotlib")
    sys.exit(1)

# ── Load results.csv ────────────────────────────────────────────────────────

data = defaultdict(lambda: {"n": [], "time": []})

try:
    with open("results.csv", newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            op = row["operation"]
            data[op]["n"].append(int(row["n"]))
            data[op]["time"].append(float(row["time_us"]))
except FileNotFoundError:
    print("results.csv not found. Build and run the C++ program first.")
    sys.exit(1)

# ── Plot ─────────────────────────────────────────────────────────────────────

O1_COLOR = "#4CAF50"   # green  — constant time
ON_COLOR = "#F44336"   # red    — linear time

O1_OPS = {"SLL push_front", "DLL push_back", "DLL pop_back"}

fig, ax = plt.subplots(figsize=(10, 6))

for op, values in data.items():
    color = O1_COLOR if op in O1_OPS else ON_COLOR
    linestyle = "-" if op in O1_OPS else "--"
    ax.plot(values["n"], values["time"], marker="o", label=op,
            color=color, linestyle=linestyle, linewidth=2, markersize=6)

# ── Styling ──────────────────────────────────────────────────────────────────

ax.set_title("Linked List Operations: O(1) vs O(n)", fontsize=15, fontweight="bold", pad=15)
ax.set_xlabel("Input Size (n)", fontsize=12)
ax.set_ylabel("Time per Operation (microseconds)", fontsize=12)

ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda y, _: f"{y:.3f}"))

ax.grid(True, linestyle="--", alpha=0.4)
ax.set_xlim(left=0)
ax.set_ylim(bottom=0)

# Legend with O-class labels
from matplotlib.lines import Line2D
legend_handles = [
    Line2D([0], [0], color=O1_COLOR, linewidth=2, linestyle="-",  label="O(1) — constant time"),
    Line2D([0], [0], color=ON_COLOR,  linewidth=2, linestyle="--", label="O(n) — linear time"),
]
for op, values in data.items():
    color = O1_COLOR if op in O1_OPS else ON_COLOR
    ls = "-" if op in O1_OPS else "--"
    legend_handles.append(Line2D([0], [0], color=color, linewidth=1.5, linestyle=ls,
                                  marker="o", markersize=5, label=f"  {op}"))

ax.legend(handles=legend_handles, fontsize=9, loc="upper left")

plt.tight_layout()
plt.savefig("results.png", dpi=150)
print("Graph saved to results.png")
plt.show()
