
idf_component_register(SRCS "DistributedDevice.cpp" "DistributedServer.cpp" "DistributedMatterAPI.cpp"
                            "Automation/Automation.cpp"  
                            "Automation/Trigger/Trigger.cpp" "Automation/Trigger/TriggerNumericState.cpp" "Automation/Trigger/TriggerState.cpp"  "Automation/Trigger/TriggerStringState.cpp" "Automation/Trigger/TriggerTime.cpp"
                            "Automation/Condition/Condition.cpp" "Automation/Condition/ConditionLogical.cpp" "Automation/Condition/ConditionNumericState.cpp" "Automation/Condition/ConditionState.cpp" "Automation/Condition/ConditionStringState.cpp" "Automation/Condition/ConditionTime.cpp" "Automation/Condition/ConditionTrigger.cpp"
                            "Automation/Action/Action.cpp" "Automation/Action/ActionCallService.cpp" "Automation/Action/ActionDelay.cpp"
                    INCLUDE_DIRS "Automation/include" "Automation/Trigger/include"  "Automation/Condition/include"  "Automation/Action/include" "include" 
                    REQUIRES "Library")