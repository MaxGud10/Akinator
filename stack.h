#pragma once

struct Node; 
typedef Node* Stack_Elem_t; // typedef unsigned long long Stack_Elem_t;

struct Stack // TODO добавить уже написанный стек. Подключать хедер
{
    int size;
    int capacity;
    Stack_Elem_t *data; 
};

int stack_ctor  (struct Stack *stack, int capacity);
void stack_dtor (struct Stack *stack);

int          stack_push (struct Stack *stack, Stack_Elem_t value);
Stack_Elem_t stack_pop  (struct Stack *stack);

int stack_is_empty (struct Stack *stack);

Stack_Elem_t look_number (struct Stack *stack, int num);