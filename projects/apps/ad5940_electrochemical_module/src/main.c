/*
 * Copyright (c) 2024 Croxel, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdatomic.h>

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/services/nus.h>

// ========================================
// Utils
#include "gpio_debounce_impl_zephyr.h"
#include "spi_impl_zephyr.h"
#include "kernel_sleep_impl_zephyr.h"
#include "spi_impl_zephyr.h"

// ========================================
// Domains
#include "ad5940_controller.h"
#include "ad5940_impl.h"

// ========================================
// Apps
#include "ad5940_hardware.h"
#include "ad5940_pins_config_impl_zephyr.h"

#include "commands_start_lock_impl_zephyr.h"
#include "commands_stop_lock_impl_zephyr.h"

#include <zephyr/logging/log.h>
#define LOG_MODULE_NAME main
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

// ========================================
// AD5940
#define SEQGenBuffLen 1000
static uint32_t SEQGenBuff[SEQGenBuffLen];

// ========================================
// Commands
#define CommandsBuffLen 3000
static uint8_t commands_buff[CommandsBuffLen];
volatile static atomic_uint_fast16_t commands_buff_final_len = 0;

typedef enum {
	COMMANDS_TYPE_REBOOT,
	COMMANDS_TYPE_STOP_BUFF,
	COMMANDS_TYPE_RESET_BUFF,
	COMMANDS_TYPE_WRITE_BUFF,
	COMMANDS_TYPE_START_BUFF,
} COMMANDS_TYPE;

typedef enum {
    WRITE_event_param,
    WRITE_aferef_cfg,
    WRITE_lploop_cfg,
    WRITE_hsloop_cfg,
    WRITE_dsp_cfg,
    WRITE_AfeCtrlSet_DataType_FifoSrc,
    WRITE_wait,
    WRITE_goto,
    WRITE_trigger,
} COMMANDS_WRITE_BUFF_TYPE;

volatile static atomic_bool commands_stop_flag = false;

fImpPol_Type HsRtiaCal;
LPDACPara_Type LpDacPara;
fImpPol_Type LpRtiaCal;

volatile static atomic_uint_fast32_t electrochemical_runner_id = 0;

AD5940_CONTROLLER_TRIGGER_PARA ad5940_controller_trigger_para = {0};

volatile static atomic_uint_fast16_t ad5940_adc_curr_len = 0;
volatile static atomic_uint_fast16_t ad5940_adc_max_len = 0;

typedef enum
{
	AD5940_ADC_TYPE_NULL,
	AD5940_ADC_TYPE_LPTIA,
	AD5940_ADC_TYPE_HSTIA,
	AD5940_ADC_TYPE_TEMPERATURE,
} AD5940_ADC_TYPE;

static volatile _Atomic AD5940_ADC_TYPE ad5940_adc_type = AD5940_ADC_TYPE_NULL;

void commands_runner(void)
{
	while(true)
	{
		commands_start_lock_wait();

		// Reset the id.
		atomic_store(&electrochemical_runner_id, 0);

		// Get commands length.
		uint16_t len = atomic_load(&commands_buff_final_len);










		// ========================================
		// ========================================
        // TODO
		// NOTE:
		// This block only for the testing.
		// For mocking commands.
		len = 1;
		// ========================================
		// ========================================










		for(size_t i=0; i<len;)
		{
			// Check whether the commands is stopping
			if(atomic_load(&commands_stop_flag)) {
				// Restore flag.
				atomic_store(&commands_stop_flag, false);
				break;
			}

			// Get command header.
			COMMANDS_WRITE_BUFF_TYPE write_type = ((COMMANDS_WRITE_BUFF_TYPE) (commands_buff + i)[0]);

			// Get command pointer.
			uint8_t *commands_buff_ptr = commands_buff + i + 1;












			// ========================================
			// ========================================
        	// TODO
			// NOTE:
			// This block only for the testing.
			write_type = WRITE_trigger;
			// ========================================
			// ========================================









			switch (write_type)
			{
			case WRITE_event_param:
				memcpy(&ad5940_controller_trigger_para.event, commands_buff_ptr, sizeof(uint8_t));
				memcpy(&ad5940_controller_trigger_para.param, commands_buff_ptr + sizeof(uint8_t), sizeof(AD5940_ELEC_PARAM));
				i += (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(AD5940_ELEC_PARAM));
				break;
			case WRITE_aferef_cfg:
				memcpy(&ad5940_controller_trigger_para.aferef_cfg, commands_buff_ptr, sizeof(AFERefCfg_Type));
				i += (sizeof(uint8_t) + sizeof(AFERefCfg_Type));
				break;
			case WRITE_lploop_cfg:
				memcpy(&ad5940_controller_trigger_para.lploop_cfg, commands_buff_ptr, sizeof(LPLoopCfg_Type));
				i += (sizeof(uint8_t) + sizeof(LPLoopCfg_Type));
				break;
			case WRITE_hsloop_cfg:
				memcpy(&ad5940_controller_trigger_para.hsloop_cfg, commands_buff_ptr, sizeof(HSLoopCfg_Type));
				i += (sizeof(uint8_t) + sizeof(HSLoopCfg_Type));
				break;
			case WRITE_dsp_cfg:
				memcpy(&ad5940_controller_trigger_para.dsp_cfg, commands_buff_ptr, sizeof(DSPCfg_Type));
				i += (sizeof(uint8_t) + sizeof(DSPCfg_Type));
				break;
			case WRITE_AfeCtrlSet_DataType_FifoSrc:
				memcpy(&ad5940_controller_trigger_para.AfeCtrlSet, commands_buff_ptr, sizeof(uint32_t));
				memcpy(&ad5940_controller_trigger_para.DataType, commands_buff_ptr + sizeof(uint32_t), sizeof(uint32_t));
				memcpy(&ad5940_controller_trigger_para.FifoSrc, commands_buff_ptr + sizeof(uint32_t) + sizeof(uint32_t), sizeof(uint32_t));
				i += (sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t));
				break;
			case WRITE_wait:
				uint64_t time;
				memcpy(&time, commands_buff_ptr, sizeof(uint64_t));
				KERNEL_SLEEP_ns(time);
				i += (sizeof(uint8_t) + sizeof(uint64_t));
				break;
			case WRITE_goto:
				uint16_t index;
				memcpy(&index, commands_buff_ptr, sizeof(uint16_t));
				i = index;
				break;
			case WRITE_trigger:
				AD5940Err err;
				
				// Reset ADC sender info.
				atomic_store(&ad5940_adc_type, AD5940_ADC_TYPE_NULL);












				// ========================================
				// ========================================
        		// TODO
				// NOTE:
				// This block only for the testing.
				#include "ad5940_const.h"
				#include "ad5940_controller_const.h"
				ad5940_controller_trigger_para.AfeCtrlSet = LPDAC2LPTIA_AfeCtrlSet;
				
				LPDAC2LPTIA_AFERefCfg(&(ad5940_controller_trigger_para.aferef_cfg));

				LPDAC2LPTIA_LPDACCfg_Type(&(ad5940_controller_trigger_para.lploop_cfg.LpDacCfg));
				ad5940_controller_trigger_para.lploop_cfg.LpDacCfg.DacData12Bit = 0x0000;
				ad5940_controller_trigger_para.lploop_cfg.LpDacCfg.DacData6Bit = 0x0000;
				ad5940_controller_trigger_para.lploop_cfg.LpDacCfg.LpDacRef = LPDACREF_2P5;
				ad5940_controller_trigger_para.lploop_cfg.LpDacCfg.LpDacSrc = LPDACSRC_MMR;

				ad5940_controller_const_LPLoopCfg_Type_methylene_blue(
					&(ad5940_controller_trigger_para.lploop_cfg)
				);
				LPDAC2LPTIA_LPAmpCfg_Type(
					&(ad5940_controller_trigger_para.lploop_cfg.LpAmpCfg), 
					ad5940_controller_trigger_para.lploop_cfg.LpAmpCfg.LpTiaRtia
				);

				ad5940_controller_trigger_para.hsloop_cfg.HsDacCfg = (HSDACCfg_Type) {0};

				ad5940_controller_const_DSPCfg_Type_methylene_blue(
					&(ad5940_controller_trigger_para.dsp_cfg)
				);
				ad5940_controller_trigger_para.dsp_cfg.ADCBaseCfg.ADCMuxN = ADCMUXN_LPTIA0_N;
				ad5940_controller_trigger_para.dsp_cfg.ADCBaseCfg.ADCMuxP = ADCMUXP_LPTIA0_P;

				ad5940_controller_trigger_para.DataType = DataType_methylene_blue;
				ad5940_controller_trigger_para.event = AD5940_CONTROLLER_EVENT_DPV;
				ad5940_controller_trigger_para.FifoSrc = FifoSrc_methylene_blue;
				ad5940_controller_trigger_para.param.dpv = methylene_blue_electrochemical_para;
				// ========================================
				// ========================================








				// ========================================
				// Calibration
				AD5940_CONTROLLER_CAL_RESULTS results;
				err = AD5940_controller_cal(
					(AD5940_CONTROLLER_CAL_PARA) {
						.ADCRefVolt = AD5940_ADCRefVolt,
						.bWithCtia = AD5940_bWithCtia,
						.event = ad5940_controller_trigger_para.event,
						.fRcal = AD5940_fRcal,
						.param = ad5940_controller_trigger_para.param,
						.VRef1p11 = AD5940_VRef1p11,
						.VRef1p82 = AD5940_VRef1p82,
					},
					(AD5940_CONTROLLER_CAL_WRITE) {
						.aferef_cfg = &ad5940_controller_trigger_para.aferef_cfg,
						.dsp_cfg = &ad5940_controller_trigger_para.dsp_cfg,
						.hsloop_cfg = &ad5940_controller_trigger_para.hsloop_cfg,
						.lploop_cfg = &ad5940_controller_trigger_para.lploop_cfg,
					},
					&results
				);

				if(err != AD5940ERR_OK) break;
				ad5940_controller_trigger_para.dsp_cfg.ADCFilterCfg.ADCRate = results.ADCRate;
				HsRtiaCal = results.HsRtiaCal;
				ad5940_controller_trigger_para.LFOSCClkFreq = results.LFOSCClkFreq;
				LpDacPara = results.LpDacPara;
				LpRtiaCal = results.LpRtiaCal;
				ad5940_controller_trigger_para.RatioSys2AdcClk = results.RatioSys2AdcClk;

				// ========================================
				// Set ADC length
				{
					uint16_t len;
					atomic_store(&ad5940_adc_curr_len, 0);
					switch (ad5940_controller_trigger_para.event)
					{
					case AD5940_CONTROLLER_EVENT_CA:
						// TODO
						atomic_store(&ad5940_adc_max_len, len);
						break;
					case AD5940_CONTROLLER_EVENT_CV:
						// TODO
						atomic_store(&ad5940_adc_max_len, len);
						break;
					case AD5940_CONTROLLER_EVENT_DPV:
						AD5940_DPV_get_FIFOCnt(
							ad5940_controller_trigger_para.param.dpv,
							&len
						);
						atomic_store(&ad5940_adc_max_len, len);
						break;
					case AD5940_CONTROLLER_EVENT_EIS:
						// TODO
						atomic_store(&ad5940_adc_max_len, len);
						break;
					default:
						break;
					}
				}









				// ========================================
				// ========================================
        		// TODO
				// NOTE:
				// This block only for the testing.
				atomic_store(&ad5940_adc_type, AD5940_ADC_TYPE_LPTIA);
				// ========================================
				// ========================================













				// ========================================
				// Trigger
				AD5940_controller_trigger(
					ad5940_controller_trigger_para
				);
				i += (sizeof(uint8_t));

				break;
			default:
				break;
			}
		}
	}
}

void commands_stopper(void)
{
	while(true)
	{
		commands_stop_lock_wait();
		atomic_store(&commands_stop_flag, true);
		/* Stop Wupt right now */
		AD5940_WUPTCtrl(bFALSE);
		/* For detailed reference on the chip's shutdown procedure, see page 129 of the datasheet.*/
		AD5940_ShutDownS();
	}
}

