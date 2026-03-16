// ============================================================================
// Big O Demo: Linked Lists
// ============================================================================
// This demo times linked list operations at increasing input sizes so you can
// SEE the difference between O(1) and O(n) growth patterns.
//
// Not graded — run it, read the output, and observe the patterns.
// ============================================================================

#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

// ── Simple Node ─────────────────────────────────────────────────────────────

struct Node {
    int data;
    Node* next;
    Node(int value, Node* next = nullptr) : data{value}, next{next} {}
};

// ── Singly Linked List (minimal, for benchmarking) ──────────────────────────

class SinglyLinkedList {
public:
    SinglyLinkedList() = default;

    ~SinglyLinkedList() {
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    // O(1) — just update the head pointer
    void push_front(int value) {
        head_ = new Node(value, head_);
        size_++;
    }

    // O(n) — must walk to the end every time
    void push_back(int value) {
        Node* new_node = new Node(value);
        if (!head_) {
            head_ = new_node;
        } else {
            Node* current = head_;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        size_++;
    }

    // O(1) — just update the head pointer
    void pop_front() {
        if (!head_) return;
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
        size_--;
    }

    // O(n) — must walk to the second-to-last node
    void pop_back() {
        if (!head_) return;
        if (!head_->next) {
            delete head_;
            head_ = nullptr;
            size_--;
            return;
        }
        Node* previous = head_;
        Node* current = head_->next;
        while (current->next) {
            previous = current;
            current = current->next;
        }
        previous->next = nullptr;
        delete current;
        size_--;
    }

    // O(n) — must scan up to every node
    bool contains(int value) const {
        Node* current = head_;
        while (current) {
            if (current->data == value) return true;
            current = current->next;
        }
        return false;
    }

    int get_size() const { return size_; }

private:
    Node* head_ = nullptr;
    int size_ = 0;
};

// ── Doubly Linked List (minimal, for benchmarking) ──────────────────────────

struct DoublyNode {
    int data;
    DoublyNode* next;
    DoublyNode* prev;
    DoublyNode(int value, DoublyNode* next = nullptr, DoublyNode* prev = nullptr)
        : data{value}, next{next}, prev{prev} {}
};

class DoublyLinkedList {
public:
    DoublyLinkedList() = default;

    ~DoublyLinkedList() {
        while (head_) {
            DoublyNode* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    // O(1) — direct pointer update
    void push_front(int value) {
        DoublyNode* new_node = new DoublyNode(value, head_);
        if (head_) head_->prev = new_node;
        head_ = new_node;
        if (!tail_) tail_ = new_node;
        size_++;
    }

    // O(1) — jump straight to tail_
    void push_back(int value) {
        DoublyNode* new_node = new DoublyNode(value, nullptr, tail_);
        if (tail_) tail_->next = new_node;
        tail_ = new_node;
        if (!head_) head_ = new_node;
        size_++;
    }

    // O(1) — direct pointer update
    void pop_front() {
        if (!head_) return;
        DoublyNode* temp = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr;
        else tail_ = nullptr;
        delete temp;
        size_--;
    }

    // O(1) — retreat via tail_->prev
    void pop_back() {
        if (!tail_) return;
        DoublyNode* temp = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr;
        else head_ = nullptr;
        delete temp;
        size_--;
    }

    int get_size() const { return size_; }

private:
    DoublyNode* head_ = nullptr;
    DoublyNode* tail_ = nullptr;
    int size_ = 0;
};

// ── Benchmark Utilities ─────────────────────────────────────────────────────

using Clock = std::chrono::high_resolution_clock;

// Returns elapsed time in microseconds
template <typename Func>
double time_us(Func&& func) {
    auto start = Clock::now();
    func();
    auto end = Clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

void print_header(const std::string& title) {
    std::cout << "\n--- " << title << " ---\n";
    std::cout << std::setw(12) << "n"
              << std::setw(15) << "time (us)"
              << std::setw(15) << "growth" << "\n";
    std::cout << std::string(42, '-') << "\n";
}

void print_row(int n, double time_us, double prev_time_us) {
    std::cout << std::setw(12) << n
              << std::setw(15) << std::fixed << std::setprecision(1) << time_us;
    if (prev_time_us > 0) {
        std::cout << std::setw(12) << std::setprecision(1) << (time_us / prev_time_us) << "x";
    }
    std::cout << "\n";
}

// ── Benchmark Result (for CSV export) ────────────────────────────────────────

struct BenchResult {
    std::string operation;
    std::string structure;
    std::string complexity;
    int n;
    double time_us;
};

// ── Main ────────────────────────────────────────────────────────────────────

int main() {
    std::cout << "============================================================\n";
    std::cout << "  Big O Demo: Linked Lists\n";
    std::cout << "============================================================\n";
    std::cout << "\nThis demo times linked list operations at increasing sizes.\n";
    std::cout << "Watch the 'growth' column:\n";
    std::cout << "  - O(1) operations: growth stays near 1x (constant)\n";
    std::cout << "  - O(n) operations: growth matches the size multiplier\n";

    std::vector<int> sizes = {1000, 5000, 10000, 50000};
    std::vector<BenchResult> results;

    // ── push_front: O(1) on both SLL and DLL ────────────────────────────

    print_header("SLL push_front - O(1)");
    double prev = 0;
    for (int n : sizes) {
        SinglyLinkedList list;
        double t = time_us([&]() {
            for (int i = 0; i < n; i++) list.push_front(i);
        });
        double per_op = t / n;
        print_row(n, per_op, prev);
        results.push_back({"push_front", "SLL", "O(1)", n, per_op});
        prev = per_op;
    }

    // ── push_back: O(n) on SLL, O(1) on DLL ────────────────────────────

    print_header("SLL push_back - O(n)  [THIS IS THE SLOW ONE]");
    prev = 0;
    for (int n : sizes) {
        SinglyLinkedList list;
        double t = time_us([&]() {
            for (int i = 0; i < n; i++) list.push_back(i);
        });
        double per_op = t / n;
        print_row(n, per_op, prev);
        results.push_back({"push_back", "SLL", "O(n)", n, per_op});
        prev = per_op;
    }

    print_header("DLL push_back - O(1)  [SAME OPERATION, DIFFERENT STRUCTURE]");
    prev = 0;
    for (int n : sizes) {
        DoublyLinkedList list;
        double t = time_us([&]() {
            for (int i = 0; i < n; i++) list.push_back(i);
        });
        double per_op = t / n;
        print_row(n, per_op, prev);
        results.push_back({"push_back", "DLL", "O(1)", n, per_op});
        prev = per_op;
    }

    // ── pop_back: O(n) on SLL, O(1) on DLL ──────────────────────────────

    print_header("SLL pop_back - O(n)");
    prev = 0;
    for (int n : sizes) {
        SinglyLinkedList list;
        for (int i = 0; i < n; i++) list.push_front(i);
        double t = time_us([&]() {
            for (int i = 0; i < n; i++) list.pop_back();
        });
        double per_op = t / n;
        print_row(n, per_op, prev);
        results.push_back({"pop_back", "SLL", "O(n)", n, per_op});
        prev = per_op;
    }

    print_header("DLL pop_back - O(1)");
    prev = 0;
    for (int n : sizes) {
        DoublyLinkedList list;
        for (int i = 0; i < n; i++) list.push_front(i);
        double t = time_us([&]() {
            for (int i = 0; i < n; i++) list.pop_back();
        });
        double per_op = t / n;
        print_row(n, per_op, prev);
        results.push_back({"pop_back", "DLL", "O(1)", n, per_op});
        prev = per_op;
    }

    // ── contains: O(n) — searching for a value not in the list ──────────

    print_header("SLL contains (worst case) - O(n)");
    prev = 0;
    for (int n : sizes) {
        SinglyLinkedList list;
        for (int i = 0; i < n; i++) list.push_front(i);
        double t = time_us([&]() {
            list.contains(-1);  // not in the list — must scan all n nodes
        });
        print_row(n, t, prev);
        results.push_back({"contains", "SLL", "O(n)", n, t});
        prev = t;
    }

    // ── Summary ─────────────────────────────────────────────────────────

    std::cout << "\n============================================================\n";
    std::cout << "  Summary: Linked List Big O\n";
    std::cout << "============================================================\n";
    std::cout << "\n";
    std::cout << "  Operation      | SLL    | DLL    | Why\n";
    std::cout << "  ---------------|--------|--------|---------------------------\n";
    std::cout << "  push_front     | O(1)   | O(1)   | Direct pointer update\n";
    std::cout << "  push_back      | O(n)   | O(1)   | SLL walks; DLL has tail_\n";
    std::cout << "  pop_front      | O(1)   | O(1)   | Direct pointer update\n";
    std::cout << "  pop_back       | O(n)   | O(1)   | SLL walks; DLL has prev_\n";
    std::cout << "  contains       | O(n)   | O(n)   | Must scan the list\n";
    std::cout << "\n";
    std::cout << "  Key takeaway: Big O describes how performance GROWS\n";
    std::cout << "  as input size increases - not the actual speed.\n";
    std::cout << "\n";
    std::cout << "  O(1) = constant: doubling n doesn't change the time\n";
    std::cout << "  O(n) = linear:   doubling n roughly doubles the time\n";

    // ── Write CSV and generate charts ──────────────────────────────────
    std::string repo_dir = REPO_DIR;
    std::string csv_path = repo_dir + "/results.csv";
    std::ofstream csv(csv_path);
    csv << "operation,structure,complexity,n,time_us\n";
    for (const auto& r : results) {
        csv << r.operation << "," << r.structure << "," << r.complexity
            << "," << r.n << "," << std::fixed << std::setprecision(4)
            << r.time_us << "\n";
    }
    csv.close();
    std::cout << "\n  Results written to CSV — generating charts...\n";

    std::string cmd = "py -3 \"" + repo_dir + "/graph.py\" --graph-only";
    std::system(cmd.c_str());

    return 0;
}
