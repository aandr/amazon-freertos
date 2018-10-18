/*
 * PEM-encoded client certificate
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char clientcredentialCLIENT_CERTIFICATE_PEM[] =
		"-----BEGIN CERTIFICATE-----\n"
		 "MIIDWTCCAkGgAwIBAgIUU/CAVZ+YBfk7jgHTGBc1Il/K0nowDQYJKoZIhvcNAQEL\n"
		 "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
		 "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE4MDcxMDIwMjU1\n"
		 "N1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
		 "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMTXTwMc8GXPB7E/tkoF\n"
		 "1tBZ1X458VYDphYSOpAhjk1SvdYWAfJBJmXRp2HeFKo9s1YhkGqFGGbTrNV6+UlR\n"
		 "hRAN9Fl+OKJ9Kz2IpGW0JkC8tcPYBmutbhRZE8HtVeniujhtIgZZrkUpd3a7e5re\n"
		 "1lYUm/AZqX1Id31jEaYam8z5zoo+cCcgU5NTJ790yKly0kXL+2Xp1UwzqeaoHYe+\n"
		 "T7TnmlpMe4DRW9vlF2i6E2u7xFPeTAy4nOjTuc51CDDdyhKnq7X3MGbj41J4Jn26\n"
		 "xIqGj9Yblxgz9fVSwwqTELMMScwTHEuYu31jzRxlYtvF64PedgS4TgWeLKtmo4LI\n"
		 "Sz0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUvqpTcDMaSJOMmfb0W7Q3EstwvrQwHQYD\n"
		 "VR0OBBYEFFRgA7ezzrkOPpMEa5QvACmCysNHMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
		 "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAfV6xLRJ7Kpkq1cnp4/eF2ikLO\n"
		 "cKmON0Hg0pH/a+2E7yg4HOE3qpLqHpFUZKwdiPllDQGH2NXM4/rZ6iNTatd3sFhz\n"
		 "Pkw0Wvv4HtQWpezwRUqAqrPNP+zUQuLbKSo55F0ZymBq16WzwSjSM+odX0N9rYBy\n"
		 "aEGvpNPQ1Ek1YBru9LjejXhV/AYVZOwDXEN054Il/RN9uQUbuQN3a1vtNtfnlzD0\n"
		 "1VRQsIp74PA4x/HfnPMXLVLG5Zgs5NlZ+/BWUyjVHlf8G/27TkMJxOe6Y4EYPzLX\n"
		 "+cLr5yNcaUhaWeTW0CP+rPqfBS7+rEuqD5wnzFJoly+dhvLu0cAeCDnHEb36\n"
		 "-----END CERTIFICATE-----";

/*
 * PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 * This is required if you're using JITR, since the issuer (Certificate
 * Authority) of the client certificate is used by the server for routing the
 * device's initial request. (The device client certificate must always be
 * sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html,
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set the below pointer to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char * clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----"
 * "...base64 data..."
 * "-----END RSA PRIVATE KEY-----";
 */
