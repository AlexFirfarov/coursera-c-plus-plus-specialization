#include "test_runner.h"

#include <vector>

template <typename T>
class Table {
public:
    Table(size_t row_cnt, size_t column_cnt) :  
        table(std::vector<std::vector<T>>(row_cnt, std::vector<T>(column_cnt, T()))) {}

    std::vector<T>& operator[](size_t index) {
        return table[index];
    }

    const std::vector<T>& operator[](size_t index) const {
        return table[index];
    }

    void Resize(size_t new_row_cnt, size_t new_column_cnt) {
        table.resize(new_row_cnt);
        for (std::vector<T>& row : table) {
            row.resize(new_column_cnt, T());
        }

        return;
    }

    std::pair<size_t, size_t> Size() const {
        if (!table.empty() && !table[0].empty()) {
            return {table.size(), table[0].size()};
        }

        return  {0, 0};
    }


private:
    std::vector<std::vector<T>> table;
};

void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
