# Big O Demo: Linked Lists

**Not graded** — this is a runnable demonstration, not an assignment.

## What This Shows

Times linked list operations at increasing input sizes to demonstrate Big O growth patterns:

| Operation | SLL | DLL | Why |
|-----------|-----|-----|-----|
| `push_front` | O(1) | O(1) | Direct pointer update |
| `push_back` | O(n) | O(1) | SLL traverses to end; DLL has `tail_` |
| `pop_front` | O(1) | O(1) | Direct pointer update |
| `pop_back` | O(n) | O(1) | SLL trailing pointer; DLL has `prev_` |
| `contains` | O(n) | O(n) | Must scan the list |

## How to Run

```bash
cmake -B build
cmake --build build
./build/big-o-demo-linked-lists
```

The output shows timing tables so you can see how O(1) operations stay flat while O(n) operations grow linearly with input size.