#define ADC_MAX_QUEUE 1000
K_MSGQ_DEFINE(_adc_quene, sizeof(uint32_t), ADC_MAX_QUEUE, 4);

#define AD5940AdcSendBuffLen 255
static uint8_t ad5940_adc_sender_buff[AD5940AdcSendBuffLen];
void ad5940_adc_sender(void)
{
	int err;
	while(true)
	{
		uint32_t ADCcode;
    	k_msgq_get(&_adc_quene, &ADCcode, K_FOREVER);

		AD5940_ADC_TYPE type = atomic_load(&ad5940_adc_type);
		if(type == AD5940_ADC_TYPE_NULL) continue;

		uint8_t *ptr = ad5940_adc_sender_buff;

		// Header
		ptr[0] = 0x00;
		ptr += 1;

		// ID
		uint32_t id = atomic_load(&electrochemical_runner_id);
		memcpy(ptr, &id, sizeof(id));
		ptr += sizeof(id);

		// Type
		ptr[0] = type;
		ptr += 1;

		// Index
		uint16_t index = atomic_load(&ad5940_adc_curr_len);
		memcpy(ptr, &index, sizeof(index));
		ptr += sizeof(index);

		// Index
		uint16_t max = atomic_load(&ad5940_adc_max_len);
		memcpy(ptr, &max, sizeof(max));
		ptr += sizeof(max);

		// Result
		float current;
		AD5940_ADCCode2Current(
			(AD5940_ADCCode2Current_Para) {
				.ADCcode = ADCcode,
				.ADCPga = ad5940_controller_trigger_para.dsp_cfg.ADCBaseCfg.ADCPga,
				.RtiaCalValue = LpRtiaCal,
				.VRef1p82 = AD5940_VRef1p82,
			},
			&current
		);
		memcpy(ptr, &current, sizeof(current));
		ptr += sizeof(current);

		uint8_t len = ptr - ad5940_adc_sender_buff;

		err = bt_nus_send(NULL, ad5940_adc_sender_buff, len);
	}
}

