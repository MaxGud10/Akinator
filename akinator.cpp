#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"
#include "akinator.h"

int main (void)
{
    Node* new_node_1 = new_node ("animal?");

    Node* new_node_2 = new_node ("poltorashka");

    Node* new_node_3 = new_node ("conducts a discussion");

    Node* new_node_4 = new_node ("Burtsev");

    Node* new_node_5 = new_node ("leads the physics department"); //🐱‍🚀


    new_node_1->left = new_node_2; 
    new_node_1->right = new_node_3;

    new_node_3->left  = new_node_4;
    new_node_3->right = new_node_5;

    // Node *new_node_12 = new_node (12);

    // Node *new_node_5 = new_node (5);

    // Node *new_node_15 = new_node (15);

    // Node *new_node_17 = new_node (17);

    // Node *new_node_70 = new_node (70);

    // Node *new_node_60 = new_node (60);

    // new_node_50->left  = new_node_12;
    // new_node_50->right = new_node_70;

    // new_node_12->left  = new_node_5;
    // new_node_12->right = new_node_15;

    // new_node_15->right = new_node_17;

    // new_node_70->left  = new_node_60;    

    // insert_node (new_node_5, 10);

    // insert_node (new_node_5, 1);


    // insert_node (new_node_70, 100);

    graph_dump (new_node_1, NULL);


    return 0;
}

//создание нового узда в дереве 
Node* new_node (char* data) // Node* ne_node (Node* parent, int data)
{
    Node* node = (Node*) calloc (1, sizeof (*node));

    printf("\n%s(): sizeof (node) = %d\n", __func__, sizeof (node));

    node->data  = data;
    node->right = NULL;
    node->left  = NULL;

    return node;
}

//вставка нового элемента в  дерево  
Node* insert_node (struct Node* node, char* data) 
{
    Node* root = node;

    while (1)
    {
        if (data < node->data)
        {
            if (node->left != NULL)
            {
                node = node->left;
                graph_dump (root, node);
            }

            else // if (node->left == NULL)
            {
                node->left = new_node (data);
                break;
            }
        }

        else
        {
            if (node->right != NULL)
            {
                node = node->right;

                graph_dump (root, node);
            }
            else // if (node->right == NULL)
            {
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

    printf ("%d", node->data);

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

    printf("%d", node->data);
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
    
    //fprintf(graph_dump, " %d ", node->data);

    preorder (node->left,  graph_dump, selection);
    preorder (node->right, graph_dump, selection);

    fprintf (graph_dump,"node%p[style = filled, shape=Mrecord, fillcolor = \"#%06x\", label = \" { data = %s | %3p |  { left = %3p | right = %3p } }\"]\n", 
                         node, (selection == node ? 0xF08000 : 0xFFC0C0), node->data, node, node->left, node->right);

    fprintf (graph_dump, "\n");

    // fprintf (graph_dump, "\nnode%p->node%p\n", node, node->left);
    // fprintf (graph_dump, "\n\nnode%p->node%p\n\n", node, node->right);

    if (node->left != NULL)
    {
        fprintf (graph_dump, "\nnode%p->node%p\n", node, node->left);
        // fprintf (graph_dump, "\n\nnode%p->node%p\n\n", node, node->right);
    }

    if (node->right != NULL)
    {
        fprintf (graph_dump, "\n\nnode%p->node%p\n\n", node, node->right);
    }

    printf("\n)");
}