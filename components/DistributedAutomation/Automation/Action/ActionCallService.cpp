//
// Created by talgarr on 01/02/24.
//

#include "ActionCallService.hpp"

ActionCallService::ActionCallService(string service, string service_data) : Action()
{
    this->service = std::move(service);
    this->service_data = std::move(service_data);
}

ActionCallService::~ActionCallService() = default;

void ActionCallService::Do()
{
    // TODO: Replace next line with endpointcall
    // DistributedAutomation::Device::Instance().TriggerIO(this->service, this->service_data);
    ESP_LOGE(this->TAG, "ActionCallService::Do %s %s", this->service.c_str(), this->service_data.c_str());
}

string ActionCallService::Print()
{
    std::string str = "ActionCallService: ";
    str += this->service;
    str += " ";
    str += this->service_data;
    return str;
}