#define AD5940ReadFifoBuffLen 100
static uint32_t ad5940_read_fifo_buff[AD5940ReadFifoBuffLen];
void ad5940_read_fifo_runner(void)
{
	while(true)
	{
		#if defined(CONFIG_VIRUS_DETECTOR)
		ad5940_intc0_lock_wait();
		#endif

		#if defined(CONFIG_JOHNNY_OVAL)
		ad5940_intc1_lock_wait();
		#endif

		uint16_t buffCount;
		AD5940_DATAFIFOISR(
			(AD5940_DATAFIFOISR_PARA) {
				.BufferSize = AD5940ReadFifoBuffLen,
				.Enable = bTRUE,
				.FIFOThresh = AD5940_DATAFIFOTHRESH_UNCHANGE,
			},
			(AD5940_DATAFIFOISR_RET) {
				.pBuff = ad5940_read_fifo_buff,
				.pBuffCount = &buffCount,
			}
		);
		

		// ========================================
		// ========================================
        // TODO
		// NOTE:
		// This block only for the testing.
		if(buffCount > 1) buffCount = 1;
		// ========================================
		// ========================================

		atomic_fetch_add(&ad5940_adc_curr_len, buffCount);
		if(atomic_load(&ad5940_adc_curr_len) >= atomic_load(&ad5940_adc_max_len)) {
			// Refer to page 107 of the datasheet
			// Enable AFE to enter sleep mode.
			AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Unlock so sequencer can put AD5940 to sleep */
			/* Stop Wupt right now */
			AD5940_WUPTCtrl(bFALSE);
			/* For detailed reference on the chip's shutdown procedure, see page 129 of the datasheet.*/
			AD5940_ShutDownS();
			atomic_fetch_add(&electrochemical_runner_id, 1);
		}

		for(size_t i=0; i<buffCount; i++)
		{
			k_msgq_put(&_adc_quene, ad5940_read_fifo_buff + i, K_NO_WAIT);
		}
	}
}

