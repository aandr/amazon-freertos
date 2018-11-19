#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOSIPConfig.h"
#include "platform.h"
#include "r_sci_rx_if.h"
#include "r_byteq_if.h"
#include "sx_ulpgn_driver.h"



const uint8_t ulpgn_return_text_ok[]          = ULPGN_RETURN_TEXT_OK;
const uint8_t ulpgn_return_text_connect[]     = ULPGN_RETURN_TEXT_CONNECT;
const uint8_t ulpgn_return_text_ring[]        = ULPGN_RETURN_TEXT_RING;
const uint8_t ulpgn_return_text_no_carrier[]  = ULPGN_RETURN_TEXT_NO_CARRIER;
const uint8_t ulpgn_return_text_error[]       = ULPGN_RETURN_TEXT_ERROR;
const uint8_t ulpgn_return_text_no_dialtone[] = ULPGN_RETURN_TEXT_NO_DIALTONE;
const uint8_t ulpgn_return_text_busy[]        = ULPGN_RETURN_TEXT_BUSY;
const uint8_t ulpgn_return_text_no_answer[]   = ULPGN_RETURN_TEXT_NO_ANSWER;

const uint8_t ulpgn_return_numeric_ok[]          = ULPGN_RETURN_NUMERIC_OK;
const uint8_t ulpgn_return_numeric_connect[]     = ULPGN_RETURN_NUMERIC_CONNECT;
const uint8_t ulpgn_return_numeric_ring[]        = ULPGN_RETURN_NUMERIC_RING;
const uint8_t ulpgn_return_numeric_no_carrier[]  = ULPGN_RETURN_NUMERIC_NO_CARRIER;
const uint8_t ulpgn_return_numeric_error[]       = ULPGN_RETURN_NUMERIC_ERROR;
const uint8_t ulpgn_return_numeric_no_dialtone[] = ULPGN_RETURN_NUMERIC_NO_DIALTONE;
const uint8_t ulpgn_return_numeric_busy[]        = ULPGN_RETURN_NUMERIC_BUSY;
const uint8_t ulpgn_return_numeric_no_answer[]   = ULPGN_RETURN_NUMERIC_NO_ANSWER;

const uint8_t ulpgn_socket_status_closed[]       = ULPGN_SOCKET_STATUS_TEXT_CLOSED;
const uint8_t ulpgn_socket_status_socket[]       = ULPGN_SOCKET_STATUS_TEXT_SOCKET;
const uint8_t ulpgn_socket_status_bound[]        = ULPGN_SOCKET_STATUS_TEXT_BOUND;
const uint8_t ulpgn_socket_status_listen[]       = ULPGN_SOCKET_STATUS_TEXT_LISTEN;
const uint8_t ulpgn_socket_status_connected[]    = ULPGN_SOCKET_STATUS_TEXT_CONNECTED;

const uint8_t ulpgn_return_dummy[]   = "";

const uint8_t * const ulpgn_result_code[ULPGN_RETURN_ENUM_MAX][ULPGN_RETURN_STRING_MAX] =
{
	/* text mode*/                  /* numeric mode */
	{ulpgn_return_text_ok,          ulpgn_return_numeric_ok          },
	{ulpgn_return_text_connect,     ulpgn_return_numeric_connect     },
	{ulpgn_return_text_ring,        ulpgn_return_numeric_ring        },
	{ulpgn_return_text_no_carrier,  ulpgn_return_numeric_no_carrier  },
	{ulpgn_return_text_error,       ulpgn_return_numeric_error       },
	{ulpgn_return_dummy,            ulpgn_return_dummy               },
	{ulpgn_return_text_no_dialtone, ulpgn_return_numeric_no_dialtone },
	{ulpgn_return_text_busy,        ulpgn_return_numeric_busy        },
	{ulpgn_return_text_no_answer,   ulpgn_return_numeric_no_answer   },
};

const uint8_t * const ulpgn_socket_status[ULPGN_SOCKET_STATUS_MAX] =
{
	ulpgn_socket_status_closed,
	ulpgn_socket_status_socket,
	ulpgn_socket_status_bound,
	ulpgn_socket_status_listen,
	ulpgn_socket_status_connected,
};

volatile uint8_t current_socket_index;
uint8_t buff[1000];
uint8_t recvbuff[1000];
sci_cfg_t   g_sx_ulpgn_sci_config[2];
sci_err_t   g_sx_ulpgn_sci_err;
volatile uint32_t g_sx_ulpgn_uart_teiflag[2];
static uint8_t timeout_overflow_flag[2];
static sci_hdl_t sx_ulpgn_uart_sci_handle[2];
static TickType_t starttime[2], thistime[2], endtime[2];
static TickType_t startbytetime[2], thisbytetime[2], endbytetime[2];
static uint8_t byte_timeout_overflow_flag[2];

uint8_t g_sx_ulpgn_return_mode;
uint8_t socket_create_flag[CREATEABLE_SOCKETS];
uint8_t socket_connect_flag[CREATEABLE_SOCKETS];
uint8_t socket_recv_buff[CREATEABLE_SOCKETS][1460];
byteq_hdl_t socket_byteq_hdl[CREATEABLE_SOCKETS];
uint32_t socket_recv_error_count[4];

static void sx_ulpgn_uart_callback_command_port(void *pArgs);
static void sx_ulpgn_uart_callback_data_port(void *pArgs);
static void timeout_init(uint8_t socket_no, uint16_t timeout_ms);
static void bytetimeout_init(uint8_t socket_no, uint16_t timeout_ms);
static int32_t check_timeout(uint8_t socket_no, int32_t rcvcount);
static int32_t check_bytetimeout(uint8_t socket_no, int32_t rcvcount);
static int32_t sx_ulpgn_serial_open(void);
static int32_t sx_ulpgn_serial_escape(void);
static int32_t sx_ulpgn_serial_data_port_open(void);
static int32_t sx_ulpgn_serial_send_basic(uint8_t serial_ch_id, uint8_t *ptextstring, uint16_t response_type, uint16_t timeout_ms, sx_ulpgn_return_code_t expect_code);
static int32_t sx_ulpgn_get_socket_status(uint8_t socket_no);
static int32_t sx_ulpgn_change_socket_index(uint8_t socket_no);
static TickType_t g_sl_ulpgn_tcp_recv_timeout = 3000;		/* ## slowly problem ## unit: 1ms */