static const char clientcredentialCLIENT_PRIVATE_KEY_PEM[] =
		"-----BEGIN RSA PRIVATE KEY-----\n"
		 "MIIEpAIBAAKCAQEAxNdPAxzwZc8HsT+2SgXW0FnVfjnxVgOmFhI6kCGOTVK91hYB\n"
		 "8kEmZdGnYd4Uqj2zViGQaoUYZtOs1Xr5SVGFEA30WX44on0rPYikZbQmQLy1w9gG\n"
		 "a61uFFkTwe1V6eK6OG0iBlmuRSl3drt7mt7WVhSb8BmpfUh3fWMRphqbzPnOij5w\n"
		 "JyBTk1Mnv3TIqXLSRcv7ZenVTDOp5qgdh75PtOeaWkx7gNFb2+UXaLoTa7vEU95M\n"
		 "DLic6NO5znUIMN3KEqertfcwZuPjUngmfbrEioaP1huXGDP19VLDCpMQswxJzBMc\n"
		 "S5i7fWPNHGVi28Xrg952BLhOBZ4sq2ajgshLPQIDAQABAoIBAApZn+fz/5f62Naz\n"
		 "GOyvfeoaryUh+tShAHysnLOzb/4U2bKGGbgZQOGdsoVs3f1NnT5sKWHQecX+X1oi\n"
		 "SzVVuf9sHGsQPf4dQsbLholqfZBt28jedQf4aI9dV7KcviW8LDqHUoCu+oDce1Gc\n"
		 "7hTonrCMCN8JGmZiBxZqvSTUxVhzuoMmKePF4/wA/xaV+lXhwNIdrrFajseBrTdE\n"
		 "LCjNq1hft0+EVKkvi1oRb02wbEpciiYQsGqnTmnpoiQm2AO4NOIJ5MMi4nwOaPts\n"
		 "4I0784Bx4wxQSEQIfTo6dFojLpsnP8CEVoRtXn9DJRcZLdHawFWWGKru776eE9sd\n"
		 "emjYNekCgYEA8WA4VsECD3Gt3nF+N/vm1ytCsTPTwURTmpUV86iTNX3VuSLzvcnI\n"
		 "aaFlkQYMRmMAclOSizehpeo8e7VmcOh1hehbDlEs5ZWIxLDLbswF3obdmWzuAaaq\n"
		 "+9dpUJSfHn/aA3yFzEoKMujIrYEK7F+29LsUF7H7gXnbqMJhJjIljBsCgYEA0MRY\n"
		 "oaZR/UR9FtGsNSoMct4HE0saYIsvMsI9EizjLfkIsPw9Pa02qxah5KYnVts0halQ\n"
		 "xOSP+gYqLqlm1X7FOzt6BSxbVSX6nYeJD37bB0QwNMx6Wwdk7W81LcYSM2adexLG\n"
		 "4kxy1E7zDjSEcTUk8ITbu7PJ0tHA2ILZZarGy4cCgYEA5xx4FSjMbjksfxYwAi6h\n"
		 "QlhbjQvQrvZbDmG9GQXJnagUoSWEN64ixHESrJ8b1bEpVk4KvF3R2ujCbLitk2Ig\n"
		 "VMy7kqPK1zAsOhgFUNrsq7p6J1MTOGV6xZPldJA5IpzMBBaQjaA+z6vvqaEq8lRa\n"
		 "+0y82ktCQyCIX7duCYJuTQ0CgYEAj8BTo2OKG1I4rCjOYoFEtyfJSXCyhf3xDwuO\n"
		 "z+AWwoYzhOL7LGgEN8TKz03V0sn4HX1b/2e++BOcoirFg4baOpIW0E8dyMDxE93K\n"
		 "89+O5C8A3qpaKioqRhAIVy+HQtuK3r8sMZEglK6UUJi7BdTpTbC+OT5wX3lNSAi5\n"
		 "t3bjkisCgYBgEo1uLmtjhRwnZganDSu6r8G02YgoDV2Myifxj1shEa8sPV3u3XvG\n"
		 "wKID5hg0OkoTdNMBWZ39WqZ/Ru7rp5qffkWpDcg35s0y8ZqQxbytQiAwgZEvIuMs\n"
		 "0b2hUbRKS9ILXBguswipz0Ev2DaGS3CSGMTUYivWwcLWmZN+ZF7fVw==\n"
		 "-----END RSA PRIVATE KEY-----";

/* The root certificate used for the encrypted echo server.
 * This certificate is self-signed, and not in the trusted catalog. */
