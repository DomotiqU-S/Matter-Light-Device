//
// Created by talgarr on 01/02/24.
//

#include "DistributedDevice.hpp"

DistributedDevice::DistributedDevice()
{
    this->running = true;
    this->automations = vector<Automation *>();
}

DistributedDevice::~DistributedDevice()
{
    this->running = false;
    for (auto &automation : this->automations)
    {
        automation->Stop();
        delete automation;
    }
    this->automations.clear();
}

bool DistributedDevice::AddAutomation(Automation *automation)
{
    for (auto &automat : this->automations)
    {
        if (automat->GetAlias() == automation->GetAlias())
        {
            return false;
        }
    }
    this->automations.push_back(automation);
    return true;
}

void DistributedDevice::RemoveAutomation(string alias)
{
    // for (auto &automation : this->automations)
    // {
    //     if (automation->GetAlias().compare(alias) == 0)
    //     {
    //         automation->Stop();
    //         this->automations.erase(std::remove(this->automations.begin(), this->automations.end(), this->automation.end()));
    //     }
    // }

    this->automations.erase(std::remove_if(this->automations.begin(), this->automations.end(), [alias](Automation *o)
                                           { return o->GetAlias().compare(alias) == 0; }),
                            this->automations.end());
    return;
}

void DistributedDevice::Run()
{
    if (this->automations.empty())
    {
        return;
    }
    this->CreateAutomationsThreads();
    while (this->running)
    {
        unique_lock<mutex> lock(this->cv_m);
        for (auto &automation : this->automations)
        {
            if (automation->HasTriggered())
            {
                if (automation->Verify())
                    automation->Do();
            }
        }
        this->cv.wait(lock);
    }
}

vector<Automation *> DistributedDevice::GetAutomations()
{
    return this->automations;
}

void DistributedDevice::CreateAutomationsThreads()
{
    for (auto automation : this->automations)
    {
        thread t(&Automation::Run, automation, &this->cv, &this->cv_m);
        t.detach();
    }
}

void DistributedDevice::Stop()
{
    unique_lock<mutex> lock(this->cv_m);
    this->running = false;
    for (auto &t : this->automations)
    {
        t->Stop();
    }
    this->cv.notify_all();
}

void DistributedDevice::TriggerIO(const string &attribute, const string &value)
{
    if (this->states[attribute].value.compare(value) != 0)
    {
        this->states[attribute] = State({time(nullptr), string(value)});
        for (auto &automation : this->automations)
        {
            automation->IO(attribute, value);
        }
    }
}

State DistributedDevice::GetAttribute(const string &attribute)
{
    if (this->states.find(attribute) == this->states.end())
    {
        return {};
    }
    return this->states[attribute];
}

// DistributedDevice *DistributedDevice::GetInstance() {
//     lock_guard<mutex> lock(mutex_singleton);
//     if (instance == nullptr)
//     {
//         instance = new DistributedDevice();
//     }
//     return instance;
// }

void DistributedDevice::SetAutomationsFromJsonString(string json_string)
{
    for (auto &automation : this->automations)
    {
        automation->Stop();
    }

    automations.clear();
    nlohmann::json j = nlohmann::json::parse(json_string);

    for (auto &automation : j)
    {
        this->AddAutomation(Json2Automation(automation));
    }
}