/**
 * @brief The global mutex to ensure that only one operation is accessing the
 * g_sx_ulpgn_semaphore flag at one time.
 */
static SemaphoreHandle_t g_sx_ulpgn_semaphore = NULL;

/**
 * @brief Maximum time in ticks to wait for obtaining a semaphore.
 */
static const TickType_t xMaxSemaphoreBlockTime = pdMS_TO_TICKS( 60000UL );



int32_t sx_ulpgn_wifi_init(void)
{
	int32_t ret;
#if ULPGN_PORT_DEBUG == 1
	DEBUG_PORT4_DDR = 1;
	DEBUG_PORT4_DR = 0;
	DEBUG_PORT7_DDR = 1;
	DEBUG_PORT7_DR = 0;
#endif
	/* Wifi Module hardware reset   */
	ULPGN_RESET_PORT_DDR = 1;
	ULPGN_RESET_PORT_DR = 0; /* Low */
	R_BSP_SoftwareDelay(26, BSP_DELAY_MILLISECS); /* 5us mergin 1us */
	ULPGN_RESET_PORT_DR = 1; /* High */
//	R_BSP_SoftwareDelay(26, BSP_DELAY_MILLISECS); /* 5us mergin 1us */

	for (uint8_t i = 0; i < CREATEABLE_SOCKETS; i++) {
		socket_create_flag[i] = 0;
		socket_connect_flag[i] = 0;
	}

	if (g_sx_ulpgn_semaphore != NULL) {
		vSemaphoreDelete(g_sx_ulpgn_semaphore);
	}

    g_sx_ulpgn_semaphore = xSemaphoreCreateMutex();

    if( g_sx_ulpgn_semaphore == NULL )
    {
    	return -1;
    }


	ret = sx_ulpgn_serial_open();
	if(ret != 0)
	{
		return ret;
	}


	ret = sx_ulpgn_serial_data_port_open();
	if(ret != 0)
	{
		return ret;
	}

	/* reboots the system */
	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATZ\r", 1000, 2000, ULPGN_RETURN_OK);
	/*
	if(ret != 0)
	{
		ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATZ\r", 1000, 2000, ULPGN_RETURN_OK);
		if(ret != 0)
		{
			// repetetive resets by unit tests cause some issues with an OK not being return here
			//return ret;
		}
	}
	*/
	g_sx_ulpgn_return_mode = 0;

	/* no echo */
	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATE0\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}
	g_sx_ulpgn_return_mode = 1;
	/* result code format = numeric */
	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATV0\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}

	/* Escape guard time = 200msec */

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATS12=1\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATS108=20\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}


	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATS103=30\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}

	/* Disconnect from currently connected Access Point, */
	ret = sx_ulpgn_serial_send_basic(0, "ATWD\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}
	current_socket_index = 0;

	/* Enable DHCP */
	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATNDHCP=1\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATTO=1\r", 3, 1000, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}

#if (ULPGN_USE_UART_NUM == 2)
	/* Command Port = HSUART1(PMOD-UART), Data Port = Debug　UART */
	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATUART=1,2\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return ret;
	}

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_DATA_PORT, NULL, 1000, 200, ULPGN_RETURN_OK);
#endif

	ret = sx_ulpgn_socket_init();

	return ret;
}

int32_t sx_ulpgn_wifi_connect(uint8_t *pssid, uint32_t security, uint8_t *ppass)
{
	int32_t ret;
	char *pstr,pstr2;
	int32_t line_len;
	int32_t scanf_ret;
	volatile char secu[3][10];
	uint32_t security_encrypt_type = 1;

	vTaskDelay(2000); // timing issue

	strcpy((char *)buff,"ATWAWPA=");
	strcat((char *)buff,(const char *)pssid);
	strcat((char *)buff,",");
	if(security != ULPGN_SECURITY_WPA && security != ULPGN_SECURITY_WPA2)
	{
		return -1;
	}
	if(security == ULPGN_SECURITY_WPA)
	{
		strcat((char *)buff,"1,");
	}
	else
	{
		strcat((char *)buff,"2,");
	}
	if(security_encrypt_type == 1)
	{
		strcat((char *)buff,"1,1,");
	}
	else
	{
		strcat((char *)buff,"0,0,");
	}
	strcat((char *)buff,(const char *)ppass);
	strcat((char *)buff,"\r");

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, buff, 3, 5000, ULPGN_RETURN_OK);
	if(0 == ret)
	{
		sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATW\r", 3, 1000, ULPGN_RETURN_OK);
	}
	return ret;
}

int32_t sx_ulpgn_wifi_disconnect() {
	sx_ulpgn_serial_escape();

	for (uint8_t i = 0; i < CREATEABLE_SOCKETS; i++) {
		if (0 != R_BYTEQ_Close(socket_byteq_hdl[i])) {
			return -1;
		}
	}

	sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATWD\r", 3, 1000, ULPGN_RETURN_OK);
    R_SCI_Close(sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT]);
#if ULPGN_USE_UART_NUM == 2
    R_SCI_Close(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT]);
#endif
    return 0;
}

static int32_t sx_ulpgn_serial_escape() {
#if ULPGN_USE_UART_NUM == 1 // escape sequence is only needed for single socket
	vTaskDelay(500 / portTICK_PERIOD_MS);
	if (R_SCI_Send(sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT], "+++", 3) == SCI_SUCCESS) {
		transparent_mode = false;
		return 0;
	} else {
		return 1;
	}
