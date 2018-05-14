/*
 * usb_pd_driver.c
 *
 * Created: 11/11/2017 23:55:12
 *  Author: jason
 */ 

#include "usb_pd_driver.h"
#include "usb_pd.h"
#include "ugui.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(t) (sizeof(t) / sizeof(t[0]))
#endif

extern struct tc_module tc_instance;
extern uint32_t g_us_timestamp_upper_32bit;
extern uint8_t display_buffer[DISP_MEM_SIZE];

uint32_t pd_task_set_event(uint32_t event, int wait_for_reply)
{
	switch (event)
	{
		case PD_EVENT_TX:
			break;
		default:
			break;
	}
	return 0;
}

const uint32_t pd_src_pdo[] = {
	PDO_FIXED( 5000, 250, PDO_FIXED_FLAGS),
	PDO_FIXED(20000, 250, PDO_FIXED_FLAGS),
};
const int pd_src_pdo_cnt = ARRAY_SIZE(pd_src_pdo);

const uint32_t pd_snk_pdo[] = {
	PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
	PDO_FIXED(9000, 500, PDO_FIXED_FLAGS),
	PDO_FIXED(20000, 500, PDO_FIXED_FLAGS),
};
const int pd_snk_pdo_cnt = ARRAY_SIZE(pd_snk_pdo);

void pd_set_input_current_limit(int port, uint32_t max_ma,
	uint32_t supply_voltage)
{

}

int pd_is_valid_input_voltage(int mv)
{
	return 1;
}

int pd_snk_is_vbus_provided(int port)
{
	return 1;
}

timestamp_t get_time(void)
{
	timestamp_t t;
	
	system_interrupt_enter_critical_section();
	t.le.lo = tc_get_count_value(&tc_instance);
	t.le.hi = g_us_timestamp_upper_32bit;
	system_interrupt_leave_critical_section();
	
	return t;
}

void pd_power_supply_reset(int port)
{
	char str[256];
	
	port_pin_set_output_level(VBUS_5V_EN_PIN, false);
	port_pin_set_output_level(VBUS_15V_EN_PIN, false);
	
	memset(display_buffer, 0x00, DISP_MEM_SIZE);
	sprintf(str, "Sink Not Connected");
	UG_PutString(0, 8, str);
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);
	return;
}

int pd_custom_vdm(int port, int cnt, uint32_t *payload,
		  uint32_t **rpayload)
{
#if 0
	int cmd = PD_VDO_CMD(payload[0]);
	uint16_t dev_id = 0;
	int is_rw, is_latest;

	/* make sure we have some payload */
	if (cnt == 0)
		return 0;

	switch (cmd) {
	case VDO_CMD_VERSION:
		/* guarantee last byte of payload is null character */
		*(payload + cnt - 1) = 0;
		//CPRINTF("version: %s\n", (char *)(payload+1));
		break;
	case VDO_CMD_READ_INFO:
	case VDO_CMD_SEND_INFO:
		/* copy hash */
		if (cnt == 7) {
			dev_id = VDO_INFO_HW_DEV_ID(payload[6]);
			is_rw = VDO_INFO_IS_RW(payload[6]);

			is_latest = pd_dev_store_rw_hash(port,
							 dev_id,
							 payload + 1,
							 is_rw ?
							 SYSTEM_IMAGE_RW :
							 SYSTEM_IMAGE_RO);

			/*
			 * Send update host event unless our RW hash is
			 * already known to be the latest update RW.
			 */
			if (!is_rw || !is_latest)
				pd_send_host_event(PD_EVENT_UPDATE_DEVICE);

			//CPRINTF("DevId:%d.%d SW:%d RW:%d\n",
			//	HW_DEV_ID_MAJ(dev_id),
			//	HW_DEV_ID_MIN(dev_id),
			//	VDO_INFO_SW_DBG_VER(payload[6]),
			//	is_rw);
		} else if (cnt == 6) {
			/* really old devices don't have last byte */
			pd_dev_store_rw_hash(port, dev_id, payload + 1,
					     SYSTEM_IMAGE_UNKNOWN);
		}
		break;
	case VDO_CMD_CURRENT:
		CPRINTF("Current: %dmA\n", payload[1]);
		break;
	case VDO_CMD_FLIP:
		/* TODO: usb_mux_flip(port); */
		break;
#ifdef CONFIG_USB_PD_LOGGING
	case VDO_CMD_GET_LOG:
		pd_log_recv_vdm(port, cnt, payload);
		break;
#endif /* CONFIG_USB_PD_LOGGING */
	}
#endif // if 0

	return 0;
}

