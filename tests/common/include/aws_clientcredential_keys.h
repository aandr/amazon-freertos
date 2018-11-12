/*
 * Amazon FreeRTOS V1.4.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef AWS_CLIENT_CREDENTIAL_KEYS_H
#define AWS_CLIENT_CREDENTIAL_KEYS_H

#include <stdint.h>

/*
 * PEM-encoded client certificate
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
static const char clientcredentialCLIENT_CERTIFICATE_PEM[] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAIgsyQyAPnWDXWOBG0N2VTl9Xnc0MA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODEwMjUyMzQy\n"\
"MjRaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCoXmIju4G2wPLeUPQt\n"\
"dTIBwS0DoqPM7X1XmY13Te0e5SAzypxQ0cNIjD9/NCoWbSdIOt/7AgAQfYUdhqpN\n"\
"zBwnMFalPOq+4jjFTfsHvN1QMN4B4PMdM58NecN/BmC1Mp5CvQgQwYucj1wNS61Q\n"\
"MpECZhwBlwmJvXiGOl1+0sFW4l7zK1yLzXJ31W1CQIuOUUxclErbXwVMKE6OnjSl\n"\
"tjJduqaAGgN1U6P6A+kqu2Rxbk9v8X+bxnZx5fY3+spbF+KPNHTi/AYmRg6AomYo\n"\
"1EnjtyChcUvFNxZ9dYO3O3o91ffV5xboBIx6HeIS3ued1JO6G4B/SgaDzxuHwQ+y\n"\
"wPrtAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDrVvmXlDL5XGRC/3APJvIQ7KcJwMB0G\n"\
"A1UdDgQWBBRjXdfD1tRVGm4W2RY6AcYVI4HxKDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEA1O+w7VbqiiS9c/+xuKG59t6k\n"\
"OOo0jSUPJmwJ45R1uOvC4ZvgRkIoJIlgc+xt9qBHGG2T3J5U6DM2B5u4tIyNYEQ7\n"\
"iTaVlUK6vvJE/3PuES395QGkJf2szynB/PqqeyhvYoo71CpVcUj4Q6ctae223Nck\n"\
"P/C0WOt08wNe9NNjikhORzB7AaBtyrCI4jqwIihdUlHkO8p7JLq1bjlm5BXwYimf\n"\
"qIizGz2xLEzDwYVMiQDaQjLnjbcQpYgIbQ9LHW7pgdMv4Sq8WcyR8Qwr2c3ByAMg\n"\
"qWnAix8ahZxi2iEM4oatccGjS7y1Dhbd18YwcT2caxRql1AlQBuzlNEMGaorjA==\n"\
"-----END CERTIFICATE-----\n";

/*
 * PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 * This is required if you're using JITR, since the issuer (Certificate
 * Authority) of the client certificate is used by the server for routing the
 * device's initial request. (The device client certificate must always be
 * sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html,
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set below to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */


