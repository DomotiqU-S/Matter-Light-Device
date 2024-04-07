//
// Created by talgarr on 01/02/24.
//

#ifndef DISTRIBUTED_SERVER_ACTION_HPP
#define DISTRIBUTED_SERVER_ACTION_HPP

#include <iostream>
#include <string>
#include <utility>
#include "json.hpp"

using namespace std;

class Action
{
public:
    explicit Action();
    virtual ~Action();
    virtual void Do() = 0;
    virtual std::string Print() = 0;
    std::string alias;
};

#endif // DISTRIBUTED_SERVER_ACTION_HPP
