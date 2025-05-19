#include "partlist.h"

void showSyntax(char** argv) {
		cout << "This program lists the partition and the length of each edge in a given tree." << endl;
		cout << "Syntax:" << endl;
		cout << "   " << argv[0] << " [tree_file]" << endl;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		showSyntax(argv);
		exit(1);
	}
	
	gTree* tree = new gTree();
	vector<pair<string,double> >  part_info;
	double rms,d;
	int i;

	tree->loadTopFile(argv[1]);
	// get partitions and the corresponding edge lengths
	tree->getPartitions(part_info);

	// show the partition and the length of each edge
	cout << "Partition\tEdge length" << endl;
	for (i=0; i<part_info.size(); i++) {
		cout << part_info[i].first << "\t" << part_info[i].second << endl;
	}

	// clear the memory allocated for the tree
	delete(tree);
}


// constructor
gNode::gNode() {
	// initialise
	name = "";
	isLeaf = false;
	id = -1;
	leaf_id = -1;
	depth = -1;
}

void gNode::replaceChild(gNode* newChild, gNode* childToReplace, double newEdgeLen) {
    int i;
    bool isReplaced = false;
    for (i=0; i<children.size(); i++) {
        if (children[i] == childToReplace) {
            children[i] = newChild;
            edgeLens[i] = newEdgeLen;
            isReplaced = true;
            break;
        }
    }
    assert(isReplaced);
}

// return the child ID x where children[x] = child
int gNode::whichChild(gNode* child) {
	int i = 0;
	while (i < children.size() && children[i] != child) {
		i++;
	}
	if (i >= children.size()) {
		// child is not found
		i = -1;
	}
	return i;
}

// add the child into the list
void gNode::addChild(gNode* child) {
    children.push_back(child);
}

// add the child and the edge length into the lists
void gNode::addChild(gNode* child, double edge_len) {
    children.push_back(child);
    edgeLens.push_back(edge_len);
}

// whether this node can reach the node t
bool gNode::canReach(gNode* parent, gNode* t) {
	int i;
	if (t == this)
		return true;
	for (i=0; i<children.size(); i++) {
		if (children[i] != parent) {
			if (children[i]->canReach(this, t))
				return true;
		}
	}
	return false;
}

// collect all the leaves
void gNode::collectAllLeaves(gNode* parent, vector<string>& allLeaves) {
	int i;

	for (i=0; i<children.size(); i++) {
		if (children[i] != parent) {
			children[i]->collectAllLeaves(this, allLeaves);
		}
	}

	if (isLeaf) {
		allLeaves.push_back(name);
	}
}

// destructor
gTree::~gTree() {
    clear();
}

// load the topology file
void gTree::loadTopFile(char* fileName) {
    int s, t;
    bool isCommentLine;
    gNode* aNode;
    ifstream fin;
    string str;
    
    isCommentLine = true;
    fin.open(fileName);
    while (isCommentLine) {
    	getline(fin, str); // only one line in the file
    	rmSpaces(str); // remove the spaces on the line
    	if (str.length() > 0 && str[0] != '#') {
    		isCommentLine = false;
    	}
    }
    fin.close();
    
    loadTreeStr(str);
}

// load the tree sequence
void gTree::loadTreeStr(string str) {
    int s, t;
    gNode* aNode;

    s = 0;
    while (s < str.length()) {
        if (str[s] == '(')
            break;
        s++;
    }
    t = str.length() - 1;
    while (t >= 0) {
        if (str[t] == ')')
            break;
        t--;
    }
    aNode = createInterNode(str, s, t);
    root = aNode;
    
    // update all the nodes
    updateInfo();
}

// clear
void gTree::clear() {
    vector<gNode*> toProcess;
    set<gNode*> inToProcess;
    set<gNode*>::iterator itr;
    gNode* cNode;
    gNode* childNode;
    int i,j;
    
    i=0;
    if (root != NULL) {
    	toProcess.push_back(root);
    	inToProcess.insert(root);
    }
    while (i < toProcess.size()) {
        cNode = toProcess[i];
        for (j=0; j<cNode->children.size(); j++) {
        	childNode = cNode->children[j];
        	itr = inToProcess.find(childNode);
        	if (itr == inToProcess.end()) {
                toProcess.push_back(childNode);
                inToProcess.insert(childNode);
            }
        }
        i++;
    }
    for (i=0; i<toProcess.size(); i++)
        delete toProcess[i];
}

// supporting functions