#endif
}


int32_t sx_ulpgn_wifi_get_macaddr(uint8_t *pmacaddr)
{
	int32_t ret;
	uint8_t macaddr[6];

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATW\r", 300, 3000, ULPGN_RETURN_OK);
	if (ret != 0) {
		return -1;
	}

	ret = sscanf((const char *)recvbuff, "%*[^\n]\n%*[^\n]\n%*[^\n]\nMac Addr     =   %2x:%2x:%2x:%2x:%2x:%2x\r", &macaddr[0], &macaddr[1], &macaddr[2], &macaddr[3], &macaddr[4], &macaddr[5]);

	if (ret == 6) {
		memcpy(pmacaddr, &macaddr[0], sizeof(macaddr));
		return 0;
	} else {
		return -1;
	}
}

int32_t sx_ulpgn_wifi_scan(WIFIScanResult_t *results, uint8_t maxNetworks) {
	int32_t ret;
	uint8_t idx = 0;
	uint8_t *bssid;
	char *ptr = recvbuff + 2;

	// TODO investigate why this never returns the full response
	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATWS\r", 5000, 8000, ULPGN_RETURN_OK);
	if (strlen(recvbuff) < 10) {
		return -1;
	}

	do {
		if (strncmp(ptr, "ssid =", 6) != 0) {
			break; // end of list
		}
		// SSID
		ret = sscanf(ptr, "ssid = %32s\r", results[idx].cSSID);
		if (ret != 1) return idx > 0 ? 0 : -1;
		while(*(ptr++) != '\n');

		// BSSID
		bssid = &results[idx].ucBSSID[0];
		ret = sscanf(ptr, "bssid = %x:%x:%x:%x:%x:%x\r", &bssid[0], &bssid[1], &bssid[2], &bssid[3], &bssid[4], &bssid[5], &bssid[6]);
		if (ret != 6) return idx > 0 ? 0 : -1;
		while(*(ptr++) != '\n');

		ret = sscanf(ptr, "channel = %d\r", &results[idx].cChannel);
		if (ret != 1) return idx > 0 ? 0 : -1;
		while(*(ptr++) != '\n');

		ret = sscanf(ptr, "indicator = %d\r", &results[idx].cRSSI);
		if (ret != 1) return idx > 0 ? 0 : -1;
		while(*(ptr++) != '\n');

		if (strncmp(ptr, "security = NONE!", 16) == 0) {
			results[idx].xSecurity = eWiFiSecurityOpen;
			while(*(ptr++) != '\n');
		} else {
			while(*(ptr++) != '\n');
			if (strncmp(ptr, "WPA", 3) == 0) {
				results[idx].xSecurity = eWiFiSecurityWPA;
			} else 	if (strncmp(ptr, "RSN/WPA2", 8) == 0) {
				results[idx].xSecurity = eWiFiSecurityWPA2;
			}
			// Note WEP is not recognized by the modem
			while(*(ptr++) != '\n');
		}

		while(*ptr != '\0' && *(ptr++) != '\n'); // end of record
	} while(++idx < maxNetworks);

	return 0;
}

int32_t sx_ulpgn_get_ip(uint32_t *ulipaddr) {
	int32_t ret;
	char *buff = recvbuff;
	uint8_t ipaddr[4];

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATNSET?\r", 500, 5000, ULPGN_RETURN_OK);
	if (ret != 0) {
		return -1;
	}
	if (buff[0] == '\n' && buff[1] == '\0') {
		buff += 2;
	}

	ret = sscanf((const char *)buff, "IP:%d.%d.%d.%d ", &ipaddr[0], &ipaddr[1], &ipaddr[2], &ipaddr[3]);

	if (ret == 4) {
		memcpy(ulipaddr, &ipaddr[0], sizeof(ipaddr));
		return 0;
	} else {
		return -1;
	}
}

uint8_t sx_ulpgn_get_avail_socket() {
	for (int8_t i = 0; i < CREATEABLE_SOCKETS; i++) {
		if (socket_create_flag[i] == 0) {
			return i;
		}
	}
	return 255;
}

