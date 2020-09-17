#include "insertion_sort.h"
#include "swap.h"

void insertion_sort(void *A, const unsigned int n, 
                    const size_t elem_size, 
                    total_order leq)
{    
    for(int i=1;i<=n;i++) //loop which expands the current sorted array with the last element
    {
        void* elem_jj = A+i*elem_size;    //next element
        void* elem_j = A+(i-1)*elem_size; //current element
        
        while(!leq(elem_j,elem_jj) && (elem_jj != A))  //pulls down the last element up to the correct position
        {
            swap(elem_j, elem_jj, elem_size);
            
            elem_jj = elem_j;    //decrease one position
            elem_j -= elem_size;
        }
    }
}
