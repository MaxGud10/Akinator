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
    int shoot_free;
};

struct Buffer
{
    char* buffer;
    char* current;
};

Node* new_node        (const char* data, Node* parent); // TODO make parent (при создании)
Node* guesse_word     (struct Node* node);
struct Node* add_info (struct Node* node);
void clean_buffer     (void);

Node* insert_new_node (struct Node* node);
Node* insert_node     (struct Node* node);


void preorder  (struct Node* node, FILE* graph_dump, struct Node* selection);
void inorder   (struct Node* node);
void postorder (struct Node* node);  

int write_data             (struct Node* node);
int print_preorder_in_file (struct Node* node, FILE* ddd, int level);

int graph_dump (struct Node* node, struct Node* selection); // TODO сделать дамп, который выводит только data (без слово data)

Node* read_data (FILE* baza, struct Buffer* buffer);
Node* read_node (int level, struct Buffer* buffer);

Node* find_object (struct Node* node, const char* search);
Node* find_nodes  (struct Node* node, const char* search);

int comparison (struct Node* node);
void compare_definitions (Node* root, const char* name1, const char* name2);