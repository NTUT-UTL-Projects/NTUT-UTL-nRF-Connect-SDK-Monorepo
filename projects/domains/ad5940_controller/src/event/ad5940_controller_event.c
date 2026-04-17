#include "ad5940_controller_event.h"

ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE AD5940_controller_event_to_adc_number(
    AD5940_CONTROLLER_EVENT event
)
{
    switch (event.event)
    {
    case AD5940_CONTROLLER_EVENT_CA:
        return event.param.ca.n_step;
    case AD5940_CONTROLLER_EVENT_CV:
        return ELECTROCHEMICAL_DEV_KIT_cv_get_n_step_total(&event.param.cv.cv);
    case AD5940_CONTROLLER_EVENT_DPV:
        return ELECTROCHEMICAL_DEV_KIT_dpv_get_n_step_total(&event.param.dpv);
    case AD5940_CONTROLLER_EVENT_TEMPERATURE:
        return event.param.temperature.n_step;
    default:
        return 0;
    }
}
