//#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdint.h> 

#include "log.h"
#include "akinator_2.h"
#include "stack.h"
#include "color.h"

#ifdef DEBUG
    #define DBG(...) __VA_ARGS__
#else
    #define DBG(...)
#endif

#define CHECK_NODE_1(node)                                            \
    if ((node) == NULL)                                               \
    {                                                                 \
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n",       \
                __LINE__, __func__);                                  \
        return -1;                                                    \
    }

#define CHECK_STACK_1(stack)                                          \
    if ((stack) == NULL)                                              \
    {                                                                 \
        printf (RED_TEXT("(%d)%s(): ") "ERROR: stack == NULL\n",      \
                __LINE__, __func__);                                  \
        return -1;                                                    \
    }

int main (void)
{
    struct Stack stack = {};

    FILE* baza = fopen ("baza.txt", "r"); // TODO: передавать через cmd
    assert (baza); // TODO: по другому обрабатывать
    struct Buffer buffer = {};

    struct Node* node = read_data (baza, &buffer);
    if (node == NULL)
    {
        printf ("\n" RED_TEXT("(%d)%s():") "ERROR node == NULL\n", __LINE__, __func__);
        return -1;
    }
    

    // akinator_game(node);
    select_mode (node);

    //compare_definitions (node, "Yuri_Dmitrievich", "ded");

    // guesse_word (node);

    // write_data (node);

    //create_definition (node);

    // create_comparison (node); // Todo 

    graph_dump (node, NULL);

    tree_dtor (node);
    buffer_dtor (&buffer);
    return 0;
}

// функция для выбора режима акинатора 
int select_mode (struct Node* node)
{
    CHECK_NODE_1(node);
    // if (node == NULL)
    // {
    //     printf (RED_TEXT("\n(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
    //     return -1;
    // }

    int mode = 1;
    while (mode)
    {
        printf (GREEN_TEXT("\n\nWhat do you wanna doing? "));
        printf (YELLOW_TEXT("[a]") GREEN_TEXT ("kinator game, ")
               YELLOW_TEXT ("[d]") GREEN_TEXT ("efinition, ")
               YELLOW_TEXT ("[c]") GREEN_TEXT ("omparison, ")
               YELLOW_TEXT ("[q]") GREEN_TEXT ("uit and write to database, ")
               YELLOW_TEXT ("[s]") GREEN_TEXT ("top\n\n"));

        int aswer_for_mode = getchar ();
        clean_buffer ();

        switch (aswer_for_mode)
        {
            case 'a':
            {
                akinator_game (node);
                break;
            }

            case 'd':
            {
                create_definition (node);
                break;
            }

            case 'c':
            {
                create_comparison (node);
                break;
            }

            case 'q':
            {
                write_data (node);
                break;
            }

            case 's':
            {
                mode = 0;
                break;
            }

            default:
            {
                printf (RED_TEXT("\n(%d)%s():") " case ERROR\n", __LINE__, __func__);
                break; 
            }
        }
    }

    return 0;
}

//функция создания нового узла в дереве 
Node* new_node (const char* data, Node* parent) 
{
    Node* node = (Node*) calloc (1, sizeof (*node)); 
    if (node == NULL) 
    {
        printf("\n%s(): Error allocating memory\n", __func__);
        return NULL;
    }

    if (data)
    {
        node->data = strdup (data);
        node->shoot_free = 1;
        DBG( printf (">>> %s(): node->data calloc'd = %p\n", __func__, node->data));

        if (node->data == NULL) 
        {
            DBG( printf("\n%s(): Error duplicating data \n", __func__));
            free(node); 
            return NULL;
        }
    }
    else
    {
        node->data = NULL;
        node->shoot_free = 0;
    }
 
    node->right  = NULL;
    node->left   = NULL;
    node->parent = parent;

    return node;
}

Node* akinator_game (struct Node* node)
{
    if (node == NULL)
    {
        printf (RED_TEXT("\n(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return NULL;
    }

    while (node && node->left && node->right)
    {
        printf (LIGHT_BLUE_TEXT("\"%s\"?")"\n", node->data);
        printf (PURPLE_TEXT("[y/n]")"\n");

        int answer = getchar ();
        clean_buffer ();       

        if      (answer == 'y') node = node->left;
        else if (answer == 'n') node = node->right;
        else printf (RED_TEXT("it isn't 'y' or 'n' --- error.")"\n");
    }

    printf (LIGHT_BLUE_TEXT("Is it the ")GREEN_TEXT("RIGHT ANSWER:")LIGHT_BLUE_TEXT(" \"%s\"?\n")PURPLE_TEXT("[y/n]")"\n", node->data);

    int final_answer = getchar ();
    clean_buffer ();

    if (final_answer == 'y')
    {
        printf ("\n" GREEN_TEXT("lol, it was so easy...") "\n\n");
        return node;
    }

    add_info (node);

    return node;
}

void clean_buffer(void) 
{
    while ((getchar()) != '\n') { ; }
}

Node* add_info (struct Node* node) 
{
    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return NULL;
    }

    struct Node* ptr_left  = new_node ("0", node);
    struct Node* ptr_right = new_node ("0", node);

    node->left  = ptr_left;
    node->right = ptr_right;

    char new_object[100];
    printf ("Who was it?\n");
    scanf  ("%s", new_object);
    ptr_left->data = strdup(new_object);
    ptr_left->shoot_free = 1;

    ptr_right->data = strdup (node->data);
    // if (node->shoot_free == 1)
    ptr_right->shoot_free = 1;

    char diff_object[200];
    printf ("What is the difference between %s and %s?\n", ptr_left->data, ptr_right->data);
    scanf (" %[^\n]", diff_object);

    node->data = strdup (diff_object);
    node->shoot_free = 1;

    return node;
}

// struct Node* guesse_word(struct Node* node) 
// {
//     char data[100]  = {};

//     while (1) 
//     {
//         printf ("Is it %s? (yes/no)\n", node->data);
//         scanf ("%s", data);

//         if (strcmp (data, "yes") == 0 || strcmp (data,"y") == 0) 
//         {
//             if (node->left != NULL) 
//             {
//                 node = node->left;
//             } 

//             else 
//             {

//                 printf ("\n\n");
//                 printf ("\nI told you so! You are a weak lump of snot\n");
//                 break;
//             }
//         } 

//         else // data = no (Is it %s?)
//         {
//             if (node->right != NULL) 
//             {
//                 node = node->right;
//             } 

//             else 
//             {
//                 printf("Do you want to insert a new object for %s? (yes/no)\n", node->data);
//                 scanf("%s", data);

//                 if (strcmp(data, "yes") == 0) 
//                 {
//                     printf("I do not know! Let's add an object to the tree.\n");
//                     node = add_info(node);
//                     break;
//                 }
                
//                 else 
//                 {
//                     printf("I told you so! You're mistaken.\n");
//                     break;
//                 }
//             }
//         }
//     }

//     return node;
// }

int create_definition (struct Node* node) // TODO: заменить node на root 
{
    CHECK_NODE_1(node);

    // if (node == NULL)
    // {
    //     printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
    //     return -1;
    // }

    char* object = NULL;
    size_t size_max = 0;

    printf (LIGHT_BLUE_TEXT("enter the object: "));
    my_getline (&object, &size_max, stdin);

    size_t size = strlen (object);
    assert (object);
    assert (size);

    if (object[size - 1] == '\n') // TODO func!!!!
        object[size - 1] =  '\0';

    DBG( fprintf (stderr, "%s\n", object); )

    Node* our_node = find_node (object, node);
    if (our_node == NULL)
    {
        printf ("\n" "'%s' -- no such node exists, " GREEN_TEXT("try another one") " or " GREEN_TEXT("add your own object.") "\n\n", object);
        free (object);
        return 1;
    }

    DBG( fprintf (stderr, "our_node = [%p]  our_node->data = %s\n", our_node, our_node->data); )

    DBG( fprintf (stderr, "object = %s, our_node->data = %s   \n", object, our_node->data); )

    struct Stack stack = {};
    stack_ctor (&stack, 10); // TODO stack_dtor (stack)

    fprintf (stderr, "\n\n" PURPLE_TEXT("DEFINITION: ") YELLOW_TEXT("%s is "), our_node->data);

    print_definition (our_node, &stack);

    fprintf (stderr, "\n\n");

    free (object);

    return 0;
}

size_t my_getline (char** lineptr, size_t* /* n */, FILE* stream)
{
    char buffer[25] = {};

    fgets (buffer, 25, stream);

    // \n

    *lineptr = strdup (buffer); 

    return strlen (*lineptr);
}

Node* find_node (const char* object, struct Node* node)
{
    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return NULL;
    }

    assert (node && "node is null");

    DBG( fprintf (stderr, "find_node starting...  node->data = '%s'    object = '%s'\n\n", node->data, object); )

    if (strcmp (object, node->data) == 0)
        return node;

    if (node->left)
    {
        DBG( fprintf (stderr, "going left: node->left->data = '%s'\n", node->left->data); )
        
        struct Node* node_left = find_node (object, node->left);
        if (node_left != NULL)
            return node_left;
    }

    if (node->right)
    {
        DBG( fprintf (stderr, "going right: node->right->data = '%s'\n", node->right->data); )
        
        struct Node* node_right = find_node (object, node->right);
        if (node_right != NULL)
            return node_right;
    }

    return NULL;
}

int print_definition (struct Node* node, struct Stack* stack)
{
    CHECK_NODE_1(node);
    // if (node == NULL)
    // {
    //     printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
    //     return -1;
    // }

    if (stack == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: stack == NULL\n", __LINE__, __func__);
        return -1;
    }

    while (1)
    {
        stack_push (stack, node);

        node = node->parent;

        if (node->parent == NULL)
            break;
    }

    stack_push (stack, node);

    printf ("\n" ">>> %s(): Stack of parents:\n", __func__);
    for (int i = 0; i < stack->size; i++)
        {
        Node* elem = look_number (stack, i);
        printf ("[%d] %p '%s'\n", i, elem, (elem? elem->data : "NULL elem"));
        }
    printf ("---\n");  // !!!

    print_stack_nodes_reverse (stack);

    return 0;
}

// функция выводит данные узлов стека в обратном порядке
//
void print_stack_nodes_reverse (struct Stack* stack) 
{
    if (stack == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: stack == NULL\n", __LINE__, __func__);
        return;
    }

    int i = stack->size - 1;

    while (1)
    {
        struct Node* node_curr  = look_number (stack, i); 
        if (node_curr == NULL)
        {
            printf ("\n" RED_TEXT("(%d)%s(): ERROR node_curr == NULL\n") "\n", __LINE__, __func__);
            return;
        }

        DBG( fprintf (stderr, "i = %d\n", i);
             fprintf (stderr, "\nnode_curr = [%p]\n", node_curr);
             fprintf (stderr, "node_curr->right = [%p]\n", node_curr->right); )

        struct Node* node_prev = look_number (stack, i - 1);
        if (node_prev == NULL)
        {
            printf ("\n" RED_TEXT("(%d)%s(): ERROR node_prev == NULL\n") "\n", __LINE__, __func__);
            return;
        }

        DBG( fprintf (stderr, "i = %d\n", i);
             fprintf (stderr, "node_prev = [%p]\n",  node_prev);
             fprintf (stderr, "node_prev->right = [%p]\n", node_prev->right); )

        DBG( fprintf (stderr, "<<< i = %d >>> if (node_curr->right = [%p, %s] =="
                                                   "node_prev      = [%p, %s] \n\n",
                    i, node_curr->right, node_curr->data,
                    node_prev, node_prev->data); )

        if (node_curr->right == node_prev)
            fprintf (stderr, YELLOW_TEXT("not a "));

        fprintf (stderr, YELLOW_TEXT("%s%c "), node_curr->data, (i != 1) ? ',' : '.');

        if (--i == 0)
            break;
    }

    stack_dtor (stack);
}

int create_comparison (struct Node* node)
{
    CHECK_NODE_1(node);

    // if (node == NULL)
    // {
    //     printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
    //     return -1;
    // }

    char* object_1 = NULL;
    char* object_2 = NULL;
    size_t size_max = 0;

    printf (LIGHT_BLUE_TEXT("enter the first object: "));
    my_getline (&object_1, &size_max, stdin);

    size_t size_1 = strlen (object_1); //FIXME - input processing
    assert (size_1); //FIXME

    if (object_1[size_1 - 1] == '\n')
        object_1[size_1 - 1] =  '\0';

    struct Node* node_1 = find_node (object_1, node);
    if (node_1 == NULL)
    {
        printf ("\n" "'%s' -- no such node exists, " GREEN_TEXT("try another one") " or " GREEN_TEXT("add your own object.") "\n\n", object_1);
        free (object_1);
        return 1;
    }

    printf (LIGHT_BLUE_TEXT("enter the second object: "));
    my_getline (&object_2, &size_max, stdin);

    size_t size_2 = strlen (object_2); //FIXME - input processing
    assert (size_2);

    if (object_2[size_2 - 1] == '\n')
        object_2[size_2 - 1] =  '\0';

    struct Node* node_2 = find_node (object_2, node);
    if (node_2 == NULL)
    {
        printf ("\n" "'%s' -- no such node exists, " GREEN_TEXT("try another one") " or " GREEN_TEXT("add your own object.") "\n\n", object_2);
        free (object_2);
        return 1;
    }

    struct Stack stack_1 = {}; stack_ctor (&stack_1, 10);
    struct Stack stack_2 = {}; stack_ctor (&stack_2, 10);

    print_comparison (node_1, node_2, &stack_1, &stack_2);

    free (object_1);
    free (object_2);

    return 0;
}

void print_comparison (struct Node* node_1, struct Node* node_2, struct Stack* stack_1, struct Stack* stack_2)
{
    if (node_1 == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node_1 == NULL\n", __LINE__, __func__);
        return;
    }

    if (node_2 == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node_2 == NULL\n", __LINE__, __func__);
        return;
    }

    if (stack_1 == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: stack_1 == NULL\n", __LINE__, __func__);
        return;
    }

    if (stack_2 == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: stack_2 == NULL\n", __LINE__, __func__);
        return;
    }

    fprintf (stderr, "\n\n" GREEN_TEXT("%s") PURPLE_TEXT(" and ") GREEN_TEXT("%s") PURPLE_TEXT(" are ") PURPLE_TEXT("BOTH: "),
             node_1->data, node_2->data);

    struct Node* node_1_old = node_1;
    struct Node* node_2_old = node_2;

    push_to_stack_until_root (stack_1, node_1); 
    push_to_stack_until_root (stack_2, node_2);

    compare_and_print(stack_1, stack_2, node_1, node_2);

    stack_dtor (stack_1);
    stack_dtor (stack_2);
}

void push_to_stack_until_root(struct Stack* stack, struct Node* node) 
{
    if (stack == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: stack == NULL\n", __LINE__, __func__);
        return;
    }

    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return;
    }

    while (node) 
    {
        stack_push(stack, node);
        node = node->parent;

        if (node->parent == NULL)
            break;
    }

    stack_push (stack, node);
}