// ========================================
// Bluetooth
#define DEVICE_NAME		CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN		(sizeof(DEVICE_NAME) - 1)

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_NUS_SRV_VAL),
};

static void notif_enabled(bool enabled, void *ctx)
{
	ARG_UNUSED(ctx);

	LOG_INF("%s() - %s\n", __func__, (enabled ? "Enabled" : "Disabled"));
}

static void received(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
	ARG_UNUSED(conn);
	ARG_UNUSED(ctx);

	LOG_INF("%s() - Len: %d, Message: %.*s\n", __func__, len, len, (char *)data);

	if(len < 1) return;
	COMMANDS_TYPE type = ((COMMANDS_TYPE) ((uint8_t *) data)[0]);
	switch (type)
	{
	case COMMANDS_TYPE_REBOOT:
		// TODO
		// Implements the reboot feature.
		break;
	case COMMANDS_TYPE_STOP_BUFF:
		commands_stop_lock_boardcast();
		break;
	case COMMANDS_TYPE_RESET_BUFF:
		atomic_store(&commands_buff_final_len, 0);
		break;
	case COMMANDS_TYPE_WRITE_BUFF:
		if(len < 2) return;
		COMMANDS_WRITE_BUFF_TYPE write_type = ((COMMANDS_WRITE_BUFF_TYPE) ((uint8_t *) data)[1]);
		uint16_t new_len = len - 1;
		uint8_t *new_data = ((uint8_t *) data) + 1;
		uint8_t *commands_buff_ptr = commands_buff + atomic_load(&commands_buff_final_len);
		switch (write_type)
		{
		case WRITE_event_param:
			if(new_len != (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(AD5940_ELEC_PARAM))) return;
			break;
		case WRITE_aferef_cfg:
			if(new_len != (sizeof(uint8_t) + sizeof(AFERefCfg_Type))) return;
			break;
		case WRITE_lploop_cfg:
			if(new_len != (sizeof(uint8_t) + sizeof(LPLoopCfg_Type))) return;
			break;
		case WRITE_hsloop_cfg:
			if(new_len != (sizeof(uint8_t) + sizeof(HSLoopCfg_Type))) return;
			break;
		case WRITE_dsp_cfg:
			if(new_len != (sizeof(uint8_t) + sizeof(DSPCfg_Type))) return;
			break;
		case WRITE_AfeCtrlSet_DataType_FifoSrc:
			if(new_len != (sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t))) return;
			break;
		case WRITE_wait:
			if(new_len != (sizeof(uint8_t) + sizeof(uint64_t))) return;
			break;
		case WRITE_goto:
			if(new_len != (sizeof(uint8_t) + sizeof(uint16_t))) return;
			break;
		case WRITE_trigger:
			if(new_len != (sizeof(uint8_t))) return;
			break;
		default:
			break;
		}
		atomic_fetch_add(&commands_buff_final_len, new_len);
		memcpy(commands_buff_ptr, new_data, new_len);
		break;
	case COMMANDS_TYPE_START_BUFF:
		commands_start_lock_boardcast();
		break;
	default:
		return;
	}
}

