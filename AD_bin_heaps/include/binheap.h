#ifndef __BIN_HEAP__
#define __BIN_HEAP__

#include <stdlib.h>
#include <string.h>

#include <total_orders.h>

/** @struct binheap_type
 *  @brief A class to represent binary heaps.
 *
 *  This class is mean to represent binary heaps.
 */
typedef struct {
    void* A;    //array containing the nodes
    unsigned int num_of_elem; //number of nodes
    unsigned int max_size; //maximum number of nodes
    size_t key_size; //size of the key type
    total_order_type leq; //this is the heap total order
    void* max_order_value; //maximum value stored in the heap until now
} binheap_type;

/**********************************************************************
 * Exchanges two nodes given two indexes
 *
 * This function swaps the memory associated with two indexes.
 *
 * @param H is the heap whose emptyness has to be tested.
 * @param i is the index of the first node to swap.
 * @param j is the index of the second node to swap.
 **********************************************************************/
void swap(binheap_type* H, const unsigned int i, const unsigned int j);

/**********************************************************************
 * Corrects the heap in order to guarantee the heap condition
 *
 * This function tests swaps nodes up to guarantee the heap condition. 
 * It is made by swapping nodes on an iterative way. A certain node is 
 * provided in such a way that that node (and succesive children) are
 * corrected. It does not guarantees the correctness of provided node's 
 * ancients.
 *
 * @param H is the heap whose emptyness has to be tested.
 * @param node is the index of the node in which heap condition fails for first.
 **********************************************************************/
void heapify(binheap_type* H, unsigned int node);

/**********************************************************************
 * Test whether the heap is empty.
 *
 * This function tests whether a heap is empty. If it is the case, it 
 * returns a value different from 0, otherwise, 0 is returned. The 
 * asymptotic time-complexity of this function is Theta(1).
 *
 * @param H is the heap whose emptyness has to be tested.
 * @return if H is not empty, the function returns 0; a number different 
 *         from 0, otherwise.
 **********************************************************************/
int is_heap_empty(const binheap_type *H);

/**********************************************************************
 * Return the minimum in the heap.
 *
 * This function returns a pointer to the minimum in the heap w.r.t. 
 * its total order. If the heap is empty, NULL is returned. The 
 * asymptotic time-complexity of this function is Theta(1).
 *
 * @param H is the heap whose minimum should be returned.
 * @return a pointer to the minimum in the heap, if H is not empty; 
 *         NULL otherwise.
 **********************************************************************/
const void *min_value(const binheap_type *H);

/**********************************************************************
 * Extract the minimum from the heap and return a pointer to it.
 *
 * This function extracts the minimum value in the heap w.r.t. its 
 * total order and returns a pointer to it. If the heap is empty, the 
 * heap is left unchanged and NULL is returned. The asymptotic 
 * time-complexity of this function is O(log n).
 *
 * @param H is the heap from which the minimum should be extracted.
 * @return a pointer to the minimum in the heap, if H is not empty; 
 *         NULL otherwise.
 **********************************************************************/
const void *extract_min(binheap_type *H);

/**********************************************************************
 * Build a new binary heap.
 *
 * This function build a new binary heap and fills it by using the
 * values in an array. Its asymptotic time-complexity is Theta(n).
 *
 * @param A is an array used a to store the keys of the heap. It is 
 *          also used to pass to this function a set of values to 
 *          initialize the heap.
 * @param num_of_elem is the number of values in A to be initially 
 *                    stored in the heap.
 * @param max_size is the maximum number of nodes in the heap (i.e., 
 *                 the size of A).
 * @param key_size is the size in bytes of the values stored in A.
 * @param leq is the total order characterizing the heap.
 * @return a pointer to the new binary heap.
 **********************************************************************/
binheap_type *build_heap(void *A, 
                         const unsigned int num_of_elem, 
                         const unsigned int max_size, 
                         const size_t key_size, 
                         total_order_type leq);

/**********************************************************************
 * Destroy a binary heap.
 *
 * This function releases the memory space required to represent a 
 * binary heap. Please, notice that the array A which stores the keys 
 * of the heap will not be freed. The asymptotic time-complexity of 
 * this function is Theta(1).
 *
 * @param H is the heap from which the minimum should be extracted.
 **********************************************************************/
void delete_heap(binheap_type *H);

/**********************************************************************
 * Decrease the value of node's key.
 *
 * This function decreases the value of a node's key and returns a 
 * pointer to the node in which the new value is stored. If either the 
 * new value is greater than of equal to the old key value or the node 
 * does not belong to the considered heap, the heap remains unchanged 
 * and NULL is returned. The asymptotic time-complexity of this 
 * function is O(log n).
 *
 * @param H is the heap in which the key value should be decreased.
 * @param node is a pointer to the node whose key should be decreased.
 * @param value is a pointer to the new value of the node's key.
 * @return a pointer to the node in which the new value is stored. If 
 *         either the new value is greater than of equal to the old key
 *         value or the node does not belong to the H, NULL is returned.
 **********************************************************************/
const void *decrease_key(binheap_type *H, void *node, const void *value);

/**********************************************************************
 * Insert a value in the heap.
 *
 * This function creates a new node and inserts the passed value as the
 * key of it. The function returns a pointer to the new node. If the 
 * heap already contains the maximum number of admissible nodes, then 
 * the heap remains unchanged and NULL is returned. The asymptotic 
 * time-complexity of this function is O(log n).
 *
 * @param H is the heap in which the key value should be decreased.
 * @param value is a pointer to the new value of the node's key.
 * @return a pointer to the node in which the new value is stored. If 
 *         either the new value is greater than of equal to the old key
 *         value or the node does not belong to the H, NULL is returned.
 **********************************************************************/
const void *insert_value(binheap_type *H, const void *value);

/**********************************************************************
 * Print a representation of the heap on the standard output.
 *
 * This function print a representation of the heap on the standard 
 * output. Since key type is unknown, a function to print a key value 
 * is required. The asymptotic time-complexity of this function is 
 * Theta(n).
 *
 * @param H is the heap in which the key value should be decreased.
 * @param key_printer is a function to print a key value.
 **********************************************************************/
void print_heap(const binheap_type *H, 
                void (*key_printer)(const void *value));

#endif // __BIN_HEAP__
