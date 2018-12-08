#ifndef SX_ULPGN_DRIVER_H
#define SX_ULPGN_DRIVER_H

#include "r_bsp_config.h"
#include "aws_wifi.h"

#define DEBUGLOG  0
#define ULPGN_PORT_DEBUG 0

/* Configuration */
#define ULPGN_USE_UART_NUM (2)


#define ULPGN_RETURN_TEXT_OK             "OK\r\n"
#define ULPGN_RETURN_TEXT_ERROR          "ERROR\r\n"

#define ULPGN_RETURN_TEXT_OK          "OK\r\n"
#define ULPGN_RETURN_TEXT_CONNECT     "CONNECT\r\n"
#define ULPGN_RETURN_TEXT_RING        "RING\r\n"
#define ULPGN_RETURN_TEXT_NO_CARRIER  "NO_CARRIER\r\n"
#define ULPGN_RETURN_TEXT_ERROR       "ERROR\r\n"
#define ULPGN_RETURN_TEXT_NO_DIALTONE "NO_DIALTONE\r\n"
#define ULPGN_RETURN_TEXT_BUSY        "BUSY\r\n"
#define ULPGN_RETURN_TEXT_NO_ANSWER   "NO_ANSWER\r\n"

#define ULPGN_RETURN_NUMERIC_OK          "0\r"
#define ULPGN_RETURN_NUMERIC_CONNECT     "1\r"
#define ULPGN_RETURN_NUMERIC_RING        "2\r"
#define ULPGN_RETURN_NUMERIC_NO_CARRIER  "3\r"
#define ULPGN_RETURN_NUMERIC_ERROR       "4\r"
#define ULPGN_RETURN_NUMERIC_NO_DIALTONE "6\r"
#define ULPGN_RETURN_NUMERIC_BUSY        "7\r"
#define ULPGN_RETURN_NUMERIC_NO_ANSWER   "8\r"


#define ULPGN_RETURN_TEXT_CONNECT     "CONNECT\r\n"
#define ULPGN_RETURN_TEXT_RING        "RING\r\n"
#define ULPGN_RETURN_TEXT_NO_CARRIER  "NO_CARRIER\r\n"
#define ULPGN_RETURN_TEXT_ERROR       "ERROR\r\n"
#define ULPGN_RETURN_TEXT_NO_DIALTONE "NO_DIALTONE\r\n"
#define ULPGN_RETURN_TEXT_BUSY        "BUSY\r\n"
#define ULPGN_RETURN_TEXT_NO_ANSWER   "NO_ANSWER\r\n"

#define ULPGN_SOCKET_STATUS_TEXT_CLOSED          "CLOSED"
#define ULPGN_SOCKET_STATUS_TEXT_SOCKET          "SOCKET"
#define ULPGN_SOCKET_STATUS_TEXT_BOUND           "BOUND"
#define ULPGN_SOCKET_STATUS_TEXT_LISTEN          "LISTEN"
#define ULPGN_SOCKET_STATUS_TEXT_CONNECTED       "CONNECTED"

#if (ULPGN_USE_UART_NUM == 2)
#define ULPGN_UART_COMMAND_PORT 0
#define ULPGN_UART_DATA_PORT    1

#define CREATEABLE_SOCKETS (4)

#else
#define ULPGN_UART_COMMAND_PORT 0
#define ULPGN_UART_DATA_PORT    ULPGN_UART_COMMAND_PORT

#define CREATEABLE_SOCKETS (1)
#endif

#if BSP_CFG_BOARD_REVISION == 1
#define ULPGN_RESET_PORT_DDR    PORTF.PDR.BIT.B5     /*PMOD PinNo.8 */
#define ULPGN_RESET_PORT_DR     PORTF.PODR.BIT.B5    /*PMOD PinNo.8 */
#if (ULPGN_USE_UART_NUM == 2)
/* Please wire [RSK(TFT-9) - ULPGN(CN4-2)], and [RSK(TFT-11) - ULPGN(CN4-3)]*/
#define SCI_TX_BUSIZ_COMMAND    SCI_CFG_CH6_TX_BUFSIZ
#define SCI_TX_BUSIZ_DATA       SCI_CFG_CH7_TX_BUFSIZ
#define SCI_RX_BUSIZ_COMMAND    SCI_CFG_CH6_RX_BUFSIZ
#define SCI_RX_BUSIZ_DATA       SCI_CFG_CH7_RX_BUFSIZ
#elif (ULPGN_USE_UART_NUM == 1)
#define SCI_TX_BUSIZ_COMMAND    SCI_CFG_CH6_TX_BUFSIZ
#define SCI_TX_BUSIZ_DATA       SCI_TX_BUSIZ_COMMAND
#define SCI_RX_BUSIZ_COMMAND    SCI_CFG_CH6_RX_BUFSIZ
#define SCI_RX_BUSIZ_DATA       SCI_RX_BUSIZ_COMMAND
#endif

