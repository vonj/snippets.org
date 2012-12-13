#include    <stdlib.h>
#include    "snipsort.h"


/*
    This is a quicksort routine to be used to sort linked-lists
    by Jon Guthrie.
*/

void    *sortl(void *list, void *(*getnext)(void *),
            void (*setnext)(void *, void *), 
            int (*compare)(void *, void *))
{
void    *low_list, *high_list, *current, *pivot, *temp;
int     result;

    /*
        Test for empty list.
    */
    if(NULL == list)
        return(NULL);

    /*
        Find the first element that doesn't have the same value as the first
        element.
    */
    current = list;
    do
    {
        current = getnext(current);
        if(NULL == current)
            return(list);
    }   while(0 == (result = compare(list, current)));

    /*
        My pivot value is the lower of the two.  This insures that the sort
        will always terminate by guaranteeing that there will be at least one
        member of both of the sublists.
    */
    if(result > 0)
        pivot = current;
    else
        pivot = list;

    /* Initialize the sublist pointers */
    low_list = high_list = NULL;

    /*
        Now, separate the items into the two sublists
    */
    current = list;
    while(NULL != current)
    {
        temp = getnext(current);
        if(compare(pivot, current) < 0)
        {
            /* add one to the high list */
            setnext(current, high_list);
            high_list = current;
        }
        else
        {
            /* add one to the low list */
            setnext(current, low_list);
            low_list = current;
        }
        current = temp;
    }

    /*
        And, recursively call the sort for each of the two sublists.
    */
    low_list  = sortl(low_list, getnext, setnext, compare);
    high_list = sortl(high_list, getnext, setnext, compare);

    /*
        Now, I have to put the "high" list after the end of the "low" list.  
        To do that, I first have to find the end of the "low" list...
    */
    current = temp = low_list;
    while(1)
    {
        current = getnext(current);
        if(NULL == current)
            break;
        temp = current;
    }

    /*
        Then, I put the "high" list at the end of the low list
    */
    setnext(temp, high_list);
    return(low_list);
}
