#include <binheap.h>
#include <string.h>
#include <stdio.h>

#define PARENT(node) ((node-1)/2)
#define LEFT_CHILD(node) (2*node+1)
#define RIGHT_CHILD(node) (2*(node+1))

#define VALID_NODE(H, node) ((H)->num_of_elem>(node))

#define ADDR(H, node) ((H)->A + (H->key_pos[node])*(H)->key_size)   //returns a pointer to the node with that index
#define INDEX_OF(H, addr) ((H)->rev_pos[(((addr)-((H)->A))/(H)->key_size)]) 

//ACTUALIZAR LA FUNCIÓN SWAP Y REVISAR EL RESTO DE FUNCIONES

void swap_keys(binheap_type *H, unsigned int n_a, unsigned int n_b)
{
    unsigned int aux;
    
    aux = H->key_pos[n_a];
    H->key_pos[n_a] = H->key_pos[n_b];
    H->key_pos[n_b] = aux;
    
    H->rev_pos[H->key_pos[n_a]] = n_a;
    H->rev_pos[H->key_pos[n_b]] = n_b;
    
    return;
}

void heapify(binheap_type* H, unsigned int node)
{
    unsigned int dst_node=node, child;
    
    do{
        node = dst_node;
        
        //Get the smallest node among node and the two children
        child = RIGHT_CHILD(node);
        if(VALID_NODE(H, child) && H->leq(ADDR(H, child), ADDR(H, dst_node)))
            dst_node = child;
        
        child = LEFT_CHILD(node);
        if(VALID_NODE(H, child) && H->leq(ADDR(H, child), ADDR(H, dst_node)))
            dst_node = child;
        
        //Change if neccessary the smallest node by the root
        if(dst_node != node)
            swap_keys(H, dst_node, node);
        
    }while(dst_node != node);
}

int is_heap_empty(const binheap_type *H)
{
    return (H)->num_of_elem;
}

const void *min_value(const binheap_type *H)
{
    if(is_heap_empty(H)) return NULL;

    return ADDR(H, 0);
}

const void *extract_min(binheap_type *H)
{
    if(is_heap_empty(H)==0){  //OBSERVATION: If I put only is_heap_empty(H), 0 is recognised as true :S
        return NULL;}
    
    //The root is swapped with the last element and then it is excluded
    //from the heap (it is not removed in terms of memory)
    swap_keys(H, 0, H->num_of_elem-1);
    H->num_of_elem--;
    
    heapify(H, 0);
    
    return ADDR(H, H->num_of_elem);
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

binheap_type *build_heap(void *A, 
                         const unsigned int num_of_elem,
                         const unsigned int max_size,  
                         const size_t key_size, 
                         total_order_type leq)
{
    //assign memory
    binheap_type* H = (binheap_type *)malloc(sizeof(binheap_type));
    
    //initialize almost all heap members
    H->num_of_elem = num_of_elem;
    H->max_size = max_size;
    H->key_size = key_size;
    H->leq = leq;
    H->A = A;
    H->max_order_value = malloc(key_size);
    
    H->rev_pos = (unsigned int *)malloc(H->max_size*sizeof(unsigned int));
    H->key_pos = (unsigned int *)malloc(H->max_size*sizeof(unsigned int));
    
    //initialize index arrays
    for(unsigned int i=0; i<num_of_elem; i++)
    {
        H->key_pos[i]=i;
        H->rev_pos[i]=i;
    }
    
    //OVERFLOW EXTRAÑO/////////////////////////////////////////////////
    
    if(num_of_elem == 0)
        return H;
    
    //get the maximum value on A and store it in max_order_value
    const void *value = find_the_max(A, num_of_elem, key_size, leq);
    
    memcpy(H->max_order_value, value, key_size);
    
    //ensure heap property
    for(unsigned int i = num_of_elem/2; i>0; i--) //Leafs holds heap property
        heapify(H, i);
    heapify(H, 0);
    
    return H;
}

void delete_heap(binheap_type *H)
{
    free(H->max_order_value);
    free(H->key_pos);
    free(H->rev_pos);
    free(H);
}

const void *decrease_key(binheap_type *H, void *node, const void *value)
{
    unsigned int node_idx = INDEX_OF(H, node);
    
    //check if node is valid and if the new value is smaller than old value
    if (!VALID_NODE(H, node_idx) || !(H->leq(value, node)))
        return NULL;

    memcpy(node, value, H->key_size);   //modify the key
    
    if(node_idx == 0)
        return node;
    
    unsigned int parent_idx = PARENT(node_idx);
    void* parent = ADDR(H, parent_idx);
    
    //if i'm not root and my father is greater after my decrease, swap me with him
    //and check again up to hold condition or root
    while(!H->leq(parent, node))
    {
        swap_keys(H, parent_idx, node_idx);
        
        node_idx = parent_idx;
        node = ADDR(H, node_idx);
        
        if(node_idx == 0)
            break;
        
        parent_idx=PARENT(node_idx);
        parent = ADDR(H, parent_idx);
    }
    
    return node;
}

const void *insert_value(binheap_type *H, const void *value)
{
    //check if all the heap is busy
    if(H->max_size == H->num_of_elem)
        return NULL;
    
    //if heap is empty or new value is greater than max value, update max value
    if(H->num_of_elem == 0 || !H->leq(value, H->max_order_value))
        memcpy(H->max_order_value, value, H->key_size);
    
    //assign to the (new) last node the max observed value
    void* new_node_addr = ADDR(H, H->num_of_elem);
    memcpy(new_node_addr, H->max_order_value, H->key_size);
    
    //decrease the key of the new node up to the value
    H->num_of_elem++;
    return decrease_key(H, new_node_addr, value);
}

void print_heap(const binheap_type *H, 
                void (*key_printer)(const void *value))
{
    unsigned int next_level_node = 1; //stores the index of the left-most node of the next next_level_node
    
    for(unsigned int node = 0; node < H->num_of_elem; node++)
    {
        if(node == next_level_node)
        {
            printf("\n");
            next_level_node=LEFT_CHILD(node);
        }
        else
            printf("\t");
        
        key_printer(ADDR(H, node));
    }
    printf("\n");
}
