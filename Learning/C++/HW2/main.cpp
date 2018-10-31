#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<string>
#include<iomanip>
#include "graph.h"
#include "err.h"
using namespace std;

#define DEBUG


void test_construction_node(Graph* g, const int ITER = 4, const int NODES_ADD = 7, const int NODES_SUB= 3);
void test_construction_edge(Graph* g, const int ITER, const int NODES, const int EDGE_ADD, const int EDGE_SUB);
//mathematically calculate default args
inline void test_construction_edge(Graph* g, const int ITER = 1, const int NODES=5)
{
	test_construction_edge(g,  ITER, NODES, NODES*NODES/5, NODES*NODES/10);
}

int main()
{
	Graph g;
//	test_construction_node(&g);	
	test_construction_edge(&g);
	return 0;
}
/***************************************************************************************************
* TEST FUNCTIONS
***************************************************************************************************/

void test_construction_node(
	Graph* g,
	const int ITER,
	const int NODES_ADD,
	const int NODES_SUB
	)
{
	const int LETTERS =  4;
	const int ALPHA_RANGE = 26;
	uint32_t id;
	char ch[LETTERS+1];
	
	for(int i =0; i<ITER; i++)
	{
		for(int j =0; j<NODES_ADD; j++)
		{
			for(int k=0; k<LETTERS; k++)
			{ ch[k] = rand()%ALPHA_RANGE+'A';	}
			ch[LETTERS] = '\0';
			string* s = new string(ch);
			id = g->addNode(s);
#ifdef DEBUG
			cout << "Added Node: " << id << endl;	
#endif
		}
#ifdef DEBUG
		if(NODES_ADD>0)
		{
			cout << endl;
			g->printGraph();
			cout <<endl;
		}
#endif
		for(int j =0; j<NODES_SUB; j++)
		{	
			id = rand()%g->getNumNodes();
#ifdef DEBUG
			cout << "Delete Node: " << id << " ";		
#endif
			g->deleteNode(id);
		}

#ifdef DEBUG
		if(NODES_SUB>0)
		{
			cout << endl;
			g->printGraph();
			cout <<endl;
		}
#endif	
	}
	return;
}

void test_construction_edge(Graph* g, const int ITER, const int NODES, const int EDGE_ADD, const int EDGE_SUB)
{
	uint32_t src_id, dest_id;
	weight_t weight;
	ERR_T err;
	test_construction_node(g, 1, NODES, 0); //build a graph of NODES nodes
	for(int i = 0; i<ITER; i++)
	{
		for(int j =0; j<EDGE_ADD; j++)
		{
			cout<<endl;
			int num_nodes = g->getNumNodes();
			if(num_nodes == 0) { cout << "Empty Graph" <<endl; return; }
			src_id =  static_cast<uint32_t> (rand()%num_nodes);
			dest_id = static_cast<uint32_t> (rand()%num_nodes);
			weight = static_cast<weight_t> (rand() % static_cast<int>(WEIGHT_MAX-1));
#ifdef DEBUG
			cout << "ADD:: Src: " << src_id << "\tDest: " << setw(2) << dest_id << "\tWeight: " << static_cast<int>(weight);
#endif	
			err = g->addEdge(src_id, dest_id, DIR_ONEDIR, weight);
			if(err != E_NONE){ printError(err); }
		}
#ifdef DEBUG
		cout << endl << endl ;
		g->printGraph() ;
		cout << endl << endl ;
#endif
		
		for(int j =0; j<EDGE_SUB; j++)
		{
			int num_nodes = g->getNumNodes() ;	
			src_id = rand()%num_nodes;
			dest_id = rand()%num_nodes;
#ifdef DEBUG
			cout << "Delete:: Src: " << src_id << "\tDest: " << dest_id << endl;
#endif	
			err = g->deleteEdge(src_id, dest_id);
			if(err != E_NONE){ printError(err); continue; } 
#ifdef DEBUG
			g->printGraph() ;
			cout << endl << endl ;
#endif
		}
	}
}
