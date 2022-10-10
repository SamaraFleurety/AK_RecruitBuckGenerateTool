#ifndef _RED_BLACK_TREE_STRING_H_
#define _RED_BLACK_TREE_STRING_H_

#define RED        0    // 红色节点
#define BLACK    1    // 黑色节点

typedef struct RBSTreeNode {
    unsigned char color;        // 颜色(RED 或 BLACK)
    char* string;          // 关键字(键值)
    int degree;
    int *thoughtOffset;
    struct RBSTreeNode* left;    // 左孩子
    struct RBSTreeNode* right;    // 右孩子
    struct RBSTreeNode* parent;    // 父结点
}RBSNode, * RBSTree;

typedef struct rbs_root {
    RBSNode* node;
}RBSRoot;

RBSRoot* rbts_create();

RBSNode* rbts_insertVal(RBSRoot* root, char* key);

void rbts_deleteNode(RBSRoot* root, RBSNode* node);

void rbts_deleteVal(RBSRoot* root, char* key);

RBSNode* rbts_search_returnNode(RBSRoot* root, const char* key, int mode);

RBSNode* rbts_successor(RBSTree x);

#endif