int32_t sx_ulpgn_socket_create(uint8_t socket_no, uint32_t type,uint32_t ipversion)
{
	int32_t ret;

    if( xSemaphoreTake( g_sx_ulpgn_semaphore, xMaxSemaphoreBlockTime ) == pdTRUE )
    {

	#if DEBUGLOG == 1
		R_BSP_CpuInterruptLevelWrite (14);
		printf("sx_ulpgn_socket_create(%d)\r\n",socket_no);
		R_BSP_CpuInterruptLevelWrite (0);
	#endif
		if(	socket_create_flag[socket_no] == 1)
		{
	        ( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}

#if ULPGN_USE_UART_NUM == 2
		ret = sx_ulpgn_change_socket_index(socket_no);
		if(ret != 0)
		{
	        ( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}
#endif
		ret = sx_ulpgn_get_socket_status(socket_no);

		sprintf((char *)buff,"ATNSOCK=%d,%d\r",(uint8_t)(type),(uint8_t)(ipversion));

		ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, buff, 10, 200, ULPGN_RETURN_OK);
	//	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, buff, 10, 200, ULPGN_RETURN_OK);
		if(ret != 0 && ret != -2)
		{
	        ( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return ret;
		}
		socket_create_flag[socket_no] = 1;
	//	ret = sx_ulpgn_serial_send_basic(socket_no, "ATNSTAT\r", 3, 200, ULPGN_RETURN_OK);

		ret = sx_ulpgn_get_socket_status(socket_no);

        /* Give back the socketInUse mutex. */
        ( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
    }
    else
    {
    	return -1;
    }

	return 0;
}


int32_t sx_ulpgn_tcp_connect(uint8_t socket_no, uint32_t ipaddr, uint16_t port)
{
	int32_t ret;
    if( xSemaphoreTake( g_sx_ulpgn_semaphore, xMaxSemaphoreBlockTime ) == pdTRUE )
    {
		if(	socket_create_flag[socket_no] == 0 || socket_connect_flag[socket_no] == 1)
		{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}

	#if DEBUGLOG == 1
		R_BSP_CpuInterruptLevelWrite (14);
		printf("sx_ulpgn_tcp_connect(%d)\r\n",socket_no);
		R_BSP_CpuInterruptLevelWrite (0);
	#endif
#if ULPGN_USE_UART_NUM == 2
		ret = sx_ulpgn_change_socket_index(socket_no);
		if(ret != 0)
		{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}
#endif
		strcpy((char *)buff,"ATNCTCP=");
		sprintf((char *)buff+strlen((char *)buff),"%d.%d.%d.%d,%d\r",(uint8_t)(ipaddr>>24),(uint8_t)(ipaddr>>16),(uint8_t)(ipaddr>>8),(uint8_t)(ipaddr),port);

#if ULPGN_USE_UART_NUM == 2
		ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, buff, 300, 10000, ULPGN_RETURN_OK);
#endif
#if ULPGN_USE_UART_NUM == 1
		ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, buff, 300, 10000, ULPGN_RETURN_CONNECT);
#endif
		if (ret == 0) {
			socket_connect_flag[socket_no] = 1;
		}
		/* Give back the socketInUse mutex. */
		( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );

    }
    else
    {
    	return -1;
    }
	return ret;

}

int32_t sx_ulpgn_tcp_send(uint8_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout_ms)
{
	int32_t timeout;
	volatile int32_t sended_length;
	int32_t lenghttmp1;
	int32_t lenghttmp2;
	int32_t ret;
	sci_err_t ercd;
//	sci_baud_t baud;

    if( xSemaphoreTake( g_sx_ulpgn_semaphore, xMaxSemaphoreBlockTime ) == pdTRUE )
    {
    	if(0 == socket_connect_flag[socket_no])
    	{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
    	}
	#if DEBUGLOG == 1
		R_BSP_CpuInterruptLevelWrite (14);
		printf("sx_ulpgn_tcp_send(%d)\r\n",socket_no);
		R_BSP_CpuInterruptLevelWrite (0);
	#endif
#if ULPGN_USE_UART_NUM == 2
		ret = sx_ulpgn_change_socket_index(socket_no);
		if(ret != 0)
		{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}
#endif
		sended_length = 0;
		timeout_init(1, timeout_ms);

		timeout = 0;

		lenghttmp2 = SCI_TX_BUSIZ_DATA;
		while(sended_length < length)
		{
			if(length - sended_length > lenghttmp2)
			{
				lenghttmp1 = lenghttmp2;
			}
			else
			{
				lenghttmp1 = length - sended_length;
			}
			g_sx_ulpgn_uart_teiflag[ULPGN_UART_DATA_PORT] = 0;
			ercd = R_SCI_Send(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT], pdata, lenghttmp1);
			if(SCI_SUCCESS != ercd)
			{
				/* Give back the socketInUse mutex. */
				( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
				return -1;
			}

			while(1)
			{
				if(0 != g_sx_ulpgn_uart_teiflag[ULPGN_UART_DATA_PORT])
				{
					break;
				}
				//if(-1 == check_timeout(0))
				//{
				//	timeout = 1;
				//	break;
				//}
			}
			//if(timeout == 1)
			//{
			//	return -1;
			//}
			sended_length += lenghttmp1;
		}
		if(timeout == 1 )
		{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}

	#if DEBUGLOG == 1
		R_BSP_CpuInterruptLevelWrite (14);
		printf("tcp %d byte send\r\n",sended_length);
		R_BSP_CpuInterruptLevelWrite (0);
	#endif

		/* Give back the socketInUse mutex. */
		( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
    }
    else
    {
    	return -1;
    }
	return sended_length;
}

int32_t sx_ulpgn_tcp_recv(uint8_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout_ms)
{
	sci_err_t ercd;
	uint32_t recvcnt = 0;
	int32_t ret;
	byteq_err_t byteq_ret;
#if DEBUGLOG == 1
	TickType_t tmptime2;
#endif
	TickType_t tmptime1;

    if( xSemaphoreTake( g_sx_ulpgn_semaphore, xMaxSemaphoreBlockTime ) == pdTRUE )
    {
    	if(0 == socket_connect_flag[socket_no])
    	{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
    	}

	#if DEBUGLOG == 1
		R_BSP_CpuInterruptLevelWrite (14);
		printf("sx_ulpgn_tcp_recv(%d)\r\n",socket_no);
		R_BSP_CpuInterruptLevelWrite (0);
	#endif
#if ULPGN_USE_UART_NUM == 2
		ret = sx_ulpgn_change_socket_index(socket_no);
		if(ret != 0)
		{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}
#endif
		if(g_sl_ulpgn_tcp_recv_timeout != 0)
		{
			timeout_init(1, timeout_ms);
		}
		while(1)
		{
#if ULPGN_USE_UART_NUM == 1
			ercd = R_SCI_Receive(sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT], (pdata + recvcnt), 1);
			if(SCI_SUCCESS == ercd)
			{
				recvcnt++;
				if(recvcnt >= length)
				{
					break;
				}
			}
			if(-1 == check_timeout(1, 0))
			{
				break;
			}
#endif
#if ULPGN_USE_UART_NUM == 2
			byteq_ret = R_BYTEQ_Get(socket_byteq_hdl[current_socket_index], (pdata + recvcnt));
			if(BYTEQ_SUCCESS == byteq_ret)
			{
				recvcnt++;
				if(recvcnt >= length)
				{
					break;
				}
			}
			if(g_sl_ulpgn_tcp_recv_timeout != 0)
			{
				if(-1 == check_timeout(1, 0))
				{
	#if DEBUGLOG == 1
					R_BSP_CpuInterruptLevelWrite (14);
					printf("recv timeout\r\n");
					R_BSP_CpuInterruptLevelWrite (0);
	#endif
					nop();
					break;
				}
			}
#endif
		}
	#if DEBUGLOG == 1
		tmptime2 = xTaskGetTickCount();
		R_BSP_CpuInterruptLevelWrite (14);
		printf("r:%06d:tcp %ld byte received.reqsize=%ld\r\n",tmptime2, recvcnt, length);
		R_BSP_CpuInterruptLevelWrite (0);
	#endif

#if ULPGN_USE_UART_NUM == 2
		if(recvcnt == 0)
		{
			ret = sx_ulpgn_get_socket_status(socket_no);
			if(ret != ULPGN_SOCKET_STATUS_CONNECTED)
			{
				/* Give back the socketInUse mutex. */
				( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
				return -1;
			}
		}
#endif
		/* Give back the socketInUse mutex. */
		( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
    }
    else
    {
    	return -1;
    }
	return recvcnt;
}

int32_t sx_ulpgn_serial_tcp_recv_timeout_set(uint8_t socket_no, TickType_t timeout_ms)
{
	g_sl_ulpgn_tcp_recv_timeout = timeout_ms;
	return 0;
}

int32_t sx_ulpgn_tcp_disconnect(uint8_t socket_no)
{
	int32_t ret = 0;
    if( xSemaphoreTake( g_sx_ulpgn_semaphore, xMaxSemaphoreBlockTime ) == pdTRUE )
    {
		if(1 == socket_create_flag[socket_no])
		{
#if DEBUGLOG == 1
			R_BSP_CpuInterruptLevelWrite (14);
			printf("sx_ulpgn_tcp_disconnect(%d)\r\n",socket_no);
			R_BSP_CpuInterruptLevelWrite (0);
#endif
#if ULPGN_USE_UART_NUM == 2
			ret = sx_ulpgn_change_socket_index(socket_no);
			if(ret != 0)
			{
				/* Give back the socketInUse mutex. */
				( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
				return -1;
			}
#endif
#if ULPGN_USE_UART_NUM == 1
			R_BSP_SoftwareDelay(201, BSP_DELAY_MILLISECS); /* 1s */
			R_SCI_Control(sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT],SCI_CMD_RX_Q_FLUSH,NULL);
			ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "+++", 3, 1000, ULPGN_RETURN_OK);
#endif
			ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATNCLOSE\r", 3, 1000, ULPGN_RETURN_OK);
			if(0 == ret)
			{
				socket_create_flag[socket_no] = 0;
				socket_connect_flag[socket_no] = 0;
#if ULPGN_USE_UART_NUM == 2
				R_BYTEQ_Flush(socket_byteq_hdl[socket_no]);
#endif
			}
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
		}
		else
		{
			/* Give back the socketInUse mutex. */
			( void ) xSemaphoreGive( g_sx_ulpgn_semaphore );
			return -1;
		}
    }
    else
    {
    	return -1;
    }
	return ret;

}

int32_t sx_ulpgn_dns_query(uint8_t *ptextstring, uint32_t *ulipaddr)
{
	uint32_t result;
	uint32_t ipaddr[4];
	int32_t func_ret;
	int32_t scanf_ret;
	strcpy((char *)buff,"ATNDNSQUERY=");
	sprintf((char *)buff+strlen((char *)buff),"%s\r",ptextstring);

	func_ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, buff, 3, 3000, ULPGN_RETURN_OK);
	if(func_ret != 0)
	{
		return -1;
	}
	scanf_ret = sscanf((const char *)recvbuff, "%d\r\n%d.%d.%d.%d\r\n",&result, &ipaddr[0], &ipaddr[1], &ipaddr[2], &ipaddr[3]);
	if(scanf_ret != 5)
	{
		return -1;
	}
	if(result != 1)
	{
		return -1;
	}
	*ulipaddr = (((uint32_t)ipaddr[0]) << 24) | (((uint32_t)ipaddr[1]) << 16) | (((uint32_t)ipaddr[2]) << 8) | ((uint32_t)ipaddr[3]);
	return 0;
}


static int32_t sx_ulpgn_serial_send_basic(uint8_t serial_ch_id, uint8_t *ptextstring, uint16_t response_type, uint16_t timeout_ms, sx_ulpgn_return_code_t expect_code)
{
#if DEBUGLOG == 1
	TickType_t tmptime1,tmptime2;
#endif
	memset(recvbuff, 0, sizeof(recvbuff));
	vTaskDelay(100);
	volatile int32_t timeout;
	sci_err_t ercd;
	uint32_t recvcnt = 0;

	timeout_init(serial_ch_id, timeout_ms);

	//uart_string_printf(ptextstring);
	//uart_string_printf("\r\n");
	if(ptextstring != NULL)
	{
		timeout = 0;
		recvcnt = 0;
		g_sx_ulpgn_uart_teiflag[serial_ch_id] = 0;
		ercd = R_SCI_Send(sx_ulpgn_uart_sci_handle[serial_ch_id], ptextstring, strlen((const char *)ptextstring));
		if(SCI_SUCCESS != ercd)
		{
			return -1;
		}

		while(1)
		{
			if(0 != g_sx_ulpgn_uart_teiflag[serial_ch_id])
	//		ercd = R_SCI_Control(sx_ulpgn_uart_sci_handle, SCI_CMD_TX_Q_BYTES_FREE, &non_used);
	//		if(non_used == SCI_TX_BUSIZ)
			{
				break;
			}
			if(-1 == check_timeout(serial_ch_id, recvcnt))
			{
				timeout = 1;
				break;
			}
		}
		if(timeout == 1)
		{
#if DEBUGLOG == 1
			R_BSP_CpuInterruptLevelWrite (14);
			printf("timeout.\r\n",tmptime1,ptextstring);
			R_BSP_CpuInterruptLevelWrite (0);
#endif
			return -1;
		}

	#if DEBUGLOG == 1
		tmptime1 = xTaskGetTickCount();
		if(ptextstring[strlen((const char *)ptextstring)-1] != '\r')
		{
			R_BSP_CpuInterruptLevelWrite (14);
			printf("s:%06d:%s\r\n",tmptime1,ptextstring);
			R_BSP_CpuInterruptLevelWrite (0);
		}
		else
		{
			R_BSP_CpuInterruptLevelWrite (14);
			printf("s:%06d:%s",tmptime1,ptextstring);
			printf("\n");
			R_BSP_CpuInterruptLevelWrite (0);
		}
	#endif
	}
	while(1)
	{
		ercd = R_SCI_Receive(sx_ulpgn_uart_sci_handle[serial_ch_id], &recvbuff[recvcnt], 1);
		if(SCI_SUCCESS == ercd)
		{
			recvcnt++;
			bytetimeout_init(serial_ch_id, response_type);
			if(recvcnt < 4)
			{
				continue;
			}
			if(recvcnt == sizeof(recvbuff)-2)
			{
				break;
			}
		}
		if(-1 == check_bytetimeout(serial_ch_id, recvcnt))
		{
			break;
		}
		if(-1 == check_timeout(serial_ch_id, recvcnt))
		{
			timeout = 1;
			break;
		}
	}
	if(timeout == 1)
	{
		return -1;
	}

#if DEBUGLOG == 1
	tmptime2 = xTaskGetTickCount();
	if(recvbuff[recvcnt-1] != '\r')
	{
		recvbuff[recvcnt] = '\r';
		recvbuff[recvcnt+1] = '\0';
	}
	else
	{
		recvbuff[recvcnt] = '\0';
	}
	printf("r:%06d:%s",tmptime2,recvbuff);
#endif
	/* Response data check */
	if(recvcnt < strlen((const char *)ulpgn_result_code[expect_code][g_sx_ulpgn_return_mode]))
	{
		return -1;
	}

	const char* expected_result = ulpgn_result_code[expect_code][g_sx_ulpgn_return_mode];
	const uint32_t expected_len = strlen(expected_result);
	uint32_t expected_offset = 0;

	if(0 != strncmp((const char *)&recvbuff[recvcnt - strlen((const char *)ulpgn_result_code[expect_code][g_sx_ulpgn_return_mode]) ],
			(const char *)ulpgn_result_code[expect_code][g_sx_ulpgn_return_mode], strlen((const char *)ulpgn_result_code[expect_code][g_sx_ulpgn_return_mode])))
	{
		if(0 == strncmp((const char *)&recvbuff[recvcnt - strlen((const char *)ulpgn_result_code[expect_code][g_sx_ulpgn_return_mode]) ],
				(const char *)ulpgn_result_code[ULPGN_RETURN_BUSY][g_sx_ulpgn_return_mode], strlen((const char *)ulpgn_result_code[ULPGN_RETURN_BUSY][g_sx_ulpgn_return_mode])))
		{
			/* busy */
			return -2;
		}
		return -1;
	}
	return 0;
}



/* return code: from 0 to 5 : socket status
 *              -1 : Error
 * */
static int32_t sx_ulpgn_get_socket_status(uint8_t socket_no)
{
	int32_t i;
	int32_t ret;
	char * str_ptr;
	uint8_t str[3][10];

	ret = sx_ulpgn_serial_send_basic(ULPGN_UART_COMMAND_PORT, "ATNSTAT\r", 3, 200, ULPGN_RETURN_OK);
	if(ret != 0)
	{
		return -1;
	}

	str_ptr = strchr((const char *)recvbuff, ',');
	if(str_ptr != NULL)
	{
		for(i = 0;i<ULPGN_SOCKET_STATUS_MAX;i++)
		{
			if(0 == strncmp((const char *)recvbuff, (const char *)ulpgn_socket_status[i], ((uint32_t)str_ptr - (uint32_t)recvbuff)))
			{
				break;
			}
		}
	}
	if(i < ULPGN_SOCKET_STATUS_MAX)
	{
		return i;
	}
	return -1;
}

#if ULPGN_USE_UART_NUM == 2
static int32_t sx_ulpgn_change_socket_index(uint8_t socket_no)
{
#if DEBUGLOG == 1
	TickType_t tmptime1,tmptime2;
#endif
	volatile int32_t timeout;
	sci_err_t ercd;
	uint32_t recvcnt = 0;
	int32_t ret;
	uint16_t i;
	uint16_t read_data = 0;;
	uint8_t before_socket_no;
	int8_t receive_ret;
	if(socket_no != current_socket_index)
	{
		R_BSP_InterruptsDisable();
		before_socket_no = current_socket_index;
#if ULPGN_PORT_DEBUG == 1
		DEBUG_PORT4_DR = 0;
#endif

		if(SCI_SUCCESS == R_SCI_Control(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT], SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ, &read_data))
		{
			if(read_data > 0)
			{
				R_SCI_Receive(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT], buff, read_data);
				for(i = 0;i<read_data;i++ )
				{
					R_BYTEQ_Put(socket_byteq_hdl[current_socket_index],*(buff+i));
				}
			}
		}
		R_BSP_InterruptsEnable();

		sprintf((char *)buff,"ATNSOCKINDEX=%d\r",socket_no);
#if 1
#if ULPGN_PORT_DEBUG == 1
		DEBUG_PORT4_DR = 1;
#endif
    	vTaskDelay(200);
#if ULPGN_PORT_DEBUG == 1
		DEBUG_PORT4_DR =0;
#endif
//			R_BSP_SoftwareDelay(200, BSP_DELAY_MILLISECS); /* 5us mergin 1us */
#endif
		while(1)
		{
			recvcnt= 0;
			receive_ret =-1;
			timeout = 0;
			g_sx_ulpgn_uart_teiflag[ULPGN_UART_COMMAND_PORT] = 0;
			ercd = R_SCI_Send(sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT], buff, strlen((const char *)buff));
			if(SCI_SUCCESS != ercd)
			{
				return -1;
			}
			while(1)
			{
				if(0 != g_sx_ulpgn_uart_teiflag[ULPGN_UART_COMMAND_PORT])
				{
					break;
				}
				if(-1 == check_timeout(ULPGN_UART_COMMAND_PORT, 1000))
				{
					timeout = 1;
					break;
				}
			}
			if(timeout == 1)
			{
				return -1;
			}
			bytetimeout_init(ULPGN_UART_COMMAND_PORT, 3);
			while(1)
			{
				ercd = R_SCI_Receive(sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT], &recvbuff[recvcnt], 1);
				if(SCI_SUCCESS == ercd)
				{
					if(recvcnt == 0)
					{
						receive_ret = recvbuff[0];
						if(receive_ret == *ulpgn_result_code[ULPGN_RETURN_OK][ULPGN_RETURN_STRING_NUMERIC])
						{
							R_BSP_InterruptsDisable();
							current_socket_index = socket_no;
	#if ULPGN_PORT_DEBUG == 1
							DEBUG_PORT4_DR = 1;
							DEBUG_PORT7_DR = socket_no;
	#endif
							ercd = R_SCI_Control(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT], SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ, &read_data);
							R_BSP_InterruptsEnable();
							if(SCI_SUCCESS == ercd)
							{
								if(read_data > 0)
								{
									R_SCI_Receive(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT], buff, read_data);
									for(i = 0;i<read_data;i++ )
									{
										R_BYTEQ_Put(socket_byteq_hdl[before_socket_no],*(buff+i));
									}
								}
							}
						}
						else if(receive_ret == *ulpgn_result_code[ULPGN_RETURN_BUSY][ULPGN_RETURN_STRING_NUMERIC])
						{
							/* do nothing */
						}
						else
						{
							return -1;
						}
					}
					recvcnt++;
					if(recvcnt < 2)
					{
#if DEBUGLOG ==1
						printf("ATNSOCKINDEX=%c\r\n",receive_ret);
#endif
						continue;
					}
					break;
				}
				if(-1 == check_bytetimeout(ULPGN_UART_COMMAND_PORT, recvcnt))
				{
					break;
				}
				if(-1 == check_timeout(ULPGN_UART_COMMAND_PORT, recvcnt))
				{
					timeout = 1;
					break;
				}
			}
			if(receive_ret == *ulpgn_result_code[ULPGN_RETURN_OK][ULPGN_RETURN_STRING_NUMERIC])
			{
				break;
			}
		}
