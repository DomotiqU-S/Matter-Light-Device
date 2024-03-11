//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_DISTRIBUTEDSERVER_HPP
#define CPP_DISTRIBUTEDSERVER_HPP

#include <map>
#include <thread>

#include "DistributedMatterAPI.hpp"
#include "Automation.hpp"

class DistributedServer
{
private:
    vector<string> devices_alias;
    map<string, vector<Automation *>> automations_by_devices;
    bool running;

    DistributedServer();
    ~DistributedServer();

    // Getter

    // Setter
    bool AddDevice(const string &alias);

    // Network
    bool AddAutomationToDevice(const string &device_alias, Automation automation);
    void RemoveAutomationFromDevice(const string &device_alias, string automation_alias);
    void FetchAutomationsFromDevice(const string &alias);
    void FetchAutomationsFromAllDevices();

    // Storage
    void WriteAutomations();
    void ReadAutomations();

public:
    static void Run();
};

#endif // CPP_DISTRIBUTEDSERVER_HPP
