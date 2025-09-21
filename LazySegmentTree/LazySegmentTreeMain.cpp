#include <iostream>
#include <vector>
#include <functional>

class LazySegmentTree
{
public:
    LazySegmentTree(std::vector<int> &arr)
    {
        size = arr.size();
        segTree.resize(4 * size);
        lazy.resize(4 * size);
        construct(0, 0, size - 1, arr);
    }

    int query(int qstart, int qend)
    {
        return query(0, 0, size - 1, qstart, qend);
    }

    void update(int qstart, int qend, int val)
    {
        update(0, 0, size - 1, qstart, qend, val);
    }

private:
    size_t size;
    std::vector<int> segTree, lazy;

    void construct(int i, int start, int end, std::vector<int> &arr)
    {
        if (start == end)
        {
            segTree[i] = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        construct(2 * i + 1, start, mid, arr);
        construct(2 * i + 2, mid + 1, end, arr);
        segTree[i] = segTree[2 * i + 1] + segTree[2 * i + 2];
    }

    int query(int i, int start, int end, int qstart, int qend)
    {
        if (lazy[i])
        {
            segTree[i] += (end - start + 1) * lazy[i];
            if (start != end)
            {
                lazy[2 * i + 1] += lazy[i];
                lazy[2 * i + 2] += lazy[i];
            }
            lazy[i] = 0;
        }

        if (qend < start || end < qstart)
            return 0;

        if (qstart <= start && end <= qend)
            return segTree[i];

        int mid = (start + end) / 2;
        return query(2 * i + 1, start, mid, qstart, qend) + query(2 * i + 2, mid + 1, end, qstart, qend);
    }

    void update(int i, int start, int end, int qstart, int qend, int val)
    {
        if (lazy[i])
        {
            segTree[i] += (end - start + 1) * lazy[i];
            if (start != end)
            {
                lazy[2 * i + 1] += lazy[i];
                lazy[2 * i + 2] += lazy[i];
            }
            lazy[i] = 0;
        }

        if (qend < start || end < qstart)
            return;

        if (qstart <= start && end <= qend)
        {
            segTree[i] += (end - start + 1) * val;
            if (start != end)
            {
                lazy[2 * i + 1] += val;
                lazy[2 * i + 2] += val;
            }
            return;
        }
        int mid = (start + end) / 2;
        update(2 * i + 1, start, mid, qstart, qend, val);
        update(2 * i + 2, mid + 1, end, qstart, qend, val);
        segTree[i] = segTree[2 * i + 1] + segTree[2 * i + 2];
    }
};

int main()
{

    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    LazySegmentTree lazySegTree(arr);

    std::cout << "Q: 0,8: " << lazySegTree.query(0, 8) << " <- Got. Expecting -> 45\n";
    std::cout << "Q: 3,5: " << lazySegTree.query(3, 5) << " <- Got. Expecting -> 15\n";
    lazySegTree.update(4, 5, 2);
    std::cout << "Q: 0,8: " << lazySegTree.query(0, 8) << " <- Got. Expecting -> 49\n";
    std::cout << "Q: 3,5: " << lazySegTree.query(3, 5) << " <- Got. Expecting -> 19\n";

    return 0;
}