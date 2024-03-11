//
// Created by talgarr on 01/02/24.
//
#include "DistributedServer.hpp"

DistributedServer::DistributedServer()
{
    this->devices_alias = vector<string>();
    this->automations_by_devices = map<string, vector<Automation *>>();

    ReadAutomations();
    this->devices_alias = DistributedMatterAPI::GetDevices();
    FetchAutomationsFromAllDevices();

    this->running = true;
}

DistributedServer::~DistributedServer()
{
    this->devices_alias.clear();
    for (auto &automations : this->automations_by_devices)
    {
        automations.second.clear();
    }
    this->automations_by_devices.clear();
}

bool DistributedServer::AddDevice(const string &alias)
{
    this->devices_alias.push_back(alias);
    this->automations_by_devices[alias] = vector<Automation *>();
    return true;
}

bool DistributedServer::AddAutomationToDevice(const string &device_alias, Automation automation)
{
    for (auto &automat : this->automations_by_devices[device_alias])
    {
        if (automat->GetAlias() == automation.GetAlias())
        {
            return false;
        }
    }
    if (DistributedMatterAPI::AddAutomationToDevice(device_alias, automation))
    {
        this->automations_by_devices[device_alias].push_back(&automation);
        return true;
    }
    else
    {
        return false;
    }
}

void DistributedServer::RemoveAutomationFromDevice(const string &device_alias, string automation_alias)
{
}

void DistributedServer::FetchAutomationsFromDevice(const string &alias)
{
    for (auto &device : this->devices_alias)
    {
        if (device == alias)
        {
            {
                vector<Automation *> automations = DistributedMatterAPI::GetAutomations(alias);
                this->automations_by_devices[alias] = automations;
                return;
            }
        }
    }
}

void DistributedServer::FetchAutomationsFromAllDevices()
{
    for (auto &device : this->devices_alias)
    {
        FetchAutomationsFromDevice(device);
    }
}

void DistributedServer::Run()
{
    DistributedServer server = DistributedServer();
}

void DistributedServer::ReadAutomations()
{
}

void DistributedServer::WriteAutomations()
{
}
