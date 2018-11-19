/*
 * Amazon FreeRTOS
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
#ifndef AWS_TEST_TCP_H
#define AWS_TEST_TCP_H

/* Non-Encrypted Echo Server.
 * Update tcptestECHO_SERVER_ADDR# and
 * tcptestECHO_PORT with IP address
 * and port of unencrypted TCP echo server. */
#define tcptestECHO_SERVER_ADDR0         54
#define tcptestECHO_SERVER_ADDR1         67
#define tcptestECHO_SERVER_ADDR2         22
#define tcptestECHO_SERVER_ADDR3         93
#define tcptestECHO_PORT                 ( 9001 )

/* Encrypted Echo Server.
 * If tcptestSECURE_SERVER is set to 1, the following must be updated:
 * 1. aws_clientcredential.h to use a valid AWS endpoint.
 * 2. aws_clientcredential_keys.h with corresponding AWS keys.
 * 3. tcptestECHO_SERVER_TLS_ADDR0-3 with the IP address of an
 * echo server using TLS.
 * 4. tcptestECHO_PORT_TLS, with the port number of the echo server
 * using TLS.
 * 5. tcptestECHO_HOST_ROOT_CA with the trusted root certificate of the
 * echo server using TLS. */
#define tcptestSECURE_SERVER             1

#define tcptestECHO_SERVER_TLS_ADDR0     54
#define tcptestECHO_SERVER_TLS_ADDR1     67
#define tcptestECHO_SERVER_TLS_ADDR2     22
#define tcptestECHO_SERVER_TLS_ADDR3     93
#define tcptestECHO_PORT_TLS             ( 9000 )


//#define tcptestECHO_SERVER_TLS_ADDR0     192
//#define tcptestECHO_SERVER_TLS_ADDR1     168
//#define tcptestECHO_SERVER_TLS_ADDR2     0
//#define tcptestECHO_SERVER_TLS_ADDR3     20
//#define tcptestECHO_PORT_TLS             ( 8080 )


/* Number of times to retry a connection if it fails. */
#define tcptestRETRY_CONNECTION_TIMES    3

/* The root certificate used for the encrypted echo server.
 * This certificate is self-signed, and not in the trusted catalog. */
static const char tcptestECHO_HOST_ROOT_CA[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIID8zCCAtugAwIBAgIUf/pQ2+SlyvLVcPzpY46xl72bT+8wDQYJKoZIhvcNAQEL\n"
		"BQAwgYgxCzAJBgNVBAYTAlVTMQswCQYDVQQIDAJXQTEOMAwGA1UEBwwFUGxhY2Ux\n"
		"FDASBgNVBAoMC1lvdXJDb21wYW55MQswCQYDVQQLDAJJVDEWMBQGA1UEAwwNd3d3\n"
		"LnlvdXJzLmNvbTEhMB8GCSqGSIb3DQEJARYSeW91ckVtYWlsQHlvdXIuY29tMB4X\n"
		"DTE4MTExNTE3MTgzM1oXDTE5MTExNTE3MTgzM1owgYgxCzAJBgNVBAYTAlVTMQsw\n"
		"CQYDVQQIDAJXQTEOMAwGA1UEBwwFUGxhY2UxFDASBgNVBAoMC1lvdXJDb21wYW55\n"
		"MQswCQYDVQQLDAJJVDEWMBQGA1UEAwwNd3d3LnlvdXJzLmNvbTEhMB8GCSqGSIb3\n"
		"DQEJARYSeW91ckVtYWlsQHlvdXIuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8A\n"
		"MIIBCgKCAQEAy+bzNjnzBd52LtYHok7mmCS+qSdFVbsNGdzKQ/ZUuOzZkktqALrt\n"
		"5yK8e3rH18+0t3MSaxadzrjmYbqacUgEL/Qo3y1Y2sD9hGkin+4nRCVUh/G3L+HV\n"
		"73dRkyH3Vxc/Km0Rad3yTD19y1jIB+WW6f+QhMJ8lauv0WKG3Uosn/k8FsFtSvW1\n"
		"qBl3IAjIwxwUmGqQVR/4h9/OWtJIhOlJ5d+5us5eKhPutBz6vGLSHIMOaWm9Nqe8\n"
		"NgWCxz4JSzKpNkdejETd/anfOMdT0xlrrZkL/uBabOlO9uOUeKXb9uW04NWLRug3\n"
		"IGBf4LMKHhz+EukUzVd0f2AKNqw8btgzqQIDAQABo1MwUTAdBgNVHQ4EFgQUDYZN\n"
		"rWUjpxjmfti0rk5IkEqDJQ0wHwYDVR0jBBgwFoAUDYZNrWUjpxjmfti0rk5IkEqD\n"
		"JQ0wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEACqI5e2ZUYQ0D\n"
		"2Xt8n5qtfF/XqvvLfxui6xPSbkJ7DyGdY0cm/5aBSYz8EAkA3NgS3JVJtC8d5KeR\n"
		"/f0iqCUh08BlffoeF9HbXnjUDgiRct3LZA1tlTW8Mq9628qPRmbBo4rg2OYudLR0\n"
		"PlrgjCX8/sH9me9pq6KlJOVkg5lw9gM4ASEgAf/IIyALixTMuAO6U0mC6050LlQ7\n"
		"ubEDgdWi5I0Hv9avwq0pxeHnF1YUq8pGXFXd0SSwzByq7ePxJGtqQxFmsjmNCpdj\n"
		"ay7lCjL96ccsyFmh9LrVq0odzivMzG7E0N/M29OKNKmZfh8ddpZmSworHPLp74Q8\n"
		"PUIAdUPPCA==\n"
		"-----END CERTIFICATE-----";

#endif /* ifndef AWS_TEST_TCP_H */
