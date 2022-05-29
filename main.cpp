#include "Node.h"
#include "State.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <unistd.h>


using namespace std;
struct CompareNodes {
    bool operator()(Node* const& n1, Node* const& n2) {
        return n1->GetFn() > n2->GetFn();
    }
};

void ReadFile(ifstream& inFS, State& initialState, State& finalState, int& numStacks, int& numBlocks, int& numRandomMoves) {
    string line;
    vector<stack<char>*>* start = new vector<stack<char>*>();
    vector<stack<char>*>* end = new vector<stack<char>*>();
    int separatorCount = 0;
    while(getline(inFS, line)) {
        istringstream inSS(line);
        int peakVal = inSS.peek();
        if(isdigit(peakVal)) { //Values
            inSS >> numStacks >> numBlocks >> numRandomMoves;
        }
        else if(peakVal == 62) {
            separatorCount++;
            continue;
        }
        else { //Blocks
            stack<char>* newStack = new stack<char>();
            char value;
            inSS >> value;
            while(inSS) {
                newStack->push(value);
                inSS >> value;
            }
            if(separatorCount == 1) {
                start->push_back(newStack);
            }
            else {
                end ->push_back(newStack);
            }
        }
    }
    initialState.BuildStack(start);
    finalState.BuildStack(end);

}

vector<Node *> AStarSearch(State& initialState, State& finalState, int max_iterations, int& counter, string hn, int& maxQueueSize) {
    Node* initial = new Node(initialState, finalState, hn);
    priority_queue<Node*, vector<Node*>, CompareNodes > frontier = priority_queue<Node*, vector<Node*>, CompareNodes >();
    vector<Node*> correctPath;
    frontier.push(initial);
    unordered_map<string, int> visited; //State and pathcost
    visited.insert(make_pair<string, int>(initial->GetState().GetKey(), initial->GetHeuristic()));
    if(initialState.GetKey() == finalState.GetKey()) {
        cout << "Found it" << endl;
    }
    while(!frontier.empty()) {
        if(frontier.size() > maxQueueSize) {
            maxQueueSize = frontier.size();
        }
        counter++;
        Node* currNode = frontier.top();
        //cout << "Popped child with fn = " << currNode->GetFn() << endl;
        frontier.pop();
        if(currNode->GetState().GetKey() == finalState.GetKey()) {
            while(currNode->GetParent() != NULL) {
                correctPath.push_back(currNode);
                currNode = currNode->GetParent();
            }
            break;
        }
        vector<Node*>* children = currNode->ProduceChildren(finalState);
        int numChildren = 0;
        for(int i = 0; i < children->size(); i++) {
            State childState = children->at(i)->GetState();

            if(visited.find(childState.GetKey()) == visited.end()) {
                int hn = children->at(i)->GetHeuristic();
                visited.insert(make_pair<string, int&>(childState.GetKey(), hn));
                //cout << "Pushed child with fn = " << fn << endl;
                frontier.push(children->at(i));
                numChildren++;
            }
        }
        if(counter % 1000 == 0) {
            cout << "Iteration: " << counter << ", Depth: " << currNode->GetPathCost() << ", Hn: " << currNode->GetHeuristic();
            cout << ", Fn: " << currNode->GetFn() << ", Qsize: " << frontier.size() << ", New Children: " << numChildren << endl;
        }
        if(counter > max_iterations) {
            cout << "FAILED" << endl;
            return correctPath;
        }

    }
    return correctPath;
}

int main(int argc, char **argv) {
    int opt;
    string heuristic = "h1";
    int max_iterations = 200000;
    bool hSpecified = false;
    if(argc < 2) {
        cout << "Specify file problem" << endl;
        return 1;
    }

    while((opt = getopt(argc, argv, "H:")) != -1) {
        switch(opt) {
            case 'H':
                heuristic = optarg;
                hSpecified = true;
                break;
        }

    }
    string filename;
    if(hSpecified) {
        filename = argv[3];
    }
    else {
        filename = argv[1];
    }

    //string heuristic(argv[2]);
    //int max_iterations = atoi(argv[3]);

    ifstream inFS;
    inFS.open("probs/" + filename + ".bwp");

    State initialState, finalState;
    int numStacks, numBlocks, numRandomMoves;
    
    ReadFile(inFS, initialState, finalState, numStacks, numBlocks, numRandomMoves);

    int counter = 0;
    int maxQueueSize = 0;
    vector<Node *> correctPath = AStarSearch(initialState, finalState, max_iterations, counter, heuristic, maxQueueSize);

    cout << "Beginning" << endl;
    initialState.PrintState();
    cout << "--------------------------------------" << endl;
    cout << "--------------------------------------" << endl;
    int count = 1;
    for(int i = correctPath.size()-1; i > -1; i--) {
        cout << "Step " << count << ", pathcost=" << correctPath[i]->GetPathCost() << ", heuristic=" << correctPath[i]->GetHeuristic();
        cout << ", f(n)=g(n)+h(n)=" << correctPath[i]->GetPathCost() + correctPath[i]->GetHeuristic() << endl;
        correctPath[i]->GetState().PrintState();
        cout << "--------------------------------------" << endl;
        count++;
    }
    cout << "Problem: " << filename << " took " << counter << " iterations with heuristic " << heuristic << endl;
    cout << "This max queue size was: " << maxQueueSize << endl;
    cout << "Hn of initial: " << initialState.ComputeCost1(finalState) << endl;


    

}