void pd_execute_data_swap(int port, int data_role)
{
	/* Do nothing */
}

int pd_check_data_swap(int port, int data_role)
{
	// Never allow data swap
	return 0;
}

int pd_check_power_swap(int port)
{
	/* Always refuse power swap */
	return 0;
}

int pd_board_checks(void)
{
	return EC_SUCCESS;
}

int pd_set_power_supply_ready(int port)
{
	char str[256];
	
	port_pin_set_output_level(VBUS_5V_EN_PIN, true);
	
	memset(display_buffer, 0x00, DISP_MEM_SIZE);
	sprintf(str, "Sink Detected");
	UG_PutString(0, 8, str);
	sprintf(str, "Sending 5V on Vbus");
	UG_PutString(0, 16, str);
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);
	
	return EC_SUCCESS; /* we are ready */
}

void pd_transition_voltage(int idx)
{	
	if (idx == 1)
	{
		port_pin_set_output_level(VBUS_5V_EN_PIN,  true);
		port_pin_set_output_level(VBUS_15V_EN_PIN, false);
	}
	else if (idx == 2)
	{
		port_pin_set_output_level(VBUS_15V_EN_PIN, true);
		port_pin_set_output_level(VBUS_5V_EN_PIN,  false);
	}
	// other idx should be ignored
}

void pd_check_dr_role(int port, int dr_role, int flags)
{
#if 0
	/* If UFP, try to switch to DFP */
	if ((flags & PD_FLAGS_PARTNER_DR_DATA) && dr_role == PD_ROLE_UFP)
	pd_request_data_swap(port);
#endif
}

void pd_check_pr_role(int port, int pr_role, int flags)
{
#if 0
	/*
	 * If partner is dual-role power and dualrole toggling is on, consider
	 * if a power swap is necessary.
	 */
	if ((flags & PD_FLAGS_PARTNER_DR_POWER) &&
	    pd_get_dual_role() == PD_DRP_TOGGLE_ON) {
		/*
		 * If we are a sink and partner is not externally powered, then
		 * swap to become a source. If we are source and partner is
		 * externally powered, swap to become a sink.
		 */
		int partner_extpower = flags & PD_FLAGS_PARTNER_EXTPOWER;

		if ((!partner_extpower && pr_role == PD_ROLE_SINK) ||
		     (partner_extpower && pr_role == PD_ROLE_SOURCE))
			pd_request_power_swap(port);
	}
#endif // if 0
}

void pd_process_source_cap_callback(int port, int cnt, uint32_t *src_caps)
{
	char str[256];
	int i;
	uint32_t ma, mv;
	
	memset(display_buffer, 0x00, DISP_MEM_SIZE);
	
	sprintf(str, "Has Power Delivery");
	UG_PutString(0, 8, str);
	
	for (i = 0; i < cnt; i++)
	{
		pd_extract_pdo_power(src_caps[i], &ma, &mv);
		sprintf(str, "%5.2f V, %5.2f A", (float)mv/1000, (float)ma/1000);
		UG_PutString(0, 8*(i+2), str);
	}
	
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);
}

int pd_board_check_request(uint32_t rdo, int pdo_cnt)
{
	int idx = RDO_POS(rdo);
	char str[256];
	uint32_t ma;

	if ((0 == idx) || (idx > pdo_cnt)) {
		return EC_ERROR_INVAL;
	}
	
	memset(display_buffer, 0x00, DISP_MEM_SIZE);
	sprintf(str, "Sink Connected");
	UG_PutString(0, 8, str);
	sprintf(str, "New Vbus Requested");
	UG_PutString(0, 16, str);
	
	if (idx == 1) {
		sprintf(str, " 5.0 V");
	} else if (idx == 2) {
		sprintf(str, "20.0 V");
	}
	
	// Max Current
	ma = (rdo & 0x3FF)*10;
	sprintf(str, "%s, %i mA", str, ma);
	UG_PutString(0, 24, str);
	
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);
	
	return EC_SUCCESS;
}