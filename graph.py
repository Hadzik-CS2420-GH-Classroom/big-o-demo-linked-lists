"""
graph.py - Visualize Big O benchmark results from the linked list demo.

Usage:
  1. Build and run the C++ program first (it writes results.csv)
  2. Run: python graph.py

Requires: plotly
  pip install plotly
"""

import csv
import sys
from collections import defaultdict

try:
    import plotly.graph_objects as go
except ImportError:
    print("plotly not found. Install it with: pip install plotly")
    sys.exit(1)

# ── Load results.csv ─────────────────────────────────────────────────────────

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

# ── Plot ──────────────────────────────────────────────────────────────────────

O1_COLOR = "#4CAF50"   # green - constant time
ON_COLOR = "#F44336"   # red   - linear time

O1_OPS = {"SLL push_front", "DLL push_back", "DLL pop_back"}

traces = []
for op, values in data.items():
    is_o1 = op in O1_OPS
    traces.append(go.Scatter(
        x=values["n"],
        y=values["time"],
        mode="lines+markers",
        name=op,
        line=dict(
            color=O1_COLOR if is_o1 else ON_COLOR,
            dash="solid" if is_o1 else "dash",
            width=2,
        ),
        marker=dict(size=6),
        hovertemplate="<b>%{fullData.name}</b><br>n = %{x:,}<br>time = %{y:.4f} µs<extra></extra>",
    ))

fig = go.Figure(traces)

fig.update_layout(
    title=dict(text="Linked List Operations: O(1) vs O(n)", font=dict(size=18)),
    xaxis=dict(
        title="Input Size (n)",
        tickformat=",",
        rangemode="tozero",
    ),
    yaxis=dict(
        title="Time per Operation (microseconds)",
        tickformat=".4f",
        rangemode="tozero",
    ),
    legend=dict(
        title="Operation (green = O(1), red = O(n))",
        font=dict(size=11),
    ),
    hovermode="x unified",
    template="plotly_dark",
    width=1000,
    height=600,
)

output = "results.html"
fig.write_html(output, include_plotlyjs="cdn")
print(f"Graph saved to {output} — open it in a browser.")
fig.show()
