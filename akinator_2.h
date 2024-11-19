#pragma once 

typedef int Node_t;



struct Node
{
    const char* data; 
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int shoot_free; // TODO should rename
};

struct Buffer
{
    char* buffer;
    char* current;
};

int   select_mode    (struct Node* node);
Node* new_node       (const char* data, Node* parent); 
void  tree_dtor      (struct Node* node);
int   buffer_dtor    (struct Buffer* buffer);

/*===========/akinator_game/=================================*/
Node* akinator_game (struct Node* node);
Node* add_info      (struct Node* node);
void  clean_buffer  (void);

/*===========/definition/=====================================*/
int    create_definition         (struct Node* node);
size_t my_getline                (char** lineptr, size_t* /* n */, FILE* stream);
Node*  find_node                 (const char* object, struct Node* node);
int    print_definition          (struct Node* node, struct Stack* stack);
void   print_stack_nodes_reverse (struct Stack* stack);

/*==========/comparation/======================================*/
int   create_comparison        (struct Node* node);
Node* find_node                (const char* object, struct Node* node);
void  print_comparison         (struct Node* node_1, struct Node* node_2, struct Stack* stack_1, struct Stack* stack_2);
void  push_to_stack_until_root (struct Stack* stack, struct Node* node);
void  compare_and_print        (struct Stack* stack_1, struct Stack* stack_2, struct Node* node_1_old, struct Node* node_2_old);
void  do_print_definition      (struct Stack* stack); // void   print_stack_nodes_reverse (struct Stack* stack);

/*==========/reader/===========================================*/
Node* read_data (FILE* baza, struct Buffer* buffer);
Node* read_node (int level,  struct Buffer* buffer);


int write_data             (struct Node* node);
int print_preorder_in_file (struct Node* node, FILE* ddd, int level);

int  graph_dump (struct Node* node, struct Node* selection); // TODO сделать дамп, который выводит только data (без слово data)
void preorder   (struct Node* node, FILE* graph_dump, struct Node* selection);
void stack_dump (struct Stack* stack);


// Node* find_object (struct Node* node, const char* search);
// Node* find_nodes  (struct Node* node, struct Stack* stack, const char* search);

// void compare_definitions (Node* root, const char* name1, const char* name2);
// int  min_size_node       (struct Stack* path1, struct Stack* path2);
// void filling_the_stack   (struct Node* node, struct Stack* stack);