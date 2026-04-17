#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_apps.h"

#include "ad5940_controller_electrochemical.h"
#include "ad5940_controller_temperature.h"

typedef enum
{
    AD5940_CONTROLLER_EVENT_CA,
    AD5940_CONTROLLER_EVENT_CV,
    AD5940_CONTROLLER_EVENT_DPV,
    AD5940_CONTROLLER_EVENT_TEMPERATURE,
} AD5940_CONTROLLER_EVENT_ENUM;

typedef struct
{
    AD5940_CONTROLLER_EVENT_ENUM event;
    union {
        AD5940_CONTROLLER_ELECTROCHEMICAL_CA_PARAM ca;
        AD5940_CONTROLLER_ELECTROCHEMICAL_CV_PARAM cv;
        AD5940_CONTROLLER_ELECTROCHEMICAL_DPV_PARAM dpv;
        AD5940_CONTROLLER_TEMPERATURE_PARAM temperature;
    } param;
} AD5940_CONTROLLER_EVENT;

ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE AD5940_controller_event_to_adc_number(
    AD5940_CONTROLLER_EVENT event
);

#ifdef __cplusplus
}
#endif
