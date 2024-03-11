//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_DISTRIBUTEDMATTERAPI_HPP
#define CPP_DISTRIBUTEDMATTERAPI_HPP

#include "Automation.hpp"

#include <vector>
#include <string>

using namespace std;

class DistributedMatterAPI
{
public:
    static bool AddAutomationToDevice(const string &device_alias, const Automation &automation);
    static bool RemoveAutomationToDevice(const string &device_alias, const string &automation_alias);
    static vector<string> GetDevices();
    static vector<Automation *> GetAutomations(const string &alias);
};

#endif // CPP_DISTRIBUTEDMATTERAPI_HPP
