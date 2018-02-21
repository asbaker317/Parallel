#include "grph.hpp"
#include "SP_structure.hpp"
#include  "tree_info.hpp"
#include "merge.hpp"
#include "merge.cpp"
#include <utility>
#include <vector>
#include "sortD.cpp"
#include "../ESSENS/Core/Basic_SetOps/Level2/unary_set_operations.hpp"
#include "sort_seq.cpp"
extern "C" {
#include "../simple-4.4J/src/simple.h"
#include "values.h"
#include "my_malloc.hpp"
}

#define DEBUG 1

/* (1) Merge the the graph list. (this list is first sorted according to
   the D(i) value. So that nodes with the same D(i) value are consequtive.
   Merge such that any element with the same
   D(i) value (or the same wk_space value) should be merged into a
   single copy. Also Within the single list, there should not be
   repeated values After merging, compact the whole thing into one new adj-list.
   */
A_Network * compact_adj_list_arr(A_Network * Y,int *p_n_vertices,vector<int_double> tmp_arr,THREADED)
{
    int my_local_entries=0,my_start,my_end,n,i,j,k,new_len,n_vertices,new_n_vertices,d,l,key;
    int * buff;
    index_t_ left,right,q; //////////////////// check this
    vector<int_double> new_ele_arr;
    A_Network * tmp_list;

#if DEBUG
    int nsorted=0;
    clock_t start,end,s1,t1,s2,t2;
    double interval=0,interval1=0,interval2=0,interval3=0,interval4;
    start=omp_get_wtime();
#endif


#if 1
    s1 = omp_get_wtime();
#endif

    n_vertices=*p_n_vertices;
    tmp_list = (A_Network *)node_malloc(sizeof(A_Network)*n_vertices,TH);
    all_radixsort_smp_s2(n_vertices,Y,tmp_list,TH);
    node_free(Y,TH);
    Y = tmp_list;
    node_Barrier();

#if 1
    t1 = omp_get_wtime();
    interval2=(t1-s1);
    interval2=interval2;
    on_one_thread printf("====Radix sorting time is %f s\n", interval2);
#endif

#if PROFILING
    s1 = omp_get_wtime();
#endif

    partition_list(Y, n_vertices, &my_start, &my_end,TH);

    buff = (int *)node_malloc(sizeof(int)*n_vertices,TH);
    node_Barrier();
    pardo(i,0,n_vertices,1) buff[i]=0;
    node_Barrier();

    pardo(i,0,n_vertices,1)
        buff[Y->at(i).adj_min_weight_key]=1;
    node_Barrier();
    prefix_sum(buff,n_vertices,TH);
    node_Barrier();

#if PROFILING
    t1 = omp_get_wtime();
    interval3=(t1-s1);
    interval3=interval3;
    on_one_thread printf("====partition list and prefix sum time %f s\n",interval3);
#endif

    new_n_vertices=buff[n_vertices-1];
    on_one_thread printf("The number of new vertices is %d\n",new_n_vertices);
    if(new_n_vertices <=1) {
        *p_n_vertices=new_n_vertices;
        node_free(Y,TH);
        node_free(buff,TH);
        return (NULL);
    }

    tmp_list=(A_Network *)node_malloc(sizeof(A_Network)*new_n_vertices,TH);


#if PROFILING
    s2= omp_get_wtime();
#endif

    i=my_start;

    while(i<=my_end)
    {
        k=1;
        j=i+1;
        n=Y->at(i).ListW.size();
        key=KEY(Y->at(i));
        while(KEY(Y->at(j))==KEY(Y->at(i)) && j<n_vertices){
            n+=Y->at(j).ListW.size();
            j++;
            k++;
        }

#if PROFILING
        nsorted+=n;
#endif
        /*now I have k consequtive nodes starting from i to be merged*/
        /*The number of adj elements for these nodes is n*/
        left.list=i;
        left.element=0;
        right.list=i+k-1;
        right.element=Y->at(right.list).ListW.size()-1;

#if PROFILING
        s1=omp_get_wtime();
        printf("%d elements to be sorted\n", n);
#endif
#if 0
        tmp_arr = (vector <int_double>*)malloc(sizeof(vector <int_double>)*(n*2));
#endif

        //copy_out(Y,left,right,tmp_arr);
#if 0
        Qsort_arr(tmp_arr,0,n-1);
#endif

     //   if(n<50) 
            insertsort(tmp_arr,n);
//        else mergesort_nr(tmp_arr,n);
//          sort(tmp_arr);


#if PROFILING
        t1=omp_get_wtime();
        interval=t1-s1;
        interval=interval;
        sort_time[MYTHREAD]+=interval;
#endif


#if 0
        check_sort_arr(tmp_arr,0,n-1);
#endif
        new_len=0;

#if PROFILING
        s1=omp_get_wtime();
#endif

        for(l=0;l<n;l++)
        {
            if(tmp_arr[l].first==key) continue;
            if(l==0) new_len++;
            else if(tmp_arr[l].first!=tmp_arr[l-1].first) new_len++;
        }
#if 0
        if(new_len==n) printf("new_len==n\n");
#endif
        //new_ele_arr = (vector<int_double> *)node_malloc(sizeof(vector<int_double>)*new_len,TH);

        k=0;
        for(l=0;l<n;l++)
        {
            if(tmp_arr[l].first==key) continue;
            if(l==0) {
                new_ele_arr[k].first=buff[tmp_arr[l].first]-1;
                new_ele_arr[k++].second=tmp_arr[l].second;
            }
            else if(tmp_arr[l].first!=tmp_arr[l-1].first) {
                new_ele_arr[k].first=buff[tmp_arr[l].first]-1;
                new_ele_arr[k++].second=tmp_arr[l].second;
            }
        }

#if PROFILING
        t1=omp_get_wtime();
        interval1+=(t1-s1);
#endif

        d=Y->at(i).adj_min_weight_key;
        d=buff[d]-1;

        Y->at(d).ListW=new_ele_arr;
        Y->at(d).ListW.resize(new_len);//=new_len;
        i=j;
#if 0
        free(tmp_arr);
#endif

    }

#if PROFILING
    t2 = omp_get_wtime();
    interval4=t2-s2;
    interval4=interval4;
    printf("====THREAD %d: time used for my turn is %f s\n", MYTHREAD,interval4);
#endif

    node_Barrier();

#if PROFILING
    end=omp_get_wtime();
    interval=end-start;
    interval=interval;
    compact_list_time[MYTHREAD]+=interval;
#endif


#if PROFILING
    printf("===== THREAD %d: my sort time for this round is %f s\n",MYTHREAD,sort_time[MYTHREAD]);
    printf("===== THREAD %d: my number of sorted elements is %d \n",MYTHREAD,nsorted);
    printf("===== THREAD %d: the time for rearranging data is %f s\n",MYTHREAD, interval1);
#endif
    *p_n_vertices=new_n_vertices;
    node_free(Y,TH);
    node_free(buff,TH);
    return(tmp_list);
}

inline int leq_(index_t_ ind_l, index_t_ ind_r)
{
    if(ind_l.list<ind_r.list || (ind_l.list==ind_r.list && ind_l.element<=ind_r.element))
        return 1;
    else return 0;
}
