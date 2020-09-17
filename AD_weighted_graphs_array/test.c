#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "SSSP.h"

graph_type* generate_graph2(unsigned int* A, unsigned int* B)
{
    graph_type* G;
    
    G = construct_graph(A, 6, sizeof(unsigned int), 2, leq_node);
    
    connect_to(G, 0, 1, 1);
    connect_to(G, 0, 2, 5);
    connect_to(G, 1, 5, 15);
    connect_to(G, 2, 3, 2);
    connect_to(G, 3, 4, 1);
    connect_to(G, 4, 5, 3);
    
    B[0]=0;
    B[1]=1;
    B[2]=5;
    B[3]=7;
    B[4]=8;
    B[5]=11;
    
    return G;
}


graph_type* generate_graph1(unsigned int* A, unsigned int* B)
{
    graph_type* G;
    
    G = construct_graph(A, 9, sizeof(unsigned int), 4, leq_node);
    
    connect_to(G, 0, 1, 1);
    connect_to(G, 0, 2, 1);
    connect_to(G, 1, 4, 1);
    connect_to(G, 1, 2, 1);
    connect_to(G, 4, 6, 1);
    connect_to(G, 4, 5, 1);
    connect_to(G, 6, 6, 1);
    connect_to(G, 6, 4, 1);
    connect_to(G, 5, 5, 1);
    connect_to(G, 5, 1, 1);
    connect_to(G, 2, 8, 1);
    connect_to(G, 2, 7, 1);
    connect_to(G, 2, 1, 1);
    connect_to(G, 7, 8, 1);
    
    B[0]=0;
    B[1]=1;
    B[2]=1;
    B[3]=get_infinity(G);
    B[4]=2;
    B[5]=3;
    B[6]=3;
    B[7]=2;
    B[8]=2;
    
    return G;
}


graph_type* generate_graph_test(unsigned int* A, unsigned int n)
{
    graph_type* G;
    
    G = construct_graph(A, n, sizeof(unsigned int), 2, leq_node);
    
    //construct connections
    unsigned int nrows = sqrt(n);
    for(unsigned int i=0; i<nrows-1; i++)
    {
        for(unsigned int j=0; j<nrows-1; j++)
        {
            connect_to(G, i*nrows+j, i*nrows+j+1, 1);      //connect to the right element
            connect_to(G, i*nrows+j, i*nrows+j+nrows, 1);  //connect to the under element
        }
    }
    
    return G;
}


unsigned int equal(unsigned int* A, unsigned int* B, const unsigned int num_of_elem)
{
    unsigned int dif=0;
    
    for(unsigned int i=0; i<num_of_elem; i++)
    {
        dif += A[i]!=B[i];
    }
    
    return !dif;
}


int correct_distances(unsigned int* B, graph_type* G, const unsigned int num_of_elem)
{
    unsigned int* D = (unsigned int*)malloc(num_of_elem*sizeof(unsigned int));
    
    for(unsigned int i=0; i<num_of_elem; i++)    //copy all distances into an array
    {
        D[i] = (&G->N[i])->distance;
    }
    
    const int eq = equal(D, B, num_of_elem);    //check if both B and D are equal
    
    free(D);
    return eq;
}


int main(void)
{
    clock_t start, end;
    double cpu_time_used;
    
    unsigned int* A = (unsigned int*)malloc(pow(2, 24)*sizeof(unsigned int));
    unsigned int* B = (unsigned int*)malloc(pow(2, 24)*sizeof(unsigned int));
    
    
    //initialize array A
    for(unsigned int i=0;i<9;i++)
    {
        A[i] = i+1;
    }
    
    //Test over graph 0
    graph_type* G0 = generate_graph1(A, B);
    dijkstra(G0, 0);
    printf("    +Test over graph 00: %u\n", correct_distances(B, G0, 9));
    destruct_graph(G0);
    
    //Test over graph 1
    graph_type* G1 = generate_graph2(A, B);
    dijkstra(G1, 0);
    printf("    +Test over graph 01: %u\n", correct_distances(B, G1, 6));
    destruct_graph(G1);
    
    //Performance test
    printf("    +Test performance:\n");
    for(unsigned int i=0; i<25; i++)
    {
        graph_type* G_test = generate_graph_test(A, pow(2, i));
        
        start = clock();
        for(unsigned int j=0; j<10; j++)
        {
            dijkstra(G_test, 0);
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cpu_time_used /= 10.;
        printf("        +n=2^%u: %f\n", i, cpu_time_used);
        destruct_graph(G_test);
    }
    
    printf("\nEND\n");
    
    free(A);
    free(B);
}
