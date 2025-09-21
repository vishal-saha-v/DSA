#include <iostream>
#include <vector>
#include <functional>

template <typename T>
class SegmentTree
{
public:
    SegmentTree(std::vector<T> &arr, std::function<void(T &, const T &, const T &)> mergeFunction, T identity_val) : IDENTITY_VAL{identity_val}
    {
        size = arr.size();
        merge = mergeFunction;
        segTree.resize(4 * size);
        construct(0, 0, size - 1, arr);
    }

    T query(int qstart, int qend)
    {
        return query(0, 0, size - 1, qstart, qend);
    }

    void update(int index, T val)
    {
        update(0, 0, size - 1, index, val);
    }

private:
    size_t size;
    std::vector<T> segTree;
    const T IDENTITY_VAL;

    std::function<void(T &, const T &, const T &)> merge;
    void construct(int i, int start, int end, std::vector<T> &arr)
    {
        if (start == end)
        {
            segTree[i] = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        construct(2 * i + 1, start, mid, arr);
        construct(2 * i + 2, mid + 1, end, arr);
        merge(segTree[i], segTree[2 * i + 1], segTree[2 * i + 2]);
    }

    T query(int i, int start, int end, int qstart, int qend)
    {
        if (qend < start || end < qstart)
            return IDENTITY_VAL;

        if (qstart <= start && end <= qend)
            return segTree[i];

        int mid = (start + end) / 2;
        T res = IDENTITY_VAL;
        merge(res, query(2 * i + 1, start, mid, qstart, qend), query(2 * i + 2, mid + 1, end, qstart, qend));
        return res;
    }

    void update(int i, int start, int end, int index, T val)
    {
        if (start == end)
        {
            segTree[i] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (index <= mid)
            update(2 * i + 1, start, mid, index, val);
        else
            update(2 * i + 2, mid + 1, end, index, val);
        merge(segTree[i], segTree[2 * i + 1], segTree[2 * i + 2]);
    }
};

int main()
{

    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int identity_val = 0;
    std::function<void(int &, const int &, const int &)> mergeFunction = [](int &res, const int &a, const int &b)
    {
        res = a + b;
    };

    SegmentTree<int>
        segTree = SegmentTree<int>(arr, mergeFunction, identity_val);

    std::cout << "Q: 0,8: " << segTree.query(0, 8) << " <- Got. Expecting -> 45\n";
    std::cout << "Q: 3,5: " << segTree.query(3, 5) << " <- Got. Expecting -> 15\n";
    segTree.update(4, 10);
    std::cout << "Q: 0,8: " << segTree.query(0, 8) << " <- Got. Expecting -> 50\n";
    std::cout << "Q: 3,5: " << segTree.query(3, 5) << " <- Got. Expecting -> 20\n";

    return 0;
}