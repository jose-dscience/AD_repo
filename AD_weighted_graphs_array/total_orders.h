#include <stdlib.h>
#include "graph.h"

typedef int (*total_order_type)(const void *a, const void *b);

int leq_float(const void *a, const void *b);
int leq_int(const void *a, const void *b);
int geq_int(const void *a, const void *b);
int leq_node(const node_type *a, const node_type *b);

int leq_float(const void *a, const void *b)
{
  return *((float*)a)<=*((float*)b);
}

int leq_int(const void *a, const void *b)
{
  return *((int*)a)<=*((int*)b);
}

int geq_int(const void *a, const void *b)
{
  return *((int*)a)>=*((int*)b);
}

int leq_node(const node_type* a, const node_type* b)
{
    return a->diatance <= b->distance;
}