void compare_and_print (struct Stack* stack_1, struct Stack* stack_2, struct Node* node_1_old, struct Node* node_2_old)
{
    int size_1 = stack_1->size - 1;
    int size_2 = stack_2->size - 1;

    struct Node* node_1_curr = NULL;
    struct Node* node_1_prev = NULL;
    struct Node* node_2_curr = NULL;
    struct Node* node_2_prev = NULL;

    while (true)
    {
        node_1_curr = look_number (stack_1, size_1);
        node_1_prev = look_number (stack_1, size_1 - 1);
        node_2_curr = look_number (stack_2, size_2);
        node_2_prev = look_number (stack_2, size_2 - 1);


        if ((stack_1->data[size_1] == stack_2->data[size_2]) && (node_1_prev == node_2_prev))
        {
            if (node_1_curr->right == node_1_prev && node_2_curr->right == node_2_prev)
                fprintf (stderr, YELLOW_TEXT("not a "));

            Stack_Elem_t x1 = stack_pop (stack_1);
            Stack_Elem_t x2 = stack_pop (stack_2);
            // stack_pop (stack_1, &x);
            // stack_pop (stack_2, &x);

            fprintf (stderr, YELLOW_TEXT("%s; "), node_1_curr->data);

            --size_1;
            --size_2;
        }
        else
        {
            break;
        }
    }

    fprintf (stderr, PURPLE_TEXT("But "));
    fprintf (stderr, GREEN_TEXT("%s")YELLOW_TEXT(" is "), node_1_old->data);
    print_stack_nodes_reverse (stack_1);

    fprintf (stderr, PURPLE_TEXT("And "));
    fprintf (stderr, GREEN_TEXT("%s")YELLOW_TEXT(" is "), node_2_old->data);
    print_stack_nodes_reverse (stack_2);

    fprintf (stderr, "\n\n");
}

