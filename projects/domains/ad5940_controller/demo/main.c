#include "ad5940_controller.h"

/** ==================== Main ==================== */
int main(int argc, char *argv[])
{
    AD5940_CONTROLLER_TRIGGER_PARA para = {0};
    for(size_t i=0; i<sizeof(para); i++)
    {
        printf("%02x", ((uint8_t *) &para)[i]);
    }
    return 0;
}
