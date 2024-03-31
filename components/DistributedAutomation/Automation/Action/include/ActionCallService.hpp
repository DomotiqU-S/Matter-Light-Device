//
// Created by talgarr on 01/02/24.
//

#ifndef DISTRIBUTED_SERVER_ACTIONCALLSERVICE_HPP
#define DISTRIBUTED_SERVER_ACTIONCALLSERVICE_HPP

#include "Action.hpp"
// #include "DistributedDevice.hpp"
#include "json.hpp"

class ActionCallService : public Action
{
private:
    string service;
    string service_data;

public:
    ActionCallService(string alias, string service, string service_data);
    ~ActionCallService() override;
    void Do() override;
};

#endif // DISTRIBUTED_SERVER_ACTIONCALLSERVICE_HPP