#if ULPGN_PORT_DEBUG == 1
#define DEBUG_PORT4_DDR PORT4.PDR.BIT.B7    /* TFT-46 */
#define DEBUG_PORT4_DR  PORT4.PODR.BIT.B7   /* TFT-46 */
#define DEBUG_PORT7_DDR PORT1.PDR.BIT.B1    /* TFT-14 */
#define DEBUG_PORT7_DR  PORT1.PODR.BIT.B1   /* TFT-14 */
#else
#endif

#elif BSP_CFG_BOARD_REVISION == 5
#define ULPGN_RESET_PORT_DDR PORTD.PDR.BIT.B0
#define ULPGN_RESET_PORT_DR PORTD.PODR.BIT.B0
#if (ULPGN_USE_UART_NUM == 2)
#define SCI_TX_BUSIZ_COMMAND SCI_CFG_CH10_TX_BUFSIZ
#define SCI_TX_BUSIZ_DATA SCI_CFG_CH10_TX_BUFSIZ
#define SCI_RX_BUSIZ_COMMAND  SCI_CFG_CH10_RX_BUFSIZ
#define SCI_RX_BUSIZ_DATA SCI_CFG_CH10_RX_BUFSIZ
#elif (ULPGN_USE_UART_NUM == 1)
#define SCI_TX_BUSIZ_COMMAND SCI_CFG_CH10_TX_BUFSIZ
#define SCI_TX_BUSIZ_DATA    SCI_TX_BUSIZ_COMMAND
#define SCI_RX_BUSIZ_COMMAND  SCI_CFG_CH10_RX_BUFSIZ
#define SCI_RX_BUSIZ_DATA     SCI_RX_BUSIZ_COMMAND
#endif
#endif



typedef enum
{
	ULPGN_RETURN_OK            = 0,
	ULPGN_RETURN_CONNECT,
	ULPGN_RETURN_RING,
	ULPGN_RETURN_NO_CARRIER,
	ULPGN_RETURN_ERROR,
	ULPGN_RETURN_DUMMY,
	ULPGN_RETURN_NO_DIALTONE,
	ULPGN_RETURN_BUSY,
	ULPGN_RETURN_NO_ANSWER,
	ULPGN_RETURN_ENUM_MAX,
}sx_ulpgn_return_code_t;

typedef enum
{
	ULPGN_RETURN_STRING_TEXT            = 0,
	ULPGN_RETURN_STRING_NUMERIC,
	ULPGN_RETURN_STRING_MAX,
}sx_ulpgn_return_string_t;

typedef enum
{
	ULPGN_SECURITY_OPEN            = 0,
	ULPGN_SECURITY_WPA,
	ULPGN_SECURITY_WPA2,
	ULPGN_SECURITY_WEP,
	ULPGN_SECURITY_UNDEFINED,
	ULPGN_SECURITY_MAX,
}sx_ulpgn_security_t;

typedef enum
{
	ULPGN_SOCKET_STATUS_CLOSED            = 0,
	ULPGN_SOCKET_STATUS_SOCKET,
	ULPGN_SOCKET_STATUS_BOUND,
	ULPGN_SOCKET_STATUS_LISTEN,
	ULPGN_SOCKET_STATUS_CONNECTED,
	ULPGN_SOCKET_STATUS_MAX,
}sx_ulpgn_socket_status_t;

int32_t sx_ulpgn_wifi_init(void);
int32_t sx_ulpgn_socket_init(void);
int32_t sx_ulpgn_wifi_get_macaddr(uint8_t *ptextstring);
int32_t sx_ulpgn_wifi_connect(uint8_t *pssid, uint32_t security, uint8_t *ppass);
int32_t sx_ulpgn_socket_create(uint8_t socket_no, uint32_t type,uint32_t ipversion);
int32_t sx_ulpgn_tcp_connect(uint8_t socket_no, uint32_t ipaddr, uint16_t port);
int32_t sx_ulpgn_tcp_send(uint8_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout);
int32_t sx_ulpgn_tcp_recv(uint8_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout);
int32_t sx_ulpgn_tcp_disconnect(uint8_t socket_no);
int32_t sx_ulpgn_dns_query(uint8_t *ptextstring, uint32_t *ulipaddr);
int32_t sx_ulpgn_serial_tcp_recv_timeout_set(uint8_t socket_no, TickType_t timeout_ms);
uint8_t sx_ulpgn_get_avail_socket();
int32_t sx_ulpgn_wifi_disconnect();
int32_t sx_ulpgn_get_ip(uint32_t *ulipaddr);
int32_t sx_ulpgn_wifi_scan(WIFIScanResult_t *results, uint8_t maxNetworks);

#endif /* #define SX_ULPGN_DRIVER_H */
