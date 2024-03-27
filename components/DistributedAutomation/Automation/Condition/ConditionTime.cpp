//
// Created by talgarr on 01/02/24.
//

#include "ConditionTime.hpp"

ConditionTime::ConditionTime(string alias, tm after, tm before, vector<string> weekday) : Condition(alias)
{
    this->after = after;
    this->before = before;
    this->weekday = move(weekday);
}

bool ConditionTime::Verify(string trigger_alias)
{
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    if (ltm->tm_hour < after.tm_hour or ltm->tm_hour > before.tm_hour)
    {
        return false;
    }
    if (ltm->tm_min < after.tm_min or ltm->tm_min > before.tm_min)
    {
        return false;
    }
    if (ltm->tm_sec < after.tm_sec or ltm->tm_sec > before.tm_sec)
    {
        return false;
    }
    if (this->weekday.empty())
    {
        return true;
    }

    string weekdays[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

    auto it = find(this->weekday.begin(), this->weekday.end(), weekdays[ltm->tm_wday]);
    return it != this->weekday.end();
}

ConditionTime::~ConditionTime() = default;

static ConditionTime *ConditionTime::Json2Condition(nlohmann::json json)
{
    string alias = json["alias"];
    tm after = json["after"];
    tm before = json["before"];
    vector<string> weekday = json["weekday"];
    return new ConditionTime(alias, after, before, weekday);
}