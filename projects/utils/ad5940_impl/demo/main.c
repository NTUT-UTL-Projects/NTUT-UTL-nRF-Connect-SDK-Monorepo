#include "ad5940_impl.h"

/** ==================== Main ==================== */
int main(int argc, char *argv[])
{
    printf("Hello world: %f", AD5940_ADCPGA_to_val_table[ADCPGA_1P5]);
    return 0;
}