#if ULPGN_PORT_DEBUG == 1
		DEBUG_PORT4_DR =0;
#endif
	}
	return 0;
}
#endif

static void timeout_init(uint8_t socket_no, uint16_t timeout_ms)
{
	starttime[socket_no] = xTaskGetTickCount();
	endtime[socket_no] = starttime[socket_no] + timeout_ms;
	if((starttime[socket_no] + endtime[socket_no]) < starttime[socket_no])
	{
		/* overflow */
		timeout_overflow_flag[socket_no] = 1;
	}
	else
	{
		timeout_overflow_flag[socket_no] = 0;
	}
}

static int32_t check_timeout(uint8_t socket_no, int32_t rcvcount)
{
	if(0 == rcvcount)
	{
		thistime[socket_no] = xTaskGetTickCount();
		if(timeout_overflow_flag[socket_no] == 0)
		{
			if(thistime[socket_no] >= endtime[socket_no] || thistime[socket_no] < starttime[socket_no])
			{
				return -1;
			}
		}
		else
		{
			if(thistime[socket_no] < starttime[socket_no] && thistime[socket_no] <= endtime[socket_no])
			{
				/* Not timeout  */
				return -1;
			}
		}
	}
	/* Not timeout  */
	return 0;
}

static void bytetimeout_init(uint8_t socket_no, uint16_t timeout_ms)
{
	startbytetime[socket_no] = xTaskGetTickCount();
	endbytetime[socket_no] = startbytetime[socket_no] + timeout_ms;
	if((startbytetime[socket_no] + endbytetime[socket_no]) < startbytetime[socket_no])
	{
		/* overflow */
		byte_timeout_overflow_flag[socket_no] = 1;
	}
	else
	{
		byte_timeout_overflow_flag[socket_no] = 0;
	}
}