void gTree::rmSpaces(string& str) {
	int i,k;
	k=0;
	for (i=0; i<str.length(); i++) {
		if (str[i] != ' ') {
			if (i > k)
				str[k] = str[i];
			k++;
		}
	}
	if (k < str.length())
		str.resize(k);
}

int gTree::getCloseBracketPos(string& str, int openBracPos) {
    int i,k;
    k=0;
    for (i=openBracPos+1; i<str.length(); i++) {
    	if (str[i] == '(') {
    	    k++;
    	} else if (str[i] == ')') {
    	    if (k==0)
    	        return i;
    	    else
    	        k--;
    	}
    }
    return -1; // not a balanced brackets
}

// create a node for str[startPos ... endPos]
// this node has to be enclosed by a pair of brackets
// i.e. str[startPos] == '(' and str[endPos] == ')'
gNode* gTree::createInterNode(string& str, int startPos, int endPos) {
    int p, s, t;
    gNode* cNode;
    gNode* childNode;
    string v;
    double edgeLen;
    cNode = new gNode();
    childNode = NULL;
    cNode->isLeaf = (str[startPos] != '('); // if the first character is '(', then it is not a leaf
    
    p=startPos+1;
    while (p<=endPos) {
         if (str[p] == '(') {
             // an internal node
             s = p;
             t = getCloseBracketPos(str, s);
             if (t==-1) {
                 cerr << "Error! The brackets are not balanced" << endl;
                 exit(1);
             }
             childNode = createInterNode(str, s, t);
             p=t+1;
         } else {
             // a leaf node
             v = "";
             while (str[p]!=',' && str[p]!=':' && str[p]!=')') {
                 v.append(1, str[p]);
                 p++;
             }
             childNode = new gNode();
             childNode->name = v;
         }
         // get the edge length if available
         edgeLen = 0.0;
         if (str[p]==':') {
             v = "";
             p++;
             while (str[p]!=',' && str[p]!=')') {
                 v.append(1, str[p]);
                 p++;
             }
             edgeLen = atof(v.c_str());
         }
         // add the child
         if (str[p]==',' || str[p]==')') {
             cNode->addChild(childNode);
             childNode->addChild(cNode);
             cNode->edgeLens.push_back(edgeLen);
             childNode->edgeLens.push_back(edgeLen);
         }
         p++;
    }
    return cNode;
}

// update all the nodes
void gTree::updateInfo() {
    vector<gNode*> toProcess;
    set<gNode*> inToProcess;
    set<gNode*>::iterator itr;
    gNode* cNode;
    gNode* childNode;
    double edgeLen;
    int i,j;
    
    i=0;
    leaves.clear();
    allNodes.clear();
    edges.clear();
    if (root != NULL) {
		root->depth = 0;
		toProcess.push_back(root);
		inToProcess.insert(root);
	}
    while (i < toProcess.size()) {
        cNode = toProcess[i];
        cNode->id = i;
        // cout << "* cNode->id = " << i << endl;
        
        allNodes.push_back(cNode);
        if (cNode->children.size() == 1) {
            // is a leaf
            leaves.push_back(cNode);
            cNode->isLeaf = true;
        } else {
            cNode->isLeaf = false;
		}
		
		for (j=0; j<cNode->children.size(); j++) {
			childNode = cNode->children[j];
			edgeLen = cNode->edgeLens[j];
			itr = inToProcess.find(childNode);
			if (itr == inToProcess.end()) {
				childNode->depth = cNode->depth + 1;
				toProcess.push_back(childNode);
				inToProcess.insert(childNode);
				edges.push_back(pair<gNode*,gNode*>(cNode,childNode));
			}
		}
        i++;
        
    }
}

// convert to a unrooted tree
void gTree::toUnRoot() {
    pair<gNode*,gNode*> child_pair;
    gNode* node_to_rm;
    
    // if this is a rooted tree, remove the root first
    if (root->children.size()==2) {
    	node_to_rm = root;
    	child_pair = rmInterNode_twoChildren(root);
    	delete node_to_rm;
    }
    
    updateInfo();
}

// check whether the tree is an unrooted tree
bool gTree::isUnRoot() {
	return (root->children.size()==3);
}

// remove a node from the array
void gTree::rmNodeFrArray(gNode* nodeToRemove) {
    int i,k;
    k=0;
    // remove that node from the array "allNodes"
    for (i=0; i<allNodes.size(); i++) {
        if (allNodes[i] != nodeToRemove) {
            if (k < i) {
                allNodes[k] = allNodes[i];
            }
            k++;
        }
    }
    allNodes.resize(k);
}

