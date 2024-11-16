//#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdint.h> 

#include "log.h"
#include "akinator.h"
#include "stack.h"
#include "color.h"

#ifdef DEBUG
    #define DBG(...) __VA_ARGS__
#else
    #define DBG(...)
#endif

int main (void)
{
    struct Stack stack = {};

    FILE* baza = fopen ("baza.txt", "r"); // TODO: передавать через cmd
    assert (baza); // TODO: по другому обрабатывать
    struct Buffer buffer = {};

    struct Node* node = read_data (baza, &buffer);
    printf ("\n\nbaza\n\n");

    DBG (fprintf (stderr, "%s(): read_data() returned node = %p\n", __func__, node));

    DBG (fprintf (stderr, "\n\nnode->data = '%s' | %p\n\n", node->data, node->data));

    //find_object (node, "Yuri_Dmitrievich");

    compare_definitions (node, "Yuri_Dmitrievich", "ded");

    // guesse_word (node);

    // write_data (node);

    graph_dump (node, NULL);

    tree_dtor (node);
    buffer_dtor (&buffer);
    return 0;
}

//создание нового узда в дереве 
Node* new_node (const char* data, Node* parent) // Node* ne_node (Node* parent, int data)
{
    Node* node = (Node*) calloc (1, sizeof (*node)); // TODO: писать название типа
    if (node == NULL) 
    {
        printf("%s(): Error allocating memory\n", __func__);
        return NULL;
    }

    node->data  = strdup (data);
    if (node->data == NULL) 
    {
        printf("%s(): Error duplicating data \n", __func__);
        free(node); 
        return NULL;
    }
    node->right  = NULL;
    node->left   = NULL;
    node->parent = parent;

    node->shoot_free = 0;

    //printf("\n%s(): sizeof (node) = %ld, node->data = %s | %p \n", __func__, sizeof (node), node->data, node->data);

    return node;
}

void clean_buffer(void) 
{
    while ((getchar()) != '\n') { ; }
}

struct Node* add_info(struct Node* node) 
{
    struct Node* ptr_left  = new_node("0", node);
    struct Node* ptr_right = new_node("0", node);

    node->left  = ptr_left;
    node->right = ptr_right;

    char new_object[100];
    printf("Who was it?\n");
    scanf("%s", new_object);
    ptr_left->data = strdup(new_object);
    ptr_left->shoot_free = 1;

    ptr_right->data = strdup(node->data);
    if (node->shoot_free == 1)
        ptr_right->shoot_free = 1;

    char diff_object[200];
    printf("What is the difference between %s and %s?\n", ptr_left->data, ptr_right->data);
    scanf(" %[^\n]", diff_object);

    node->data = strdup(diff_object);
    node->shoot_free = 1;

    return node;
}

struct Node* guesse_word(struct Node* node) 
{
    char data[100]  = {};

    while (1) 
    {
        printf ("Is it %s? (yes/no)\n", node->data);
        scanf ("%s", data);

        if (strcmp (data, "yes") == 0 || strcmp (data,"y") == 0) 
        {
            if (node->left != NULL) 
            {
                node = node->left;
            } 

            else 
            {

                printf ("\n\n");
                printf ("\nI told you so! You are a weak lump of snot\n");
                break;
                // printf("Do you want to insert a new object for %s? (yes/no)\n", node->data);
                // scanf("%s", data);

                // if (strcmp(data, "yes") == 0) 
                // {
                //     printf("I do not know! Let's add an object to the tree.\n");
                //     node = add_info(node);
                //     break;
                // } 

                // else 
                // {
                //     printf("I told you so! You're mistaken.\n");
                //     break;
                // }
            }
        } 

        else // data = no (Is it %s?)
        {
            if (node->right != NULL) 
            {
                node = node->right;
            } 

            else 
            {
                printf("Do you want to insert a new object for %s? (yes/no)\n", node->data);
                scanf("%s", data);

                if (strcmp(data, "yes") == 0) 
                {
                    printf("I do not know! Let's add an object to the tree.\n");
                    node = add_info(node);
                    break;
                }
                
                else 
                {
                    printf("I told you so! You're mistaken.\n");
                    break;
                }
            }
        }
    }

    return node;
}

