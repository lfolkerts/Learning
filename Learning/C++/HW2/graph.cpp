#include<vector>
#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<list>
#include<string>
#include<algorithm>

#include "graph.h"
#include "err.h"
using namespace std;
#define DEBUG
/*static*/ uint32_t Graph::_id;

/****************** Graph  **************************/
/*** public ***/
Graph::Graph(){_id=0;} //default constructor is empty graph

/////////////// Graph Building //////////////////////
uint32_t Graph::addNode(T_NodeData *node_data)
{
	Node* n_ptr = new Node(_id++, node_data);
	nodes.push_back(n_ptr); ///ids may not correspond to index
	return n_ptr->getID();
}

Graph::Node* Graph::findNode(uint32_t id)
{
	Node* n_found = nullptr ;
	for(auto n : nodes)
	{ 
		if(id == n->getID())
		{ 
			n_found = n ;
			break ; 
		}
	}
	return n_found ;
}

void Graph::deleteNode(uint32_t id)
{
	Node* n  ;
	auto n_it = nodes.begin() ;
	for( ; n_it != nodes.end(); n_it++)
	{	
		n = *n_it ;		
		if(n->getID() == id){ break ; }
	}
	if(n->getID() == id)
	{
		nodes.erase(n_it) ;
		delete n ;
	}
	return ;
}

ERR_T Graph::addEdge(uint32_t src_id, uint32_t dest_id,  DIR_T direction, weight_t weight)
{
	Node	*src, *dest;
	//ERR_T err;

	src = findNode(src_id) ;
	if(src == nullptr || src->getID()!=src_id){ return E_NO_SRC; }
	dest = findNode(dest_id);
	if(dest == nullptr || dest->getID()!=dest_id){ return E_NO_DEST; }
	
	Edge *edge = new Edge(src, dest, weight, direction);
	if(edge==nullptr){ return E_NOMEM; }
	if(edge->getDir() == DIR_BIDIR)
	{
		src->addOut(edge);
		src->addIn(edge);
		dest->addOut(edge);
		dest->addIn(edge);
	}
	else if (edge->getDir() == DIR_ONEDIR)
	{
		src->addOut(edge);
		dest->addIn(edge);
	}
	return E_NONE;

}
ERR_T Graph::deleteEdge(uint32_t src_id, uint32_t dest_id)
{
	Node *src, *dest;
	Edge *e_delete = nullptr ;
	//find src nod
	src = findNode(src_id);
	if(src == nullptr || src->getID() != src_id){ return E_NO_SRC ; }	
	if(src->getEdges(NEND_EOUT).empty()) { return E_NO_EDGE ; }
	//find edge
	for(Edge* e : src->getEdges(NEND_EOUT))
	{
		if(e == nullptr){ continue ; }
		
		dest = e->getDest() ; 
		if(dest != nullptr && dest->getID() == dest_id)
		{
			cout << "\tEdge Found: " << dest_id ;
			e_delete = e ;
			break ;
		}
	}
	if(e_delete == nullptr || dest->getID() != dest_id)
	{
		return E_NO_EDGE ;
	}
	delete e_delete ;
	cout << "\tDeleted Edge" << endl ;
	return E_NONE ;	
}


///////////////// Graph Info ////////////////////
int Graph::getNumNodes(){ return static_cast<int> (nodes.size());}

//////////////// PRINT/DEBUG /////////////////////
void Graph::printGraph(ostream& out)
{
	vector<Node*>::iterator n_it;
	if(nodes.size() == 0){cout<< "No Nodes" << endl; return;}
	for(n_it = nodes.begin(); n_it < nodes.end(); n_it++)
	{
		out << (*n_it)->getID() << " : " << (*n_it)->getInfo()->c_str() << "\t" ;
		(*n_it)->printEdges(out) ;
		cout << endl ;
	}
	return;
}
//end Graph functions
/*************** Graph Nodes ************************/
/*** public ***/
Graph::Node::Node(uint32_t id, T_NodeData *info): id(id), info(info){} 
Graph::Node::~Node()
{
	while(!e_in.empty())
	{ 
		Edge* e = *(e_in.begin()) ;
		delete e ;
	}
	while(!e_out.empty())
	{ 
		Edge* e = *(e_out.end()) ;
		delete e ;
	}
	delete info ;
}
uint32_t Graph::Node::getID()
{
	return id ;
}
T_NodeData* Graph::Node::getInfo()
{
	return info ;
}


list<Graph::Edge*> Graph::Node::getEdges(NEND_T nend)
{
	if(NEND_EIN == nend){ return e_in; }
	else { return e_out ; } 
}	
void Graph::Node::detachEdge(Edge* e, NEND_T nend)
{	
	if(NEND_EIN == nend)
	{
		e_in.remove(e) ;
	}
	if(NEND_EOUT == nend)
	{
		e_out.remove(e) ;
	}
	return ;
}
void Graph::Node::addIn(Graph::Edge* e)
{
	if(e != nullptr)
	{
		e_in.push_back(e) ;
	}
	return ; 
	
}
void Graph::Node::addOut(Graph::Edge* e)
{
	if(e != nullptr)
	{
		e_out.push_back(e) ;
	}
	return ; 
}
void Graph::Node::printEdges(ostream& out)
{
	for(auto l_it = e_out.begin(); l_it != e_out.end(); l_it++)
	{
		if((*l_it)->getDir()==DIR_BIDIR){ out << "*";}
		cout << (*l_it)->getDest()->getID() << "\t";
	}
	return;
}
/**************** Graph Edges ************************/
/*** public ***/
Graph::Edge::Edge(Node* src, Node* dest, weight_t weight, DIR_T direction): src(src), dest(dest), weight(weight), dir(direction){}
Graph::Edge::~Edge()
{
		//need to detach edges so no null pointers exist
	src->detachEdge(this, NEND_EOUT) ;
	dest->detachEdge(this, NEND_EIN) ;
	cout << "Detached Edges" <<endl ;
	if(dir == DIR_BIDIR)
	{
		src->detachEdge(this, NEND_EIN) ;
		dest->detachEdge(this, NEND_EOUT) ;
	}		
}

DIR_T Graph::Edge::getDir()
{
	return dir ;
}
Graph::Node* Graph::Edge::getSrc()
{
	return src ;
}
Graph::Node* Graph::Edge::getDest()
{
	return dest ;
}



