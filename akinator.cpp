#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "log.h"
#include "akinator.h"

int main (void)
{
    //Node* new_node_1 = new_node ("animal?");

    // insert_node (new_node_1, "keeping in the registry");

    // insert_node (new_node_1, "poltorashka");

    // insert_node (new_node_1, "quadrober");

    /*===================================================================================================================================*/

    Node* new_node_1 = new_node ("animal?");

    Node* new_node_2 = new_node ("poltorashka");

    Node* new_node_3 = new_node ("conducts a discussion");

    Node* new_node_4 = new_node ("Burtsev");

    Node* new_node_5 = new_node ("leads the physics department"); //🐱‍🚀


    new_node_1->left  = new_node_2; 
    new_node_1->right = new_node_3;

    new_node_3->left  = new_node_4;
    new_node_3->right = new_node_5;

    guesse_word (new_node_1);

    write_data (new_node_1);

    graph_dump (new_node_1, NULL);


    return 0;
}

//создание нового узда в дереве 
Node* new_node (const char* data) // Node* ne_node (Node* parent, int data)
{
    Node* node = (Node*) calloc (1, sizeof (*node));

    printf("\n%s(): sizeof (node) = %ld\n", __func__, sizeof (node));

    node->data  = strdup (data);
    // node->data  = data;
    node->right = NULL;
    node->left  = NULL;

    return node;
}

Node* guesse_word (struct Node* node)
{
    Node* root = node;

    char data[100] = "";

    while (1) 
    {
        printf("(%d) is it %s this? (yes/no)", __LINE__, node->data);
        scanf("%s", data); 
        printf("\n(%d) 0_scanf\n", __LINE__);

        if (strcmp(data, "yes") == 0) 
        {
            if (node->left != NULL) 
            {
                node = node->left;
            } 

            else 
            {
                printf (" (%d) Do you want to insert a new object %s? (yes/no)\n", node->left, __LINE__, node->data);

                scanf ("%s", data);
                printf ("\n1_scanf\n");

                if (strcmp(data, "yes") == 0)
                {
                    printf("(%d) LEFT: I do not know! Let's add obj in my knowledge | node->data = '%s'\n", __LINE__, node->data);

                    node->left = insert_new_node (node);
                    printf ("\nnode->data = %s\n", data);
                    break;
                }

                else
                {
                    printf ("\nI told you so! You're a leather bag Danka\n");
                    break;
                }
            }
        } 

        else 
        {
            if (node->right != NULL) 
            {
                node = node->right;
            } 

            else 
            {
                printf ("(%d) Do you want to insert a new object %s? (yes/no)\n", __LINE__, node->data );

                scanf ("%s", data);
                printf ("\n(%d) 2_scanf\n", __LINE__);

                if (strcmp(data, "yes") == 0)
                {
                    printf("(%d) RIGHT: I do not know! Let's add %s in my knowledge.\n", __LINE__, data);

                    node->right = insert_new_node (node);
                    printf ("\ndata = %s\n", data);
                    break;
                }         

                else
                {
                    printf ("\nI told you so! You're a leather bag Danka\n");
                    break;
                }       
            }
        }
    }

    return node;
}

Node* insert_new_node (struct Node* node) // Node* insert_new_node (struct Node* node)
{
    printf ("\n(%d)Who was it ?\n", __LINE__);

    char data[100] = "";

    scanf ("%s", data);
    printf("\n(%d) >>> ddlx\n", __LINE__);

    node->left  = new_node (data);
    node->right = new_node (data);

    //return node;
    return 0;
}

int write_data (struct Node* node)
{
    FILE* baza = fopen ("baza.txt", "w");
    if (baza == NULL)
    {
        printf ("\n%s(): ERROR open baza.txt\n", __func__);
    }

    //printf ()
    print_preorder (node, baza, 0);

    fclose (baza);

    return 0;
}

int print_preorder (struct Node* node, FILE* baza, int level)
{
    assert (node);

    fprintf (baza, "%s{ ", level * 4, "");

    fprintf (baza, "\"%s\" ", node->data);

    if (node->left)
        fprintf(baza, "\n");

    if (node->left) print_preorder (node->left, baza, level + 1);

    if (node->right) print_preorder (node->right, baza, level + 1);

    fprintf (baza, "%*s} \n", node->left ? level * 4 : 0, "");

    return 0;
}

//вставка нового элемента в  дерево  
Node* insert_node (struct Node* node) 
{
    Node* root = node;

    char data[100] = "";

    printf ("\nAnd who is this\n");
    scanf ("%s ", data);

    while (1)
    {
        if (data < node->data)
        {
            if (node->left != NULL)
            {
                printf ("\nIt is %s\n", data);

                node = node->left;
                graph_dump (root, node);
            }

            else // if (node->left == NULL)
            {
                printf ("\nAnd who is this\n");
                scanf ("%s ", data);

                node->left = new_node (data);
                break;
            }
        }

        else
        {
            if (node->right != NULL)
            {
                printf ("\nIt is %s\n", data);

                node = node->right;

                graph_dump (root, node);
            }
            else // if (node->right == NULL)
            {
                printf ("\nAnd who is this\n");
                scanf ("%s ", data);

                node->right = new_node (data);
                break;
            }
        }
    }   
    // Node* pointer = 0;
    // if (node == NULL)  // хуй 
    // {
    //     pointer = new_node (data);
    //     if (pointer == NULL)
    //     {
    //         printf ("\nERROR: new_node returned 0\n");
    //         return NULL;
    //     }
    //     return 0;
    // }
    
    // if (data < node->data) 
    // {
    //     node->left  = insert_node(pointer1, data);
    // }

    // else 
    //     node->right = insert_node(node->right, data);
    
    return node;
}

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

    // if (node->left) 
    // {
    //     printf("(");
    //     inorder(node->left);
    //     printf(")");
    // }

    // printf("%d", node->data);

    // if (node->right) 
    // {
    //     printf("(");
    //     inorder(node->right);
    //     printf(")");
    // }
}

// Обход в порядке Postorder
void postorder(struct Node* node) 
{
    if (node == NULL) 
        return;

    printf (" ) ");

    if (node->left)  postorder (node->left);

    if (node->right) postorder (node->right);

    // if (node->left) 
    // {
    //     printf("(");
    //     postorder(node->left);
    //     printf(")");
    // }

    // if (node->right) 
    // {
    //     printf("(");
    //     postorder(node->right);
    //     printf(")");
    // }

    //printf("%d", node->data);
}

int graph_dump (struct Node* node, struct Node* selection)
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

    // fprintf (graph_dump, "\nnode%p->node%p\n", node, node->left);
    // fprintf (graph_dump, "\n\nnode%p->node%p\n\n", node, node->right);

    printf("\n)");
}