int write_data (struct Node* node)
{
    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return -1;
    }

    FILE* baza = fopen ("baza.txt", "w");
    if (baza == NULL)
    {
        printf ("\n%s(): ERROR open baza.txt\n", __func__);
        return -1;
    }

    print_preorder_in_file (node, baza, 0); 

    fclose (baza);

    return 0;
}

int print_preorder_in_file (struct Node* node, FILE* baza, int level)
{
    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return -1;
    }

    assert (node);

    fprintf (baza, "%*s{ ", level * 4, "");

    fprintf (baza, "\"%s\" ", node->data);

    if (node->left)
        fprintf(baza, "\n");

    if (node->left)  print_preorder_in_file (node->left,  baza, level + 1);

    if (node->right) print_preorder_in_file (node->right, baza, level + 1);

    fprintf (baza, "%*s} \n", (node->left) ? level * 4 : 0, "");

    return 0;
}


Node* read_data (FILE* baza, struct Buffer* buffer)
{
    if (buffer == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: buffer == NULL\n", __LINE__, __func__);
        return NULL;
    }

    if (baza == NULL) 
    {
        printf("\nError: file pointer is NULL\n");
        return NULL;
    }

    struct stat st = {};
    if (fstat (fileno (baza), &st) == -1)
    {
        printf ("\nError getting file info\n");
        return NULL;
    }

    long file_size = st.st_size;

    buffer->buffer = (char*) calloc ((size_t) file_size + 1, sizeof(buffer->buffer[0]));
    printf (">>> %s(): buffer->buffer calloc'd = %p\n", __func__, buffer->buffer);
    if (buffer->buffer == NULL)
    {
        printf ("\nError allocating memory\n");
        return NULL;
    }

    DBG( printf ("\n\n\n\nbuffer->buffer = [%p]\n\n\n\n", buffer->buffer); )

    size_t count = fread (buffer->buffer, sizeof(buffer->buffer[0]), (size_t) file_size, baza);
    if (count != (size_t)file_size) // if ((long) count != file_size)
    {
        printf ("count = %zu != file_size = %ld", count, file_size);
        return NULL;
    }

    fclose (baza); // TODO: вынести до сюда в отдельную функцию (общая функция, мб из других проектов)
    buffer->current = buffer->buffer;

    Node* root = read_node(0, buffer);
    if (root == NULL) 
    {
        printf ("\n\n\n(%d)%s(): I discovered root == NULL\n", __LINE__, __func__);
        free(buffer->buffer);
    }

    printf ("\nI'll return it now %p\n", root);
    return root;
}