int write_data (struct Node* node)
{
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
        printf("Error: invalid buffer\n");
        return NULL;
    }

    DBG ( printf ("\n"); )
    DBG ( INDENT; printf ("Starting read_node(). Cur = %.40s..., [%p]. buffer = [%p]\n", buffer->current,  buffer->current, buffer->buffer); )

    int n = -1; // TODO: название получше
    sscanf (buffer->current, " { %n", &n);
    if (n < 0) 
    { 
        DBG ( INDENT; printf ("No '{' found. Return NULL.\n"); ) 
        return NULL; 
    }

    buffer->current += n;

    DBG ( INDENT; printf ("Got an '{'. Creating a node. Cur = %.40s..., [%p]. buffer = [%p]\n", buffer->current,  buffer->current, buffer->buffer); )

    Node* node = new_node ("", NULL); // !?
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
        DBG ( INDENT; printf ("No DATA found. Return NULL.\n"); ) 
        return NULL; 
    }

    if (bgn < 0 || end < 0 || end < bgn) // проверка что не выходим за пределы buffer
    {
        printf("\nInvalid bounds for data. Return NULL.\n");
        return NULL;
    }

    *(buffer->current + end) = '\0';
    node->data = buffer->current + bgn;

    DBG ( INDENT; printf ("Got a NAME: '%s'. Cur = %.40s..., [%p]. buffer = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )

    buffer->current += n;

    DBG ( INDENT; printf ("Shifted CURRENT_PTR: '%s'. Cur = %.40s..., [%p]. buffer = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )

    n = -1;
    char chr = '\0';
    sscanf (buffer->current, " %c %n", &chr, &n);
    if (n < 0) 
    { 
        DBG ( INDENT; printf ("No ending symbol (1) found. Return NULL.\n"); ) 
        return NULL; 
    }

    if (chr == '}')
    {
        buffer->current += n;
        //DBG ( INDENT; printf ("Got a '}', SHORT Node END (data = '%s'). Return node. Cur = %.40s..., [%p]. buffer_ptr = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )
        return node;
    }

    DBG ( INDENT; printf ("'}' NOT found. Supposing a left/right subtree. Reading left node. Cur = %.40s..., [%p]. buffer_ptr = [%p]\n", buffer->current, buffer->current, buffer->buffer); )

    DBG (printf ("\nnode->left = %p\n", node->left);)
    node->left = read_node (level + 1, buffer);
    if (node->left == NULL)
    {
        printf ("\n(%d) node->left == NULL\n", __LINE__);
        return NULL;
    }

    node->left->parent = node;  // передаем parent предыдущего узла 

    DBG ( INDENT; printf ("\n" "LEFT subtree read. Data of left root = '%s'\n\n", node->left->data); )

    DBG ( printf ("Reading right node. Cur = %.40s...\n", buffer->current); )

    node->right = read_node (level + 1, buffer);
    if (node->right == NULL)
    {
        printf ("\n(%d) node->right == NULL\n", __LINE__);
        return NULL;
    }

    node->right->parent = node; 

    DBG ( INDENT; printf ("\n" "RIGHT subtree read. Data of right root = '%s'\n", node->right->data); )

    chr = '\0';
    sscanf (buffer->current, " %c %n", &chr, &n);
    if (n < 0) 
    { 
        DBG ( INDENT; printf ("No ending symbol (2) found. Return NULL.\n"); ) 
        return NULL; 
    }

    if (chr == '}') // ?
    {
        buffer->current += n;

        DBG ( INDENT; printf ("Got a '}', FULL Node END (data = '%s'). Return node. Cur = %.40s..., [%p]. buffer_ptr = [%p]\n", node->data, buffer->current, buffer->current, buffer->buffer); )

        return node;
    }

    DBG (printf ("\n\n(%d)%s(): I return %p\n\n", __LINE__, __func__, node));
    return node;

    //return NULL;
}

void compare_definitions(struct Node* node, const char* name1, const char* name2) 
{
    struct Stack path1 = {};
    struct Stack path2 = {};

    stack_ctor (&path1, 20);
    stack_ctor (&path2, 20);

    // Node* node1 = NULL;
    // Node* node2 = NULL;

    Node* node1 = find_object (node, name1);
    Node* node2 = find_object (node, name2);

    // printf (GREEN_TEXT("\nnode1")" = %p|" LIGHT_BLUE_TEXT("node2") = %p\nmane1 = '%s' | name2 = '%s'"", node1, node2, name1, name2);
    // // printf(RED "\nnode1 = %p | node2 = %p\nname1 = '%s' | name2 = '%s'" CLEAR_COLOR,
    // //               node1, node2, name1, name2);

    // printf("\n"GREEN_TEXT("node1")" = %p | "LIGHT_BLUE_TEXT("node2") " = %p\n"
    //            GREEN_TEXT("mane1")" = '%s' | "LIGHT_BLUE_TEXT("name2") " = '%s'" CLEAR_COLOR, 
    //            node1, node2, name1, name2);

    printf("\n" GREEN_TEXT("node1") " = %p     | " LIGHT_BLUE_TEXT("node2") " = %p\n"
                GREEN_TEXT("mane1") " = '%s' | " LIGHT_BLUE_TEXT("name2") " = '%s'" CLEAR_COLOR, 
                node1, node2, name1, name2);


    // наполнить path1, parth2 узлами от корня до корня 
    filling_the_stack (node, &path1);
    stack_dump (&path1);
    filling_the_stack (node, &path2);
    stack_dump (&path2);

    // надо выводить стек который мы

    // функцию распечатки стека // size | от 0 до size 

    printf("\nThe general part:\n");
    int general_part_length = 0;

    printf ("\npath1.size = %d | path2.size = %d\n", path1.size, path2.size);
    int min_path_lenght = min_size_node (&path1, &path2);
    printf ("\nmin_path_length = %d\n", min_path_lenght);

    for (int i = 0; i < min_path_lenght; i++) 
    {
        Node* num1 = look_number (&path1, i);// TODO node
        Node* num2 = look_number (&path2, i);

        fprintf (stderr, "\n(num1 = %p | num2 = %p) num1->data = '%s' | num2->data = '%s'\n", num1, num2, num1->data, num2->data);

        if (strcmp(num1->data, num2->data) == 0) 
        {
            printf ("%s ", num1->data);
            general_part_length = i + 1;
        } 
        else 
        {
            break;
        }
    }

    printf("\n1) The different part:\n");
    for (int i = general_part_length; i < path1.size; i++) 
    {
        Node* num = look_number (&path1, i);
        fprintf (stderr, "%s ", num->data);
    }

    printf("\n2) The different part:\n");
    for (int i = general_part_length; i < path2.size; i++) 
    {
        Node* num = look_number(&path2, i);
        fprintf(stderr, "%s ", num->data);
    }

    printf("\n");

    stack_dtor(&path1);
    stack_dtor(&path2);
}

void filling_the_stack (struct Node* node, struct Stack* stack) 
{
    printf ("\n\n>>> %s(): Starting...\n", __func__);

    while (node != NULL) 
    {
        printf ("\n<parent = %p | data = '%s'>\n", node->parent, node->data);

        stack_push (stack, node);
        
        node = node->parent;
    } 

    printf ("<<< %s(): End\n", __func__);
}

void stack_dump (struct Stack* stack) 
{
    assert (stack);
    assert (stack->nodes);
    assert (stack->size);

    printf("Size of stack: %d\n", stack->size);
    printf("Elements in stack:\n");

    for (int i = 0; i < stack->size; ++i) 
    {
        Node* node = stack->data[i];
        if (node) 
        {
            printf("Node %d:" YELLOW_TEXT("data =") RED_TEXT("'%s'\n"), i, node->data);
        } 
        else 
        {
            printf(RED_TEXT("Node %d: NULL\n"), i);
        }
    }
}

// void filling_the_stack (struct Node* node, struct Stack* stack)
// {
//     while (1)
//     {
//         stack_push (stack, node);
//         node = node->parent;

//         if (node->parent == NULL)
//             break;
//     }

//     stack_push (stack, node);
// }

//функция которая ищет минимальный размер двух ветвей дерева 
int min_size_node (struct Stack* path1, struct Stack* path2)
{
    assert (path1);
    assert (path2);

    int size1 = path1->size;
    int size2 = path2->size;

    return (size1 < size2) ? size1 : size2;
}

int min (int a, int b)
{
    return (a < b)? a : b;
}

// определение 
Node* find_object (struct Node* node, const char* search) // TODO: сделать, чтобы пользователь вводил кого надо найти, а не параметром 
{
    if (node == NULL)
        return 0;

    struct Stack stack = {};

    stack_ctor (&stack, 20);

    printf (PURPLE_TEXT(">>> %s():")"strcmp (%s, %s)...\n", __func__, node->data, search);

    if (strcmp (node->data, search) == 0)
    {
        printf (GREEN_TEXT(">>> %s():")"strcmp (...) == 0 !!!\n", __func__);
     
        find_nodes (node, &stack, search);

        return node;
    }

    else
    {
        printf (RED_TEXT(">>> %s():")"strcmp (...) != 0\n", __func__);

        printf ("\nmeet %s but this is not %s\n", node->data, search);
    }

    Node* found_left = find_object (node->left, search);
    if (found_left != NULL)
        return node;

    Node* found_right = find_object (node->right, search);
    if (found_right != NULL)
        return node;

    stack_dtor (&stack);

    return NULL;
}

Node* find_nodes (struct Node* node, struct Stack* stack, const char* search)
{
    assert (node);
           //Node* reverse [20] = {};
    // struct Stack stack = {};

    // stack_ctor (&stack, 20);

    printf ("\nI'm find '%s'\n", search);

    Node* cur_node = node;

    int count = 0; 
    while (cur_node != NULL)
    {
        assert (count < 20);
        //reverse[count] = cur_node;
        count++;

        stack_push (stack, cur_node); 
        cur_node = cur_node->parent;
    }

    printf ("\ncount = %d\n", count);
    for (int i = count - 1; i >= 1; i--) // - 1 / + 1
    {
        //fprintf (stderr, "\ni = %d reverse[i] = %p reverse[i]->parent = %p", i, reverse[i], reverse[i]->parent);
        Node* num   = look_number (stack, i);
        Node* num_1 = look_number (stack, i - 1);

        //fprintf (stderr, "\ni =%d| num = %p(%s), num_1 = %p(%s) , \n", i, num, num->data,num_1, num_1->data);

        if (num   != NULL &&
            num_1 != NULL &&
            num->right == num_1)
        {
            fprintf (stderr, "NOT");
        }

        // if (reverse[i]     != NULL &&  
        //     reverse[i - 1] != NULL && 
        //     reverse[i]->right == reverse[i - 1])
        // {
        //     fprintf (stderr, "not ");
        // }
        
        fprintf (stderr, "'%s\n\n'", num->data);
        //fprintf (stderr,"'%s'\n", reverse[i]->data);

    } 

    return node;
}

//вставка нового элемента в  дерево  
// Node* insert_node (struct Node* node) 
// {
//     Node* root = node;

//     char data[100] = "";

//     printf ("\nAnd who is this\n");
//     scanf ("%s ", data);

//     while (1)
//     {
//         if (data < node->data)
//         {
//             if (node->left != NULL)
//             {
//                 printf ("\nIt is %s\n", data);

//                 node = node->left;
//                 graph_dump (root, node);
//             }

//             else // if (node->left == NULL)
//             {
//                 printf ("\nAnd who is this\n");
//                 scanf ("%s ", data);

//                 node->left = new_node (data, );
//                 break;
//             }
//         }

//         else
//         {
//             if (node->right != NULL)
//             {
//                 printf ("\nIt is %s\n", data);

//                 node = node->right;

//                 graph_dump (root, node);
            
//             }
//             else // if (node->right == NULL)
//             {
//                 printf ("\nAnd who is this\n");
//                 scanf ("%s ", data);

//                 node->right = new_node (data);
//                 break;
//             }
//         }
//     }   
    
//     return node;
// }

// Обход в порядке Inorder
void inorder (struct Node* node) 
{
    if (node == NULL) 
        return;

    printf (" ( ");

    if (node->left) inorder (node->left);

    printf ("%s", node->data);

    if (node->right) inorder (node->right);

    printf (" ) ");
}

// Обход в порядке Postorder
void postorder(struct Node* node) 
{
    if (node == NULL) 
        return;

    printf (" ) ");

    if (node->left)  postorder (node->left);

    if (node->right) postorder (node->right);
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

// Обход в порядке Preorder
void preorder (struct Node* node, FILE* graph_dump, struct Node* selection) 
{
    if (node == NULL)
        return;

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

    // assert (node && "node is NULL in delete_sub_tree()\n");

    // node->parent = NULL;

    // if (node->left)  tree_dtor (node->left);

    // if (node->right) tree_dtor (node->right);

    // if (node->shoot_free == 1)
    //     free ((void*)node->data);

    // node->shoot_free = 0;

    // free (node);

    if (node == NULL) 
    {
        return;
    }

    node->parent = NULL;

    tree_dtor (node->left);
    tree_dtor (node->right);
    
    if (node->data != NULL) 
    {
        free((void*)node->data); 
    }

    node->data = NULL;
    
    free(node);
}

int buffer_dtor (struct Buffer* buffer)
{
    assert (buffer);

    buffer->current = NULL;

    free (buffer->buffer);

    return 0;
}