#ifndef _MERGE_H_
#define _MERGE_H_

#include "grph.hpp"
#include "/home/asbaker/Parallel_testing/simple-4.4J/src/simple.h"

#define NCACHE_INTS (1024*64)
#define KEY(A) A.adj_min_weight_key
#define ELEMENT(A,index) A->at(index.list).ListW[index.element]
#define V(A,index) A->at(index.list).ListW[index.element].first
#define W(A,index) A->at(index.list).ListW[index.element].second


//A[index.list].my_neighbors[index.element]
//A[index.list].my_neighbors[index.element].v
//A[index.list].my_neighbors[index.element].w

typedef struct index_{
    int list,element;
} index_t_;

index_t_ Partition(A_Network * Y,index_t_ q, index_t_ r);
void compact(vector <int_double> * new_ele_arr,A_Network * Y, index_t_ start, index_t_
end,int *buff,int d);
void incr_(A_Network * Y, index_t_ *ind);
void decr_(A_Network * Y, index_t_ *ind);  
//A_Network * compact_adj_list (A_Network * Y,int *p_n_vertices,THREADED);
A_Network * compact_adj_list_2 (A_Network * Y,int *p_n_vertices,THREADED);
//A_Network * compact_adj_list_3 (A_Network * Y,int *p_n_vertices,THREADED);
A_Network * compact_adj_list_arr (A_Network * Y,int *p_n_vertices,vector <int_double> * tmp_arr,THREADED);
inline int leq_(index_t_ ind_l, index_t_ ind_r);

#endif
