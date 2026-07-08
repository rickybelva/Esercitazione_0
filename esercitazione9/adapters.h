#pragma once
#include <queue>
#include <stack>
class fifo {
public:
    fifo() = default;
    void put(int x) { q_.push(x); }
    int get() { int x = q_.front(); q_.pop(); return x; }
    bool empty() const { return q_.empty(); }
private:
    std::queue<int> q_;
};
class lifo {
public:
    lifo() = default;
    void put(int x) { s_.push(x); }
    int get() { int x = s_.top(); s_.pop(); return x; }
    bool empty() const { return s_.empty(); }
private:
    std::stack<int> s_;
};