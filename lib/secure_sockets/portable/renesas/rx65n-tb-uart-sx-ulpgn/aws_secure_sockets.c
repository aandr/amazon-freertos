/*
 * Amazon FreeRTOS Secure Socket for FreeRTOS+TCP V1.1.2
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
#include "FreeRTOSIPConfig.h"
#include "list.h"
#include "string.h"
// #include "FreeRTOS_IP.h"
//#include "FreeRTOS_Sockets.h"
#include "aws_secure_sockets.h"
#include "sx_ulpgn_driver.h"
#include "aws_tls.h"
#include "task.h"
#include "aws_pkcs11.h"

struct freertos_sockaddr
{
	/* _HT_ On 32- and 64-bit architectures, the addition of the two uint8_t
	fields doesn't make the structure bigger, due to alignment.
	The fields are inserted as a preparation for IPv6. */

	/* sin_len and sin_family not used in the IPv4-only release. */
	uint8_t sin_len;		/* length of this structure. */
	uint8_t sin_family;		/* FREERTOS_AF_INET. */
	uint16_t sin_port;
	uint32_t sin_addr;
};

/* Internal context structure. */
typedef struct SSOCKETContext
{
    Socket_t xSocket;
    char * pcDestination;
    void * pvTLSContext;
    BaseType_t xRequireTLS;
    BaseType_t xSendFlags;
    BaseType_t xRecvFlags;
    uint32_t ulSendTimeout;
    uint32_t ulRecvTimeout;
    char * pcServerCertificate;
    uint32_t ulServerCertificateLength;
    char ** ppcAlpnProtocols;
    uint32_t ulAlpnProtocolsCount;
    BaseType_t xConnectAttempted;
} SSOCKETContext_t, * SSOCKETContextPtr_t;

/*
 * Helper routines.
 */

/**
 * @brief Maximum number of sockets.
 *
 * 16 total sockets
 */
#define MAX_NUM_SSOCKETS    (1)

/**
 * @brief Number of secure sockets allocated.
 *
 * Keep a count of the number of open sockets.
 */
static uint8_t ssockets_num_allocated = 0;


/*
 * @brief Network send callback.
 */
static BaseType_t prvNetworkSend( void * pvContext,
                                  const unsigned char * pucData,
                                  size_t xDataLength )
{
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) pvContext; /*lint !e9087 cast used for portability. */

    return sx_ulpgn_tcp_send(pucData, xDataLength, pxContext->ulSendTimeout);
}
/*-----------------------------------------------------------*/

/*
 * @brief Network receive callback.
*/
static BaseType_t prvNetworkRecv( void * pvContext,
                                 unsigned char * pucReceiveBuffer,
                                 size_t xReceiveLength )
{
	BaseType_t receive_byte;
   SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) pvContext; /*lint !e9087 cast used for portability. */


   receive_byte = sx_ulpgn_tcp_recv(pucReceiveBuffer, xReceiveLength, pxContext->ulRecvTimeout);

   if(xReceiveLength == 64)
   {
   	nop();
   }
   return receive_byte;
}
/*-----------------------------------------------------------*/

/*
 * Interface routines.
 */

