//
// Created by talgarr on 01/02/24.
//

#include "TriggerTime.hpp"

TriggerTime::TriggerTime(string alias, const string &pattern, bool one_time) : Trigger(std::move(alias))
{
    this->running = false;
    this->one_time = one_time;
    this->SetPattern(pattern);
}

void TriggerTime::Run(condition_variable *cv_mother, mutex *cv_m_mother)
{
    this->running = true;
    this->cv_mother = cv_mother;
    this->cv_m_mother = cv_m_mother;

    while (this->running)
    {
        std::time_t now = std::time(nullptr);
        std::time_t next = cron::cron_next(this->pattern, now);
        std::unique_lock<std::mutex> lock(this->cv_m);
        this->cv.wait_for(lock, chrono::seconds(next - now), [&]
                          { return not this->running; });
        if (!this->running)
            break;
        this->has_triggered = true;
        this->cv_mother->notify_all();
        if (this->one_time)
        {
            this->running = false;
        }
    }
}

void TriggerTime::Stop()
{
    this->running = false;
    this->cv.notify_one();
}

void TriggerTime::SetPattern(const string &pattern_)
{
    // try {
    this->pattern = cron::make_cron(pattern_);
    // }
    // catch (cron::bad_cronexpr const &ex) {
    //     std::cerr << ex.what() << '\n';
    // }
}

bool TriggerTime::IsRunning() const
{
    return this->running;
}

TriggerTime::~TriggerTime() = default;

string TriggerTime::Print()
{
    return "TriggerTime: " + this->alias + " " + cron::to_string(this->pattern);
}