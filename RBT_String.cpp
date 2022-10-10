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
    // ���x�����Һ��ӣ���"x�ĺ�̽��"Ϊ "�����Һ���Ϊ������������С���"��
    if (x->right != NULL)
        return rbts_minimumNode(x->right);

    // ���xû���Һ��ӡ���x���������ֿ��ܣ�
    // (01) x��"һ������"����"x�ĺ�̽��"Ϊ "���ĸ����"��
    // (02) x��"һ���Һ���"�������"x����͵ĸ���㣬���Ҹø����Ҫ��������"���ҵ������"��͵ĸ����"����"x�ĺ�̽��"��
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
    // ���x�������ӣ���"x��ǰ�����"Ϊ "��������Ϊ���������������"��
    if (x->left != NULL)
        return rbts_maximumNode(x->left);

    // ���xû�����ӡ���x���������ֿ��ܣ�
    // (01) x��"һ���Һ���"����"x��ǰ�����"Ϊ "���ĸ����"��
    // (01) x��"һ������"�������"x����͵ĸ���㣬���Ҹø����Ҫ�����Һ���"���ҵ������"��͵ĸ����"����"x��ǰ�����"��
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
    // ����x���Һ���Ϊy
    RBSNode* y = x->right;

    // �� ��y�����ӡ� ��Ϊ ��x���Һ��ӡ���
    // ���y�����ӷǿգ��� ��x�� ��Ϊ ��y�����ӵĸ��ס�
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // �� ��x�ĸ��ס� ��Ϊ ��y�ĸ��ס�
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        //tree = y;            // ��� ��x�ĸ��ס� �ǿսڵ㣬��y��Ϊ���ڵ�
        root->node = y;            // ��� ��x�ĸ��ס� �ǿսڵ㣬��y��Ϊ���ڵ�
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
        else
            x->parent->right = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
    }

    // �� ��x�� ��Ϊ ��y�����ӡ�
    y->left = x;
    // �� ��x�ĸ��ڵ㡱 ��Ϊ ��y��
    x->parent = y;
}

static void rbstree_right_rotate(RBSRoot* root, RBSNode* y)
{
    // ����x�ǵ�ǰ�ڵ�����ӡ�
    RBSNode* x = y->left;

    // �� ��x���Һ��ӡ� ��Ϊ ��y�����ӡ���
    // ���"x���Һ���"��Ϊ�յĻ����� ��y�� ��Ϊ ��x���Һ��ӵĸ��ס�
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // �� ��y�ĸ��ס� ��Ϊ ��x�ĸ��ס�
    x->parent = y->parent;

    if (y->parent == NULL)
    {
        //tree = x;            // ��� ��y�ĸ��ס� �ǿսڵ㣬��x��Ϊ���ڵ�
        root->node = x;            // ��� ��y�ĸ��ס� �ǿսڵ㣬��x��Ϊ���ڵ�
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // ��� y�������ڵ���Һ��ӣ���x��Ϊ��y�ĸ��ڵ���Һ��ӡ�
        else
            y->parent->left = x;    // (y�������ڵ������) ��x��Ϊ��x�ĸ��ڵ�����ӡ�
    }

    // �� ��y�� ��Ϊ ��x���Һ��ӡ�
    x->right = y;

    // �� ��y�ĸ��ڵ㡱 ��Ϊ ��x��
    y->parent = x;
}

static void rbstree_insert_fixup(RBSRoot* root, RBSNode* node)
{
    RBSNode* parent, * gparent;

    // �������ڵ���ڣ����Ҹ��ڵ����ɫ�Ǻ�ɫ��
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //�������ڵ㡱�ǡ��游�ڵ�����ӡ�
        if (parent == gparent->left)
        {
            // Case 1����������ڵ��Ǻ�ɫ
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

            // Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
            if (parent->right == node)
            {
                RBSNode* tmp;
                rbstree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӡ�
            rb_set_black(parent);
            rb_set_red(gparent);
            rbstree_right_rotate(root, gparent);
        }
        else//����z�ĸ��ڵ㡱�ǡ�z���游�ڵ���Һ��ӡ�
        {
            // Case 1����������ڵ��Ǻ�ɫ
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

            // Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ�������
            if (parent->left == node)
            {
                RBSNode* tmp;
                rbstree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ��ӡ�
            rb_set_black(parent);
            rb_set_red(gparent);
            rbstree_left_rotate(root, gparent);
        }
    }

    // �����ڵ���Ϊ��ɫ
    rb_set_black(root->node);
}


static void rbstree_insert(RBSRoot* root, RBSNode* node)
{
    RBSNode* y = NULL;
    RBSNode* x = root->node;

    // 1. �����������һ�Ŷ�������������ڵ���ӵ�����������С�
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
            y->left = node;                // ���2������node��������ֵ�� < ��y��������ֵ������node��Ϊ��y�����ӡ�
        else
            y->right = node;            // ���3��(��node��������ֵ�� >= ��y��������ֵ��)��node��Ϊ��y���Һ��ӡ�
    }
    else
    {
        root->node = node;                // ���1����y�ǿսڵ㣬��node��Ϊ��
    }

    // 2. ���ýڵ����ɫΪ��ɫ
    node->color = RED;

    // 3. ������������Ϊһ�Ŷ��������
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
    p->color = BLACK; // Ĭ��Ϊ��ɫ

    return p;
}

RBSNode* rbts_insertVal(RBSRoot* root, char* key)
{
    RBSNode* node;    // �½����

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


//ģʽ1�Ǿ�ȷ������2���ҵ�һ������ֵ��3���ҵ�һ����Сֵ
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