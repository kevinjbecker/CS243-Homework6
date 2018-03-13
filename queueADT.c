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
