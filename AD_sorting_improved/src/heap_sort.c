#include "heap_sort.h"
#include "binheap.h"

void heap_sort(void *A, const unsigned int n, 
               const size_t elem_size, 
               total_order leq)
{
    //build the heap with the values of A
    void* A_cpy = malloc((n+1)*elem_size);
    memcpy(A_cpy, A, (n+1)*elem_size);
    binheap_type* H = build_heap(A_cpy, n+1, 100000, elem_size, leq);
    
    void* A_i;
    unsigned int i=0;
    while(is_heap_empty(H))
    {
        A_i = A+i*elem_size;
        memcpy(A_i, extract_min(H), elem_size);
        i++;
    }
    
    free(A_cpy);
    delete_heap(H);
}
