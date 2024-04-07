//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_DISTRIBUTEDDEVICE_HPP
#define CPP_DISTRIBUTEDDEVICE_HPP

#include <map>
#include <thread>
#include <condition_variable>
#include <algorithm>
#include <cstring>
#include "json.hpp"
#include "JsonUtils.hpp"

#include "esp_log.h"
#include "Automation.hpp"
#include "State.hpp"
#include "StateCollection.hpp"
#include "DistributedMatterAPI.hpp"

namespace DistributedAutomation
{
    static const char *TAG = "Device";
    class Device
    {
    private:
        vector<Automation *> automations;
        bool running;
        mutex cv_m;
        condition_variable cv;

        Device();
        ~Device();

        void CreateAutomationsThreads();

    public:
        static Device &Instance()
        {
            static Device instance;
            return instance;
        };

        // Setter
        bool AddAutomation(Automation *automation);
        void RemoveAutomation(string alias);
        void SetAutomationsFromJson(nlohmann::json json);

        // Getter
        vector<Automation *> GetAutomations();

        void UpdateAutomations();

        void TriggerIO(const string &attribute, const string &value);

        void Run();
        void Stop();
    };
}

#endif // CPP_DISTRIBUTEDDEVICE_HPP
