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
"MIIDWjCCAkKgAwIBAgIVAMhd/LFMgNSuspuL0O4jFUhrDvv6MA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODEwMjIyMjQ1\n"\
"MzdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDOzS/deIrVlWTYCQ44\n"\
"94ApJV7N6gyt59coK2gmKhkMun0M1C8Fc7y47ZLug2l8QVjlydf8gIaBPyqRcTgg\n"\
"loLNqPWRdQa7FhgSKP/X2gAd9bT7BHdAoda8kUAtVOsLpoLSQ3Lch9z1dvm5QRB9\n"\
"MU6TwnIg07pYfxQDdCo9Zzj4mziK1vUMpMu8rA/AlBAaHMSgesnj8laM6kyqzPJf\n"\
"pXO50JwalydFj8rhOUMMKdhOTWiyN/fbXYHTBpP3+asfVJp+8cLcvnc1QSZhXlD/\n"\
"gRdaD2coSre0Cf2//g7o3ZYBTkIqfchP+aj7MTeNrU430ocQFKiidP0K3OUzN0hi\n"\
"/8eDAgMBAAGjYDBeMB8GA1UdIwQYMBaAFOQrlnIDLKJxiQojq3ybZh9kXF1bMB0G\n"\
"A1UdDgQWBBTVkVtXVvMgbkTJetvHt5MF2Bs+vDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAjBkdPv9h3B8bCRakai0JogI4\n"\
"aC+Mc9dd3ryVECWCsBmWZDuvvce37FtXduvinjk0wHV9T2KtEC9L79gqw83yV2Ae\n"\
"aLDEAv8QtQfVrunBejHguRzDgEf5ZoNJNNBvOluo7u8Vz0HNoD/pJNL8GE9vU30O\n"\
"FApOkTi70emo4/wecOo8ffPj57I8sk0lAFQo2jR8Hz7O4wMOcL3ccCkcoPraJpRC\n"\
"2H8bJM7KhprMYDvqlHyg394AfwkHZCX2w4DtmSeHOSfCjx5Zy6cY+gnHKS9Vm7Dq\n"\
"l60tqQ34fcwTM4Hl0WpUSPGmiAWD+lwhGPOZbIDxtwjY01o+pVUV1522O8h0nw==\n"\
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
static const char * clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;


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
"MIIEowIBAAKCAQEAzs0v3XiK1ZVk2AkOOPeAKSVezeoMrefXKCtoJioZDLp9DNQv\n"\
"BXO8uO2S7oNpfEFY5cnX/ICGgT8qkXE4IJaCzaj1kXUGuxYYEij/19oAHfW0+wR3\n"\
"QKHWvJFALVTrC6aC0kNy3Ifc9Xb5uUEQfTFOk8JyINO6WH8UA3QqPWc4+Js4itb1\n"\
"DKTLvKwPwJQQGhzEoHrJ4/JWjOpMqszyX6VzudCcGpcnRY/K4TlDDCnYTk1osjf3\n"\
"212B0waT9/mrH1SafvHC3L53NUEmYV5Q/4EXWg9nKEq3tAn9v/4O6N2WAU5CKn3I\n"\
"T/mo+zE3ja1ON9KHEBSoonT9CtzlMzdIYv/HgwIDAQABAoIBAB92omistVrHjv87\n"\
"aXmLVmlsMR9dE67FzjtjV0YvM353RR1Vjw3iTP1J0AgP4FWyT4cujXjySQSTCrP0\n"\
"KNj0bVRC95G431xJgUIKENrTZR8pye5DAK3JsbUg2EXNXLF6FP9OmcQpu5lxD496\n"\
"8bpKbS0X8WtDK1/Zfb8ExoRvFwDjvp7TSO5IdxrV1ImZdDc+V7F22eRmcWE44OMB\n"\
"8WgGCOEUpqhTlWyvTlWPWDZb12GApdMyXbJNhlG3ADw+xOlwN6So11Bt8X5IsVrP\n"\
"nEr+OB0DJPe3xcSComFummQAzLCrtV1kPVmL7VGEyPnlViB6Tb55r2XcflsvxjVh\n"\
"Y8RD7ZkCgYEA+H6lh5QOOP9TQndLlbwJ3CYkS+CJobR4XNGAs3L0ib2b7oeCU5t3\n"\
"V2LwpC5cLWJFEpM0CFVDUH0Zx4ZugUFUpqyWijqrwAQ28wW5IdzYZVx8E3UKnccw\n"\
"upK0WbUFuRc44b4FRX46yh/UxoF3aljp2JELSR/j6YTT9oU4ndKisX8CgYEA1Qwr\n"\
"ejd3s9oam9oWVyzxvQQXz85BGK0mu89DdFqeJgJUDxYx0dWsBwWsXyZozKY/qA1l\n"\
"1cfmd+iYTiUCCEEF2v9nDs1uPzwTOLyXx4YBhrKMUX7oJ18QY2WKg8N8pkRezf/t\n"\
"5rlfmJmhv/qqtVPld3S0V7Di3pdPecEUyzsOI/0CgYBEUITvReu3DbJ1BljYPE61\n"\
"oCLAXWVY+EVStyHbs5G4wFC6/bP9ijL/6ipf6ILc0zRMC/31AIO1//5zVOjSTgSY\n"\
"ob/tFn4bFs2IeB1SSuWO0CMoMjgN8jHaSPSd6N8+6DxyCwxl83L0GFYlkHgk23bG\n"\
"W/GvU74Td7nR/wP2l260vwKBgFZs+2429i5VuhmjXRC8QMymzmoUh7/U7jOeHwUi\n"\
"halxpFwCau9uuLyYhpaK5FSw1H3+AWO6Fjpqdu6sgKiMiC2REmOk3qZ0Xm/TwdbE\n"\
"+/WClP2yd8cQ/3rzBD/zm6tvbO4wdnf0l102ZbGVpI6unBXbmBVCwqX9x9Xih+bx\n"\
"7AiZAoGBAKTEGuK1E+VtrPw3GKm/Mz5W8CmmgKSrMwLQ+KS9ysnwadnKTBENZu0J\n"\
"zb3GZffSN71VzDYanVLtSGHGxladuA6uHxr6MBY12DoYbZSg8lvxlmza8jxHY5k2\n"\
"6s46k0oB/8qoyPxfNFPBkQAEP7ShBEfo/RApcnY2hV7UpunDxtHj\n"\
"-----END RSA PRIVATE KEY-----\n";

/* The constants above are set to const char * pointers defined in aws_demo_runner.c,
 * and externed here for use in C files.  NOTE!  THIS IS DONE FOR CONVENIENCE
 * DURING AN EVALUATION PHASE AND IS NOT GOOD PRACTICE FOR PRODUCTION SYSTEMS
 * WHICH MUST STORE KEYS SECURELY. */
static const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH = sizeof( clientcredentialCLIENT_CERTIFICATE_PEM );

/*
 * Length of device private key included from aws_clientcredential_keys.h .
 */

static const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH = sizeof( clientcredentialCLIENT_PRIVATE_KEY_PEM );

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
