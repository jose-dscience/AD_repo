#include <math.h>
#include "select.h"
#include "swap.h"
#include "quick_sort.h"

unsigned int select_pivot(void *A, const unsigned int n,
                          const size_t elem_size,
                          total_order leq)
{
    const unsigned int threshold = 10;      //threshold to use the base case
    const unsigned int nchuncks = 5;        //number of chuncks
    const unsigned int nelems = ceil((n+1)/nchuncks); //number of elements per chunck
    
    //base case
    if(n<threshold)
    {
        quick_sort(A, n, elem_size, leq);
        return n/2;
    }
    
    //loop over chuncks (last chunck out of the loop)
    for(unsigned int c=0;c<nchuncks-2;c++)
    {
        quick_sort_select(A+c*nelems*elem_size, nelems, elem_size,leq);                      //sort the chunck
        swap(A+c*elem_size, A+(c*nelems+(nelems/2))*elem_size, elem_size);  //put the element in the middle at the beggining of the array
    }
    const unsigned int remaining_elems = n-(nchuncks-1)*nelems;     //remaining iteration
    quick_sort_select(A+(nchuncks-1)*nelems*elem_size, remaining_elems, elem_size,leq);
    
    swap(A+(nchuncks-1)*elem_size, A+((nchuncks-1)*nelems+remaining_elems/2)*elem_size, elem_size);
    
    //return the mean
    return select_index(A, nchuncks-1, nchuncks/2, elem_size, leq);
}


unsigned int select_index(void *A, const unsigned int n, 
                          const unsigned int i,
                          const size_t elem_size, 
                          total_order leq)
{
    const unsigned int threshold = 10;
    
    //base case
    if(n<threshold)         
    {
        quick_sort(A, n, elem_size, leq);
        return i;
    }
    
    const unsigned int p=select_pivot(A, n, elem_size, leq);   //select the pivot
    const unsigned int k=partition(A, n, elem_size, leq, p);   //make the partitions in base to the pivot
    
    //return or search the value in a subpartition
    if(i==k)
        return k;
    else if(i<k)
        return select_index(A, k-1, i, elem_size, leq);
    else
        return select_index(A+(k+1)*elem_size, n-(k+1), i, elem_size, leq);
}

void quick_sort_select(void *A, const unsigned int n, 
                       const size_t elem_size, 
                       total_order leq)
{
    if(n>1)  //note that n is the number of elements minus one
    {        
        unsigned int l=0;
        
        unsigned int p_index;
        unsigned int p;
        while(l<n-1)
        {
            p = select_pivot(A+l*elem_size, n-l, elem_size, leq); //use select's pivot
            
            p_index = partition(A+l*elem_size, n-l, elem_size, leq, p); //partition the array
            quick_sort_select(A+l*elem_size, p_index, elem_size, leq);
            l=(l+p_index)+1;
        }
    }
}
