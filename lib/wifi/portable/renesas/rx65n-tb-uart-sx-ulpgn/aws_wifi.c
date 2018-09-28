/*
 * Amazon FreeRTOS Wi-Fi V1.0.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_wifi.c
 * @brief WiFi Interface.
 */
#include <stdio.h>
#include <string.h>
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* Socket and WiFi interface includes. */
#include "aws_wifi.h"

/* WiFi configuration includes. */
#include "aws_wifi_config.h"

/* WiFi configuration includes. */
#include "platform.h"
#include "r_sci_rx_if.h"
#include "sx_ulpgn_driver.h"

/**
 * @brief Wi-Fi initialization status.
 */
static BaseType_t xWIFIInitDone;
static BaseType_t xWIFIConnected;
static uint32_t prvConvertSecurityFromSilexAT( WIFISecurity_t xSecurity );

static uint32_t prvConvertSecurityFromSilexAT( WIFISecurity_t xSecurity )
{
    uint32_t xConvertedSecurityType = ULPGN_SECURITY_UNDEFINED;

    switch( xSecurity )
    {
        case eWiFiSecurityOpen:
            xConvertedSecurityType = ULPGN_SECURITY_OPEN;
            break;

        case eWiFiSecurityWEP:
            xConvertedSecurityType = ULPGN_SECURITY_WEP;
            break;

        case eWiFiSecurityWPA:
            xConvertedSecurityType = ULPGN_SECURITY_WPA;
            break;

        case eWiFiSecurityWPA2:
            xConvertedSecurityType = ULPGN_SECURITY_WPA2;
            break;

        case eWiFiSecurityNotSupported:
            xConvertedSecurityType = ULPGN_SECURITY_UNDEFINED;
            break;
    }

    return xConvertedSecurityType;
}

/*-----------------------------------------------------------*/

static StaticSemaphore_t xControlSemaphoreBuffer;
static SemaphoreHandle_t xControlSemaphore;
static bool xControlSemaphoreInit;

