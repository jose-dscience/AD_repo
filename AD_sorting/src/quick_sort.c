#include "quick_sort.h"
#include "swap.h"

unsigned int partition(void* A, const unsigned int n,
                const size_t elem_size,
                total_order leq,
                const unsigned int p) //pivot index
{
    void* pivot = A+p*elem_size;
    
    if(n<=1)
        return 0;
    
    swap(A, pivot, elem_size);  //swap the pivot with the first element
    
    unsigned int i=1;
    unsigned int j=n-1;
    
    while(i<=j)  //loop up to i and j get met
    {
        if(!leq(A+i*elem_size, A))  //is the second element greater? Put it at i!
        {
            swap(A+i*elem_size, A+j*elem_size, elem_size);
            j--;
        }
        else
            i++;
    }
    
    swap(A, A+j*elem_size, elem_size); //put the pivot "in the middle"
    
    return j;
}

void quick_sort(void *A, const unsigned int n, 
                const size_t elem_size, 
                total_order leq)
{
    const unsigned int p = 0; //set (by chosen) the pivot as the first element
    unsigned int p_index;
    
    if(n>1)
    {
        unsigned int l=0;
        
        unsigned int p_index;
        while(l<n-1)
        {
            p_index = partition(A+l*elem_size, n-l, elem_size, leq, p); //partition the array
            quick_sort(A+l*elem_size, p_index, elem_size, leq);
            l=(l+p_index)+1;
        }
    }
    
    return;
}


int check(void* A, const unsigned int n, const size_t elem_size, total_order leq)
{
    if(n==1) return 1;
    
    int res=1;
    for(unsigned int i=1; i<n; i++)
    {
        res*=leq(A+(i-1)*elem_size, A+i*elem_size);
    }
    
    return res;
}
