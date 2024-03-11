//
// Created by talgarr on 01/02/24.
//

#ifndef DISTRIBUTED_SERVER_ACTION_HPP
#define DISTRIBUTED_SERVER_ACTION_HPP

#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Action {
public:
    explicit Action(std::string alias);
    virtual ~Action();
    virtual void Do() = 0;

    std::string alias;
};


#endif //DISTRIBUTED_SERVER_ACTION_HPP
