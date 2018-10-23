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
/*** private ***/
vector<Graph::Node*>::iterator Graph::findNode(uint32_t id)
{
	n_it = find_if(nodes.begin(), nodes.end(), 
		[id](const Node* node)
		{ return id == node->getID(); });
	return n_it;
}

bool isSameNode(uint32_t id){return ((bool)(id == this->getID()) ;

/*** public ***/
Graph::Graph(){_id=0;} //default constructor is empty graph

/////////////// Graph Building //////////////////////
uint32_t Graph::addNode(T_Node *node)
{
	Node* n_ptr = new Node(_id++, node);
	nodes.push_back(n_ptr); ///ids may not correspond to index
	return n_ptr->getID();
}

T_Node * Graph::getNode(uint32_t id)
{
	Node* n = *(findNode(id));
	if(n==nullptr || n->getID() != id){ return nullptr;}
	return n->getInfo();
}

T_Node * Graph::deleteNode(uint32_t id)
{
	vector<Node*>::iterator n_it;
	Node* n;
	T_Node *ret;

	n_it = findNode(id);	
	n = *n_it;
	if(n==nullptr||n->getID()!=id){return nullptr;}
	ret = n ->getInfo();
	//delete node
	nodes.erase(n_it);
	delete n;
	return ret;	
}

ERR_T Graph::addEdge(uint32_t src_id, uint32_t dest_id,  DIR_T direction, weight_t weight)
{
	Node	*src, * dest;
	//ERR_T err;

	src = *(findNode(src_id));
	if(src == nullptr || src->getID()!=src_id){ return E_NO_SRC; }
	dest = *(findNode(dest_id));
	if(dest == nullptr || dest->getID()!=dest_id){ return E_NO_DEST; }
	
	Edge *edge = new Edge(src, dest, weight, direction);
	if(edge==nullptr){ return E_NOMEM; }
	//TODO: Try/catch
	if(edge->getDir() == DIR_BIDIR)
	{
		edge->getSrc()->addOut(edge);
		edge->getSrc()->addIn(edge);
		edge->getDest()->addOut(edge);
		edge->getDest()->addIn(edge);
	}
	else if (edge->getDir() == DIR_ONEDIR)
	{
		edge->getSrc()->addOut(edge);
		edge->getDest()->addIn(edge);
	}
	return E_NONE;

}
ERR_T Graph::deleteEdge(uint32_t src_id, uint32_t dest_id)
{
	vector<Node*>::iterator src_it, dest_it;
	Node* src, *dest;
	Edge* e_src, *e_dest;
	//find src node
	src_it = findNode(src_id);
	src = *src_it;
	//remove edge
	e_src = src->removeEdgeFromLists(dest_id, NEND_SRC);
	
	if(e_src == nullptr){ return E_NO_SRC; }

	//go to dest node
	dest = e_src->getDest();
	e_dest = dest->removeEdgeFromLists(e_src, NEND_DEST);
	if(e_dest != e_src){ return E_BADGRAPH; }
	cout << "SHFNFKNLKDNlknflkNLANDLFNLFNLKNSL" <<endl;
	//delete edge object
	//delete e_src; 
	//get rid of poison pointers
	e_src = e_dest = nullptr;

	return E_NONE;
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
		out << (*n_it)->getID() << " : " << (*n_it)->getInfo()->c_str() << endl;
		out << "\t";
		(*n_it)->printOutEdges(out);
	}
	return;
}
//end Graph functions
/**************** Graph Edges ************************/
/*** private ***/
//Graph::: scope can still see them though
/*** public ***/
Graph::Edge::Edge(Node* src, Node* dest, weight_t weight, DIR_T direction): src(src), dest(dest), weight(weight), dir(direction){}
Graph::Node* Graph::Edge::getSrc(){return src; }
Graph::Node* Graph::Edge::getDest(){return dest;}
DIR_T Graph::Edge::getDir(){ return dir; }

/*************** Graph Nodes ************************/
/*** private ***/
list<Graph::Edge*>::iterator Graph::Node::findEdge(uint32_t partner_id, list<Edge*> list)
{
	std::list<Edge*>::iterator e_it;
	
	e_it = find_if(list.begin(), list.end(),
		[partner_id](Edge* e)
		{return e->getDest()->getID() == partner_id; }) ;
	return e_it;
}

/*** public ***/
Graph::Node::Node(uint32_t id, T_Node *info): id(id), info(info){} 

uint32_t Graph::Node::getID() const{return id;}
T_Node* Graph::Node::getInfo(){return info;}
void Graph::Node::setInfo(T_Node* info){ this->info = info; }			

//TODO: Add try catch to below
ERR_T Graph::Node::addIn(Edge* e){ e_in.push_back(e); return E_NONE; }
ERR_T Graph::Node::addOut(Edge* e){ e_out.push_back(e); return E_NONE; }

Graph::Edge* Graph::Node::removeEdgeFromLists(Edge* e, NEND_T end)
{	
	std::list<Edge*> list;

	if(e==nullptr){ return nullptr;}
	if(end == NEND_SRC){ list = e_out; }
	else if(end == NEND_DEST){ list = e_in; }
	else{ return nullptr; }

	list.remove(e);
	
	if(e->getDir() != DIR_BIDIR){ return e; }
	else
	{
		if(list == e_in) { list = e_out; }
		if(list == e_out){ list = e_in;}
		list.remove(e);
	}
	return e;
}
Graph::Edge* Graph::Node::removeEdgeFromLists(uint32_t partner_id, NEND_T end)
{
	Edge* e;
	std::list<Edge*> list;
	std::list<Edge*>::iterator e_it;
	
	//get correct list
	if(end == NEND_SRC){ list = e_out; }
	else if(end == NEND_DEST){ list = e_in; }
	else{ return nullptr; }
	//find edge
	e_it = findEdge(partner_id, list);
	e = *e_it;
	if(e == nullptr || 
		(end == NEND_SRC  && e->getDest()->getID() != partner_id) ||
		(end == NEND_DEST && e->getSrc()->getID()  != partner_id))
	{
		return nullptr; 
	}
	//erase from list
	cout << (*e_it)->getDest()->getID()<<endl;
	list.erase(e_it);
	cout<<e->getDest()->getID() <<endl;	
	//must delete entry from other list if bidirectional
	if(e->getDir() != DIR_BIDIR){ return e; }
	else
	{
		Edge* e_bidir;
		
		//list swap
		if(list == e_in) { list = e_out; }
		if(list == e_out){ list = e_in;}
		
		//almost the same as above
		e_it = this->findEdge(partner_id, list);
		e_bidir = *e_it;
		if(e != e_bidir) //should be same edge
		{
			return nullptr; 
		}
		list.erase(e_it);
	}
	return e;
}

//print
void Graph::Node::printOutEdges(ostream& out, const char bidir)
{
	list<Edge*>::iterator l_it; 
	
	for(l_it = e_out.begin(); l_it != e_out.end(); l_it++)
	{
		if((*l_it)->getDir()==DIR_BIDIR){ out << "*";}
		cout << (*l_it)->getDest()->getID() << "\t";
	}
	return;
}
//operator overloading
bool Graph::Node::operator==(const uint32_t nid){ return (nid == this->id) ; }