static int32_t check_bytetimeout(uint8_t socket_no, int32_t rcvcount)
{
	if(0 != rcvcount)
	{
		thisbytetime[socket_no] = xTaskGetTickCount();
		if(byte_timeout_overflow_flag[socket_no] == 0)
		{
			if(thisbytetime[socket_no] >= endbytetime[socket_no] || thisbytetime[socket_no] < startbytetime[socket_no])
			{
				return -1;
			}
		}
		else
		{
			if(thisbytetime[socket_no] < startbytetime[socket_no] && thisbytetime[socket_no] <= endbytetime[socket_no])
			{
				/* Not timeout  */
				return -1;
			}
		}
	}
	/* Not timeout  */
	return 0;
}

static int32_t sx_ulpgn_serial_open(void)
{
	sci_err_t   my_sci_err;

#if (BSP_CFG_BOARD_REVISION == 1)
	R_SCI_PinSet_SCI6();
#elif (BSP_CFG_BOARD_REVISION == 5)
	R_SCI_PinSet_SCI10();
#endif

	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.baud_rate    = 115200;
	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.clk_src      = SCI_CLK_INT;
	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.data_size    = SCI_DATA_8BIT;
	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.parity_en    = SCI_PARITY_OFF;
	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.parity_type  = SCI_EVEN_PARITY;
	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.stop_bits    = SCI_STOPBITS_1;
	g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT].async.int_priority = 15;    // 1=lowest, 15=highest

