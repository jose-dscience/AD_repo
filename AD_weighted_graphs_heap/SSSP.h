#include "graph.h"
#include "binheap.h"

/**********************************************************************
 * Init SSSP.
 *
 * This function initializes the graph in order to apply Dijkstra's 
 * algorithm consistently. This initialization is done by setting
 * all the node's distances to infinity and the predecessor node to
 * NULL (no predecessor).
 *
 * @param G is the graph structure.
 * @return infinity substitute
 **********************************************************************/
void init_SSSP(graph_type* G)
{
    node_type* N_i;
    const unsigned int inf = get_infinity(G);
    for(unsigned int i=0; i<G->num_of_elem; i++)
    {
        N_i = &(G->N[i]);
        N_i->pred = NULL;
        N_i->distance = inf;
    }
    
    return;
}


/**********************************************************************
 * Relax.
 *
 * This function updates the distances of all the neighbours of a 
 * given node. 
 *
 * @param u is the node whose neighbours should be updated.
 **********************************************************************/
void relax(node_type* u, binheap_type* H)
{
    node_type* neighbour;
    unsigned int* w;
    for(unsigned int i=0; i<u->n_links; i++)
    {
        neighbour = (node_type*)(u->link)[i];
        w = &(u->weight[i]);
        
        if(u->distance + *w < neighbour->distance)
        {
            node_type neighbour_cpy;
            memcpy(&neighbour_cpy, neighbour, sizeof(node_type));
            (&neighbour_cpy)->distance = u->distance + *w;
            (&neighbour_cpy)->pred = u;
            decrease_key(H, neighbour, &neighbour_cpy);  //guarantee the heap condition
        }
    }
       
    return;
}

/**********************************************************************
 * Dijkstra.
 *
 * This function computes the distances of a weighted graph by using the
 * Dijkstra algorithm.
 *
 * @param G is the graph.
 * @param s_indx is the index associated with the seed node.
 **********************************************************************/
void dijkstra(graph_type* G, const unsigned int s_indx)
{
    init_SSSP(G);
    
    node_type* s = &(G->N[s_indx]);  //set pointer to the starting node
    s->distance = 0;
    
    binheap_type* H = build_heap(G->N, G->num_of_elem, G->num_of_elem+1, G->node_size, G->leq);
    
    node_type* u;
    while(is_heap_empty(H))
    {
        u = (node_type*)extract_min(H);
        relax(u, H);
    }
    
    
    delete_heap(H);
    
    return;
}
