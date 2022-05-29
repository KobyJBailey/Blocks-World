#ifndef NODE_H
#define NODE_H

#include "State.h"
#include <iostream>

class Node {
public: 
    Node(State state, State finalState, std::string hNum);
    Node(State state, State finalState, Node* parent, std::string hNum);
    ~Node();
    State GetState();
    int GetHeuristic();
    int GetPathCost();
    int GetFn();
    Node* GetParent();
    int ComputeHeuristic(State final);
    std::vector<Node*>* ProduceChildren(State finalStat);
    friend bool operator<(Node& lhs, Node& rhs);
private:
    State state;
    Node* parent;
    std::string hNum;
    int fn;
    int hn;
    int pathcost;    
};

#endif