struct bt_nus_cb nus_listener = {
	.notif_enabled = notif_enabled,
	.received = received,
};

static void mtu_exchange_cb(
	struct bt_conn *conn, 
	uint8_t err,
	struct bt_gatt_exchange_params *params
)
{
    LOG_INF("MTU exchange %u %s (%u)", 
		bt_conn_index(conn),
		err == 0U ? "successful" : "failed", 
		bt_gatt_get_mtu(conn)
	);
	return;
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	static struct bt_gatt_exchange_params exchange_params;
	exchange_params.func = mtu_exchange_cb;

	err = bt_gatt_exchange_mtu(conn, &exchange_params);
	if (err) { LOG_ERR("MTU exchange failed: %d", err); }

	err = bt_conn_le_data_len_update(conn, BT_LE_DATA_LEN_PARAM_MAX);
	if (err) { LOG_ERR("Data length update failed: %d", err); }

	err = bt_conn_le_phy_update(conn, BT_CONN_LE_PHY_PARAM_2M);
	if (err) { LOG_ERR("Phy update request failed: %d",  err); }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_INF("Disconnected: %s (reason %u)", addr, reason);

	return;
}

static void recycled(void)
{
    LOG_INF("Connection object recycled — can start advertising\n");
	int err;
	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) { LOG_ERR("Failed to start advertising: %d\n", err); }
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
	.recycled = recycled,
};

// ========================================
// thread

static k_tid_t commands_runner_tid;
static struct k_thread commands_runner_thread;
K_THREAD_STACK_DEFINE(commands_runner_stack, 16384);
#define commands_runner_prio 3

static k_tid_t commands_stopper_tid;
static struct k_thread commands_stopper_thread;
K_THREAD_STACK_DEFINE(commands_stopper_stack, 16384);
#define commands_stopper_prio 2

static k_tid_t ad5940_adc_sender_tid;
static struct k_thread ad5940_adc_sender_thread;
K_THREAD_STACK_DEFINE(ad5940_adc_sender_stack, 16384);
#define ad5940_adc_sender_prio 1

