#include <iostream>
#include <vector>

// Max Sparse table.
class SparseTable
{
public:
    SparseTable(std::vector<int> &arr)
    {
        size = arr.size();
        initLog();
        int levels = log2[size];
        sTable.resize(levels + 1);
        construct(arr);
    }

    int query(int qstart, int qend)
    {
        int length = qend - qstart + 1;
        int level = log2[length];
        return std::max(sTable[level][qstart], sTable[level][qend - (1 << level) + 1]);
    }

private:
    void initLog()
    {
        if (log2.empty() == false)
            return;
        log2.resize(size + 1);
        log2[1] = 0;
        for (int i = 2; i <= size; i++)
        {
            log2[i] = log2[i / 2] + 1;
        }
    }

    void construct(std::vector<int> &arr)
    {
        sTable[0] = arr;
        for (int level = 1; level < sTable.size(); level++)
        {
            sTable[level].resize(size - (1 << level) + 1);
            for (int i = 0; i + (1 << level) <= size; i++)
            {
                sTable[level][i] = std::max(sTable[level - 1][i], sTable[level - 1][i + (1 << (level - 1))]);
            }
        }
    }

    size_t size;
    std::vector<std::vector<int>> sTable;
    // This also defines the vector. Be careful to not redefine it in the same translation unit.
    inline static std::vector<int> log2;
};

int main()
{
    std::vector<int> arr{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    SparseTable stable{arr};

    std::cout << "Q 0,9: " << stable.query(0, 9) << " <- Got. Expected 9\n";
    std::cout << "Q 3,6: " << stable.query(3, 6) << " <- Got. Expected 6\n";

    return 0;
}