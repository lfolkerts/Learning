#ifndef GRAPH_H
#define GRAPH_H

#include<cstdint>
#include<list>
#include<vector>
#include<string>
#include "err.h"
using namespace std;

typedef enum DIR_T
{
	DIR_BIDIR,
	DIR_ONEDIR,
	DIR_NOT_VALID
}DIR_T;

typedef enum NEND_T //node end type
{
	NEND_EIN,
	NEND_EOUT
}NEND_T;

typedef string T_NodeData ;
typedef uint8_t weight_t;
 
const weight_t NO_WEIGHT = 0xff ;
const weight_t WEIGHT_MAX = (NO_WEIGHT-1);

class Graph
{
	private:
	class Node;
	class Edge;
	vector<Node*> nodes; //fetch T_Node info
	static uint32_t _id;
	
	public:	
	Graph(); //default constructor is empty graph
	//graph construction- nodes	
	uint32_t addNode(T_NodeData *node); //return id, or 0xFFFFFFFF for error
	void deleteNode(uint32_t id); //need to delete object in application
	Node* findNode(uint32_t id); //can edit info via pointer
	//graph construction - edges
	ERR_T addEdge(uint32_t src, uint32_t dest, DIR_T direction=DIR_BIDIR, weight_t weight=NO_WEIGHT);
	ERR_T deleteEdge(uint32_t src, uint32_t dest, DIR_T direction=DIR_NOT_VALID, weight_t weight=NO_WEIGHT);
	ERR_T findEdge(uint32_t src, uint32_t dest) ;
	//graph info
	int getNumNodes();
	//algorithms
	//print/debug
	void printGraph(ostream& out = cout);
}; //end Graph Class Declaration

class Graph::Node
{
	private:
	uint32_t id;
	T_NodeData *info;
	list<Edge*> e_out;	
	list<Edge*> e_in;
	//find functions
	list<Graph::Edge*>::iterator findEdge(uint32_t partner_id, list<Edge*> list);
	
	public:
	Node(uint32_t id, T_NodeData *info = nullptr);
	~Node() ;
	//Get Functions
	uint32_t getID() ;
	T_NodeData* getInfo() ;
	list<Edge*> getEdges(NEND_T nend) ;
	//Graph Building
	void addOut(Edge* e) ;
	void addIn(Edge* e) ;
	void detachEdge(Edge* e, NEND_T nend) ;
	//print and debug
	void printEdges(ostream& out) ;
}; //end Graph::Node Class Declaration

class Graph::Edge
{
	private:
	Node *src, *dest;
	weight_t weight;	
	DIR_T dir;
	
	public:
	Edge(Node* src, Node* dest, weight_t weight = WEIGHT_MAX, DIR_T direction = DIR_BIDIR);
	~Edge() ;
	//Get Functions
	DIR_T getDir() ;
	Node* getSrc() ;
	Node* getDest() ;
	weight_t getWeight() ;
};//end Graph::Edge Class Declaration

#endif