WIFIReturnCode_t WIFI_On( void )
{
	if (!xControlSemaphoreInit) {
		xControlSemaphore = xSemaphoreCreateMutexStatic(&xControlSemaphoreBuffer);
		xControlSemaphoreInit = true;
		xSemaphoreGive(xControlSemaphore);
	}
	xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

    WIFIReturnCode_t xRetVal = eWiFiFailure;

    //uart_string_printf("WiFiOn()\r\n");

    /* One time Wi-Fi initialization */
    if( xWIFIInitDone == pdFALSE )
    {
#if 0
    	/* This buffer is used to store the semaphore's data structure
         * and therefore must be static. */
        static StaticSemaphore_t xSemaphoreBuffer;

        /* Start with all the zero. */
        memset( &( xWiFiModule ), 0, sizeof( xWiFiModule ) );

        /* Create the semaphore used to serialize Wi-Fi module operations. */
        xWiFiModule.xSemaphoreHandle = xSemaphoreCreateMutexStatic( &( xSemaphoreBuffer ) );

        /* Initialize semaphore. */
        xSemaphoreGive( xWiFiModule.xSemaphoreHandle );
#endif
        /* Wi-Fi init done*/
        xWIFIInitDone = pdTRUE;
    }

    if(0 == sx_ulpgn_init())
    {
    	xRetVal = eWiFiSuccess;
    }

    xSemaphoreGive(xControlSemaphore);

	return xRetVal;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Off( void )
{
    xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

	sx_ulpgn_wifi_disconnect();

    xSemaphoreGive(xControlSemaphore);

	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

//eWiFiSecurityOpen = 0, /**< Open - No Security. */
//eWiFiSecurityWEP,      /**< WEP Security. */
//eWiFiSecurityWPA,      /**< WPA Security. */
//eWiFiSecurityWPA2,     /**< WPA2 Security. */

WIFIReturnCode_t WIFI_ConnectAP( const WIFINetworkParams_t * const pxNetworkParams )
{
    xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

	//uart_string_printf("WIFI_ConnectAP\r\n");
	int32_t ret;
	uint32_t convert_security;

	if (WIFI_IsConnected()) {
	    xSemaphoreGive(xControlSemaphore);
		return eWiFiSuccess;
	}


	convert_security = prvConvertSecurityFromSilexAT(pxNetworkParams->xSecurity);
	ret = sx_ulpgn_wifi_connect(
			pxNetworkParams->pcSSID,
			convert_security,
			pxNetworkParams->pcPassword);

	xSemaphoreGive(xControlSemaphore);

	if(ret != 0)
	{
		return eWiFiFailure;
	}

	xWIFIConnected = 1;
	return eWiFiSuccess;
}

BaseType_t WIFI_IsConnected( void ) {
	//uart_string_printf("WIFI_IsConnected");
	return xWIFIConnected;
}

/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Disconnect( void )
{
	uint8_t ret;

    xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

	//uart_string_printf("WIFI_Disconnect\r\n");
	xWIFIConnected = 0;
	// There appear to be modem bugs with repeat connection and disconnection, so we'll do a full restart instead
	ret = 0 == sx_ulpgn_wifi_disconnect() && 0 == sx_ulpgn_init();
    xSemaphoreGive(xControlSemaphore);

	if (ret == 1) {
		return eWiFiSuccess;
	} else {
		return eWiFiFailure;
	}
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Reset( void )
{
    return WIFI_Disconnect();
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Scan( WIFIScanResult_t * pxBuffer,
                            uint8_t ucNumNetworks )
{
	int32_t ret;

	if (pxBuffer == NULL || ucNumNetworks < 1) {
		return eWiFiFailure;
	}

	xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

    ret = sx_ulpgn_wifi_scan(pxBuffer, ucNumNetworks);
    xSemaphoreGive(xControlSemaphore);
    if (ret == 0) {
    	return eWiFiSuccess;
    } else {
    	return eWiFiFailure;
    }
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetMode( WIFIDeviceMode_t xDeviceMode )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMode( WIFIDeviceMode_t * pxDeviceMode )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkAdd( const WIFINetworkProfile_t * const pxNetworkProfile,
                                  uint16_t * pusIndex )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkGet( WIFINetworkProfile_t * pxNetworkProfile,
                                  uint16_t usIndex )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkDelete( uint16_t usIndex )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Ping( uint8_t * pucIPAddr,
                            uint16_t usCount,
                            uint32_t ulIntervalMS )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetIP( uint8_t * pucIPAddr )
{
	int32_t ret;
	if (pucIPAddr == NULL) {
		return eWiFiFailure;
	}

	xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

    ret = sx_ulpgn_get_ip(pucIPAddr);
	xSemaphoreGive(xControlSemaphore);

    if (ret == 0) {
    	return eWiFiSuccess;
    } else {
    	return eWiFiFailure;
    }
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMAC( uint8_t * pucMac )
{
	uint32_t ret;

	if (pucMac == NULL)
		return eWiFiFailure;

	xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

	ret = sx_ulpgn_wifi_get_macaddr(pucMac);

	xSemaphoreGive(xControlSemaphore);

	if (ret == 0) {
		return eWiFiSuccess;
	} else {
		return eWiFiFailure;
	}
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetHostIP( char * pcHost,
                                 uint8_t * pucIPAddr )
{
	int32_t ret;

	if (pucIPAddr == NULL) {
		return eWiFiFailure;
	}
	xSemaphoreTake(xControlSemaphore, portMAX_DELAY);

	ret = sx_ulpgn_dns_query(pcHost, pucIPAddr);

	xSemaphoreGive(xControlSemaphore);

	if (ret == 0) {
		return eWiFiSuccess;
	} else {
		return eWiFiFailure;
	}
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StartAP( void )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StopAP( void )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_ConfigureAP( const WIFINetworkParams_t * const pxNetworkParams )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetPMMode( WIFIPMMode_t xPMModeType,
                                 const void * pvOptionValue )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetPMMode( WIFIPMMode_t * pxPMModeType,
                                 void * pvOptionValue )
                                 {
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/