#define INDENT printf ("%*s", level*2, "")

Node* read_node (int level, struct Buffer* buffer)
{
    if (buffer == NULL || buffer->current == NULL) 
    {
        printf("(%d)%s(): Error: invalid buffer\n", __LINE__, __func__);
        return NULL;
    }

    DBG( printf ("\n"); )
    DBG( INDENT; printf ("Starting read_node(). Cur = %.40s..., [%p]. buffer = [%p]\n", buffer->current,  buffer->current, buffer->buffer); )

    int n = -1; // TODO: название получше
    sscanf (buffer->current, " { %n", &n);
    if (n < 0) 
    { 
        DBG( INDENT; printf ("No '{' found. Return NULL.\n"); ) 
        return NULL; 
    }

    buffer->current += n;

    DBG( INDENT; printf ("Got an '{'. Creating a node. Cur = %.40s..., [%p]. buffer = [%p]\n", buffer->current,  buffer->current, buffer->buffer); )

    Node* node = new_node (NULL, NULL); // !?
    if (node == NULL)
    {
        printf("\n\n(%d)%s(): new_node can not memory \n", __LINE__, __func__);
        return NULL;
    }

    n = -1;
    int bgn = 0, end = 0;
    sscanf (buffer->current, " \"%n%*[^\"]%n\" %n", &bgn, &end, &n);
    if (n < 0) 
    { 
        DBG( INDENT; printf ("No DATA found. Return NULL.\n"); ) 
        return NULL; 
    }

    if (bgn < 0 || end < 0 || end < bgn) // проверка что не выходим за пределы buffer
    {
        printf("\nInvalid bounds for data. Return NULL.\n");
        return NULL;
    }

    *(buffer->current + end) = '\0';

    node->data = buffer->current + bgn;
    node->shoot_free = 0;

    DBG( INDENT; printf ("Got a NAME: '%s'. Cur = %.40s..., [%p]. buffer = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )

    buffer->current += n;

    DBG( INDENT; printf ("Shifted CURRENT_PTR: '%s'. Cur = %.40s..., [%p]. buffer = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )

    n = -1;
    char chr = '\0';
    sscanf (buffer->current, " %c %n", &chr, &n);
    if (n < 0) 
    { 
        DBG( INDENT; printf ("No ending symbol (1) found. Return NULL.\n"); ) 
        return NULL; 
    }

    if (chr == '}')
    {
        buffer->current += n;
        return node;
    }

    DBG( INDENT; printf ("'}' NOT found. Supposing a left/right subtree. Reading left node. Cur = %.40s..., [%p]. buffer_ptr = [%p]\n", buffer->current, buffer->current, buffer->buffer); )

    DBG( printf ("\nnode->left = %p\n", node->left);)
    node->left = read_node (level + 1, buffer);
    if (node->left == NULL)
    {
        printf ("\n(%d) node->left == NULL\n", __LINE__);
        return NULL;
    }

    node->left->parent = node;  // передаем parent предыдущего узла 

    DBG( INDENT; printf ("\n" "LEFT subtree read. Data of left root = '%s'\n\n", node->left->data); )

    DBG( printf ("Reading right node. Cur = %.40s...\n", buffer->current); )

    node->right = read_node (level + 1, buffer);
    if (node->right == NULL)
    {
        printf ("\n(%d) node->right == NULL\n", __LINE__);
        return NULL;
    }

    node->right->parent = node; 

    DBG( INDENT; printf ("\n" "RIGHT subtree read. Data of right root = '%s'\n", node->right->data); )

    chr = '\0';
    sscanf (buffer->current, " %c %n", &chr, &n);
    if (n < 0) 
    { 
        DBG( INDENT; printf ("No ending symbol (2) found. Return NULL.\n"); ) 
        return NULL; 
    }

    if (chr == '}') 
    {
        buffer->current += n;

        DBG( INDENT; printf ("Got a '}', FULL Node END (data = '%s'). Return node. Cur = %.40s..., [%p]. buffer_ptr = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )

        return node;
    }

    DBG( printf ("\n\n(%d)%s(): I return %p\n\n", __LINE__, __func__, node));

    return node;
}

void stack_dump (struct Stack* stack) 
{
    if (stack == NULL)
    {
        printf ("\n" RED_TEXT("(%d)%s:") "ERROR stack == NULL\n", __LINE__, __func__);
        return;
    }

    if (stack->size)
    {
        printf ("\n" RED_TEXT ("(%d)%s():") "ERROR stack->size == NULL\n", __LINE__, __func__);
    }

    printf(PURPLE "Size of stack: %d\n" CLEAR_COLOR, stack->size);
    printf("Elements in stack:\n");

    for (int i = 0; i < stack->size; ++i) 
    {
        Node* node = stack->data[i];
        assert (stack->data);
        if (node) 
        {
            printf(BLUE_TEXT("Node ") " %d:" YELLOW_TEXT(" data") " = " RED_TEXT("'%s'\n"), i, node->data);
        } 

        else 
        {
            printf(RED_TEXT("Node %d: NULL\n"), i);
        }
    }
}

int graph_dump (struct Node* node, struct Node* selection) // TODO: сделать да/нет и покрасить детей в другой цвет 
{
    FILE* graph_dump = fopen ("graph_dump.dot", "w");
    if (graph_dump == NULL)
    {
        printf ("\nERROR open graph_dump\n");
        return -1;
    }

    fprintf (graph_dump, "digraph\n{\n");
    fprintf (graph_dump, "rankdir = \"TB\";\n");

    // fprintf(graph_dump, " %d ", node->data);
    fprintf (graph_dump, "\n");

    preorder (node, graph_dump, selection);

    fprintf (graph_dump, "}");

    fclose (graph_dump);

    system ("dot graph_dump.dot -T png -o graph_dump.png");

    //system ("mspaint graph_dump.png");

    return 0;
}

//Обход в порядке Preorder
void preorder (struct Node* node, FILE* graph_dump, struct Node* selection) 
{
    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return;
    }

    if (graph_dump == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: graph_dump == NULL\n", __LINE__, __func__);
        return;
    }

    printf(" ( ");

    fprintf (graph_dump, "node%p[style = filled, shape=Mrecord, fillcolor = \"#%06x\", label = \" { data = %s | %3p |  { left = %3p | right = %3p } }\"]\n", 
                          node, (selection == node ? 0xF08000 : 0xFFC0C0), node->data, node, node->left, node->right);

    if (node->left != NULL)
    {
        fprintf (graph_dump, "\nnode%p -> node%p\n", node, node->left);
        // fprintf (graph_dump, "\n\nnode%p->node%p\n\n", node, node->right);
    }

    if (node->right != NULL)
    {
        fprintf (graph_dump, "\n\nnode%p -> node%p\n\n", node, node->right);
    }

    fflush (graph_dump);

    //fprintf(graph_dump, " %d ", node->data);

    printf (" (%d) ", __LINE__);

    preorder (node->left,  graph_dump, selection);

    printf (" (%d) ", __LINE__);

    preorder (node->right, graph_dump, selection);

    printf (" (%d) ", __LINE__);

    printf("\n)");
}

void tree_dtor(struct Node* node) 
{
    if (node == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: node == NULL\n", __LINE__, __func__);
        return;
    }

    DBG( printf (">>> %s(): destructing %p (data = '%s', should_free = %d)...\n", __func__, node, node->data, node->shoot_free));

    node->parent = NULL;

    if (node->left)  tree_dtor (node->left);
    if (node->right) tree_dtor (node->right);
    
    if (node->shoot_free == 1)
    {
        DBG( printf (">>> %s(): node->data free'd = %p\n", __func__, node->data));
        
        free ((void*) node->data); 
        node->data = NULL;
        node->shoot_free = 0;
    }
    
    free(node);
}

int buffer_dtor (struct Buffer* buffer)
{
    if (buffer == NULL)
    {
        printf (RED_TEXT("(%d)%s(): ") "ERROR: buffer == NULL\n", __LINE__, __func__);
        return -1;
    }

    buffer->current = NULL;

    DBG( printf (">>> %s(): buffer->buffer free'd = %p\n", __func__, buffer->buffer));

    free (buffer->buffer);
    buffer->buffer = NULL;

    return 0;
}

// get_data () - возращает data но все проверяет ( если выздвал gedata (узел)  если я передал null то она и вренет null)
// set_dara - усанавливает значение data = чему-то там ( новое значени. но проверить что насчтоящее значеие основобождено) то есть если shoot_free для данного узла показ что надо освобоить data, то ее надо осн=вободить, перед присваиванием data.  дальше она принимет указательб строчку, надо ее strdup или нет  (иногда ее надо strdup а иногда нет). 