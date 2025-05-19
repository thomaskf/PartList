#ifndef _EDGEDIFF_
#define _EDGEDIFF_

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <cassert>
#include <math.h>

using namespace std;
class gNode {
public:
    vector<gNode*> children;
    vector<double> edgeLens;
    string name;
    bool isLeaf;
    int id;
    int leaf_id;
    int depth;

	// constructor
	gNode();
	
    void replaceChild(gNode* newChild, gNode* childToReplace, double newEdgeLen);
    
//     int numDescendants(gNode* parent);
    
    // return the child ID x where children[x] = child
    int whichChild(gNode* child);

    // add the child into the list
    void addChild(gNode* child);

	// add the child and the edge length into the lists
	void addChild(gNode* child, double edge_len);

	// whether this node can reach the node t
	bool canReach(gNode* parent, gNode* t);
	
	// collect all the leaves
	void collectAllLeaves(gNode* parent, vector<string>& allLeaves);
};

// rooted or unrooted tree
class gTree {
public:

    // root / an internal node
    gNode* root;
    
    // leave nodes
    vector<gNode*> leaves;
    
    // all nodes
    vector<gNode*> allNodes;
    
    // edges
    vector<pair<gNode*,gNode*> > edges;
    
    // destructor
    ~gTree();

	// load the topology file
	void loadTopFile(char* fileName);
    
    // load the tree sequence
    void loadTreeStr(string treeStr);
    
    // clear
    void clear();
    
	// supporting functions

	void rmSpaces(string& str);

    int getCloseBracketPos(string& str, int openBracPos);

    // create a node for str[startPos ... endPos]
    // this node has to be enclosed by a pair of brackets
    // i.e. str[startPos] == '(' and str[endPos] == ')'
    gNode* createInterNode(string& str, int startPos, int endPos);
    
	// update all the information
	void updateInfo();
    
    // convert to a unrooted tree
    void toUnRoot();
    
    // check whether the tree is an unrooted tree
    bool isUnRoot();

	// get partitions and the corresponding edge lengths
	void getPartitions(vector<pair<string,double> >&  part_info);
	
private:

	// supplementary functions

	// remove a node from the array
	void rmNodeFrArray(gNode* nodeToRemove);

	// remove an edge from the array
	void rmEdgeFrArray(gNode* n1, gNode* n2);
	
	// remove an internal node with two children
	// return on both of the children if removal is successful
	// return pair<NULL,NULL>, otherwise
	pair<gNode*,gNode*> rmInterNode_twoChildren(gNode* nodeToRm);
	
	// assign the root randomly to an internal node connected to sNode
	void assignRootRandomly(gNode* aNode);
};

#endif