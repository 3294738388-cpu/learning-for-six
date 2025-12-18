#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include <iostream>
#include "C:\Users\TX\Desktop\Data Structure\include\Linklist.h"

typedef LinkList Queueptr;
typedef struct {
    Queueptr front; // Queue front pointer
    Queueptr rear;  // Queue rear pointer
}LinkQueue;

void InitQueue_L(LinkQueue &Q);
void DestoryQueue_L(LinkQueue &Q);
void ClearQueue_L(LinkQueue &Q);
bool QueueEmpty_L(LinkQueue &Q);
int QueueLength_L(LinkQueue &Q);
bool GetHead_L(LinkQueue &Q, int &e);
void EnQueue_L(LinkQueue &Q, int e);
bool DeQueue_L(LinkQueue &Q, int &e);
void QueuePrint_L(LinkQueue &Q);

void InitQueue_L(LinkQueue &Q)
{
    Q.front = new LNode; // Create a new node for the front
    if(!Q.front) {
        std::cerr << "Memory allocation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    Q.front->next = NULL; // Initialize next to NULL
    Q.rear = Q.front; // Rear points to the same node as front
}

void DestoryQueue_L(LinkQueue &Q)
{
    Queueptr p;
    while(Q.front) {
        p = Q.front; // Save the current front node
        Q.front = Q.front->next; // Move front to the next node
        delete p; // Free the current front node
    }
    Q.rear = NULL; // Set rear to NULL
}

void ClearQueue_L(LinkQueue &Q)
{
    DestoryQueue_L(Q); // Destroy the current queue
    InitQueue_L(Q); // Reinitialize the queue
}

bool QueueEmpty_L(LinkQueue &Q)
{
    return Q.front == Q.rear; // Queue is empty if front and rear are the same
}

int QueueLength_L(LinkQueue &Q)
{
    int len = 0;
    Queueptr p = Q.front->next; // Start from the first element
    while(p) {
        len++; // Increment length for each node
        p = p->next; // Move to the next node
    }
    return len; // Return the total length
}

bool GetHead_L(LinkQueue &Q, int &e)
{
    if(QueueEmpty_L(Q)) {
        std::cerr << "Queue is empty!" << std::endl;
        return false; // Queue is empty
    }
    e = Q.front->next->data; // Get the data from the node after front
    return true; // Successfully retrieved head element
}

void EnQueue_L(LinkQueue &Q, int e)
{
    Queueptr s = new LNode; // Create a new node
    if(!s) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return; // Memory allocation failed
    }
    s->data = e; // Set the data
    s->next = NULL; // New node will be the last node, so next is NULL
    Q.rear->next = s; // Link the new node after rear
    Q.rear = s; // Update rear to point to the new node
}

bool DeQueue_L(LinkQueue &Q, int &e)
{
    if(QueueEmpty_L(Q)) {
        std::cerr << "Queue is empty!" << std::endl;
        return false; // Queue is empty
    }
    Queueptr p = Q.front->next; // Node to be deleted
    e = p->data; // Get the data from the node
    Q.front->next = p->next; // Remove the node from the queue
    if(Q.rear == p) { // If the deleted node was the rear node
        Q.rear = Q.front; // Update rear to point to front
    }
    delete p; // Free the memory of the deleted node
    return true; // Successfully dequeued an element
}

void QueuePrint_L(LinkQueue &Q)
{
    if(QueueEmpty_L(Q)) {
        std::cout << "Queue is empty." << std::endl;
        return; // Queue is empty
    }
    Queueptr p = Q.front->next; // Start from the first element
    while(p) {
        std::cout << p->data << " "; // Print the data
        p = p->next; // Move to the next node
    }
    std::cout << std::endl; // New line after printing all elements
}

#endif // LINKQUEUE_H
