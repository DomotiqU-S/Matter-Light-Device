//
// Created by talgarr on 01/02/24.
//

#ifndef DISTRIBUTED_SERVER_ACTIONCALLSERVICE_HPP
#define DISTRIBUTED_SERVER_ACTIONCALLSERVICE_HPP

#include "Action.hpp"
// #include "DistributedDevice.hpp"
#include "json.hpp"
#include "esp_log.h"

class ActionCallService : public Action
{
private:
    string service;
    string service_data;
    const char *TAG = "ActionCallService";

public:
    ActionCallService(string service, string service_data);
    ~ActionCallService() override;
    void Do() override;
    string Print() override;
};

#endif // DISTRIBUTED_SERVER_ACTIONCALLSERVICE_HPP
