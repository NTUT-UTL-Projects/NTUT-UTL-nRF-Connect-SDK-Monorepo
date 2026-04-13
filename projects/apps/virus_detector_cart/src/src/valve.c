#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

#include "valve.h"
#include "uln2003a.h"
#include "stepper_motor.h"

#define USER_PATH DT_PATH(zephyr_user)
static const struct gpio_dt_spec gpio_devices[] = {
	GPIO_DT_SPEC_GET(USER_PATH, valve0_gpios),
	GPIO_DT_SPEC_GET(USER_PATH, valve1_gpios),
	GPIO_DT_SPEC_GET(USER_PATH, valve2_gpios),
	GPIO_DT_SPEC_GET(USER_PATH, valve3_gpios),
};
#define GPIO_DEVICES_LENGTH sizeof(gpio_devices) / sizeof(gpio_devices[0])

static StepperMotor _stepper_motor;

void gpio_write(uint8_t pin, bool value) {
    gpio_pin_set_dt(gpio_devices + pin, value);
}

void sleep(uint32_t time)
{
    k_sleep(K_USEC(time));
}

int valve_init(void)
{
    int err = 0;
    for(size_t i=0; i<GPIO_DEVICES_LENGTH; i++)
    {
        err = gpio_pin_configure_dt(&(gpio_devices[i]), GPIO_OUTPUT_ACTIVE);
		if (err) {
			// printk("Error: GPIO device %s is not ready\n", gpio_devices[i]);
			return err;
		}
    }
    StepperMotor_Init(
        &_stepper_motor,
        ULN2003A_COIL_COUNT,
        STEPPER_HALF_STEP,
        gpio_write,
        sleep,
        ULN2003A_STEPS_PER_REV
    );
    return err;
}

void valve_turn(const uint8_t direction)
{
    static float degree = 0.0f;
    float rotate_degree = 0.0f;
    switch (direction)
    {
    case VALVE_DIRECTION_120:
        rotate_degree = 120.0f - degree;
        degree = 120.0f;
        break;

    case VALVE_DIRECTION_240:
        rotate_degree = 240.0f - degree;
        degree = 240.0f;
        break;

    case VALVE_DIRECTION_360:
        rotate_degree = 360.0f - degree;
        degree = 360.0f;
        break;
    
    default:
        break;
    }
    if(rotate_degree < 0.0f) rotate_degree = 360.0f + rotate_degree;
    StepperMotor_RotateToAngle(&_stepper_motor, rotate_degree, 4E4);
}
