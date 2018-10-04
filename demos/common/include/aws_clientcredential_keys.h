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
		"MIIDWjCCAkKgAwIBAgIVAIlV9GLuRv59/l5YSdnG8o/mnBT5MA0GCSqGSIb3DQEB\n"
		"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"
		"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODA4MjkyMzA2\n"
		"MTFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"
		"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2VlfYClRQWe76zNSm\n"
		"gp5r2fFHHcbucqHzwLFdoLXI/OqVM46F5w9nDbCICRK7ASTEa3wAhLVz+hUUKPhK\n"
		"kRieKwCERq/7J4703MzX1bHMxmIKUwnbpbd+37yfUfdjPr8LLege4fEcjVw0+i1o\n"
		"EisI38kQ/SwZrPEi5hzxvzmhXMCk1J394YHGnN+u3fE9HJWp0GWqForcIsGif60A\n"
		"kA7RTtGauwGELFCOrz9ULG4hh0XjMwGosa5LKJ79aJfFKvoC5DKbjotBQrrnhZ1s\n"
		"XlY2PUGmvV8HRFk90qcxEO3dzvO8YSYo12NcH7SGUdKDkAHLqBmb+RWvA9oIIskZ\n"
		"DglzAgMBAAGjYDBeMB8GA1UdIwQYMBaAFK1WayrKLjesZfl0dA59W/nc7OG7MB0G\n"
		"A1UdDgQWBBTwg8fDWzxp8yhiu1vXiyGYNlzurjAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"
		"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAvB+NX2KSvIPAuTXwz16uSLZi\n"
		"HlpYZqcZIEeKOD08xNd6tzOGtg7x55tyq09gqjpZLT+EQNhPkLTMchiRxAc9d0+x\n"
		"G+AXmBD5FX0oFCnEftWc1p2cisJM9tLNUcuP2QfyhqwLD4scaUynMXau5Uqfqom4\n"
		"XsqfKfR4y4QxT6cmfuQE+FMZl1PauQuTUiT0CshmcU/Z5/Iq+I0z33gCFMU3fAKu\n"
		"ZAn7+4faUmMVZtW1QdehbSjqw5yk8+tGmUOvyV7jXp0cYyBe4UVqzWJvSlysb4YE\n"
		"F1XOMxMggeT5H9DkPaI/4H2l5JT1Xg2jD+XUlHcY9yL/g+JxtXyWUD2e2otntQ==\n"
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
		"MIIEowIBAAKCAQEAtlZX2ApUUFnu+szUpoKea9nxRx3G7nKh88CxXaC1yPzqlTOO\n"
		"hecPZw2wiAkSuwEkxGt8AIS1c/oVFCj4SpEYnisAhEav+yeO9NzM19WxzMZiClMJ\n"
		"26W3ft+8n1H3Yz6/Cy3oHuHxHI1cNPotaBIrCN/JEP0sGazxIuYc8b85oVzApNSd\n"
		"/eGBxpzfrt3xPRyVqdBlqhaK3CLBon+tAJAO0U7RmrsBhCxQjq8/VCxuIYdF4zMB\n"
		"qLGuSyie/WiXxSr6AuQym46LQUK654WdbF5WNj1Bpr1fB0RZPdKnMRDt3c7zvGEm\n"
		"KNdjXB+0hlHSg5ABy6gZm/kVrwPaCCLJGQ4JcwIDAQABAoIBAB53ErpQD4JVZG6X\n"
		"JrijfmC6y2t8eC6DDLgza+8e2QC58V/4e0m4L4SibupryathJ4mCA/f6NG8qS7dQ\n"
		"mRusXpaezLlNLj4if+o9siQl2k1CoELlWPJQH/ac32EnStRUOYteojf2vFhu7gcT\n"
		"/d4zmukiqu+hvWqU0L1qAVALCVACbOd/HLgjvLjRdM942539loQpU9gez2AmO42g\n"
		"05RwglvamjWeFR26VsRIEjKVqd1rHwVfgK0zfdj9ak1QWILOeR15PAj+zFpro/7V\n"
		"VlVyNaOkuZ5Chhdoc4U607xtO9Ugbjs8NXDIkldTfv4/FFSOC2Ht0FwI4CgCfTdZ\n"
		"b5GZczECgYEA6huz2WAPi0X5Q1xyvp/pPEtR+rZ+9XWcazsB9iDNo9QsNuPfykEi\n"
		"idUCBj+oKUWDYH1OaY02HeCyzSVLvs9u5KzWX0i3QRvkj9dXZsL6xP0ZaMNePyiR\n"
		"jnUSvRxf+Ng9kWTA8fnbOmTdj8FaATNXCP6H5G+RdKDCsbUQ9esmmp0CgYEAx2NM\n"
		"5wxAeoi5XJ2o/H7Hm1m6aHx79h7Ood/6P9hqlncas2W5cH+CToryIg9UbM/tuPdk\n"
		"Pgj2a2tFuOe290dl3QpwK0UfFBCzce/u4DwOWU3YZadIxBXoKwUDOSqBKhVpp6U7\n"
		"2GvRHcsHdHN3l/8iq3bUUBAWW/kyxi7pE8ZPr08CgYEAlboQBbftWRrwuAdZj9eA\n"
		"eUKG9P1hJ6YH24EK2RrTyrL5cEW/rICKEXM9bzepWnQN+VlLHDDnaqZ6VMBEuI28\n"
		"k96C4MAGDEtFnEWSrbOIZRKm/JzvGx/cMCtqR67tJp9OiT3SNTCi6K74EWgNI3Vt\n"
		"deAsk4pTki77NmLirRWvQ/UCgYANrOrmYfm+LciA6LNhdhScvB/D8N3TuLM1eRZB\n"
		"8McGwO6M5Wo9pBE879zBp0mEGRLV/+nZM2YrYfUrRIFRHo2HBqNA5IHIL4kPRISe\n"
		"zp+xUlyIRFwCTvpYkQRylE69ozBW9kdPhb2XDIjbXNwUCcTH++N+BkxyTHpGC4ui\n"
		"9pQakwKBgDXrs7SNc6znOSttFXdkYzS0j6d/T1DsAedDcWhxfjHvotLXrz5HYkiB\n"
		"cgcWoiShPEshahcax47HlGxvDs3tQK4s5aJhycGbAquIN4NmQ+gP66CdXraPFJpb\n"
		"waYijDhAcdr/6pgNUm7FTd8OeqZgdeVqOeUnVEK4ajIHHGhrnyck\n"
		"-----END RSA PRIVATE KEY-----\n";
