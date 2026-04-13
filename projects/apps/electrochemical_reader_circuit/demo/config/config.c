#include "config.h"

hardware_t hardware = {};

// ==================================================
// AD5940
// ==================================================

#define SEQGenBuffLen 1000
uint32_t SEQGenBuff[SEQGenBuffLen];

AD5940_CONTROLLER_TRIGGER_PARA ad5940_controller_trigger_para = {
};

fImpPol_Type HsRtiaCal;
LPDACPara_Type LpDacPara;
fImpPol_Type LpRtiaCal;

// ==================================================
// Threads
// ==================================================

void AD5940_STOP(void) {
	return;
}

// --------------------------------------------------
// AD5940 ADC FIFO task

#define AD5940_ADC_FIFO_BUFF_LEN 100
uint32_t ad5940_adc_fifo_buff[AD5940_ADC_FIFO_BUFF_LEN];
volatile atomic_uint_fast32_t ad5940_adc_target_len = 0;
volatile atomic_uint_fast32_t ad5940_adc_curr_len = 0;

// --------------------------------------------------
// Bluetooth send AD5940 ADC FIFO

volatile atomic_uint_fast16_t bluetooth_to_ad5940_adc_curr_len = 0;
