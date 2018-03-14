///
/// File: queueADT.c
///
/// Description: An abstract data type for a queue module.
///
/// @author kjb2503 : Kevin Becker
///
// // // // // // // // // // // // // // // // // // // // // // // // // // //

#include <stdlib.h> // malloc, free
#include <stdbool.h> // boolean data members
#include <assert.h> // used for the assert in que_destroy(1)

// a QNode is a structure which contains the data being input into a queue
typedef struct qnode_s{
    // the data of our node
    void *data;
    // the node behind us (if we're first, this is the second node; can be NULL)
    struct qnode_s *behind;
} QNode;

// our Queue structure
typedef struct queue_s{
    // the front of our queue (can be NULL if empty queue)
    QNode *first;
    // the end of our queue (can be NULL if empty queue)
    QNode *last;
    // the size of our queue
    int size;
    // the comparison function used to insert nodes
    int (*cmp)(const void*, const void*);
} * QueueADT;

// this define is so our above definition of *QueueADT doesn't get overwritten
#define _QUEUE_IMPL_
// includes our header file which defines what functions we need to provide
#include "queueADT.h"


/// creates an empty queue with comparison function pointer passed as argument
QueueADT que_create( int (*cmp)(const void*a,const void*b) )
{
    // pointer to a new queue (set to NULL for now)
    QueueADT queue = NULL;
    // allocates enough space for our queue
    queue = malloc(sizeof(struct queue_s));

    // sets our first and last nodes to null
    queue->first = NULL;
    queue->last = NULL;
    // current size is 0
    queue->size = 0;
    // sets our comparison function (which may be NULL) to the one passed in
    queue->cmp = cmp;

    // returns our new, empty queue
    return queue;
}


/// clears a queue of all data, freeing all blocks of allocated memory
void que_clear( QueueADT queue ) {
    // we only want to do this if we have an actual queue and it's not empty
    if (queue != NULL && !que_empty(queue))
    {
        // when we get here we are guaranteed that there is a first node
        // declares two nodes: one definitely has data, the other might not
        QNode *curNode = queue->first, *nextNode = curNode->behind;

        // keep freeing until our nextNode is NULL
        while (nextNode != NULL)
        {
            // frees our current node
            free(curNode);
            // sets our current node to next
            curNode = nextNode;
            // sets next node to the new current's behind node
            nextNode = curNode->behind;
        }

        // clears the last node from heap
        // this is required since we don't free the last node in the loop
        free(curNode);
        // sets our curNode to NULL (nextNode is already NULL)
        curNode = NULL;

        // last thing we do is set our queue's size to 0; we're done
        queue->size = 0;
    }
}


/// destroys the queue by clearing all data and freeing the queue from memory
void que_destroy( QueueADT queue )
{
    // clears all the data out of the queue
    // why implement the same thing twice?
    que_clear(queue);

    // in order to free our queue we need to have an empty queue
    assert(que_empty(queue));

    // frees our queue (only difference for destroy function)
    free(queue);

    // sets queue to NULL
    queue = NULL;
}


/// inserts any 64-bit data into the queue
void que_insert( QueueADT queue, void * data )
{
    // creates a new node with data of data
    QNode *newNode = NULL;
    // allocates enough space for our QNode on the heap
    newNode = malloc(sizeof(QNode));
    // sets our data in place
    newNode->data = data;
    // our new QNode might have something behind it, but we don't know
    newNode->behind = NULL;

    // if we have an empty queue we can immediately just put the node in
    // (no comparisons are needed)
    if(queue->size == 0)
    {
        // sets both the first and last element to new node
        queue->first = newNode;
        queue->last = newNode;
    }
    // if we were given a comparison function, use it
    else if(queue->cmp != NULL)
    {
        // start from the first node and go back
        QNode **cmpNode = &queue->first;
        // keeps going until we find a spot to put our new item in
        // while: node not null, and we are still greater than selected element
        // we insert once compare returns <= 0, or cmpNode is NULL
        while(*cmpNode != NULL && queue->cmp(data, (*cmpNode)->data) > 0)
            // sets our new cmpNode
            cmpNode = &(*cmpNode)->behind;

        // we've now escaped our loop, we found where we should input newNode
        // if cmpNode is NULL, we've hit the end of our queue; simple append
        if(*cmpNode == NULL)
            queue->last = newNode;
        // if it ISN'T NULL, we need to insert ourselves in the middle of queue
        else
            newNode->behind = (*cmpNode);

        // we always need to set previous' new behind regardless
        (*cmpNode) = newNode;
    }
    // if we DON'T have a comparison function, we insert on the end
    else
    {
        // last node's next member is set to our new node
        queue->last->behind = newNode;
        // so is the last element of queue set to our new node
        queue->last = newNode;
    }

    // adds one to the size of the queue (we just added one in)
    queue->size++;
}


/// removes the first element in our queue and frees the memory
void * que_remove( QueueADT queue )
{
    // ( last ) QNode -> QNode -> QNode -> QNode ( first )
    // pulls in our first node
    QNode *firstNode = queue->first;

    // copies the first node's data into temp var
    void* toReturn = firstNode->data;
    // changes our first node to be the second member
    queue->first = firstNode->behind;
    // subtracts one from the size
    queue->size--;

    // destroys our first node (we're done with it)
    free(firstNode);

    // returns the value we just removed
    return toReturn;
}


/// returns if the queue is empty or not
bool que_empty( QueueADT queue )
{
    return (queue->size == 0) ? true : false;
}
