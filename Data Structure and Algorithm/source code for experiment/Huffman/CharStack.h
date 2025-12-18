#ifndef CHARSTACK_H
#define CHARSTACK_H

#include <iostream>
#define STACK_INIT_SIZE 100

typedef struct {
    char *base;    // Stack base pointer
    int top;       // Stack top pointer
    int stackSize; // Current stack size
} CharStack;

void InitStack_sq(CharStack &S, int msize);
bool StackEmpty_sq(CharStack &S);
bool StackFull_sq(CharStack &S);
void DestoryStack_sq(CharStack &S);
bool GetTop_sq(CharStack &S, char &e);
void Push_sq(CharStack &S, char e);
void Pop_sq(CharStack &S, char &e);
void StackTraverse_sq(CharStack &S);
void ClearStack_sq(CharStack &S);
int StackLength_sq(CharStack &S);
int StackFind_sq(CharStack &S, char e);

// Initialize the stack
void InitStack_sq(CharStack &S, int msize = STACK_INIT_SIZE)
{
    S.base = new char[msize];
    if (!S.base) {
        std::cerr << "Memory allocation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    S.top = -1; // Initialize top to -1 indicating stack is empty
    S.stackSize = msize;
}

// Check if the stack is empty
bool StackEmpty_sq(CharStack &S)
{
    return S.top == -1;
}

// Check if the stack is full
bool StackFull_sq(CharStack &S)
{
    return S.top >= S.stackSize - 1;
}

// Destroy the stack
void DestoryStack_sq(CharStack &S)
{
    delete[] S.base; // Free allocated memory
    S.top = -1;      // Reset top
    S.stackSize = 0; // Reset stack size
}

// Get the top element of the stack
bool GetTop_sq(CharStack &S, char &e)
{
    if (S.top == -1) {
        std::cerr << "Stack is empty!" << std::endl;
        return false; // Stack is empty
    }
    e = S.base[S.top]; // Get the top element
    return true;       // Successfully retrieved top element
}

// Push an element onto the stack
void Push_sq(CharStack &S, char e)
{
    if (S.top >= S.stackSize - 1) {
        std::cerr << "Stack overflow!" << std::endl;
        return; // Stack is full
    }
    S.base[++S.top] = e; // Increment top and push element
}

// Pop the top element from the stack
void Pop_sq(CharStack &S, char &e)
{
    if (S.top == -1) {
        std::cerr << "Stack underflow!" << std::endl;
        return; // Stack is empty
    }
    e = S.base[S.top--]; // Pop the top element and decrement top
}

// Traverse and print the stack
void StackTraverse_sq(CharStack &S)
{
    if (S.top == -1) {
        std::cout << "Stack is empty." << std::endl;
        return;
    }
    for (int i = S.top; i >= 0; --i) {
        std::cout << S.base[i] << " ";
    }
    std::cout << std::endl;
}

// Clear the stack
void ClearStack_sq(CharStack &S)
{
    S.top = -1; // Reset top to -1 indicating stack is empty
}

// Get the length of the stack
int StackLength_sq(CharStack &S)
{
    return S.top + 1; // Length is top index + 1
}

// Find the index of element e in the stack, return -1 if not found
int StackFind_sq(CharStack &S, char e)
{
    for (int i = S.top; i >= 0; --i) {
        if (S.base[i] == e) {
            return i;
        }
    }
    return -1;
}

#endif