#if (BSP_CFG_BOARD_REVISION == 1)
    my_sci_err = R_SCI_Open(SCI_CH6, SCI_MODE_ASYNC, &g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT], sx_ulpgn_uart_callback_command_port, &sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT]);
#elif  (BSP_CFG_BOARD_REVISION == 5)
    my_sci_err = R_SCI_Open(SCI_CH10, SCI_MODE_ASYNC, &g_sx_ulpgn_sci_config[ULPGN_UART_COMMAND_PORT], sx_ulpgn_uart_callback_command_port, &sx_ulpgn_uart_sci_handle[ULPGN_UART_COMMAND_PORT]);
#endif

    if(SCI_SUCCESS != my_sci_err)
    {
    	return -1;
    }

    return 0;

}

#if ULPGN_USE_UART_NUM == 2
static int32_t sx_ulpgn_serial_data_port_open(void)
{
	sci_err_t   my_sci_err;

#if (BSP_CFG_BOARD_REVISION == 1)
	R_SCI_PinSet_SCI7();
#elif (BSP_CFG_BOARD_REVISION == 5)
	R_SCI_PinSet_SCI6();
#endif

	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.baud_rate    = 115200;
	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.clk_src      = SCI_CLK_INT;
	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.data_size    = SCI_DATA_8BIT;
	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.parity_en    = SCI_PARITY_OFF;
	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.parity_type  = SCI_EVEN_PARITY;
	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.stop_bits    = SCI_STOPBITS_1;
	g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT].async.int_priority = 15;    // 1=lowest, 15=highest

