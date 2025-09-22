#include <cassert>
#include <iostream>

struct Node
{
    int key;
    Node *left, *right;

    Node(int key)
    {
        this->key = key;
        left = right = nullptr;
    }

#ifdef DEBUG
    ~Node()
    {
        std::cout << "Deleting " << key << "\n";
    }
#endif
};

class BST
{
private:
    Node *root = nullptr;

    void deleteInPostOrder(Node *root);

    Node *insert(Node *root, int key);
    Node *deleteKey(Node *root, int key);

    Node *getMin(Node *root);

public:
    BST() = default;
    ~BST();

    void insert(int key);
    void deleteKey(int key);
    bool search(int key);
};

BST::~BST()
{
    deleteInPostOrder(root);
}

void BST::insert(int key)
{
    root = insert(root, key);
}

void BST::deleteKey(int key)
{
    root = deleteKey(root, key);
}

bool BST::search(int key)
{
    Node *root = this->root;
    while (root)
    {
        if (root->key == key)
            return true;
        if (key > root->key)
            root = root->right;
        else
            root = root->left;
    }
    return false;
}

void BST::deleteInPostOrder(Node *root)
{
    if (root == nullptr)
        return;
    deleteInPostOrder(root->left);
    deleteInPostOrder(root->right);
    delete root;
}

Node *BST::insert(Node *root, int key)
{
    if (root == nullptr)
        return new Node(key);

    if (key < root->key)
    {
        root->left = insert(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = insert(root->right, key);
    }
    return root;
}

Node *BST::deleteKey(Node *root, int key)
{
    if (root == nullptr)
        return nullptr;
    if (root->key == key)
    {
        if (root->left == nullptr)
        {
            Node *newRoot = root->right;
            delete root;
            return newRoot;
        }
        else if (root->right == nullptr)
        {
            Node *newRoot = root->left;
            delete root;
            return newRoot;
        }
        else
        {
            Node *minRoot = getMin(root->right);
            root->key = minRoot->key;
            root->right = deleteKey(root->right, minRoot->key);
        }
    }
    else if (key < root->key)
    {
        root->left = deleteKey(root->left, key);
    }
    else
    {
        root->right = deleteKey(root->right, key);
    }
    return root;
}

Node *BST::getMin(Node *root)
{
    while (root->left)
        root = root->left;

    return root;
}

int main()
{

    BST tree;

    // Insert
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    // Search
    assert(tree.search(50));
    assert(tree.search(30));
    assert(!tree.search(100));

    // Delete leaf
    tree.deleteKey(20);
    assert(!tree.search(20));

    // Delete node with one child
    tree.insert(25);
    assert(tree.search(25));
    tree.deleteKey(25);
    assert(!tree.search(25));

    // Delete node with two children
    tree.deleteKey(30);
    assert(!tree.search(30));

    // Delete root
    tree.deleteKey(50);
    assert(!tree.search(50));

    // Duplicate insert
    tree.insert(40);
    tree.insert(40);
    assert(tree.search(40));

    return 0;
}