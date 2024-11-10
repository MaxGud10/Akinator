#pragma once 

typedef int Node_t;

struct Tree
{
    Node_t* root;
};

struct Node
{
    int data; //
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};

Node* new_node (int data);
Node* insert_node(struct Node* node, int data);
void preorderTraversal(struct Node* node);
void inorderTraversal(struct Node* node); 

void preorder  (struct Node* node, FILE* graph_dump, struct Node* selection);
void inorder   (struct Node* node);
void postorder (struct Node* node);  

int graph_dump (struct Node* node, struct Node* selection);