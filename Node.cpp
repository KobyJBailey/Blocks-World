#include "Node.h"
using namespace std; 

Node::Node(State state, State finalState, string hNum) {
    this->state = state;
    this->hNum = hNum;
    parent = NULL;
    hn = ComputeHeuristic(finalState);
    pathcost = 0;
    fn = pathcost + hn;
}
Node::Node(State state, State finalState, Node* parent, string hNum) {
    this->state = state;
    this->parent = parent;
    this->hNum = hNum;
    hn = ComputeHeuristic(finalState);
    pathcost = parent->GetPathCost();
    pathcost++;
    fn = pathcost + hn;
}

Node::~Node() {
    
}
State Node::GetState() {
    return state;
}
int Node::GetHeuristic() {
    return hn;
}

int Node::GetPathCost() {
    return pathcost;
}

int Node::GetFn() {
    return fn;
}

Node* Node::GetParent() {
    return parent;
}
int Node::ComputeHeuristic(State final) {
    if(hNum == "h0") {
        return this->state.ComputeCost0(final);
    }
    else if(hNum == "h1") {
        return this->state.ComputeCost1(final);
    }
    else if(hNum == "h2"){
        return this->state.ComputeCost2(final);
    }
    else {
        return this->state.ComputeCost3(final);
    }
}

vector<Node*>* Node::ProduceChildren(State finalState) {
    vector<Node*>* children = new vector<Node*>();
    vector<State*>* nextMoves = new vector<State*>();
    nextMoves = state.ProduceNextMoves();
    for(int i = 0; i < nextMoves->size(); i++) {
        Node* newNode = new Node(*nextMoves->at(i), finalState, (Node*)this, this->hNum);
        children->push_back(newNode);
    }
    return children;
}

bool operator<(Node& lhs, Node& rhs) {
    return lhs.fn < rhs.fn;
}