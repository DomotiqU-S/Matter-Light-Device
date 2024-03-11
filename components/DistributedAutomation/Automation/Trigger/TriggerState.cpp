//
// Created by talgarr on 01/02/24.
//

#include "TriggerState.hpp"

TriggerState::TriggerState(string alias, string attribute, time_t for_s) : Trigger(std::move(alias))
{
    this->attribute = std::move(attribute);
    this->for_s = for_s;
    if (this->for_s > 0)
    {
        this->trigger_time = new TriggerTime(this->alias + "_TriggerTime", "0 0 0 1 1 ?", true);
    }
    else
    {
        this->trigger_time = nullptr;
    }
}

TriggerState::~TriggerState()
{
    if (this->trigger_time != nullptr)
    {
        this->trigger_time->Stop();
    }
    delete this->trigger_time;
}

void TriggerState::Run(condition_variable *cv_mother, mutex *cv_m_mother)
{
    this->cv_mother = cv_mother;
    this->cv_m_mother = cv_m_mother;
}

void TriggerState::Stop()
{
    if (this->trigger_time != nullptr)
    {
        this->trigger_time->Stop();
    }
}

void TriggerState::IOSup()
{
    if (this->for_s > 0)
    {
        if (this->trigger_time->IsRunning())
        {
            return;
        }
        time_t now = time(nullptr);
        time_t diff = now + this->for_s;
        tm *tm = new struct tm();
        cron::utils::time_to_tm(&diff, tm);
        string next = to_string(tm->tm_sec) + " " + to_string(tm->tm_min) + " " + to_string(tm->tm_hour) + " " +
                      to_string(tm->tm_mday) + " " + to_string(tm->tm_mon + 1) + " *";
        delete tm;
        this->trigger_time->SetPattern(next);
        std::thread(&TriggerTime::Run, this->trigger_time, this->cv_mother, this->cv_m_mother).detach();
    }
    else
    {
        this->has_triggered = true;
        this->cv_mother->notify_one();
    }
}