// remove an edge from the array
void gTree::rmEdgeFrArray(gNode* n1, gNode* n2) {
    int i,k;
    k=0;
    for (i=0; i<edges.size(); i++) {
        if ((edges[i].first == n1 && edges[i].second==n2) || (edges[i].first == n2 && edges[i].second==n1)) {
            // remove the edge
        } else {
            if (k < i) {
                edges[k] = edges[i];
            }
            k++;
        }
    }
    edges.resize(k);
}

// remove an internal node with two children
// return on both of the children if removal is successful
// return pair<NULL,NULL>, otherwise
pair<gNode*,gNode*> gTree::rmInterNode_twoChildren(gNode* nodeToRm) {
    gNode* n1 = NULL;
    gNode* n2 = NULL;
    double edgeLen1, edgeLen2, newEdgeLen;
    int i;
    
    if (nodeToRm->children.size()==2) {
        n1 = nodeToRm->children[0];
        n2 = nodeToRm->children[1];
        edgeLen1 = nodeToRm->edgeLens[0];
        edgeLen2 = nodeToRm->edgeLens[1];
        newEdgeLen = edgeLen1 + edgeLen2;
        n1->replaceChild(n2, nodeToRm, newEdgeLen);
        n2->replaceChild(n1, nodeToRm, newEdgeLen);
        rmEdgeFrArray(nodeToRm, n1);
        rmEdgeFrArray(nodeToRm, n2);
        rmNodeFrArray(nodeToRm);
        edges.push_back(pair<gNode*,gNode*>(n1,n2));
        
        if (root == nodeToRm || (!n1->canReach(NULL, root))) {
			// it is a unrooted tree with the root previously assigned to node_1
			// or the root is not inside the tree
			assignRootRandomly(n1);
		}
    }

    return pair<gNode*,gNode*>(n1,n2);
}

// assign the root randomly to an internal node connected to sNode
void gTree::assignRootRandomly(gNode* aNode) {
	int i,j;
	gNode* cNode;
	gNode* childNode;
	vector<gNode*> toProcess;
	set<gNode*> inToProcess;
    set<gNode*>::iterator itr;
	
    i=0;
    root = NULL;
    if (aNode != NULL) {
    	toProcess.push_back(aNode);
    	inToProcess.insert(aNode);
    }
    while (i < toProcess.size()) {
        cNode = toProcess[i];
        if (cNode->children.size() == 3) {
        	root = cNode;
        	break;
        }
		for (j=0; j<cNode->children.size(); j++) {
			childNode = cNode->children[j];
			itr = inToProcess.find(childNode);
			if (itr == inToProcess.end()) {
				toProcess.push_back(childNode);
				inToProcess.insert(childNode);
			}
		}
        i++;
    }
    if (root == NULL)
    	root = aNode;
}

string vector2str(vector<string>& str_vector) {
	int i;
	string s = "";
	sort(str_vector.begin(), str_vector.end());
	for (i=0; i<str_vector.size(); i++) {
		if (i>0)
			s.append(",");
		s.append(str_vector[i]);
	}
	return s;
}

struct paircomp {
  bool operator() (const pair<string,double>& lhs, const pair<string,double>& rhs) const
  {return lhs.first<rhs.first;}
};

// get partitions and the corresponding edge lengths
void gTree::getPartitions(vector<pair<string,double> >&  part_info) {
	gNode *node1, *node2;
	string part;
	string s1, s2;
	int i;
	double len;
	
	// clear the array
	part_info.clear();
	
	// change to unrooted tree if it is a rooted tree
	if (!isUnRoot())
		toUnRoot();
    
    vector<string> allLeaves;
	for (i=0; i<edges.size(); i++) {
		node1 = edges[i].first;
		node2 = edges[i].second;
		allLeaves.clear();
		node1->collectAllLeaves(node2, allLeaves);
		sort(allLeaves.begin(),allLeaves.end());
		s1 = vector2str(allLeaves);
		allLeaves.clear();
		node2->collectAllLeaves(node1, allLeaves);
		sort(allLeaves.begin(),allLeaves.end());
		s2 = vector2str(allLeaves);
		if (s1 < s2)
			part = s1 + "::" + s2;
		else
			part = s2 + "::" + s1;
		len =  node1->edgeLens[node1->whichChild(node2)];
		part_info.push_back(pair<string,double>(part,len));
	}
	sort(part_info.begin(), part_info.end());
}
