#include <string.h>
#include <stdio.h>

#define VALID_ELEM(Q, indx) ((Q)->num_of_elem>(indx))

#define ADDR(Q, indx) ((Q)->A + (Q->I[indx])*(Q)->key_size)   //returns a pointer to the node with that index
#define INDEX_OF(Q, addr) ((Q)->I_rev[(((addr)-((Q)->A))/(Q)->key_size)])   

typedef struct {
    void* A;    //array containing the nodes
    unsigned int* I;    //array containing the positions
    unsigned int* I_rev;    //array containing the indexes
    unsigned int num_of_elem; //number of nodes
    unsigned int max_size; //maximum number of nodes
    size_t key_size; //size of the key type
    total_order_type leq; //this is the heap total order
    void* max_order_value; //maximum value stored in the array until now
} array_type;

void swap_keys(array_type *Q, unsigned int n_a, unsigned int n_b)
{
    unsigned int aux;
    
    aux = Q->I[n_a];
    Q->I[n_a] = Q->I[n_b];
    Q->I[n_b] = aux;
    
    Q->I_rev[Q->I[n_a]] = n_a;
    Q->I_rev[Q->I[n_b]] = n_b;
}

int is_array_empty(const array_type *Q)
{
    return (Q)->num_of_elem;
}

const void *min_value(const array_type *Q)
{
    if(!is_array_empty(Q)) return NULL;
    
    void* min = ADDR(Q, 0);
    for(unsigned int i=0; i<Q->num_of_elem; i++)
    {
        if(Q->leq(ADDR(Q, i), min))
            min=ADDR(Q, i);
    }

    return min;
}

const void *extract_min(array_type *Q)
{
    if(is_array_empty(Q)==0){  
        return NULL;
    }
    
    unsigned int i = INDEX_OF(Q, min_value(Q)); //localize index of min value
    swap_keys(Q, i, Q->num_of_elem-1);          //swap the min and the last position
    Q->num_of_elem--;                           //exclude the last element (the min)
    
    return ADDR(Q, Q->num_of_elem);
}

const void *find_the_max(void* A, 
                         const unsigned int num_of_elem, 
                         const size_t key_size, 
                         total_order_type leq)
{
    if(num_of_elem==0)
        return NULL;
    
    //run over all the values and set the max. The step is the key_size
    const void* max_value = A;
    for(const void* addr=A+key_size; addr!=A+num_of_elem*key_size;addr+=key_size)
    {
        if (!leq(addr, max_value))
            max_value = addr;
    }
    
    return max_value;
}

array_type *build_array(void *A, 
                         const unsigned int num_of_elem,
                         const unsigned int max_size,  
                         const size_t key_size, 
                         total_order_type leq)
{
    //assign memory
    array_type* Q = (array_type *)malloc(sizeof(array_type));
    //programar bien la asignacion de memoria
    
    //initialize almost all heap members
    Q->num_of_elem = num_of_elem;
    Q->max_size = max_size;
    Q->key_size = key_size;
    Q->leq = leq;
    Q->A = A;
    Q->max_order_value = malloc(key_size);
    Q->I =  (unsigned int*)malloc(num_of_elem*sizeof(unsigned int));
    Q->I_rev =  (unsigned int*)malloc(num_of_elem*sizeof(unsigned int));
    
    
    if(num_of_elem == 0)
        return Q;
    
    //initialize index arrays
    for(unsigned int i=0; i<num_of_elem; i++)
    {
        Q->I[i]=i;
        Q->I_rev[i]=i;
    }
    
    //get the maximum value on A and store it in max_order_value
    const void *value = find_the_max(A, num_of_elem, key_size, leq);
    
    memcpy(Q->max_order_value, value, key_size);
    
    return Q;
}

void delete_array(array_type *Q)
{
    free(Q->max_order_value);
    free(Q->I);
    free(Q->I_rev);
    free(Q);
}

const void *insert_value(array_type *Q, const void *value)
{
    //check if all the heap is busy
    if(Q->max_size == Q->num_of_elem)
        return NULL;
    
    //if heap is empty or new value is greater than max value, update max value
    if(Q->num_of_elem == 0 || !Q->leq(value, Q->max_order_value))
        memcpy(Q->max_order_value, value, Q->key_size);
    
    //assign to the (new) last node the max observed value
    void* new_node_addr = ADDR(Q, Q->num_of_elem);
    memcpy(new_node_addr, Q->max_order_value, Q->key_size);
    
    //decrease the key of the new node up to the value
    Q->num_of_elem++;
    return ADDR(Q, Q->num_of_elem-1);
}
