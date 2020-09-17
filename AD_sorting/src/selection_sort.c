#include "selection_sort.h"
#include "swap.h"

void selection_sort(void *A, const unsigned int n, 
                    const size_t elem_size, 
                    total_order leq)
{
    unsigned int max_index=0;
    
    for(unsigned int i=n;i>0;i--) //this loop for each position set the max value
    {
        max_index=0;
        for(unsigned int j=1;j<=i;j++)  //this loop searchs the max value
        {
            if(!leq(A+j*elem_size,A+max_index*elem_size))
                max_index=j;
        }
        
        swap(A+max_index*elem_size, A+i*elem_size, elem_size);  //put the max elment at the end
    }
    
    return;
}
