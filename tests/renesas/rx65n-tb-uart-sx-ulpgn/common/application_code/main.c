/*
 * Amazon FreeRTOS V1.1.1
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

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
// #include "FreeRTOS_IP.h" /* FIX ME: Delete if you are not using the FreeRTOS-Plus-TCP library. */

/* Test includes */
#include "aws_test_runner.h"

/* AWS library includes. */
#include "aws_system_init.h"
#include "aws_logging_task.h"
#include "aws_wifi.h"
#include "aws_clientcredential.h"

/* Logging Task Defines. */
#define mainLOGGING_MESSAGE_QUEUE_LENGTH    ( 15 )
#define mainLOGGING_TASK_STACK_SIZE         ( configMINIMAL_STACK_SIZE * 8 )

/* Unit test defines. */
#define mainTEST_RUNNER_TASK_STACK_SIZE     ( configMINIMAL_STACK_SIZE * 16 )

/* The task delay for allowing the lower priority logging task to print out Wi-Fi 
 * failure status before blocking indefinitely. */
#define mainLOGGING_WIFI_STATUS_DELAY       pdMS_TO_TICKS( 1000 )

/* The name of the devices for xApplicationDNSQueryHook. */
#define mainDEVICE_NICK_NAME				"vendor_demo" /* FIX ME.*/

/* Static arrays for FreeRTOS-Plus-TCP stack initialization for Ethernet network 
 * connections are declared below. If you are using an Ethernet connection on your MCU 
 * device it is recommended to use the FreeRTOS+TCP stack. The default values are 
 * defined in FreeRTOSConfig.h. */

/* Default MAC address configuration.  The application creates a virtual network
 * connection that uses this MAC address by accessing the raw Ethernet data
 * to and from a real network connection on the host PC.  See the
 * configNETWORK_INTERFACE_TO_USE definition for information on how to configure
 * the real network connection to use. */
const uint8_t ucMACAddress[ 6 ] =
{
    configMAC_ADDR0,
    configMAC_ADDR1,
    configMAC_ADDR2,
    configMAC_ADDR3,
    configMAC_ADDR4,
    configMAC_ADDR5
};

/* The default IP and MAC address used by the application.  The address configuration
 * defined here will be used if ipconfigUSE_DHCP is 0, or if ipconfigUSE_DHCP is
 * 1 but a DHCP server could not be contacted.  See the online documentation for
 * more information. */
static const uint8_t ucIPAddress[ 4 ] =
{
    configIP_ADDR0,
    configIP_ADDR1,
    configIP_ADDR2,
    configIP_ADDR3
};
static const uint8_t ucNetMask[ 4 ] =
{
    configNET_MASK0,
    configNET_MASK1,
    configNET_MASK2,
    configNET_MASK3
};
static const uint8_t ucGatewayAddress[ 4 ] =
{
    configGATEWAY_ADDR0,
    configGATEWAY_ADDR1,
    configGATEWAY_ADDR2,
    configGATEWAY_ADDR3
};
static const uint8_t ucDNSServerAddress[ 4 ] =
{
    configDNS_SERVER_ADDR0,
    configDNS_SERVER_ADDR1,
    configDNS_SERVER_ADDR2,
    configDNS_SERVER_ADDR3
};

/**
 * @brief Application task startup hook for applications using Wi-Fi. If you are not 
 * using Wi-Fi, then start network dependent applications in the vApplicationIPNetorkEventHook
 * function. If you are not using Wi-Fi, this hook can be disabled by setting 
 * configUSE_DAEMON_TASK_STARTUP_HOOK to 0.
 */
void vApplicationDaemonTaskStartupHook( void );

/**
 * @brief Application IP network event hook called by the FreeRTOS+TCP stack for
 * applications using Ethernet. If you are not using Ethernet and the FreeRTOS+TCP stack,
 * start network dependent applications in vApplicationDaemonTaskStartupHook after the
 * network status is up.
 */
//void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent );

/**
 * @brief Connects to Wi-Fi.
 */
static void prvWifiConnect( void );

/**
 * @brief Initializes the board.
 */
static void prvMiscInitialization( void );

/*-----------------------------------------------------------*/

/**
 * @brief Application runtime entry point.
 */
int main( void )
{
    while(1)
    {
    	vTaskDelay(10000);
    }

    return 0;
}
/*-----------------------------------------------------------*/

