#include "State.h"
#include <math.h>
#include <iostream>
using namespace std;

State::State() {
    stacks = new vector<stack<char>*>();
}

State::~State() {

}
void State::BuildStack(vector<stack<char>*>* input) {
    stacks = input;
    string key;
    vector<stack<char>*> copy;
    for(int i = 0; i < stacks->size(); i++) {
        stack<char>* currStack = new stack<char>(*stacks->at(i));
        copy.push_back(currStack);
    }
    for(int i = 0; i < copy.size(); i++) {
        stack<char>* currStack = copy[i];
        while(!currStack->empty()) {
            char block = currStack->top();
            currStack->pop();
            key.push_back(block);
        }
        key.push_back('|');
    }
    this->key = key;
}

vector<State*>* State::ProduceNextMoves() {
    vector<State*>* nextMoves = new vector<State*>();
    for(int i = 0; i < stacks->size(); i++) { //For each stack
        if(stacks->at(i)->size() == 0) {
            continue;
        }
        char block = stacks->at(i)->top();
        for(int j = 0; j < stacks->size(); j++) { //For each stack that isn't selected by the outerloop
            if(i != j) {
                vector<stack<char>*>* newStacks = new vector<stack<char>*>();
                for(int k = 0; k < stacks->size(); k++) {
                    stack<char>* newStack = new stack<char>(*stacks->at(k));
                    if(k == i) {
                        newStack->pop();
                    }
                    if(k == j) {
                        newStack->push(block);
                    }
                    newStacks->push_back(newStack);
                }
                
                State* newState = new State();
                newState->BuildStack(newStacks);
                nextMoves->push_back(newState);
            }
        }
    }
    return nextMoves;
}

std::string State::GetKey() {
    return key;
}

void State::PrintState() {
    for(int i = 0; i < stacks->size(); i++) {
        stack<char>* currStack = stacks->at(i);
        string line;
        while(!currStack->empty()) {
            
            char top = currStack->top();
            line.push_back(top);
            currStack->pop();
        }
        for(int i = line.size()-1; i > -1; i--) {
            cout << line[i];
        }
        cout << endl;
        line.clear();
    }
}

int State::ComputeCost0(State final) {
    return 1;
}

int State::ComputeCost1(State final) {
    string fKey(final.key);
    string currKey(this->key);
    for(int i = 0; i < fKey.size(); i++) {
        fKey[i] = final.key[(fKey.size()-1)-i];
        currKey[i] = this->key[(fKey.size()-1)-i];
    }
    int multiplier = 1;
    int sum = 0;
    double numBlocksOutOfPlace = 0;
    for(int i = fKey.size()-1; i > -1; i--) {
        if(currKey[i] == '|') {
            multiplier = 1;
        }
        
        if(fKey[i] != currKey[i]) {
            sum += 1*multiplier;
            numBlocksOutOfPlace++;
        }
        multiplier++;
    }
    if(numBlocksOutOfPlace != 0) {
        return ((int)ceil(sum/numBlocksOutOfPlace))+numBlocksOutOfPlace;
    }
    else {
        return sum;
    }

}

int State::ComputeCost2(State final) {

    string fKey(final.key);
    string currKey(this->key);
    for(int i = 0; i < fKey.size(); i++) {
        fKey[i] = final.key[(fKey.size()-1)-i];
        currKey[i] = this->key[(fKey.size()-1)-i];
    }
    
    int blocks = 0;
    int block2 = 0;
    char prevChar;
    for(int i = 0; i < fKey.size(); i++) {
        if(fKey[i] == '|') {
            prevChar = ' ';
            continue;
        }
        if(currKey[i] == prevChar) {
            block2++;
        }
        else if(currKey[i] != fKey[i]) {
            blocks++;
        }
        prevChar = fKey[i];

    }
        
    return blocks*2 + block2*4;
}

int State::ComputeCost3(State final) {
    vector<vector<char> *>* currState = new vector<vector<char> *>();
    vector<vector<char> *>* finalState = new vector<vector<char> *>();
    currState = MakeMatrices(*this);
    finalState = MakeMatrices(final);
    int sum = 0;
     for(int i = 0; i < currState->size(); i++) {

         vector<char>* currStack = currState->at(i);
         vector<char>* finalStack = finalState->at(i);
         int currStackSize = currState->at(i)->size();
         int finalStackSize = finalState->at(i)->size();

         for(int j = 0; j < currStackSize; j++) {
             char currTop = ' ';
             char currBottom = ' ';
             char currLeft = ' ';
             char currRight = ' ';

             char finalTop = ' ';
             char finalBottom = ' ';
             char finalLeft = ' ';
             char finalRight = ' ';

             if(j+1 < currStackSize) {
                 currTop = currStack->at(j+1);
             }
        
             if(j-1 > -1) {
                 currBottom = currStack->at(j-1);
             }
           
             if(i+1 < currState->size()) {
                 if(j < currState->at(i+1)->size()) {
                     currRight = currState->at(i+1)->at(j);
                 }
            }
        
            if(i-1 > -1) {
                 if(j < currState->at(i-1)->size()) {
                     currLeft = currState->at(i-1)->at(j);
                 }
            }
         

            if(j+1 < finalStackSize) {
                finalTop = finalStack->at(j+1);
            }
       
            if(j-1 > -1 && j-1 < finalStackSize) {
                finalBottom = finalStack->at(j-1);
            }
     
            if(i+1 < finalState->size()) {
                if(j < finalState->at(i+1)->size()) {
                    finalRight = finalState->at(i+1)->at(j);
                }
            }
         
            if(i-1 > -1) {
                if(j < finalState->at(i-1)->size()) {
                    finalLeft = finalState->at(i-1)->at(j);
                }
            }

            if(currTop != finalTop) {
                sum++;
            }
            if(currBottom != finalBottom) {
                sum += 3;
            }
            if(currLeft != finalLeft) {
                sum+= 2;
            }
            if(currRight != finalRight) {
                sum+= 2;
            }
         }
     }
     return sum;
    
}

vector<vector<char> *>* State::MakeMatrices(State state) {
    vector<stack<char> *>* copyStacks = new vector<stack<char>*>();
    for(int i  = 0; i < state.stacks->size(); i++) {
        stack<char>* newStack = new stack<char>(*state.stacks->at(i));
        copyStacks->push_back(newStack);
    }
    vector<vector<char> *>* matrix = new vector<vector<char> *>();
    string reverseLine;
    for(int i = 0; i < copyStacks->size(); i++) {
        stack<char>* currStack = copyStacks->at(i);
        char block;
        while(!currStack->empty()) {
            block = currStack->top();
            currStack->pop();
            reverseLine.push_back(block);
        }
        matrix->push_back(new vector<char>);
        for(int j = reverseLine.size()-1; j > -1; j--) {
            matrix->at(i)->push_back(reverseLine[i]);
        }
        reverseLine.clear();
    }
    return matrix;
}

bool operator==(State& lhs, State& rhs) {
    for(int i = 0; i < lhs.stacks->size(); i++) {
        if(rhs.stacks->size() > i) {
            if(*rhs.stacks->at(i) != *lhs.stacks->at(i)) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}