static const char tcptestECHO_HOST_ROOT_CA[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n"
		"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n"
		"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n"
		"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n"
		"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n"
		"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n"
		"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n"
		"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n"
		"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n"
		"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n"
		"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n"
		"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n"
		"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n"
		"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n"
		"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n"
		"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n"
		"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n"
		"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n"
		"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n"
		"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n"
		"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n"
		"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n"
		"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n"
		"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n"
		"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n"
		"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n"
		"-----END CERTIFICATE-----";

/*
 * PEM-encoded client certificate.
 *
 * Certificate for P-256 elliptic curve key.
 */
static const char tlstestCLIENT_CERTIFICATE_PEM_EC[] =
		"-----BEGIN CERTIFICATE-----\n"
		 "MIICizCCAXOgAwIBAgIVAORZ2KnKZE9kmfp8rwWIk9MCQ0w+MA0GCSqGSIb3DQEB\n"
		 "CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"
		 "IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODA3MTAyMDQw\n"
		 "MjFaFw00OTEyMzEyMzU5NTlaMBoxCzAJBgNVBAYTAlVTMQswCQYDVQQIDAJOQzBZ\n"
		 "MBMGByqGSM49AgEGCCqGSM49AwEHA0IABPCR+HVGGSeILAj842n8q5SrABbROPlg\n"
		 "Yt2WE7Iwdxhi5QX9yE4ib8TZNbk7lwp1uHs3ULvwYHWiw19xyQKpgm6jYDBeMB8G\n"
		 "A1UdIwQYMBaAFKBKj/Px7A32rwAF1rShzeAD/WRmMB0GA1UdDgQWBBQn6aTQjSLC\n"
		 "Y1YwyGRyrmqEqJsPATAMBgNVHRMBAf8EAjAAMA4GA1UdDwEB/wQEAwIHgDANBgkq\n"
		 "hkiG9w0BAQsFAAOCAQEAf9rXIFcOOn0GAUn8x5bCfV4If6lKlOs+A0vF6iIiUEQJ\n"
		 "R9CK8dvDrNvATkzEvMXh8SqH+ziJai2dDO0TTDjOBOlUrf9DRLsGujQIA7lIxk0e\n"
		 "r0HUyJx8XILy6Op7P22qEc2Q73tcRtBw/AzyoD83I7wR6cpPkZQGZQtJN2RC8vZG\n"
		 "eGv9jzAXpvywUDtQ+Tbs/+yrL3cdI5iM/0Pb9WCbV031ZU29WXIKWcQrs359tdrP\n"
		 "O2FXTPoWQRbYukUqLFxWBOtBAyDIyuGaQdeAzMkTN+pU/zxIgrheqsMHRHe2reos\n"
		 "Sb4Y/9ipSlZFbQoOU4EohUePT7rp/fVHs34533hdWg==\n"
		 "-----END CERTIFICATE-----";

/*
 * PEM-encoded client private key.
 *
 * This is a P-256 elliptic curve key.
 */
static const char tlstestCLIENT_PRIVATE_KEY_PEM_EC[] =
		"-----BEGIN EC PRIVATE KEY-----\n"
		 "MHcCAQEEILEBPX5yheUqUIRBRWinqTwxvo1lJZeEgS5kO7ElupYGoAoGCCqGSM49\n"
		 "AwEHoUQDQgAE8JH4dUYZJ4gsCPzjafyrlKsAFtE4+WBi3ZYTsjB3GGLlBf3ITiJv\n"
		 "xNk1uTuXCnW4ezdQu/BgdaLDX3HJAqmCbg==\n"
		 "-----END EC PRIVATE KEY-----";


/* One character of this certificate has been changed in the issuer
 * name from Amazon Web Services to Amazon Web Cervices. */
static const char tlstestCLIENT_CERTIFICATE_PEM_MALFORMED[] = "Paste client certificate here.";

/* Certificate which is not trusted by the broker. */
static const char tlstestCLIENT_UNTRUSTED_CERTIFICATE_PEM[] = "Paste client certificate here.";

/* Private key corresponding to the untrusted certificate. */
static const char tlstestCLIENT_UNTRUSTED_PRIVATE_KEY_PEM[] = "Paste client private key here.";

/* Device certificate created using BYOC instructions. */
static const char tlstestCLIENT_BYOC_CERTIFICATE_PEM[] = "Paste client certificate here.";

/* Device private key created using BYOC instructions. */
static const char tlstestCLIENT_BYOC_PRIVATE_KEY_PEM[] = "Paste client private key here.";