int32_t SOCKETS_Close( Socket_t xSocket )
{
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */
    uint32_t ulProtocol;
    int32_t lReturn;

    if( ( xSocket != SOCKETS_INVALID_SOCKET ) && ( NULL != pxContext ) )
    {
        /* Clean-up destination string. */
        if( NULL != pxContext->pcDestination )
        {
            vPortFree( pxContext->pcDestination );
        }

        /* Clean-up server certificate. */
        if( NULL != pxContext->pcServerCertificate )
        {
            vPortFree( pxContext->pcServerCertificate );
        }

        /* Clean-up application protocol array. */
        if( NULL != pxContext->ppcAlpnProtocols )
        {
            for( ulProtocol = 0;
                 ulProtocol < pxContext->ulAlpnProtocolsCount;
                 ulProtocol++ )
            {
                if( NULL != pxContext->ppcAlpnProtocols[ ulProtocol ] )
                {
                    vPortFree( pxContext->ppcAlpnProtocols[ ulProtocol ] );
                }
            }

            vPortFree( pxContext->ppcAlpnProtocols );
        }

        /* Clean-up TLS context. */
        if( pdTRUE == pxContext->xRequireTLS )
        {
            TLS_Cleanup( pxContext->pvTLSContext );
        }

        /* Close the underlying socket handle. */
        sx_ulpgn_tcp_disconnect();

        /* Free the context. */
        vPortFree( pxContext );
        lReturn = SOCKETS_ERROR_NONE;

        /* Renesas */
    	if(ssockets_num_allocated > 0)
    	{
    		ssockets_num_allocated--;
    	}
    	else
    	{
    		ssockets_num_allocated = 0;
    	}

    }
    else
    {
        lReturn = SOCKETS_EINVAL;
    }

    return lReturn;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Connect( Socket_t xSocket,
                         SocketsSockaddr_t * pxAddress,
                         Socklen_t xAddressLength )
{
    int32_t lStatus = SOCKETS_ERROR_NONE;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */
    TLSParams_t xTLSParams = { 0 };
    struct freertos_sockaddr xTempAddress = { 0 };

    if( ( pxContext != SOCKETS_INVALID_SOCKET ) && ( pxAddress != NULL ) && ((uint8_t)(pxAddress->ulAddress) != 0 ))
    {
        /* A connection was attempted. If this function fails, then the socket is invalid and the user
         * must call SOCKETS_Close(), on this socket, and SOCKETS_Socket() to get a new socket. */
        pxContext->xConnectAttempted = pdTRUE;

        /* Connect the wrapped socket. */
        xTempAddress.sin_addr = pxAddress->ulAddress;
        xTempAddress.sin_family = pxAddress->ucSocketDomain;
        xTempAddress.sin_len = ( uint8_t ) sizeof( xTempAddress );
        xTempAddress.sin_port = pxAddress->usPort;

        if (0 != sx_ulpgn_tcp_connect(SOCKETS_ntohl(pxAddress->ulAddress), SOCKETS_ntohs(pxAddress->usPort))) {
        	lStatus = SOCKETS_SOCKET_ERROR;
        }

        /* Negotiate TLS if requested. */
        if( ( SOCKETS_ERROR_NONE == lStatus ) && ( pdTRUE == pxContext->xRequireTLS ) )
        {
            xTLSParams.ulSize = sizeof( xTLSParams );
            xTLSParams.pcDestination = pxContext->pcDestination;
            xTLSParams.pcServerCertificate = pxContext->pcServerCertificate;
            xTLSParams.ulServerCertificateLength = pxContext->ulServerCertificateLength;
            //xTLSParams.ppcAlpnProtocols = ( const char ** ) pxContext->ppcAlpnProtocols;
            //xTLSParams.ulAlpnProtocolsCount = pxContext->ulAlpnProtocolsCount;
            xTLSParams.pvCallerContext = pxContext;
            xTLSParams.pxNetworkRecv = prvNetworkRecv;
            xTLSParams.pxNetworkSend = prvNetworkSend;
            lStatus = TLS_Init( &pxContext->pvTLSContext, &xTLSParams );

            if( SOCKETS_ERROR_NONE == lStatus )
            {
                lStatus = TLS_Connect( pxContext->pvTLSContext );
                if( lStatus < 0 )
                {
                	lStatus = SOCKETS_TLS_HANDSHAKE_ERROR;
                }
            }
        }
    }
    else
    {
        lStatus = SOCKETS_SOCKET_ERROR;
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

uint32_t SOCKETS_GetHostByName( const char * pcHostName )
{
	uint32_t ulAddr = 0;
	uint32_t ret;

	ret = sx_ulpgn_dns_query(pcHostName, &ulAddr);
	if(0 == ret)
	{
		ulAddr = SOCKETS_htonl( ulAddr );
	}
	return ulAddr;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Recv( Socket_t xSocket,
                      void * pvBuffer,
                      size_t xBufferLength,
                      uint32_t ulFlags )
{
    int32_t lStatus = SOCKETS_SOCKET_ERROR;
    int32_t recvStatus;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */

    if( ( xSocket != SOCKETS_INVALID_SOCKET ) &&
        ( pvBuffer != NULL ) )
    {
        pxContext->xRecvFlags = ( BaseType_t ) ulFlags;

        if( pdTRUE == pxContext->xRequireTLS )
        {
            /* Receive through TLS pipe, if negotiated. */
            lStatus = TLS_Recv( pxContext->pvTLSContext, pvBuffer, xBufferLength );
        }
        else
        {
            /* Receive unencrypted. */
            lStatus = prvNetworkRecv( pxContext, pvBuffer, xBufferLength );
        }
    }
    else
    {
        lStatus = SOCKETS_EINVAL;
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Send( Socket_t xSocket,
                      const void * pvBuffer,
                      size_t xDataLength,
                      uint32_t ulFlags )
{
    int32_t lStatus = SOCKETS_SOCKET_ERROR;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */

    if( ( xSocket != SOCKETS_INVALID_SOCKET ) &&
        ( pvBuffer != NULL ) )
    {
        pxContext->xSendFlags = ( BaseType_t ) ulFlags;

        if( pdTRUE == pxContext->xRequireTLS )
        {
            /* Send through TLS pipe, if negotiated. */
            lStatus = TLS_Send( pxContext->pvTLSContext, pvBuffer, xDataLength );
        }
        else
        {
            /* Send unencrypted. */
            lStatus = prvNetworkSend( pxContext, pvBuffer, xDataLength );
        }
    }
    else
    {
        lStatus = SOCKETS_EINVAL;
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_SetSockOpt( Socket_t xSocket,
                            int32_t lLevel,
                            int32_t lOptionName,
                            const void * pvOptionValue,
                            size_t xOptionLength )
{
    int32_t lStatus = SOCKETS_ERROR_NONE;
    TickType_t xTimeout;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */
    char ** ppcAlpnIn = ( char ** ) pvOptionValue;
    size_t xLength = 0;
    uint32_t ulProtocol;

    if( ( xSocket != SOCKETS_INVALID_SOCKET ) && ( xSocket != NULL ) )
    {
        switch( lOptionName )
        {
            case SOCKETS_SO_SERVER_NAME_INDICATION:

                /* Non-NULL destination string indicates that SNI extension should
                 * be used during TLS negotiation. */
                if( NULL == ( pxContext->pcDestination =
                                       ( char * ) pvPortMalloc( 1U + xOptionLength ) ) )
                {
                    lStatus = SOCKETS_ENOMEM;
                }
                else
                {
                    memcpy( pxContext->pcDestination, pvOptionValue, xOptionLength );
                    pxContext->pcDestination[ xOptionLength ] = '\0';
                }

                break;

            case SOCKETS_SO_TRUSTED_SERVER_CERTIFICATE:

                /* Non-NULL server certificate field indicates that the default trust
                 * list should not be used. */
                if( NULL == ( pxContext->pcServerCertificate =
                                       ( char * ) pvPortMalloc( xOptionLength ) ) )
                {
                    lStatus = SOCKETS_ENOMEM;
                }
                else
                {
                    memcpy( pxContext->pcServerCertificate, pvOptionValue, xOptionLength );
                    pxContext->ulServerCertificateLength = xOptionLength;
                }

                break;

            case SOCKETS_SO_REQUIRE_TLS:

                pxContext->xRequireTLS = pdTRUE;

                break;

            case SOCKETS_SO_ALPN_PROTOCOLS:

                /* Do not set the ALPN option if the socket is already connected. */
                if( pxContext->xConnectAttempted == pdTRUE )
                {
                    lStatus = SOCKETS_EISCONN;
                    break;
                }

                /* Allocate a sufficiently long array of pointers. */
                pxContext->ulAlpnProtocolsCount = 1 + xOptionLength;

                if( NULL == ( pxContext->ppcAlpnProtocols =
                                  ( char ** ) pvPortMalloc( pxContext->ulAlpnProtocolsCount * sizeof( char * ) ) ) )
                {
                    lStatus = SOCKETS_ENOMEM;
                }
                else
                {
                    pxContext->ppcAlpnProtocols[
                        pxContext->ulAlpnProtocolsCount - 1 ] = NULL;
                }

                /* Copy each protocol string. */
                for( ulProtocol = 0;
                     ( ulProtocol < pxContext->ulAlpnProtocolsCount - 1 ) &&
                     ( pdFREERTOS_ERRNO_NONE == lStatus );
                     ulProtocol++ )
                {
                    xLength = strlen( ppcAlpnIn[ ulProtocol ] );

                    if( NULL == ( pxContext->ppcAlpnProtocols[ ulProtocol ] =
                                      ( char * ) pvPortMalloc( 1 + xLength ) ) )
                    {
                        lStatus = SOCKETS_ENOMEM;
                    }
                    else
                    {
                        memcpy( pxContext->ppcAlpnProtocols[ ulProtocol ],
                                ppcAlpnIn[ ulProtocol ],
                                xLength );
                        pxContext->ppcAlpnProtocols[ ulProtocol ][ xLength ] = '\0';
                    }
                }

                break;

            case SOCKETS_SO_NONBLOCK:
            	pxContext->ulSendTimeout = 1;
            	pxContext->ulRecvTimeout = 2;

                break;

            case SOCKETS_SO_RCVTIMEO:
                  /* Comply with Berkeley standard - a 0 timeout is wait forever. */
                  xTimeout = *( ( const TickType_t * ) pvOptionValue ); /*lint !e9087 pvOptionValue passed should be of TickType_t */

                  if( xTimeout == 0U )
                  {
                      xTimeout = portMAX_DELAY;
                  }
              	pxContext->ulRecvTimeout = xTimeout;
      //            sx_ulpgn_serial_tcp_timeout_set(xTimeout);
                  break;
              case SOCKETS_SO_SNDTIMEO:
                  /* Comply with Berkeley standard - a 0 timeout is wait forever. */
                  xTimeout = *( ( const TickType_t * ) pvOptionValue ); /*lint !e9087 pvOptionValue passed should be of TickType_t */

                  if( xTimeout == 0U )
                  {
                      xTimeout = portMAX_DELAY;
                  }
              	pxContext->ulSendTimeout = xTimeout;
      //            sx_ulpgn_serial_tcp_timeout_set(xTimeout);
                  break;

            case SOCKETS_SO_WAKEUP_CALLBACK:
            	if (0 == sx_ulpgn_set_wakeup_callback((void *)pvOptionValue, pxContext)) {
            		lStatus = pdFREERTOS_ERRNO_NONE;
            	}
            	break;
            default:
            	// TODO
            	/*
                lStatus = FreeRTOS_setsockopt( pxContext->xSocket,
                                               lLevel,
                                               lOptionName,
                                               pvOptionValue,
                                               xOptionLength );
                */
            	lStatus = SOCKETS_EINVAL;
                break;
        }
    }
    else
    {
        lStatus = SOCKETS_EINVAL;
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Shutdown( Socket_t xSocket,
                          uint32_t ulHow )
{
	if (*(uint32_t *)xSocket != 0 || (ulHow != 0 && ulHow != 1 && ulHow != 2)) {
		return SOCKETS_EINVAL;
	}
	return sx_ulpgn_tcp_disconnect();
}
/*-----------------------------------------------------------*/

/**
 * @brief Creates a TCP socket.
 *
 * This call allocates memory and claims a socket resource.
 *
 * @sa SOCKETS_Close()
 *
 * @param[in] lDomain Must be set to SOCKETS_AF_INET. See @ref SocketDomains.
 * @param[in] lType Set to SOCKETS_SOCK_STREAM to create a TCP socket.
 * No other value is valid.  See @ref SocketTypes.
 * @param[in] lProtocol Set to SOCKETS_IPPROTO_TCP to create a TCP socket.
 * No other value is valid. See @ref Protocols.
 *
 * @return
 * * If a socket is created successfully, then the socket handle is
 * returned
 * * @ref SOCKETS_INVALID_SOCKET is returned if an error occurred.
 */
/* Renesas */

Socket_t SOCKETS_Socket( int32_t lDomain,
                         int32_t lType,
                         int32_t lProtocol )
{
    int32_t lStatus = SOCKETS_ERROR_NONE;
    SSOCKETContextPtr_t pxContext = NULL;

    /* Ensure that only supported values are supplied. */
    configASSERT( lDomain == SOCKETS_AF_INET );
    configASSERT( lType == SOCKETS_SOCK_STREAM );
    configASSERT( lProtocol == SOCKETS_IPPROTO_TCP );

    if (WIFI_IsConnected() == 0) {
    	return SOCKETS_INVALID_SOCKET;
    }

	if(ssockets_num_allocated >= MAX_NUM_SSOCKETS)
	{
        lStatus = SOCKETS_SOCKET_ERROR;
	}

	 if( SOCKETS_ERROR_NONE == lStatus )
	 {
		/* Allocate the internal context structure. */
		if( NULL == ( pxContext = pvPortMalloc( sizeof( SSOCKETContext_t ) ) ) )
		{
			lStatus = SOCKETS_ENOMEM;
		}
	}

    if( SOCKETS_ERROR_NONE == lStatus )
    {
        memset( pxContext, 0, sizeof( SSOCKETContext_t ) );
        pxContext->xSocket = 0;

        if (-1 == sx_ulpgn_socket_create(0, 4)) {
        	lStatus = SOCKETS_SOCKET_ERROR;
        }
    }

    if( SOCKETS_ERROR_NONE != lStatus )
    {
        vPortFree( pxContext );
        return SOCKETS_INVALID_SOCKET;
    }

    else
    {
    	if(ssockets_num_allocated < MAX_NUM_SSOCKETS)
    	{
    		ssockets_num_allocated++;
    	}
    	return pxContext;
    }
}
/*-----------------------------------------------------------*/

BaseType_t SOCKETS_Init( void )
{
    /* Empty initialization for this port. */
    return pdPASS;
}
/*-----------------------------------------------------------*/

static CK_SESSION_HANDLE xPkcs11Session = 0;
static CK_FUNCTION_LIST_PTR pxPkcs11FunctionList = NULL;

/*-----------------------------------------------------------*/

static CK_RV prvSocketsGetCryptoSession( CK_SESSION_HANDLE *pxSession,
                                         CK_FUNCTION_LIST_PTR_PTR ppxFunctionList )
{
    CK_RV xResult = 0;
    CK_C_GetFunctionList pxCkGetFunctionList = NULL;
    static CK_SESSION_HANDLE xPkcs11Session = 0;
    static CK_FUNCTION_LIST_PTR pxPkcs11FunctionList = NULL;
    CK_ULONG ulCount = 1;
    CK_SLOT_ID xSlotId = 0;

    portENTER_CRITICAL( );

    if( 0 == xPkcs11Session )
    {
        /* One-time initialization. */

        /* Ensure that the PKCS#11 module is initialized. */
        if( 0 == xResult )
        {
            pxCkGetFunctionList = C_GetFunctionList;
            xResult = pxCkGetFunctionList( &pxPkcs11FunctionList );
        }

        if( 0 == xResult )
        {
            xResult = pxPkcs11FunctionList->C_Initialize( NULL );
        }

        /* Get the default slot ID. */
        if( 0 == xResult )
        {
            xResult = pxPkcs11FunctionList->C_GetSlotList(
                CK_TRUE,
                &xSlotId,
                &ulCount );
        }

        /* Start a session with the PKCS#11 module. */
        if( 0 == xResult )
        {
            xResult = pxPkcs11FunctionList->C_OpenSession(
                xSlotId,
                CKF_SERIAL_SESSION,
                NULL,
                NULL,
                &xPkcs11Session );
        }
    }

    portEXIT_CRITICAL( );

    /* Output the shared function pointers and session handle. */
    *ppxFunctionList = pxPkcs11FunctionList;
    *pxSession = xPkcs11Session;

    return xResult;
}
/*-----------------------------------------------------------*/


uint32_t ulRand( void )
{
    CK_RV xResult = 0;
    CK_C_GetFunctionList pxCkGetFunctionList = NULL;
    CK_ULONG ulCount = 1;
    uint32_t ulRandomValue = 0;
    CK_SLOT_ID xSlotId = 0;

    portENTER_CRITICAL();

    if( 0 == xPkcs11Session )
    {
        /* One-time initialization. */

        /* Ensure that the PKCS#11 module is initialized. */
        if( 0 == xResult )
        {
            pxCkGetFunctionList = C_GetFunctionList;
            xResult = pxCkGetFunctionList( &pxPkcs11FunctionList );
        }

        if( 0 == xResult )
        {
            xResult = pxPkcs11FunctionList->C_Initialize( NULL );
        }

        /* Get the default slot ID. */
        if( 0 == xResult )
        {
            xResult = pxPkcs11FunctionList->C_GetSlotList(
                CK_TRUE,
                &xSlotId,
                &ulCount );
        }

        /* Start a session with the PKCS#11 module. */
        if( 0 == xResult )
        {
            xResult = pxPkcs11FunctionList->C_OpenSession(
                xSlotId,
                CKF_SERIAL_SESSION,
                NULL,
                NULL,
                &xPkcs11Session );
        }
    }

    if( 0 == xResult )
    {
        /* Request a sequence of cryptographically random byte values using
         * PKCS#11. */
        xResult = pxPkcs11FunctionList->C_GenerateRandom( xPkcs11Session,
                                                          ( CK_BYTE_PTR ) &ulRandomValue,
                                                          sizeof( ulRandomValue ) );
    }

    portEXIT_CRITICAL();

    /* Check if any of the API calls failed. */
    if( 0 != xResult )
    {
        ulRandomValue = 0;
    }

    return ulRandomValue;
}
/*-----------------------------------------------------------*/
