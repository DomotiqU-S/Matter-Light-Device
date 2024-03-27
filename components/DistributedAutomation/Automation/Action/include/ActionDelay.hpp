//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_ACTIONDELAY_HPP
#define CPP_ACTIONDELAY_HPP

#include "Action.hpp"
#include <thread>
#include "json.hpp"

class ActionDelay : public Action
{
private:
    time_t delay_s;

public:
    ActionDelay(string alias, time_t delay_s);
    ~ActionDelay() override;
    void Do() override;
};

#endif // CPP_ACTIONDELAY_HPP
