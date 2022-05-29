#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory>
#include <stack>
#include <string>

class State {
    public:
    State();
    ~State();
    void BuildStack(std::vector<std::stack<char>*>* input);
    std::vector<State*>* ProduceNextMoves();
    void PrintState();
    int ComputeCost0(State final);
    int ComputeCost1(State final);
    int ComputeCost2(State final);
    int ComputeCost3(State final);
    std::vector<std::vector<char> *>* MakeMatrices(State state);
    std::string GetKey();
    friend bool operator==(State& lhs, State& rhs);
    private:
        std::vector<std::stack<char>*>* stacks;
        std::string key;
};

#endif