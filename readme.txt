================================= README.TXT ================================= 
===                       Submission by: KEVIN BECKER                      ===
==============================================================================

This is just a bit of explanation to my queue implementation.  The QueueADT 
stores: the first item, the last item, the size of the queue itself, as well
as the comparison function.  The reason I chose to keep track of the first 
and last element is so that in the event of FIFO ordering, the que_insert 
function won't have to keep going through the entire queue in search of the 
last node. Which brings to how the data itself is stored.  Each "inserted"
data element is stored in a QNode which stores the data (as type void*), and
the element behind it (which may be NULL if it is the last element).  This
way the queue stores nodes of data and only has one pointer to the comparion
function.  Having each datum stored inside a single type of container makes
creating a node easier, and allows for easier maniuplation.  Alternatively,
an array queue could be used, but by using nodes, the only allocation 
funciton required was malloc(1).

==============================================================================
