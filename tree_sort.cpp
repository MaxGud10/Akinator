#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"

typedef int Node_t;

struct Tree
{
    Node_t* root;
};

struct Node
{
    int data;
    struct Node* left;
    struct Node* right;
    //Node_t* parent; 
};

int new_node (struct Node** node, int data);

struct Node* insert_node(struct Node* node, int data);
void preorderTraversal(struct Node* node); 
void inorderTraversal(struct Node* node);
void postorderTraversal(struct Node* node); 


int main (void)
{
    struct Node* root = {};
    
    if (new_node(&root, 50) != 0)
        return -1;
    
    if (insert_node(root, 12) != 0)
        return -1;

    if (insert_node(root, 5) != 0)
        return -1;

    if (insert_node(root, 15) != 0)
        return -1;

    if (insert_node(root, 17) != 0)
        return -1;

    if (insert_node(root, 70) != 0)
        return -1;

    if (insert_node(root, 60) != 0)
        return -1;

    printf("Preorder traversal: ");
    preorderTraversal(root);
    printf("\n");

    printf("Inorder traversal: ");
    inorderTraversal(root);
    printf("\n");

    printf("Postorder traversal: ");
    postorderTraversal(root);
    printf("\n");

    return 0;
}

// Создание нового узла
Node* new_node (int data) // Node* ne_node (Node* parent, int data)
{
    Node* node = (Node*) calloc (1, sizeof (*node));

    printf("%s(): sizeof (node) = %d", __func__, sizeof (node));

    node->data = data;
    node->right = NULL;
    node->left = NULL;

    return node;
}
// int new_node(struct Node* node, int data) 
// {
//     *node = (struct Node*) calloc (1, sizeof(*node));
    
//     if (*node == NULL) 
//     {
//         log_printf("\nMemory error\n");
//         return -1; 
//     }
    
//     (*node)->data = data;
//     (*node)->left = NULL;
//     (*node)->right = NULL;

//     return node; 
// }

// Вставка нового узла в дерево
struct Node* insert_node(struct Node* node, int data) 
{
    if (node == NULL) 
    {
        new_node(&node, data);
        return node;
    }
    
    if (data < node->data) 
        node->left = insert_node(node->left, data);
    
    else 
        node->right = insert_node(node->right, data);
    
    return node;
}

// Обход в порядке Preorder
void preorderTraversal(struct Node* node) 
{
    if (node == NULL) return;
    
    printf("%d", node->data);
    if (node->left || node->right) 
    {
        printf("(");
        preorderTraversal(node->left);
        printf(")(");
        preorderTraversal(node->right);
        printf(")");
    }
}

// Обход в порядке Inorder
void inorderTraversal(struct Node* node) 
{
    if (node == NULL) return;

    if (node->left) 
    {
        printf("(");
        inorderTraversal(node->left);
        printf(")");
    }

    printf("%d", node->data);

    if (node->right) 
    {
        printf("(");
        inorderTraversal(node->right);
        printf(")");
    }
}

// Обход в порядке Postorder
void postorderTraversal(struct Node* node) 
{
    if (node == NULL) return;

    if (node->left) 
    {
        printf("(");
        postorderTraversal(node->left);
        printf(")");
    }

    if (node->right) 
    {
        printf("(");
        postorderTraversal(node->right);
        printf(")");
    }

    printf("%d", node->data);
}
