#include "bubble_sort.h"
#include "swap.h"

void bubble_sort(void *A, const unsigned int n, 
                 const size_t elem_size, 
                 total_order leq)
{
    for(unsigned int i=n;i>=1;i--) //loop which sets the ordered element
    {
        for(unsigned int j=0; j<i-1; j++) //loops that pulls the greatest element up to the end
        {
            void* elem_j = A+j*elem_size;       //current element
            void* elem_jj = A+(j+1)*elem_size;  //next element
            
            if(!leq(elem_j, elem_jj))
                swap(elem_j, elem_jj, elem_size);
        }
    }
    
    return;
}
