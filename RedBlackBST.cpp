#include <cassert>

using namespace std;

class RedBlackBST {
    static const bool RED = true;
    static const bool BLACK = false;

    struct TreeNode {
        TreeNode(int v): val(v), color(RED), left(nullptr), right(nullptr) {}

        int val;
        bool color;
        TreeNode* left;
        TreeNode* right;
    };

    TreeNode* m_root;

public:
    RedBlackBST(): m_root(nullptr) {}

    void insert(int v)
    {
        m_root = insert(m_root, v);
        m_root->color = BLACK;
    }

    void check() { assert(isBST() && is23() && isBalanced()); }

private:
    TreeNode* insert(TreeNode* root, int v) {
        if (!root)
            return new TreeNode(v);
        if (v < root->val)
            root->left = insert(root->left, v);
        else if (v > root->val)
            root->right = insert(root->right, v);
        else
            return root;

        return balance(root);
    }

    TreeNode* balance(TreeNode* root) {
        if (!isRed(root->left) && isRed(root->right))
            root = rotateLeft(root);
        if (isRed(root->left) && isRed(root->left->left))
            root = rotateRight(root);
        if (isRed(root->left) && isRed(root->right))
            flipColors(root);
        return root;
    }

    bool isRed(TreeNode* root) { return root && root->color == RED; }

    TreeNode* rotateLeft(TreeNode* root) {
        TreeNode* node = root->right;
        root->right = node->left;
        node->left = root;
        node->color = root->color;
        root->color = RED;
        return node;
    }

    TreeNode* rotateRight(TreeNode* root) {
        TreeNode* node = root->left;
        root->left = node->right;
        node->right = root;
        node->color = root->color;
        root->color = RED;
        return node;
    }

    void flipColors(TreeNode* root) {
        root->color = !root->color;
        root->left->color = !root->left->color;
        root->right->color = !root->right->color;
    }

private:
    bool isBST() {
        TreeNode* prev = nullptr;
        return isBST(m_root, prev);
    }

    bool isBST(TreeNode* root, TreeNode* &prev) {
        if (!root) return true;
        if(!isBST(root->left, prev)) return false;
        if (prev && prev->val > root->val) return false;
        prev = root;
        return isBST(root->right, prev);
    }

    bool is23() { return is23(m_root); }

    bool is23(TreeNode* root) {
        if (!root) return true;
        if (isRed(root->right)) return false;
        if (isRed(root) && isRed(root->left)) return false;
        return is23(root->left) && is23(root->right);
    }

    bool isBalanced() {
        int black = 0;
        TreeNode* node = m_root;
        while (node) {
            if (!isRed(node)) black++;
            node = node->left;
        }
        return isBalanced(m_root, black);
    }

    bool isBalanced(TreeNode* root, int black) {
        if (!root) return black == 0;
        if (!isRed(root)) black--;
        return isBalanced(root->left, black) && isBalanced(root->right, black);
    }
};

int main() {
    RedBlackBST root;
    for (int i = 1; i <= 100; i++)
        root.insert(i);
    root.check();
    return 0;
}
