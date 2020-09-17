#include "graph.h"
#include "arrays.h"

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
void relax(node_type* u)
{
    node_type* neighbour;
    unsigned int* w;
    for(unsigned int i=0; i<u->n_links; i++)
    {
        neighbour = (node_type*)(u->link)[i];
        w = &(u->weight[i]);
        
        if(u->distance + *w < neighbour->distance)
        {
            neighbour->distance = u->distance + *w;
            neighbour->pred = u;
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
    
    array_type* Q = build_array(G->N, G->num_of_elem, G->num_of_elem+1, G->node_size, G->leq);
    
    node_type* u;
    while(is_array_empty(Q))
    {
        u = (node_type*)extract_min(Q);
        relax(u);
    }
    
    delete_array(Q);
    
    return;
}
