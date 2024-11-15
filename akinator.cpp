#define DEBUG

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

#ifdef DEBUG
    #define DBG(...) __VA_ARGS__
#else
    #define DBG(...)
#endif

int main (void)
{
    struct Stack stack = {};

    // stack_ctor (&stack, 20);

    // stack_push (&stack, 10);  

    // Stack_Elem_t x = stack_pop (&stack);

    // printf ("x = %d", x);

    // stack_dtor (&stack);

    // return 0;

    FILE* baza = fopen ("baza.txt", "r"); // TODO: передавать через cmd
    assert (baza); // TODO: по другому обрабатывать
    struct Buffer buffer = {};


    printf ("\n>>>> ddlx\n");

    struct Node* node = read_data (baza, &buffer);
    printf ("baza");

    fprintf (stderr, "%s(): read_data() returned node = %p\n", __func__, node);

    fprintf (stderr, "\n\nnode->data = '%s' | %p\n\n", node->data, node->data);

    //find_object (node, "Yuri_Dmitrievich");

    compare_definitions (node, "Yuri_Dmitrievich", "ded");
    // guesse_word (node);

    // write_data (node);

    graph_dump (node, NULL);

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

    printf("\n%s(): sizeof (node) = %ld, node->data = %s | %p \n", __func__, sizeof (node), node->data, node->data);

    return node;
}

// Node* guesse_word (struct Node* node)
// {
//     //Node* root = node;

//     char data[100] = "";

//     while (1) 
//     {
//         printf("(%d) is it %s this? (yes/no)", __LINE__, node->data);
//         scanf("%s", data); 

//         if (strcmp(data, "yes") == 0) 
//         {
//             if (node->left != NULL) 
//             {
//                 node = node->left;
//             } 

//             else 
//             {
//                 printf (" (%d) Do you want to insert a new object %s? (yes/no)\n", __LINE__, node->data);

//                 scanf ("%s", data);

//                 if (strcmp(data, "yes") == 0)
//                 {
//                     printf("(%d) LEFT: I do not know! Let's add obj in my baza | node->data = '%s'\n", __LINE__, node->data);

//                     node->left = insert_new_node (node);
//                     printf ("\nnode->data = %s\n", data);
//                     break;
//                 }

//                 else
//                 {
//                     printf ("\nI told you so! You're a leather bag Danka\n");
//                     break;
//                 }
//             }
//         } 

//         else 
//         {
//             if (node->right != NULL) 
//             {
//                 node = node->right;
//             } 

//             else 
//             {
//                 printf ("(%d) Do you want to insert a new object %s? (yes/no)\n", __LINE__, node->data );

//                 scanf ("%s", data);
//                 printf ("\n(%d) 2_scanf\n", __LINE__);

//                 if (strcmp(data, "yes") == 0)
//                 {
//                     printf("(%d) RIGHT: I do not know! Let's add %s in my knowledge.\n", __LINE__, data);

//                     node->right = insert_new_node (node);
//                     printf ("\ndata = %s\n", data);
//                     break;
//                 }         

//                 else
//                 {
//                     printf ("\nI told you so! You're a leather bag Danka\n");
//                     break;
//                 }       
//             }
//         }
//     }

//     return node;
// }

// Node* insert_new_node (struct Node* node) // Node* insert_new_node (struct Node* node)
// {
//     printf ("\n(%d)Who was it ?\n", __LINE__);

//     char data[100] = "";

//     scanf ("%s", data);
//     printf("\n(%d) >>> ddlx\n", __LINE__);

//     node->left  = new_node (data);
//     printf ("\nWhat is the difference betwen %s and %s. %s is......\n", node->left,);
//     node->right = new_node (data);

//     //return node;
//     return 0;
// }

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

