#ifndef _RED_BLACK_TREE_STRING_H_
#define _RED_BLACK_TREE_STRING_H_

#define RED        0    // ��ɫ�ڵ�
#define BLACK    1    // ��ɫ�ڵ�

typedef struct RBSTreeNode {
    unsigned char color;        // ��ɫ(RED �� BLACK)
    char* string;          // �ؼ���(��ֵ)
    int degree;
    int *thoughtOffset;
    struct RBSTreeNode* left;    // ����
    struct RBSTreeNode* right;    // �Һ���
    struct RBSTreeNode* parent;    // �����
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

