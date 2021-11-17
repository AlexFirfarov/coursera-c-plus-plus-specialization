#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        T* new_object_ptr;

        if (!deallocated.empty()) {
            new_object_ptr = deallocated.front();
            deallocated.pop();
        } else {
          new_object_ptr = new T;
        }
        allocated.insert(new_object_ptr);

        return new_object_ptr;
    }

    T* TryAllocate() {
        if (deallocated.empty()) {
            return nullptr;
        }

        T* new_object_ptr = deallocated.front();
        deallocated.pop();
        allocated.insert(new_object_ptr);

        return new_object_ptr;
    }

    void Deallocate(T* object) {
        auto it = allocated.find(object);

        if (it == allocated.end()) {
            throw std::invalid_argument("");
        }

        deallocated.push(*it);
        allocated.erase(it);
    }

    ~ObjectPool() {
        for (T* ptr : allocated) {
            delete ptr;
        }

        while (!deallocated.empty()) {
            delete deallocated.front();
            deallocated.pop();
        }
    }

private:
    std::set<T*> allocated;
    std::queue<T*> deallocated;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