#if (BSP_CFG_BOARD_REVISION == 1)
    my_sci_err = R_SCI_Open(SCI_CH7, SCI_MODE_ASYNC, &g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT], sx_ulpgn_uart_callback_data_port, &sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT]);
#elif  (BSP_CFG_BOARD_REVISION == 5)
    my_sci_err = R_SCI_Open(SCI_CH6, SCI_MODE_ASYNC, &g_sx_ulpgn_sci_config[ULPGN_UART_DATA_PORT], sx_ulpgn_uart_callback_data_port, &sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT]);
#endif

    if(SCI_SUCCESS != my_sci_err)
    {
    	return -1;
    }

    return 0;
}
#endif
int32_t sx_ulpgn_socket_init(void)
{
#if ULPGN_USE_UART_NUM == 2

	int i;
	for(i = 0;i<CREATEABLE_SOCKETS; i++)
	{
		if(BYTEQ_SUCCESS != R_BYTEQ_Open(socket_recv_buff[i], 1460, &socket_byteq_hdl[i]))
		{
			return -1;
		}
	}
#endif

	/* Success. */
	return 0;

}

static void sx_ulpgn_uart_callback_command_port(void *pArgs)
{
    sci_cb_args_t   *p_args;

    p_args = (sci_cb_args_t *)pArgs;

    if (SCI_EVT_RX_CHAR == p_args->event)
    {
        /* From RXI interrupt; received character data is in p_args->byte */
        R_NOP();
    }
#if SCI_CFG_TEI_INCLUDED
	else if (SCI_EVT_TEI == p_args->event)
	{
		g_sx_ulpgn_uart_teiflag[ULPGN_UART_COMMAND_PORT] = 1;
		R_NOP();

	}
#endif
    else if (SCI_EVT_RXBUF_OVFL == p_args->event)
    {
        /* From RXI interrupt; rx queue is full; 'lost' data is in p_args->byte
           You will need to increase buffer size or reduce baud rate */
        R_NOP();
    }
    else if (SCI_EVT_OVFL_ERR == p_args->event)
    {
        /* From receiver overflow error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_NOP();
    }
    else if (SCI_EVT_FRAMING_ERR == p_args->event)
    {
        /* From receiver framing error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_NOP();
    }
    else if (SCI_EVT_PARITY_ERR == p_args->event)
    {
        /* From receiver parity error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_NOP();
    }
    else
    {
        /* Do nothing */
    }

} /* End of function my_sci_callback() */

#if ULPGN_USE_UART_NUM == 2
static void sx_ulpgn_uart_callback_data_port(void *pArgs)
{
    sci_cb_args_t   *p_args;
    sci_err_t ret;
    uint8_t data;

    p_args = (sci_cb_args_t *)pArgs;

    if (SCI_EVT_RX_CHAR == p_args->event)
    {
    	if(socket_create_flag[current_socket_index] == 1)
    	{

    		ret = R_SCI_Receive(sx_ulpgn_uart_sci_handle[ULPGN_UART_DATA_PORT], &data, 1);
    		if (ret != 0)
			{
				socket_recv_error_count[current_socket_index]++;
				return;
			}

			/* From RXI interrupt; received character data is in p_args->byte */

    		ret = R_BYTEQ_Put(socket_byteq_hdl[current_socket_index], p_args->byte);
    		//ret = R_BYTEQ_Put(socket_byteq_hdl[current_socket_index], data);
			if (ret != 0) {
				socket_recv_error_count[current_socket_index]++;
				return;
			}
			else
			{
			}
    	}
    	else
    	{
    	}
    }
#if SCI_CFG_TEI_INCLUDED
	else if (SCI_EVT_TEI == p_args->event)
	{
		g_sx_ulpgn_uart_teiflag[ULPGN_UART_DATA_PORT] = 1;
		R_NOP();
	}
#endif
    else if (SCI_EVT_RXBUF_OVFL == p_args->event)
    {
        /* From RXI interrupt; rx queue is full; 'lost' data is in p_args->byte
           You will need to increase buffer size or reduce baud rate */
        R_NOP();
    }
    else if (SCI_EVT_OVFL_ERR == p_args->event)
    {
        /* From receiver overflow error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_NOP();
    }
    else if (SCI_EVT_FRAMING_ERR == p_args->event)
    {
        /* From receiver framing error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_NOP();
    }
    else if (SCI_EVT_PARITY_ERR == p_args->event)
    {
        /* From receiver parity error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_NOP();
    }
    else
    {
        /* Do nothing */
    }

} /* End of function my_sci_callback() */
#endif
