#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
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

    bool empty() { return m_root == nullptr; }

    void insert(int v)
    {
        m_root = insert(m_root, v);
        m_root->color = BLACK;
    }

    bool isRed(TreeNode* root) { return root && root->color == RED; }

    TreeNode* insert(TreeNode* root, int v)
    {
        if (!root) return new TreeNode(v);
        if (v < root->val) root->left = insert(root->left, v);
        else if (v > root->val) root->right = insert(root->right, v);
        else return root;

        return balance(root);
    }

    void deleteMin()
    {
        if (!m_root) return;
        m_root = deleteMin(m_root);
        if (m_root) m_root->color = BLACK;
    }

    TreeNode* deleteMin(TreeNode* root)
    {
        // 当没有左儿子时，表示达到最左侧
        if (!root->left) {
            cout << "delete " << root->val << endl;
            delete root;
            return nullptr; // 没有左儿子，就一定没有右儿子，因为
                            // 如果右儿子为2节点，则黑色不平衡
                            // 如果右儿子非2节点，则原树不符合红黑树性质
        }
        
        // 如果左儿子是一个2节点，尝试从兄弟“借”一个节点过来
        if (!isRed(root->left) && !isRed(root->left->left))
            root = moveRedLeft(root);

        root->left = deleteMin(root->left);

        // 修复上面可能的moveRedLeft操作
        return balance(root);
    }

    void deleteMax()
    {
        if (!m_root) return;
        m_root = deleteMax(m_root);
       if (m_root) m_root->color = BLACK;
    }

    TreeNode* deleteMax(TreeNode* root)
    {
        if (isRed(root->left))
            root = rotateRight(root);

        if (!root->right) {
            cout << "delete " << root->val << endl;
            delete root;
            return nullptr;
        }

        if (!isRed(root->right) && !isRed(root->right->left)) {
            root = moveRedRight(root);
        }

        root->right = deleteMax(root->right);

        return balance(root);
    }

    TreeNode* max()
    {
        auto root = m_root;
        while (root) root = root->right;
        return root;
    }

    TreeNode* min()
    {
        auto root = m_root;
        while (root) root = root->left;
        return root;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 辅助函数
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    TreeNode* balance(TreeNode* root)
    {
        if (!isRed(root->left) && isRed(root->right)) root = rotateLeft(root);
        if (isRed(root->left) && isRed(root->left->left)) root = rotateRight(root);
        if (isRed(root->left) && isRed(root->right)) flipColors(root);
        return root;
    }

    TreeNode* rotateLeft(TreeNode* root)
    {
        TreeNode* node = root->right;
        root->right = node->left;
        node->left = root;
        node->color = root->color;
        root->color = RED;
        return node;
    }

    TreeNode* rotateRight(TreeNode* root)
    {
        TreeNode* node = root->left;
        root->left = node->right;
        node->right = root;
        node->color = root->color;
        root->color = RED;
        return node;
    }

    void flipColors(TreeNode* root)
    {
        root->color = !root->color;
        root->left->color = !root->left->color;
        root->right->color = !root->right->color;
    }

    TreeNode* moveRedLeft(TreeNode* root)
    {
        flipColors(root);
        if (isRed(root->right->left)) {
            root->right = rotateRight(root->right);
            root = rotateLeft(root);
            flipColors(root);
        }
        return root;
    }

    TreeNode* moveRedRight(TreeNode* root)
    {
        flipColors(root);
        if (isRed(root->left->left)) {
            root = rotateRight(root);
            flipColors(root);
        }
        return root;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 校验代码
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void check()
    {
        if (isBST() && is23() && isBalanced()) {
            cout << "OK" << endl;
        }
    }

    bool isBST()
    {
        TreeNode* prev = nullptr;
        return isBST(m_root, prev);
    }

    bool isBST(TreeNode* root, TreeNode* &prev)
    {
        if (!root) return true;
        if(!isBST(root->left, prev)) return false;
        if (prev && prev->val > root->val) return false;
        prev = root;
        return isBST(root->right, prev);
    }

    bool is23() { return is23(m_root); }
    bool is23(TreeNode* root)
    {
        if (!root) return true;
        if (isRed(root->right)) return false;
        if (isRed(root->left) && isRed(root->left->left)) return false;
        return is23(root->left) && is23(root->right);
    }

    bool isBalanced()
    {
        int black = 0;
        TreeNode* node = m_root;
        while (node) {
            if (!isRed(node)) black++;
            node = node->left;
        }
        return isBalanced(m_root, black);
    }

    bool isBalanced(TreeNode* root, int black)
    {
        if (!root) return black == 0;
        if (!isRed(root)) black--;
        return isBalanced(root->left, black) && isBalanced(root->right, black);
    }
};

int main()
{
    srand((unsigned)time(NULL));
    auto root = new RedBlackBST;
    for (int i = 1; i <= 50; ++i) root->insert(i);
    for (int i = 0; i < 25; ++i) root->deleteMax();
    for (int i = 0; i < 10; ++i) root->deleteMin();
    if (root->empty()) cout << "empty" << endl;
    root->check();
    return 0;
}
