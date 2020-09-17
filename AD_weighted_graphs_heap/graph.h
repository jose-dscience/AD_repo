#include<stdlib.h>
#include<string.h>

typedef struct{
    const void* key;              //key of the node
    size_t key_size;        //size of key type
    void** link;            //array of pointer to the connected nodes
    unsigned int* weight;  //array of weights, each one associated to each link
    unsigned int n_links;   //number of adjaccent nodes (links)
    void* pred;             //pointer to the precedent node
    unsigned int distance;  //global distance of the current node
    unsigned int closed;   //has been this node closed?
    unsigned int max_link; //maximum number of links
} node_type;

typedef int (*total_order_type)(const node_type *a, const node_type *b);
int leq_node(const node_type* a, const node_type* b)
{
    return a->distance <= b->distance;
}

typedef struct {
    node_type* N;                    //array containing the nodes
    unsigned int num_of_elem;   //number of nodes on the graph
    size_t key_size;            //size of the key
    size_t node_size;           //size of the node
    unsigned int max_distance;  //a distance greater than every possible distance in the graph
    total_order_type leq;
    } graph_type;
    
/**********************************************************************
 * Construct the node.
 *
 * This function initialize a node around some key.
 *
 * @param node is a pointer to the node to initialize
 * @param key is the key of the node.
 * @param key_size is the size of the key on bits.
 * @param max_link is the maximum number of nodes to which the constructed node.
 *  will be able to point
 * @return a pointer to the constructed node
 **********************************************************************/
void init_node(node_type* node, const void* key, const size_t key_size, const unsigned int max_link)
{
    node->key = key;
    node->key_size = key_size;
    node->link = malloc(max_link*sizeof(node_type*));
    node->weight = (unsigned int*)malloc(max_link*sizeof(unsigned int));
    node->n_links = 0;
    node->pred = NULL;
    node->distance = 10000000;
    node->closed = 0;
    node->max_link = 4;
    
    return;
}


/**********************************************************************
 * Delete node.
 *
 * This function remove a node by releasing its memory.
 *
 * @param node is the key of the node.
 * @param key_size is the node that will be removed.
 **********************************************************************/
void delete_node(node_type* node)
{
    free(node->link);
    free(node->weight);
}


/**********************************************************************
 * Connect nodes.
 *
 * This function creates a directional link among an origin node and a destiny node.
 * If the number of links is maximum the connection is not created.
 *
 * @param origin is the node that will point to dest node.
 * @param dest is the destination node.
 * @param weight is the associated weight to the link.
 **********************************************************************/
void connect_nodes(node_type* origin, node_type* dest, const unsigned int weight)
{
    if(origin->n_links >= origin->max_link)
        return;
    
    node_type** pos = (node_type**) &(origin->link[origin->n_links]);        //point to the nlinks position to write the link
    unsigned int* w_pos = &(origin->weight[origin->n_links]);  //the same with the weight
    
    memcpy(pos, &dest, sizeof(node_type*));  //copy the direction of dest into po
    memcpy(w_pos, &weight, sizeof(unsigned int));    //copy the weight into w_pos
    
    origin->n_links++;
    
    return;
}

/**********************************************************************
 * Construct to.
 *
 * This function connects an origin node to a destiny node given its indexes
 * and the graph.
 *
 * @param G is the graph structure.
 * @param origin_indx is the index associated with the origin node
 * @param dest_indx is the index of the destiny node
 **********************************************************************/
void connect_to(graph_type* G, const unsigned int origin_indx, const unsigned int dest_indx, const unsigned int weight)
{
    return connect_nodes(&(G->N[origin_indx]), &(G->N[dest_indx]), weight);
}


/**********************************************************************
 * Construct graph.
 *
 * This function construct a graph (without connections) given an array of keys.
 *
 * @param A is the array whose keys will be the keys of the nodes on the graph.
 * @param num_of_elem is the number of elements on the graph.
 * @param key_size is the size of the key.
 * @param max_link is the maximum number of nodes to which the constructed node.
 *  will be able to point.
 * @return a pointer to the constructed graph.
 **********************************************************************/
graph_type* construct_graph(const unsigned int* A, const unsigned int num_of_elem, const size_t key_size, const unsigned int max_link, const total_order_type leq)
{
    graph_type* G = (graph_type*)malloc(sizeof(graph_type));
    
    G->num_of_elem = num_of_elem;
    G->key_size = key_size;
    G->node_size = sizeof(node_type);
    G->max_distance = 0;
    G->leq = leq;
    G->N = (node_type*)malloc(num_of_elem*sizeof(node_type));
    
    node_type* N_i;
    for(unsigned int i=0; i<num_of_elem; i++)   //construct all the nodes
    {
        N_i = &(G->N[i]);
        
        init_node(N_i, &A[i], key_size, max_link);
    }
    
    return G;
}


/**********************************************************************
 * Destruct graph.
 *
 * This function deletes a graph releasing its associated memory.
 *
 * @param G is the graph structure.
 **********************************************************************/
void destruct_graph(graph_type* G)
{
    node_type N_i;
    for(unsigned int i=0; i<G->num_of_elem; i++)
    {
        N_i = ((node_type*)(G->N))[i];
        delete_node(&N_i);
    }
    
    free(G->N);
    free(G);
}


/**********************************************************************
 * Get infinity.
 *
 * This function computes a value suitable as infinity. This is 
 * done by taking infinity as the sum of all the weights i.e. the maximum
 * distance that a node could have in an ideal situation.
 *
 * @param G is the graph structure.
 * @return infinity substitute
 **********************************************************************/
unsigned int get_infinity(graph_type* G)
{
    unsigned int max=0;
    
    node_type* N_i;
    for(unsigned int i=0; i<G->num_of_elem; i++)
    {
        N_i = &(G->N[i]);
        for(unsigned int j=0; j<(N_i->n_links); j++)  //add all the weights to max
        {
            max += N_i->weight[j];
        }
    }
    
    return max;
}
