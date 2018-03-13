///
/// File: queueADT.c
///
/// Description: An abstract data type for a queue module.
///
/// @author kjb2503 : Kevin Becker
///
// // // // // // // // // // // // // // // // // // // // // // // // // // //

#include <stdio.h> // print (idk if needed)
#include <string.h>

// the nodes that go into our queue structure defined below
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
    int (*cmp)(const void*a, const void*b);
} * QueueADT;

#define _QUEUE_IMPL_
#include "queueADT.h"

QueueADT que_create( int (*cmp)(const void*a,const void*b) )
{
    // pointer to a new queue (set to NULL for now)
    QueueADT queue = NULL;
    // allocates enough space for our queue
    queue = malloc(sizeof(queue_s));
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


void que_destroy( QueueADT queue )
{
    
}


void que_clear( QueueADT queue )
{
    
}


void que_insert( QueueADT queue, void * data )
{
    // creats a new node
    QNode *newNode = NULL;
    newNode = malloc(sizeof(QNode));
    newNode->data = data;
    newNode->next = NULL;

    // if we have an empty queue we can immediately just put the node in
    // (no comparison needed)
    if(queue->size == 0)
    {
        // sets both the first and last element to new node
        queue->first = queue->last = newNode;
        // we can immediately return here we are done
        return;
    }
    // if we weren't given a comparison function, FIFO style
    else if(queue->cmp == NULL)
    {
        // last node's next member is set to our new node
        queue->last->next = newNode;
        // so is the last element of queue set to our new node
        queue->last = newNode;
    }
    // if we HAVE a comparison function, we use that for insertion
    else
    {
        // start from the first node and go back
        QNode **n = &queue->first;
        // keeps goint unil we find a spot to put our new item in
        // while: n is not null, and we are still greater than selected element
        while(*n != NULL && queue->cmp((*n)->data, data) > 0)
            // sets our n to the next node based upon 
            n = &(*n)->next;

        // we've escaped our loop, we found where we should input newNode
        // if *n is NULL, we've hit the end of our queue
        if(*n == NULL)
            queue->last = newNode;
        else
            
    }

    // adds one to the size of the queue (we just added one in)
    queue->size += 1;
}


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
    queue->size -= 1;

    // destroys our first node (we're done with it)
    free(firstNode);

    // returns the value we just removed
    return toReturn;
}


bool que_empty( QueueADT queue )
{
    return (queue->size == 0) ? true : false;
}
