#include "ad5940_hardware.h"

#if defined(CONFIG_VIRUS_DETECTOR)

// Our circuit use GPIO0 as interrupt pin.
AGPIOCfg_Type AD5940_gpio_cfg = {
    .FuncSet = GP0_INT,
    .InputEnSet = 0,
    .OutputEnSet = AGPIO_Pin0,
    .OutVal = 0,
    .PullEnSet = AGPIO_Pin0,
};

#endif

#if defined(CONFIG_JOHNNY_OVAL)

// Our circuit use GPIO7 as interrupt pin.
AGPIOCfg_Type AD5940_gpio_cfg = {
    .FuncSet = GP0_GPIO | GP7_INT,
    .InputEnSet = 0,
    .OutputEnSet = AGPIO_Pin7,
    .OutVal = 0,
    .PullEnSet = AGPIO_Pin7,
};

#endif
