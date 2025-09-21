#include <cassert>
#include <iostream>
#include <vector>

template <typename T>
class FenwickTree
{
public:
    FenwickTree(std::vector<T> &arr)
    {
        size = arr.size();
        fTree.resize(size + 1);
        construct(arr);
    }

    T query(int qstart, int qend)
    {
        return prefixQuery(qend) - prefixQuery(qstart - 1);
    }

    T prefixQuery(int index)
    {
        T sum = 0;
        index++;

        while (index > 0)
        {
            sum += fTree[index];
            index -= (index & -index);
        }
        return sum;
    }

    void increment(int index, T inc)
    {
        index++;
        while (index <= size)
        {
            fTree[index] += inc;
            index += index & (-index);
        }
    }

private:
    size_t size;
    std::vector<T> fTree;

    void construct(std::vector<T> &arr)
    {
        // Flexing O(n) construction.
        for (int i = 1; i <= size; i++)
        {
            fTree[i] += arr[i - 1];
            int next = i + (i & -i);
            if (next <= size)
                fTree[next] += fTree[i];
        }
    }
};

void test_prefix_query()
{
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    FenwickTree<int> ft(arr);

    assert(ft.prefixQuery(0) == 1);
    assert(ft.prefixQuery(1) == 3);
    assert(ft.prefixQuery(4) == 15);
    assert(ft.prefixQuery(8) == 45);

    std::cout << "Fenwick tree prefix query passed\n";
}

void test_increment()
{
    std::vector<int> arr = {1, 2, 3, 4, 5};
    FenwickTree<int> ft(arr);

    ft.increment(2, 5); // arr[2] += 5, arr = {1,2,8,4,5}
    assert(ft.prefixQuery(2) == 1 + 2 + 8);
    assert(ft.prefixQuery(4) == 1 + 2 + 8 + 4 + 5);

    ft.increment(0, -1); // arr[0] -= 1, arr = {0,2,8,4,5}
    assert(ft.prefixQuery(0) == 0);
    assert(ft.prefixQuery(1) == 2);

    std::cout << "Fenwick tree increment passed\n";
}

void test_query_range()
{
    std::vector<int> arr = {5, 3, 7, 9, 6};
    FenwickTree<int> ft(arr);

    // query(1, 3) should be arr[1] + arr[2] + arr[3] = 3 + 7 + 9 = 19
    // Note: The query method in the original code has a bug (uses undefined 'start' and 'end')
    // For this test, we will only test prefixQuery
    assert(ft.query(1, 3) == 3 + 7 + 9);

    std::cout << "Fenwick Tree range query passed\n";
}

int main()
{
    test_prefix_query();
    test_increment();
    test_query_range();
    std::cout << "All FenwickTree tests passed!" << std::endl;
    return 0;
}