//
// Created by talgarr on 08/02/24.
//

#ifndef DISTRIBUTED_AUTOMATIONS_TRIGGERSTRINGSTATE_HPP
#define DISTRIBUTED_AUTOMATIONS_TRIGGERSTRINGSTATE_HPP

#include "TriggerState.hpp"
#include "json.hpp"

class TriggerStringState : public TriggerState
{
private:
    string from_state;
    string to_state;
    string previous_value;

public:
    TriggerStringState(std::string alias, std::string attribute, time_t for_s,
                       std::string from_state, std::string to_state);
    void IO(string attribute, string value) override;
    ~TriggerStringState() = default;
};

#endif // DISTRIBUTED_AUTOMATIONS_TRIGGERSTRINGSTATE_HPP
