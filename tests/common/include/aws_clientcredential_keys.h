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
		"MIIDWTCCAkGgAwIBAgIUTfwVHFWGWViQLamSytHyYWFmFbkwDQYJKoZIhvcNAQEL\n"
		"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
		"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE4MTAxNDE4NDkx\n"
		"NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
		"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALubJt3bWzcoQSDkV0He\n"
		"5aSHt8lcF2NQ1WRDL4XSAYT4RVkfMFdFrAeXVwE8YH/pvdhbRSTJxFC23PBW4eSd\n"
		"UgKjPE3sw0t6QT298M5WswogezpC0BDLMnvGaYA7gvtjMn4265ygwnndvzhIdupE\n"
		"EoXDuUIqYkxYoMuKKcqogXAvlJTGZHUK3DhIxeQyAELFs4swdTcJBqHt//B3/psA\n"
		"Cod4ll5JUTDmCiolrtq1GBwKPiexwqMaNnzw6l3TFYGjLlKyyPC/B4OgNjNVqCJZ\n"
		"I8vUDhFDnryG6X/v00rgYnOh1ErIsC+vOkFzRL6YY4F/WTgokhPMs+Vt6V5D/t5e\n"
		"qxkCAwEAAaNgMF4wHwYDVR0jBBgwFoAUmeL/A+PM4VPhYcXzvepZ2cjUvCQwHQYD\n"
		"VR0OBBYEFDtWb/OH3eW/dI//h61CyUoY8oksMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
		"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCCWkHC8kBdzl7hzv6CFEH4llnz\n"
		"t53cl5jXJjNMZ6rFKq2a1dPo2UykQjhwvMjtuCelpgTSJh35dnX/LP7unWVNjv2f\n"
		"HPjSUWc79KTHjMY6YghI3hT4tsgRJlcKpnV1mBCC0pM8Fgmg9EUF3GuVtbZm9EKE\n"
		"Xs0DmeWDkfBAiq7mEMZ++rMsCwUHeJaZ+MQlqP7vAZwE9tTtReIlvUamjmivKS7A\n"
		"gjcxD64po9JtT6Q5AO6Qa1TTrC0iM0jlUCQiEBdklGBY0b+LkdF6tLO7iYuEUT1+\n"
		"JYaUBTqb+E1sMzxP+llnl9O617SuAFQ4Svu6duPOdhxtjU5sByc9/1ld/2xL\n"
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
 * If you're not using JITR, set the below pointer to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char *clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

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
		"MIIEpAIBAAKCAQEAu5sm3dtbNyhBIORXQd7lpIe3yVwXY1DVZEMvhdIBhPhFWR8w\n"
		"V0WsB5dXATxgf+m92FtFJMnEULbc8Fbh5J1SAqM8TezDS3pBPb3wzlazCiB7OkLQ\n"
		"EMsye8ZpgDuC+2MyfjbrnKDCed2/OEh26kQShcO5QipiTFigy4opyqiBcC+UlMZk\n"
		"dQrcOEjF5DIAQsWzizB1NwkGoe3/8Hf+mwAKh3iWXklRMOYKKiWu2rUYHAo+J7HC\n"
		"oxo2fPDqXdMVgaMuUrLI8L8Hg6A2M1WoIlkjy9QOEUOevIbpf+/TSuBic6HUSsiw\n"
		"L686QXNEvphjgX9ZOCiSE8yz5W3pXkP+3l6rGQIDAQABAoIBAQC3lv+UAMPk2EnT\n"
		"7GDPn1dPiVj7flmM4V/SPEVXScoe2+7RZkoiPZY534pPNUyiIJWeEsM5WxH4gtr3\n"
		"f66T0LieyCuNfnMne744s5Xh9spC0MKGozeGF7ORmPtJd/OYGt0g0pIDtvZZ1/z3\n"
		"EUL9whv2NzEmZN/JIrpINS7smbYBl4AqavfQxMTWFBUO2sSnk5d+Kdl1R8d/c+4S\n"
		"ojspDHFoRFizHnAhIqh4rb3C7R1EeqPc1hrXT7Stu0mE571yXkYtwU5Ew39UAvId\n"
		"cjD9+h5wiK5lYf03MDs3NeZruTm+mYBmAILXSdTBaU4vfCT77xKkDk0vDOeuk4GU\n"
		"emgEIuTZAoGBAO0KTP6XT9lp0AzUKgLCI4LTiN8DlSiTIO9DEQredNH+Su9X6T+A\n"
		"EjoPzVonqjeBDS49hryyhaZzrKkxd4y2mBkRKCCbEtcCyxnDoNyONLDCwm9j9gq1\n"
		"+C5Piafj9toXOJRFZ5By+mgOXuTN+/jujVMfnlTh/STuYqrElrKDmlTrAoGBAMqc\n"
		"n6kup5z35rIzhIHrexmRHSyNvRZFbMnjJwPJKLSNqG53KJikUoqH/p71DYLV244R\n"
		"lOgj92+Hz094dxVQgkuVCGsL9LWSE0S36/tfukk1WP6iw3kV1vpzahEdUt/Ndycp\n"
		"vVfwtO7ESY2TwY+HP+oYDqcgmz/BxlL/Zmebec8LAoGBAIKVkiUPJqYXbM4kc6Bh\n"
		"Yz7HWjfBuYJIy3QD9neOZbzp4k5Il+6rl171JaPtBhDdv20h6AA8OPSh9RSgc8PJ\n"
		"M7FbikesJozctRbUduIKVWyrXoIPB5SO+tJIbwVWyL8M29aTBaLGNLzubciO0akp\n"
		"giGw4YDRnquhyClbc8mURolRAoGACAT/BNFF7R6a0P7dBc9uRerKftL1YwZIcg09\n"
		"pBa4N71Lp2OsBa6rXbRYTa5hZIZWdF8Zx750ssNrXzfjbsKLBbbWaHZGr1Ei1ht5\n"
		"ia+WTgOAjEOg/Q/yPGOJFThV6WnWh4RNbEeowbKfPq1UgfvWecuN+Ot/eZyP3wUg\n"
		"5oTu41UCgYAKv8YmuP99skaTUDi8UE/z/X6x0JWY1qo/gDcKfJarjPdpN/xyfAqH\n"
		"zcMm1ep3zANal3WaIG1wfMxrclMgdUqkGtijas6FwtKdEvkuZanfwEkve6ktuHy3\n"
		"QWEEankFHqYWr63QmmVoiGwGNJe4FXdzuNx9f1FdQsRn9Ia3kYS9bg==\n"
		"-----END RSA PRIVATE KEY-----\n";