// исправь эту ошибку не исправляя функцию find_nodesто есть можешь использовать функции find_nodes и  find_object, но их не исправляй
void compare_definitions(Node* root, const char* name1, const char* name2) 
{
    struct Stack path1 = {};
    struct Stack path2 = {};

    stack_ctor (&path1, 20);
    stack_ctor (&path2, 20);

    find_object (root, name1);
    find_object (root, name2);

    // Get paths for each object
    Node* node1 = root;
    Node* node2 = root;

    find_nodes (node1, name1);
    find_nodes (node2, name2);

    // if (path1.size == 0 && path2.size == 0) 
    // {
    //     printf("Both paths are empty. Cannot compare definitions.\n");
    //     stack_dtor(&path1);
    //     stack_dtor(&path2);
    //     return; // Выход из функции, если оба пути пустые
    // }

    // else if (path1.size == 0) 
    // {
    //     printf("Path for '%s' is empty. Cannot compare definitions.\n", name1);
    //     stack_dtor(&path1);
    //     stack_dtor(&path2);
    //     return; // Выход из функции, если только path1 пуст
    // }
    
    // else if (path2.size == 0) 
    // {
    //     printf("Path for '%s' is empty. Cannot compare definitions.\n", name2);
    //     stack_dtor(&path1);
    //     stack_dtor(&path2);
    //     return; // Выход из функции, если только path2 пуст
    // }


    printf("\nThe general part:\n");
    int general_part_length = 0;

    printf ("\npath1.size = %d |  path2.size = %d\n", path1.size, path2.size);
    int min_path_length = path1.size < path2.size ? path1.size : path2.size;
    printf ("\nmin_path_length = %d\n", min_path_length);

    for (int i = 0; i < min_path_length; i++) 
    {
        Node* num1 = look_number (&path1, i);
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



// //функция сравнения определений 
// int comparison (struct Node* node)
// {
//     if (node == NULL)
//     {
//         fprintf (stderr, "\n(%d)%s(): ERROR node == NULL\n");
//         return 0;
//     }

//     char* object_1 = NULL;
//     char* object_2 = NULL;

//     //printf ("\nchoose which objects you want to compare\n"); 

//     printf ("enter the first object: ");
//     getline (&object_1, &size_max, stdin);

//     size_t size_1 = strlen (object_1); //FIXME - input processing
//     assert (size_1); //FIXME

//     if (object_1[size_1 - 1] == '\n')
//         object_1[size_1 - 1] =  '\0';



//     return 0;
// }

// определение 
Node* find_object (struct Node* node, const char* search) // TODO: сделать, чтобы пользователь вводил кого надо найти, а не параметром 
{
    if (node == NULL)
        return 0;


    if (strcmp (node->data, search) == 0)
    {

        find_nodes (node, search);
    }

    else
    {
        printf ("\nmeet %s but this is not %s\n", node->data, search);
    }

    //STACK_CTOR (&stack, 50);

    Node* found_left = find_object (node->left, search);
    if (found_left != NULL)
        return node;

    Node* found_right = find_object (node->right, search);
    if (found_right != NULL)
        return node;

    return NULL;
}

Node* find_nodes (struct Node* node, const char* search)
{
    assert (node);
           //Node* reverse [20] = {};
    struct Stack stack = {};

    stack_ctor (&stack, 20);

    printf ("\nI'm find '%s'\n", search);

    Node* cur_node = node;

    int count = 0; 
    while (cur_node != NULL)
    {
        assert (count < 20);
        //reverse[count] = cur_node;
        count++;

        stack_push (&stack, cur_node); 
        cur_node = cur_node->parent;
    }

    printf ("\ncount = %d\n", count);
    for (int i = count - 1; i >= 1; i--) // - 1 / + 1
    {
        //fprintf (stderr, "\ni = %d reverse[i] = %p reverse[i]->parent = %p", i, reverse[i], reverse[i]->parent);
        Node* num   = look_number (&stack, i);
        Node* num_1 = look_number (&stack, i - 1);

        //fprintf (stderr, "\ni =%d| num = %p(%s), num_1 = %p(%s) , \n", i, num, num->data,num_1, num_1->data);

        if (num  != NULL &&
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
    
    stack_dtor (&stack);

    return node;
}


// Node* read_file (struct Buffer* buffer, FILE* baza, int data) // int .. 
// {
//     assert (buffer);

//     printf ("\n");

//     int n = 0; // ! -1
//     sscanf (buffer->current, " { %n", &n);
//     if (n = 0)
//     {
//         printf ("No '{' found. Return NULL.\n");
//         return NULL;
//     }

//     buffer->current += n // Перемещаем указатель текущей позиции в буфере 

//     зкштеа ()

//     // char buffer [100] = "";

//     int skip = skip_space (buffer);

//     if (skip != 0)
//     {
//         new_node (buffer); / /?

//     }


// }

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


// void freeTree(Node* node) 
// {
//     if (node == NULL) 
//         return;
//     free (node->left);
//     free (node->right);
//     free (node->data); // Не забываем освободить строку
//     free (node);
// }
