#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RBT_String.h"

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

RBSRoot* rbts_create()
{
    RBSRoot* root = (RBSRoot*)malloc(sizeof(RBSRoot));
    root->node = NULL;

    return root;
}

static RBSNode* ssearch(RBSTree x, char* target)
{
    if (x == NULL || strcmp(target, x->string) == 0)
        return x;

    if (strcmp(target, x->string) < 0)
        return ssearch(x->left, target);
    else
        return ssearch(x->right, target);
}

static RBSNode* rbts_minimumNode(RBSTree tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}

/*int rbt_minimumVal(RBRoot* root, int* val)
{
    RBNode* node;

    if (root)
        node = rbt_minimumNode(root->node);

    if (node == NULL)
        return -1;

    *val = node->key;
    return 0;
}*/

static RBSNode* rbts_maximumNode(RBSTree tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}

/*int rbt_maximumVal(RBRoot* root, int* val)
{
    RBNode* node;

    if (root)
        node = rbt_maximumNode(root->node);

    if (node == NULL)
        return -1;

    *val = node->key;
    return 0;
}*/


RBSNode* rbts_successor(RBSTree x)
{
    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (x->right != NULL)
        return rbts_minimumNode(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    RBSNode* y = x->parent;
    while ((y != NULL) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

RBSNode* rbts_predecessor(RBSTree x)
{
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return rbts_maximumNode(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    RBSNode* y = x->parent;
    while ((y != NULL) && (x == y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}



static void rbstree_left_rotate(RBSRoot* root, RBSNode* x)
{
    // 设置x的右孩子为y
    RBSNode* y = x->right;

    // 将 “y的左孩子” 设为 “x的右孩子”；
    // 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // 将 “x的父亲” 设为 “y的父亲”
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        //tree = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
        root->node = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
    }

    // 将 “x” 设为 “y的左孩子”
    y->left = x;
    // 将 “x的父节点” 设为 “y”
    x->parent = y;
}

static void rbstree_right_rotate(RBSRoot* root, RBSNode* y)
{
    // 设置x是当前节点的左孩子。
    RBSNode* x = y->left;

    // 将 “x的右孩子” 设为 “y的左孩子”；
    // 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // 将 “y的父亲” 设为 “x的父亲”
    x->parent = y->parent;

    if (y->parent == NULL)
    {
        //tree = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
        root->node = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->left = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }

    // 将 “y” 设为 “x的右孩子”
    x->right = y;

    // 将 “y的父节点” 设为 “x”
    y->parent = x;
}

static void rbstree_insert_fixup(RBSRoot* root, RBSNode* node)
{
    RBSNode* parent, * gparent;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left)
        {
            // Case 1条件：叔叔节点是红色
            {
                RBSNode* uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是右孩子
            if (parent->right == node)
            {
                RBSNode* tmp;
                rbstree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rbstree_right_rotate(root, gparent);
        }
        else//若“z的父节点”是“z的祖父节点的右孩子”
        {
            // Case 1条件：叔叔节点是红色
            {
                RBSNode* uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是左孩子
            if (parent->left == node)
            {
                RBSNode* tmp;
                rbstree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是右孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rbstree_left_rotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root->node);
}


static void rbstree_insert(RBSRoot* root, RBSNode* node)
{
    RBSNode* y = NULL;
    RBSNode* x = root->node;

    // 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    while (x != NULL)
    {
        y = x;
        if (strcmp(node->string, x->string) < 0)
            x = x->left;
        else
            x = x->right;
    }
    rb_parent(node) = y;

    if (y != NULL)
    {
        if (strcmp(node->string, y->string) < 0)
            y->left = node;                // 情况2：若“node所包含的值” < “y所包含的值”，则将node设为“y的左孩子”
        else
            y->right = node;            // 情况3：(“node所包含的值” >= “y所包含的值”)将node设为“y的右孩子”
    }
    else
    {
        root->node = node;                // 情况1：若y是空节点，则将node设为根
    }

    // 2. 设置节点的颜色为红色
    node->color = RED;

    // 3. 将它重新修正为一颗二叉查找树
    rbstree_insert_fixup(root, node);
}

static RBSNode* create_rbstree_node(char* key, RBSNode* parent, RBSNode* left, RBSNode* right)
{
    RBSNode* p;

    if ((p = (RBSNode*)malloc(sizeof(RBSNode))) == NULL)
        return NULL;
    p->string = (char*)malloc(strlen(key) + 3);
    strcpy(p->string, key);
    p->left = left;
    p->right = right;
    p->parent = parent;
    p->color = BLACK; // 默认为黑色

    return p;
}

RBSNode* rbts_insertVal(RBSRoot* root, char* key)
{
    RBSNode* node;    // 新建结点

    if ((node = create_rbstree_node(key, NULL, NULL, NULL)) == NULL)
        return NULL;

    rbstree_insert(root, node);

    return node;
}

static void rbstree_delete_fixup(RBSRoot* root, RBSNode* node, RBSNode* parent)
{
    RBSNode* other;

    while ((!node || rb_is_black(node)) && node != root->node)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                rbstree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbstree_right_rotate(root, other);
                    other = parent->right;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbstree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                rbstree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbstree_left_rotate(root, other);
                    other = parent->left;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbstree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

void rbts_deleteNode(RBSRoot* root, RBSNode* node)
{
    RBSNode* child, * parent;
    int color;

    if ((node->left != NULL) && (node->right != NULL))
    {
        RBSNode* replace = node;

        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else
            root->node = replace;

        child = replace->right;
        parent = rb_parent(replace);
        color = rb_color(replace);

        if (parent == node)
        {
            parent = replace;
        }
        else
        {
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbstree_delete_fixup(root, child, parent);
        free(node);

        return;
    }

    if (node->left != NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    color = node->color;

    if (child)
        child->parent = parent;

    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root->node = child;

    if (color == BLACK)
        rbstree_delete_fixup(root, child, parent);
    free(node);
}

void rbts_deleteVal(RBSRoot* root, char* key)
{
    RBSNode* z;

    if ((z = ssearch(root->node, key)) != NULL)
        rbts_deleteNode(root, z);
}


//模式1是精确搜索，2是找第一个更大值，3是找第一个更小值
RBSNode* rbts_search_returnNode(RBSRoot* root, const char* key, int mode)
{
    RBSNode* x = root->node;
    if (x == NULL) return NULL;
    while (x != NULL && strcmp(key, x->string) != 0)
    {
        if (strcmp(x->string, key) < 0) {
            if (!x->right && mode != 1) {
                if (mode == 3) return x;
                else {
                    while (x != NULL && strcmp(x->string, key) < 0) {
                        x = rbts_successor(x);
                    }
                    return x;
                }
            }
            x = x->right;
        }
        else {
            if (!x->left && mode != 1) {
                if (mode == 2) return x;
                else {
                    while (x != NULL && strcmp(x->string, key) > 0) x = rbts_predecessor(x);
                    return x;
                }
            }
            x = x->left;
        }
    }
    return x;
}