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
#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAIwn/s1kFbt57kgPCTx8bq/oCbReMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODEyMTAwMTUw\n"\
"MjVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDUqVPDQDr2FzsMMYRi\n"\
"m3hTUJT4U4cDnwZfHNZWMKWCNsIwPPjf7iOavtvJei1SFCoj2UHQJ1ZG4l8161BC\n"\
"M70rt4dK31R7NWi1F8iaCUs4SJoTeCSouIGntPaPQynIvj6SrVBnflsOLEtxXz0E\n"\
"8HIIDhIVmv1o4zan1rrG7qqwvHeHCszYYFX5yJlCaKF4N5TTmnFH9UKePQKexB8W\n"\
"5X/aGeao8Bpy0jBw2TKXYF+vdnqym7VZVPTTzy7nhkAibWHJNNbXTbpaa1KgN0GV\n"\
"55GAkqFDiqGoI+3e5ROkOJc8ZqBQuKTRHEELu1WWBQyxqJJTJ6pcar5ZHIk+TOCL\n"\
"lKQNAgMBAAGjYDBeMB8GA1UdIwQYMBaAFLk5JrgQRgTzgd6Z+EHayLdtf1erMB0G\n"\
"A1UdDgQWBBSGIxVCpmvlnOIF9l0UV64m+pxbRDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAJEtNJWxd7B+PQZ5VgxpPZA9R\n"\
"Np+KxrfT39fa2oNgX97Z/Q0HuL9i76Tsxs3mZP8xg1yKSdTdIiQ6KrOKiIT22aT7\n"\
"rF+xyID5EbpH/P4/O0T7sDvWHdo3SZ/M6W1kkdUytWg7np9w4wHawUlkCKraWvHj\n"\
"dEZGbECyolfjJ8qZn4B9D/PqJkFSDg6T/vopD4RkesMBaZbQDlTeIWuoKUQfrnQ4\n"\
"HJ3hAOtvOGBVDjGmWq9nJk77PHw+89k2YxwRthXPb3R1FzSML1l1GShz7P4SNmQG\n"\
"Wlo66Ryw+FuOIM1C4hsDegg+6SU6kr0cPJoL7HdFSVUaZTDuSCVupOsXJ81eog==\n"\
"-----END CERTIFICATE-----\n"

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
#define keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM  NULL

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 */
#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEogIBAAKCAQEA1KlTw0A69hc7DDGEYpt4U1CU+FOHA58GXxzWVjClgjbCMDz4\n"\
"3+4jmr7byXotUhQqI9lB0CdWRuJfNetQQjO9K7eHSt9UezVotRfImglLOEiaE3gk\n"\
"qLiBp7T2j0MpyL4+kq1QZ35bDixLcV89BPByCA4SFZr9aOM2p9a6xu6qsLx3hwrM\n"\
"2GBV+ciZQmiheDeU05pxR/VCnj0CnsQfFuV/2hnmqPAactIwcNkyl2Bfr3Z6spu1\n"\
"WVT0088u54ZAIm1hyTTW1026WmtSoDdBleeRgJKhQ4qhqCPt3uUTpDiXPGagULik\n"\
"0RxBC7tVlgUMsaiSUyeqXGq+WRyJPkzgi5SkDQIDAQABAoIBAAwtmi9fOUEoeWgs\n"\
"WcFv18cloTJLTjsF206NDzUmWgQH8LzkcPgZ4g4Vie6SwY+mufcNlPg/mHA8vGkR\n"\
"DaqM3fNn8iZ2fZcEqvdb4k/34kfZ2IDu62KCFmKtOUVg+j6Vrm2l4SncEYjRPozC\n"\
"gHMM9erCK4TzODnOcS3RXSbROhMezQ8d3Wp4KuHPVtzdathxkHJo4dZkftJ7mccV\n"\
"wrpsg7k8VOgBIG+o4emaseDOzex5itzRXEVBXousuYLHBQZmBARs0wV5vC3aMzEo\n"\
"0AFhvr/JqGPnebVVY8k5PL9FyxVtAlvRMxzK7u/wrHH+PXrCnQEaAIRBv75hqRx6\n"\
"ytxc8qECgYEA+EHTMKKMrjJzscN5r0vEh7oHULvofPJOvCAWS+QjSnEqCiX2uEXG\n"\
"OWkQGRbw/isVd936jVCneYFBJWRnPVEAj6HJT7O3uh4bHStZLvI9zbrLrzoGERxw\n"\
"4O5qSdF3rG+0M5zprZyJ9yuUeQeDYZtTsfaMBiVPZnlKa5IOtQ2DtgUCgYEA20tL\n"\
"FSCkJ+HrVZODqwmzllgrxNns5wdf1KuYE/y31JDWVyJQu69PYz/zbSnlRQ2578Zb\n"\
"pGkndWKQ9SaW34YdvBdmp4Tw84IRhNS0hTjtoHh3DpRoJYiw/vt1z5Yk9cRM8JF1\n"\
"rkmCaeAtqA5bgWjconHSTi6hYUdW0rK6xSnnzGkCgYAYPlVhan5ShJrHwU8gz0Vh\n"\
"+8i7GZ6alQ8bUAbdYmoP2v1AAHUXibjR4nCwAo2GRK7UBdlx825nbzftbuCmHELx\n"\
"w9rk7agDhO0g7BrJet6OOK+rbP8qrJxZWu8GRm5paH8wiyJG0gKnIJpfN1ydK44A\n"\
"0xLZhss6Np4N4zWMeMVkAQKBgAXJ7Qd3vx2WZyHxLO6c6PK1DSK7JffLSApTJUaR\n"\
"y0B/Okk+jKcHr67NB14AVx8Fh7z+nXivle0RVwZBqIXqAmruqzUM21dyPYgmHUXP\n"\
"BObRh0MJQ96JxzJT7P431m3uaqGHFXYuVpT45/uhtWSrIfffzU9nB7eLrI0PTsLZ\n"\
"olbRAoGAAmbn1h0uAdzG1JEo+fIYumywraddZNyt808DQmZBpeFb58ImKZXeZIfb\n"\
"ZJFxsYnUUHeTIX4h+cji1l+hwjQacS//Nq432Tt8cT5rZ37MrufWsqLqMBxpLqTF\n"\
"rst9nPBWGTZfi8buvaZCkIcBu3YTn9WEUzHwPF7yZRRmkuTysL4=\n"\
"-----END RSA PRIVATE KEY-----\n"

/* The constants above are set to const char * pointers defined in aws_demo_runner.c,
 * and externed here for use in C files.  NOTE!  THIS IS DONE FOR CONVENIENCE
 * DURING AN EVALUATION PHASE AND IS NOT GOOD PRACTICE FOR PRODUCTION SYSTEMS
 * WHICH MUST STORE KEYS SECURELY. */
extern const char clientcredentialCLIENT_CERTIFICATE_PEM[];
extern const char* clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM;
extern const char clientcredentialCLIENT_PRIVATE_KEY_PEM[];
extern const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH;
extern const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH;

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
