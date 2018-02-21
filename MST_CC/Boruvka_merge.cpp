extern "C" {
  #include "../simple-4.4J/src/simple.h"
  #include "../simple-4.4J/src/umd.h"

//      #include "../simple-4.4J/src/simple-f.h"
//      #include "../simple-4.4J/src/simple-f-defs.h"
//      #include "../simple-4.4J/src/umd-f.h"

#include <sys/types.h>
  #include <values.h>
  /*#include "my_malloc.hpp"*/
  void   node_Barrier_sync(THREADED);
  void   *node_malloc(int bytes, THREADED);
  void   *node_malloc_l(long bytes, THREADED);
  void   node_free(void *, THREADED);
}
#include <vector>
#include <utility>
#include "merge_arr.cpp"
#include "grph.hpp"
#include "merge.hpp"
#include "my_malloc.hpp"


void Boruvka_sparse_merge(A_Network *Y, int n_vertices, thread_inf *ti)
{

  clock_t start,end,s1,t1;
  double interval1=0,interval2=0,interval3=0,interval;
  double ti_findmin=0,ti_cc=0,ti_compact=0;
  vector <int_double> * tmp_arr; // was vector <int_double>, maybe should be ADJ_Bundle?

  int newly_dead=0,*p_alive,*p_total,*D,*dead_matrix,*total_matrix;
  int min,min_ind,k,l,w,i,j,total=0;

  D = (int *)node_malloc(sizeof(int)*n_vertices,TH);
  dead_matrix = (int *)node_malloc(sizeof(int)*THREADS,TH);
  total_matrix = (int *)node_malloc(sizeof(int)*THREADS,TH);
  p_alive = (int *)node_malloc(sizeof(int),TH);
  p_total = (int *)node_malloc(sizeof(int),TH);

  tmp_arr = (vector <int_double> *)malloc(sizeof(vector <int_double>)*(n_vertices));
  on_one_thread {
	*p_alive = n_vertices;
	*p_total = 0;

  }

  node_Barrier();

  while(1) {

  	start = omp_get_wtime();
	on_one_thread printf(" Alive vertices:%d, total weights:%d\n",n_vertices,*p_total);
	newly_dead=0;
	total=0;

	pardo(i,0,n_vertices,1){
	 D[i]=i;
    }
	node_Barrier();

	s1 = omp_get_wtime();
	pardo(i,0,n_vertices,1)
	{
		min=MAXINT;
		for(k=0;k<Y->at(i).ListW.size();k++)
          	{
            		j=Y->at(i).ListW[k].first;
            		if(i!=j && min>Y->at(i).ListW[k].second ){
                 		min = min>Y->at(i).ListW[k].second;
                 		min_ind=j;
					}
            }
		D[i]=min_ind;
    }

	node_Barrier();
	t1 = omp_get_wtime();
	interval = t1 - s1;
	interval = interval ;
	ti_findmin += interval;

	s1 = omp_get_wtime();
	pardo(i,0,n_vertices,1)
		if(i==D[D[i]] && i>D[i]) D[i]=i;
	node_Barrier();

	on_one printf("before pj\n");

	pardo(i,0,n_vertices,1)
		while(D[i]!=D[D[i]]){
			D[D[i]]=D[D[D[i]]];
		 	D[i]=D[D[i]];
		}
	node_Barrier();
	on_one printf("after pj\n");

	pardo(i,0,n_vertices,1)
	{
		Y->at(i).adj_min_weight_key=D[i];
		for(j=0;j<Y->at(i).ListW.size();j++)
			Y->at(i).ListW[j].first=D[Y->at(i).ListW[j].first];
	}

	node_Barrier();
	t1 = omp_get_wtime();
	interval = t1-s1;
	interval = interval ;
	ti_cc += interval;
	s1 = omp_get_wtime();

	Y=compact_adj_list_arr(Y,&n_vertices,tmp_arr,TH);
	if(n_vertices<=1) break;

	t1 = omp_get_wtime();
	interval=t1-s1;
	interval=interval;
	ti_compact+=interval;
	on_one printf("Time used for compacting list is %f s\n",interval);
	on_one_thread printf("**** compact_adj_list_arr: n_vertices is now %d\n",n_vertices);

	node_Barrier();
	end = omp_get_wtime();
	interval1=end-start;
	on_one_thread printf("=== Time for this round: %f s\n\n", interval1);

  } /*while*/

  on_one printf("The total weight is %d\n",*p_total);
  on_one printf("METRICS: Time used for find_min %f s, cc %f s, compact %f s\n",ti_findmin, ti_cc, ti_compact);
  node_Barrier();
  //free(tmp_arr);
  node_free(p_alive,TH);
  node_free(p_total,TH);
  node_free(dead_matrix,TH);
  node_free(total_matrix,TH);
  node_free(D,TH);
}
