#pragma once 

typedef int Node_t;

struct Tree
{
    Node_t* root;
};

struct Node
{
    const char* data; 
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};

struct Buffer
{
    char* buffer;
    char* current;
};

Node* new_node    (const char* data);
Node* guesse_word (struct Node* node);

Node* insert_new_node (struct Node* node);
Node* insert_node (struct Node* node);

// x

void preorder  (struct Node* node, FILE* graph_dump, struct Node* selection);
void inorder   (struct Node* node);
void postorder (struct Node* node);  

int write_data             (struct Node* node);
int print_preorder_in_file (struct Node* node, FILE* ddd, int level);

int graph_dump (struct Node* node, struct Node* selection);

Node* read_data (FILE* baza, struct Buffer* buffer);
Node* read_node (int level, struct Buffer* buffer);