/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 */
static const char clientcredentialCLIENT_PRIVATE_KEY_PEM[] = \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEpAIBAAKCAQEAqF5iI7uBtsDy3lD0LXUyAcEtA6KjzO19V5mNd03tHuUgM8qc\n"\
"UNHDSIw/fzQqFm0nSDrf+wIAEH2FHYaqTcwcJzBWpTzqvuI4xU37B7zdUDDeAeDz\n"\
"HTOfDXnDfwZgtTKeQr0IEMGLnI9cDUutUDKRAmYcAZcJib14hjpdftLBVuJe8ytc\n"\
"i81yd9VtQkCLjlFMXJRK218FTChOjp40pbYyXbqmgBoDdVOj+gPpKrtkcW5Pb/F/\n"\
"m8Z2ceX2N/rKWxfijzR04vwGJkYOgKJmKNRJ47cgoXFLxTcWfXWDtzt6PdX31ecW\n"\
"6ASMeh3iEt7nndSTuhuAf0oGg88bh8EPssD67QIDAQABAoIBAGvSLYT2giX3wFBw\n"\
"hT5LK+AFxn7qe30zpS4hDoAVbkF3t7yTBrGW8zkVX2BlloJQGnTeByumZ0YoK8Px\n"\
"xkQV7v2FricZMLdXBcYMnzKgVxYQhUuss+PCeBvYG0LQYcQIC2NvzKd2WqLDB88X\n"\
"5IBUMZrYqxy1jY0gwfsri045R0kMmoruhyKhJWjhHePQl8txEkwbc2m2NlxFZqPk\n"\
"4dqaL4CR+Gc7jheXginDn6uk5acxopdeckMZ43R8/1hG3jQJsOkEms+VXj0TbkeP\n"\
"NeRsHJ9EPGHr4H9wkRYjydUDD58aFMKXJwxHBi50/0zF/clSTpPSMXY3uYL9Tk5q\n"\
"14dKPAECgYEA1D28WxjhXFZrYoKZFGzOC/evo4jB9hcG4lNoug2tYvyQg9gQFSSN\n"\
"xVwRM3SdBrOqZMv69Sp33o7DaPnIkcmiWlpTNqmM2PoUZ7uD648cNWqlb7gRx/9M\n"\
"IUsCFVTZKrooTS295s2PGuHHkEP4rQg7SC0Fm86TZDxcDvBwejLpjQECgYEAyxUF\n"\
"r+gYtK/gZYsv9i0UIMi/WbunQGrT+ZQqEVuzntbcv4Wt4LTDJo4/p8coZ4+xNDDt\n"\
"hYK7qMyt1x8P0lS+MUZby0LrFOF5YdleAUREjPVzJITfSI+eFe4ZFP5sEPqVGpOw\n"\
"0V2cQOqMY57vb6A0p/euDLO7TUZdYY/opp1Mce0CgYBmNp0hmifeWlg5BwBGD+hj\n"\
"ZxOVRGV06L4N/XFwOOoJw+ytMj78j7AfHinCiCSfd6NWC85+h8dWzZZaVgE8BMlF\n"\
"jj6JErqcPEPbeTkdFrzVFNk7poAZu1+koQUN1OEJbMG/+Yl9P/3tSlLbrYBzhIVY\n"\
"SyZqVOAy48bKAw2ILEDCAQKBgQCnABmWJzGx3HOWbVg6g6VqtEqOuv71vSOluI7D\n"\
"iXpzI1OSd5aeE2frm8yC5wa16vcY6SdZxEEXtYE7RwLWvvNMSASSaOwyVpxft8VT\n"\
"G1WX6PwPyuhmnhutAwhDOFmbuMTGfy9HbEhjJb3e6kMRs1e5zLanIWHkUPnXkeUK\n"\
"po3HnQKBgQCNgpZvQp2U8xumJRza0D8sHwUQbhW9FtcwaUFZNOthIdVG06bbwlpQ\n"\
"HxGbg4WhOvxLUMaWEOFGE+ZbNxE/8fzdxOfJABb1iDIkOhSP1ZsjcRLW44MzkO2X\n"\
"+ROwZnb5mtjxtdJidGLtlDy6u/+KcqydygHMThQoa3pnFdGl3D2ohg==\n"\
"-----END RSA PRIVATE KEY-----\n";

static const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH = sizeof( clientcredentialCLIENT_CERTIFICATE_PEM );

/*
 * Length of device private key included from aws_clientcredential_keys.h .
 */

static const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH = sizeof( clientcredentialCLIENT_PRIVATE_KEY_PEM );


/* The constants above are set to const char * pointers defined in aws_demo_runner.c,
 * and externed here for use in C files.  NOTE!  THIS IS DONE FOR CONVENIENCE
 * DURING AN EVALUATION PHASE AND IS NOT GOOD PRACTICE FOR PRODUCTION SYSTEMS
 * WHICH MUST STORE KEYS SECURELY. */
static const char* clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */

