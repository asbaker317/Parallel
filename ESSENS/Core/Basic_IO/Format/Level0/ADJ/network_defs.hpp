#ifndef ADJ_NETWORK_DEFS_HPP
#define ADJ_NETWORK_DEFS_HPP



//#include"/home/asbaker/Parallel_testing/simple-4.4J/src/simple.h"
#include"structure_defs.hpp"
#include<list>
using namespace std;




//Structure in STATIC Adjacency List---For diagram go to ()
//Rows=global ID of the rows
//For edges connected with Rows
//NListW.first=Column number
//NListW.second=Value of edge
struct ADJ_Bundle
{
 	int Row; //might not use?
  int adj_min_weight_key;
	vector <int_double> ListW;
      // ex .....Y->at(i).ListW[j].first;
      // Listw.first == grph_list[i].my_neighbors[j].v
      // Listw.second == grph_list[i].my_neighbors[j].w
      // ListW.size() == grph_list[i].n_neighbors

	//Constructor
	ADJ_Bundle() {ListW.resize(0);}

	//Destructor
	void clear()
	{while(!ListW.empty()) {ListW.pop_back();}}


};

typedef  vector<ADJ_Bundle> A_Network;


#endif