static void prvMiscInitialization( void )
{
    /* FIX ME: Perform any hardware initializations, that don't require the RTOS to be 
     * running, here.
     */
	uart_config();
	configPRINT_STRING(("Hello World.\r\n"));

}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
    /* FIX ME: Perform any hardware initialization, that require the RTOS to be
     * running, here. */
	prvMiscInitialization();
    

    /* FIX ME: If your MCU is using Wi-Fi, delete surrounding compiler directives to 
     * enable the unit tests and after MQTT, Bufferpool, and Secure Sockets libraries 
     * have been imported into the project. If you are not using Wi-Fi, see the 
     * vApplicationIPNetworkEventHook function. */
        if( SYSTEM_Init() == pdPASS )
        {
            /* Connect to the Wi-Fi before running the tests. */
            prvWifiConnect();

            /* Create the task to run unit tests. */
            xTaskCreate( TEST_RUNNER_RunTests_task,
                         "RunTests_task",
                         mainTEST_RUNNER_TASK_STACK_SIZE,
                         NULL,
                         tskIDLE_PRIORITY,
                         NULL );
        }
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void prvWifiConnect( void )
{
        WIFINetworkParams_t xNetworkParams;
        WIFIReturnCode_t xWifiStatus;
        uint8_t ucTempIp[4] = { 0 };

        xWifiStatus = WIFI_On();

        if( xWifiStatus == eWiFiSuccess )
        {
            configPRINTF( ( "Wi-Fi module initialized. Connecting to AP...\r\n" ) );
        }
        else
        {
            configPRINTF( ( "Wi-Fi module failed to initialize.\r\n" ) );

            /* Delay to allow the lower priority logging task to print the above status. 
             * The while loop below will block the above printing. */
            vTaskDelay( mainLOGGING_WIFI_STATUS_DELAY );

            while( 1 )
            {
            }
        }

        /* Setup parameters. */
        xNetworkParams.pcSSID = clientcredentialWIFI_SSID;
        xNetworkParams.ucSSIDLength = sizeof( clientcredentialWIFI_SSID );
        xNetworkParams.pcPassword = clientcredentialWIFI_PASSWORD;
        xNetworkParams.ucPasswordLength = sizeof( clientcredentialWIFI_PASSWORD );
        xNetworkParams.xSecurity = clientcredentialWIFI_SECURITY;
        xNetworkParams.cChannel = 0;

        xWifiStatus = WIFI_ConnectAP( &( xNetworkParams ) );

        if( xWifiStatus == eWiFiSuccess )
        {
            configPRINTF( ( "Wi-Fi Connected to AP. Creating tasks which use network...\r\n" ) );
            
            xWifiStatus = WIFI_GetIP( ucTempIp );
            if ( eWiFiSuccess == xWifiStatus ) 
            {
                configPRINTF( ( "IP Address acquired %d.%d.%d.%d\r\n",
                                ucTempIp[ 0 ], ucTempIp[ 1 ], ucTempIp[ 2 ], ucTempIp[ 3 ] ) );
            }
        }
        else
        {
            configPRINTF( ( "Wi-Fi failed to connect to AP.\r\n" ) );

            /* Delay to allow the lower priority logging task to print the above status. 
             * The while loop below will block the above printing. */
            vTaskDelay( mainLOGGING_WIFI_STATUS_DELAY );

            while( 1 )
            {
            }
        }
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/


#if 0
/**
 * @brief Loop forever if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    portDISABLE_INTERRUPTS();

    /* Loop forever */
    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/
#endif

/*-----------------------------------------------------------*/

/**
* @brief User defined application hook to process names returned by the DNS server.
*/
#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 )
    BaseType_t xApplicationDNSQueryHook( const char * pcName )
    {
        /* FIX ME. If necessary, update to applicable DNS name lookup actions. */

        BaseType_t xReturn;

        /* Determine if a name lookup is for this node.  Two names are given
         * to this node: that returned by pcApplicationHostnameHook() and that set
         * by mainDEVICE_NICK_NAME. */
        if( strcmp( pcName, pcApplicationHostnameHook() ) == 0 )
        {
            xReturn = pdPASS;
        }
        else if( strcmp( pcName, mainDEVICE_NICK_NAME ) == 0 )
        {
            xReturn = pdPASS;
        }
        else
        {
            xReturn = pdFAIL;
        }

        return xReturn;
    }
	
#endif /* if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) */
/*-----------------------------------------------------------*/

/**
 * @brief User defined assertion call. This function is plugged into configASSERT.
 * See FreeRTOSConfig.h to define configASSERT to something different.
 */
void vAssertCalled2(const char * pcFile,
	uint32_t ulLine)
{
    /* FIX ME. If necessary, update to applicable assertion routine actions. */

	const uint32_t ulLongSleep = 1000UL;
	volatile uint32_t ulBlockVariable = 0UL;
	volatile char * pcFileName = (volatile char *)pcFile;
	volatile uint32_t ulLineNumber = ulLine;

	(void)pcFileName;
	(void)ulLineNumber;

	printf("vAssertCalled %s, %ld\n", pcFile, (long)ulLine);

	/* Setting ulBlockVariable to a non-zero value in the debugger will allow
	* this function to be exited. */

}
/*-----------------------------------------------------------*/

/**
 * @brief User defined application hook need by the FreeRTOS-Plus-TCP library.
 */
#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
    const char * pcApplicationHostnameHook(void)
    {
        /* FIX ME: If necessary, update to applicable registration name. */

        /* This function will be called during the DHCP: the machine will be registered 
         * with an IP address plus this name. */
        return clientcredentialIOT_THING_NAME;
    }

#endif
