//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_TRIGGERNUMERICSTATE_HPP
#define CPP_TRIGGERNUMERICSTATE_HPP

#include <vector>
#include <string>
#include <utility>
#include "TriggerState.hpp"
#include "json.hpp"

using namespace std;

class TriggerNumericState : public TriggerState
{
private:
    //    string value_template;
    double above;
    double below;

public:
    TriggerNumericState(string alias, string attribute, time_t for_s, double above, double below);
    void IO(string attribute, string value) override;
    ~TriggerNumericState();
    static TriggerNumericState *Json2Trigger(nlohmann::json json);
};

#endif // CPP_TRIGGERNUMERICSTATE_HPP
