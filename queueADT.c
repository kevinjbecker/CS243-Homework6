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

// a QNode is a structure which contains the data being input into a queue
typedef struct qnode_s{
    // the data of our node
    void *data;
    // the NEXT member (if we're first, this is the second node)
    struct qnode_s *next;
} QNode;

// our Queue structure
typedef struct queue_s{
    // the front of our queue
    QNode *first;
    // the end of our queue
    QNode *last;
    // the size of our queue
    int size;
    // the comparison function used to insert nodes
    int (*cmp)(const void*, const void*);
} * QueueADT;

// this define is so our above definition doesn't get overwritten
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
    // we only want to do this if we have an actual queue and its size is > 0
    if (queue != NULL && queue->size > 0)
    {
        // declares two nodes: one definitely has data, the other might not
        QNode *curNode = queue->first, *nextNode = curNode->next;

        // goes until we hit the last node
        while (nextNode != NULL)
        {
            // frees our current node
            free(curNode);
            //
            curNode = nextNode;
            nextNode = nextNode->next;
        }

        // clears the last node from heapspace
        free(curNode);
        curNode = NULL;

        // last thing we do is set our queue's size to 0
        queue->size = 0;
    }
}


/// destroys the queue by clearing all data and freeing the queue from memory
void que_destroy( QueueADT queue )
{
    // clears all the data out of the queue
    // why implement the same thing twice?
    que_clear(queue);

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
    newNode = malloc(sizeof(QNode));
    newNode->data = data;
    newNode->next = NULL;

    // if we have an empty queue we can immediately just put the node in
    // (no comparison needed)
    if(queue->size == 0)
    {
        // sets both the first and last element to new node
        queue->first = newNode;
        queue->last = newNode;
    }
    // if we weren't given a comparison function, FIFO style
    else if(queue->cmp != NULL)
    {
        // the usefulness of the double pointer here is so we can effectively
        // insert our node into the middle of the queue if needbe
        // start from the first node and go back
        QNode **cmpNode = &queue->first;
        // keeps going until we find a spot to put our new item in
        // while: node not null, and we are still greater than selected element
        while(*cmpNode != NULL && queue->cmp(data, (*cmpNode)->data) > 0)
            // set our compare node to be the next node of compare node
            cmpNode = &(*cmpNode)->next;

        // we've escaped our loop, we found where we should input newNode
        // if node is NULL, we've hit the end of our queue; simple append
        if(*cmpNode == NULL)
            queue->last = newNode;
            // if it ISN'T NULL, we need to insert ourselves in the middle of queue
        else
            // the next node for newNode is the pointer to cmpNode we compare
            // greater than
            newNode->next = (*cmpNode);

        // insert ourself into the queue
        (*cmpNode) = newNode;
    }
    // if we HAVE a comparison function, we use that for insertion
    else
    {
        // last node's next member is set to our new node
        queue->last->next = newNode;
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
    queue->first = firstNode->next;
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
