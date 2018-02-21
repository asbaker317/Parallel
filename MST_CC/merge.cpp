#include "grph.hpp"
#include "merge.hpp"
#include "prefix_sum.cpp"

extern "C" {
#include "../simple-4.4J/src/simple.h"
#include "values.h"
}

#define DEBUG 1


/* (1) Merge the the graph list. (this list is first sorted according to
   the D(i) value. So that nodes with the same D(i) value are consequtive.
   Merge such that any element with the same
   D(i) value (or the same wk_space value) should be merged into a
   single copy. Also Within the single list, there should not be
   repeated values
   After merging, compact the whole thing into one new adj-list.
   */

int partition_list(A_Network * Y, int n_vertices, int * mystart, int *myend,THREADED)
{

    int * buff,i,j,seg,start,end,search_start,*arr, n_elements;

    if(n_vertices<THREADS) {
        if(MYTHREAD==0) {
            *mystart=0;
            *myend=n_vertices-1;
        } else {
            *mystart=0;
            *myend=-1;
        }
        return(0);
    }


    buff = (int *)node_malloc(sizeof(int)*n_vertices,TH);
    arr = (int *)node_malloc(sizeof(int)*THREADS,TH);
    pardo(i,0,n_vertices,1)
    {
        buff[i]=Y->at(i).ListW.size();
    }

    node_Barrier();
    prefix_sum(buff,n_vertices,TH);

    n_elements=buff[n_vertices-1];
    seg = n_elements/THREADS;
    on_one_thread printf("n_elements is %d\n", n_elements);

    pardo(i,0,n_vertices,1)
    {
        search_start=i;
        break;
    }

    j=search_start;
    while(buff[j]<(MYTHREAD+1)*seg && j<n_vertices) j++;
    while(buff[j-1]>=(MYTHREAD+1)*seg && j>=0) j--;

    if (MYTHREAD==THREADS-1) * myend=n_vertices-1;
    else
        *myend=j;

    arr[MYTHREAD]=j;
    node_Barrier();

    if(MYTHREAD==0) *mystart=0;
    else *mystart=arr[MYTHREAD-1]+1;

    while(*mystart>0 && Y->at(*mystart).adj_min_weight_key==Y->at(*mystart-1).adj_min_weight_key)
        (*mystart)++;
    while((*myend<n_vertices-1) && Y->at(*myend).adj_min_weight_key==Y->at(*myend+1).adj_min_weight_key)
        (*myend)++;

    if(*mystart<0) *mystart=0;

    node_Barrier();
    node_free(buff,TH);
    node_free(arr,TH);
}

//int copy_out(A_Network * Y, index_t_ left, index_t_ right, ADJ_Bundle *tmp_arr)
//{
//    int l=0;
//    index_t_ i;
//
//    for(i=left; leq_(i,right);incr_(Y,&i))
//    {
//        tmp_arr[l++]=Y->at(i.list).ListW[i.element];
//    }
//}