static k_tid_t ad5940_read_fifo_runner_tid;
static struct k_thread ad5940_read_fifo_runner_thread;
K_THREAD_STACK_DEFINE(ad5940_read_fifo_runner_stack, 16384);
#define ad5940_read_fifo_runner_prio 0

// ========================================
// main
int main(void)
{
	int err;

	// ========================================
	// Lock
	err = ad5940_intc0_lock_init_impl_zephyr();
	if (err) return err;
	err = ad5940_intc1_lock_init_impl_zephyr();
	if (err) return err;
	err = commands_start_lock_init_impl_zephyr();
	if (err) return err;
	err = commands_stop_lock_init_impl_zephyr();
	if (err) return err;

	// ========================================
	// GPIO
	#if defined(CONFIG_VIRUS_DETECTOR)
	err = ad5940_intc0_lock_init_impl_zephyr();
	if (err) return err;

	err = AD5940_intc0_impl_zephyr_init(
		&ad5940_gpio0_ctx,
		&ad5940_gpio0_dt,
		ad5940_intc0_lock_boardcast
	);
	if (err) return err;
	#endif

	#if defined(CONFIG_JOHNNY_OVAL)
	err = ad5940_intc1_lock_init_impl_zephyr();
	if (err) return err;
	err = AD5940_intc1_impl_zephyr_init(
		&ad5940_gpio7_ctx,
		&ad5940_gpio7_dt,
		ad5940_intc1_lock_boardcast
	);
	if (err) return err;
	#endif

	#if defined(CONFIG_VIRUS_DETECTOR)
	err = AD5940_Rst_impl_zephyr_init(&ad5940_reset_dt);
	if (err) return err;
	#endif

	// ========================================
	// SPI
	err = AD5940_spi_impl_zephyr_init(
		&spi_1_cfg,
		&spi_1_done_sig,
		spi_1_device,
		&spi_1_cs
	);
	if (err) return err;

	// ========================================
	// AD5940

	err = AD5940_controller_reset(
		SEQGenBuff,
		SEQGenBuffLen,
		#if defined(CONFIG_VIRUS_DETECTOR)
		AD5940_CONTROLLER_RESET_OPTION_HAREWARE
		#endif
		#if defined(CONFIG_JOHNNY_OVAL)
		AD5940_CONTROLLER_RESET_OPTION_SOFEWARE
		#endif
	);
	if (err) return err;

	// Set gpio
	ad5940_controller_trigger_para.gpio_cfg = AD5940_gpio_cfg;

	// ========================================
	// Bluetooth Peripheral NUS
	LOG_INF("Bluetooth Peripheral NUS\n");

	err = bt_nus_cb_register(&nus_listener, NULL);
	if (err) {
		LOG_ERR("Failed to register NUS callback: %d\n", err);
		return err;
	}

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Failed to enable bluetooth: %d\n", err);
		return err;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Failed to start advertising: %d\n", err);
		return err;
	}

	LOG_INF("Initialization complete\n");

	// ========================================
	// Threads

	commands_runner_tid = k_thread_create(
		&commands_runner_thread,
		commands_runner_stack,
		K_THREAD_STACK_SIZEOF(commands_runner_stack),
		commands_runner,
		NULL, NULL, NULL,
		commands_runner_prio, 0,
		K_NO_WAIT
	);

	commands_stopper_tid = k_thread_create(
		&commands_stopper_thread,
		commands_stopper_stack,
		K_THREAD_STACK_SIZEOF(commands_stopper_stack),
		commands_stopper,
		NULL, NULL, NULL,
		commands_stopper_prio, 0,
		K_NO_WAIT
	);

	ad5940_adc_sender_tid = k_thread_create(
		&ad5940_adc_sender_thread,
		ad5940_adc_sender_stack,
		K_THREAD_STACK_SIZEOF(ad5940_adc_sender_stack),
		ad5940_adc_sender,
		NULL, NULL, NULL,
		ad5940_adc_sender_prio, 0,
		K_NO_WAIT
	);

	ad5940_read_fifo_runner_tid = k_thread_create(
		&ad5940_read_fifo_runner_thread,
		ad5940_read_fifo_runner_stack,
		K_THREAD_STACK_SIZEOF(ad5940_read_fifo_runner_stack),
		ad5940_read_fifo_runner,
		NULL, NULL, NULL,
		ad5940_read_fifo_runner_prio, 0,
		K_NO_WAIT
	);

	return 0;
}
