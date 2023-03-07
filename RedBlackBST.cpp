#include <cassert>
#include <iostream>

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

    bool contains(int v) {
        if (!m_root)
            return false;

        auto node = m_root;
        while (node) {
            if (v < node->val)
                node = node->left;
            else if (v > node->val)
                node = node->right;
            else
                return true;
        }

        return false;
    }

    void insert(int v) {
        m_root = insert(m_root, v);
        m_root->color = BLACK;
    }

    void erase(int v) {
        if (!m_root)
            return;

        if (!contains(v))
            return;

        if (!isRed(m_root->left) && !isRed(m_root->right))
            m_root->color = RED;

        m_root = erase(m_root, v);
        if (m_root)
            m_root->color = BLACK;
    }

    void check() {
        assert(isBST());
        assert(is23());
        assert(isBalanced());
    }

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

    TreeNode* erase(TreeNode* root, int v) {
        if (v < root->val) {
            if (!isRed(root->left) && !isRed(root->left->left))
                root = moveRedLeft(root);
            root->left = erase(root->left, v);
        } else if (v > root->val) {
            if (isRed(root->left))
                root = rotateRight(root);
            if (!isRed(root->right) && !isRed(root->right->left))
                root = moveRedRight(root);
            root->right = erase(root->right, v);
        } else {
            if (isRed(root->left)) {
                root = rotateRight(root);
                return erase(root, v); // 如果发生过旋转，下面的3个if都是不会执行的，这一行可以删除掉
                // root->right = erase(root->right, v);
                // return balance(root);
            }

            if (!root->left && !root->right) {
                delete root;
                return nullptr;
            }

            if (!isRed(root->right) && !isRed(root->right->left))
                root = moveRedRight(root);
            
            if (v == root->val) {
                TreeNode* node = root->right;
                while (node->left) node = node->left;
                root->val = node->val;
                node->val = v;
            }

            root->right = erase(root->right, v);
        }

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

    TreeNode* moveRedLeft(TreeNode* root) {
        flipColors(root);
        if (isRed(root->right->left)) {
            root->right = rotateRight(root->right);
            root = rotateLeft(root);
            flipColors(root);
        }
        return root;
    }

    TreeNode* moveRedRight(TreeNode* root) {
        flipColors(root);
        if (isRed(root->left->left)) {
            root = rotateRight(root);
            flipColors(root);
        }
        return root;
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
    int n;
    cout << "n = ";
    while (cin >> n) {
        root.erase(n);
        root.check();
        cout << "n = ";
    }
    return 0;
}
