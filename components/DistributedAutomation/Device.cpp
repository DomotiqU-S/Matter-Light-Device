//
// Created by talgarr on 01/02/24.
//

#include "Device.hpp"

DistributedAutomation::Device::Device()
{
    this->running = true;
    this->automations = vector<Automation *>();
}

DistributedAutomation::Device::~Device()
{
    this->running = false;
    for (auto &automation : this->automations)
    {
        automation->Stop();
        delete automation;
    }
    this->automations.clear();
}

bool DistributedAutomation::Device::AddAutomation(Automation *automation)
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

void DistributedAutomation::Device::RemoveAutomation(string alias)
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

void DistributedAutomation::Device::Run()
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

vector<Automation *> DistributedAutomation::Device::GetAutomations()
{
    return this->automations;
}

void DistributedAutomation::Device::CreateAutomationsThreads()
{
    for (auto automation : this->automations)
    {
        thread t(&Automation::Run, automation, &this->cv, &this->cv_m);
        t.detach();
    }
}

void DistributedAutomation::Device::Stop()
{
    unique_lock<mutex> lock(this->cv_m);
    this->running = false;
    for (auto &t : this->automations)
    {
        t->Stop();
    }
    this->cv.notify_all();
}

void DistributedAutomation::Device::TriggerIO(const string &attribute, const string &value)
{
    if (StateCollection::Instance().GetAttribute(attribute).value.compare(value) != 0)
    {
        StateCollection::Instance().SetAttribute(attribute, State({time(nullptr), string(value)}));
        for (auto &automation : this->automations)
        {
            automation->IO(attribute, value);
        }
    }
}

void DistributedAutomation::Device::SetAutomationsFromJson(nlohmann::json json)
{
    ESP_LOGI(DistributedAutomation::TAG, "SetAutomationsFromJson");
    // for (auto &automation : this->automations)
    // {
    //     automation->Stop();
    // }

    automations.clear();
    ESP_LOGI(DistributedAutomation::TAG, "SetAutomationsFromJson: %s", json.dump().c_str());
    auto automation = JsonUtils::Json2Automation(json);
    cout << automation->Print() << endl;
    this->AddAutomation(automation);
}