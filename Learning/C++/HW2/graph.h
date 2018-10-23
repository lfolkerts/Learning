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
	DIR_ONEDIR
}DIR_T;

typedef enum NEND_T //node end type
{
	NEND_SRC,
	NEND_DEST
}NEND_T;

typedef string T_Node;
typedef uint8_t weight_t;

const weight_t WEIGHT_MAX = 0xff;

class Graph
{
	private:
	class Node;
	class Edge;
	vector<Node*> nodes; //fetch T_Node info
	static uint32_t _id;
	vector<Graph::Node*>::iterator findNode(uint32_t id); //can edit info via pointer
	bool isSameNode(uint32_t id) ;
	
	public:	
	Graph(); //default constructor is empty graph
	//graph construction
	uint32_t addNode(T_Node *node); //return id, or 0xFFFFFFFF for error
	T_Node *deleteNode(uint32_t id); //need to delete object in application
	T_Node *getNode(uint32_t id); //can edit info via pointer
	ERR_T addEdge(uint32_t src, uint32_t dest, DIR_T direction=DIR_BIDIR, weight_t weight = WEIGHT_MAX);
	ERR_T deleteEdge(uint32_t src, uint32_t dest);
	//graph info
	int getNumNodes();
	//algorithms
	//print/debug
	void printGraph(ostream& out = cout);
}; //end Graph Class Declaration

class Graph::Edge
{
	private:
	Node* src, *dest;
	weight_t weight;	
	DIR_T dir;
	
	public:
	Edge(Node* src, Node* dest, weight_t weight = WEIGHT_MAX, DIR_T direction = DIR_BIDIR);
	Node* getSrc();
	Node* getDest();
	DIR_T getDir();
		
};//end Graph::Edge Class Declaration

class Graph::Node
{
	private:
	uint32_t id;
	T_Node *info;
	list<Edge*> e_out;	
	list<Edge*> e_in;
	
	list<Edge*>::iterator findEdge(uint32_t partner_id, list<Edge*> list);
	
	public:
	Node(uint32_t id, T_Node *info = nullptr);
	uint32_t getID() const;
	T_Node* getInfo();
	void setInfo(T_Node* info);			
	ERR_T addIn(Edge* e);
	ERR_T addOut(Edge* e);
	Edge* removeEdgeFromLists(Edge* e, NEND_T end);
	Edge* removeEdgeFromLists(uint32_t partner_id, NEND_T end);
	void printOutEdges(ostream& out, const char bidir = '*');
	//operator overloading
	bool operator==(const uint32_t nid);
}; //end Graph::Node Class Declaration

#endif
