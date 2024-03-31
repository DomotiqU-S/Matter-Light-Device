//
// Created by talgarr on 01/02/24.
//

#include "ActionCallService.hpp"

ActionCallService::ActionCallService(string alias, string service, string service_data)
    : Action(std::move(alias))
{
    this->service = std::move(service);
    this->service_data = std::move(service_data);
}

ActionCallService::~ActionCallService() = default;

void ActionCallService::Do()
{
    // TODO: Replace next line with endpointcall
    // DistributedDevice::Instance().TriggerIO(this->service, this->service_data);
}
