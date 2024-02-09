idf_component_register(SRCS "IODriver.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_matter
                    REQUIRES device
                    REQUIRES LightDriver)
