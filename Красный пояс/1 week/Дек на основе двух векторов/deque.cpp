#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T>
class Deque {
public:
    Deque() {}

    bool Empty() const {
        return front.empty() && back.empty();
    }

    size_t Size() const {
        return front.size() + back.size();
    }

    T& operator[](size_t index) {
        size_t front_size = front.size();
        return index < front_size ? front[front_size - index - 1] : back[index - front_size];
    }

    const T& operator[](size_t index) const {
        size_t front_size = front.size();
        return index < front_size ? front[front_size - index - 1] : back[index - front_size];
    }

    T& At(size_t index) {
        if (index >= front.size() + back.size()) {
            throw std::out_of_range("out of range");
        }
        return operator[](index);
    }

    const T& At(size_t index) const {
        if (index >= front.size() + back.size()) {
            throw std::out_of_range("out of range");
        }
        return operator[](index);
    }

    T& Front() {
        return front.empty() ? back[0] : front.back();
    }

    const T& Front() const {
        return front.empty() ? back[0] : front.back();
    }

    T& Back() {
        return back.empty() ? front[0] : back.back();
    }

    const T& Back() const {
        return back.empty() ? front[0] : back.back();
    }

    void PushFront(const T& t) {
        front.push_back(t);
    }

    void PushBack(const T& t) {
        back.push_back(t);
    }

private:
    std::vector<T> front;
    std::vector<T> back;
};
