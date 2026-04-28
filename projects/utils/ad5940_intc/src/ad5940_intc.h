#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    int AD5940_intc0_lock_boardcast(void);

    int AD5940_intc0_lock_wait(void);

    int AD5940_intc1_lock_boardcast(void);

    int AD5940_intc1_lock_wait(void);

#ifdef __cplusplus
}
#endif
