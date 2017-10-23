# kotiheating-arduino

IoT KotiHeating project for remote heating control realized with Arduino HW.

## Functionality

KotiHeating offers 3 heating modes:
1) OFF - heating is set to non-freezing level (5°C)
2) MANUAL - allows set heating level from 18°C to 28°C
3) AUTOMATIC - reads temperature from locally saved matrix


## WiFi Connectivity

KotiHeating exposes following endpoints via WIFI:
- TBD.
- TBD.

## Internet Connectivity

Kotiheating is periodically connecting to server endpoint to update AUTOMATIC matrix 
and to save last known state of the device.

Server matrix can be changed on the server TBD.

Server expose last known state of the KotiHeating via API TBD.


### HTTPS connection
Solution based on:
- https://github.com/esp8266/Arduino/issues/2556
- https://hackaday.io/project/12482-garage-door-opener/log/45617-connecting-the-esp8266-with-tls

openssl s_client -connect www.kotopeky.cz:443

copy certificate to cert.pem
-----BEGIN CERTIFICATE-----
MIICdTCCAd4CCQCpR/0/YtqmrjANBgkqhkiG9w0BAQUFADB/MQswCQYDVQQGEwJD
WjETMBEGA1UECAwKU29tZS1TdGF0ZTEXMBUGA1UEBwwOSHJhZGVjIEtyYWxvdmUx
ETAPBgNVBAoMCFJvc3RpLmN6MREwDwYDVQQDDAhyb3N0aS5jejEcMBoGCSqGSIb3
DQEJARYNaW5mb0Byb3N0aS5jejAeFw0xMzA0MjkwMDU3NDlaFw0xNDA0MjkwMDU3
NDlaMH8xCzAJBgNVBAYTAkNaMRMwEQYDVQQIDApTb21lLVN0YXRlMRcwFQYDVQQH
DA5IcmFkZWMgS3JhbG92ZTERMA8GA1UECgwIUm9zdGkuY3oxETAPBgNVBAMMCHJv
c3RpLmN6MRwwGgYJKoZIhvcNAQkBFg1pbmZvQHJvc3RpLmN6MIGfMA0GCSqGSIb3
DQEBAQUAA4GNADCBiQKBgQC7xSvV88rnKf/j2XyMNzaahIwhO7OvizvMkeX+OGYA
P2mbUMFWCJNTL22udWvpUNXQ1PsjxvQ94c2dgZzKqt3YrEukCPuq+fvgMZ+bNs6C
bPD9qr3rKD2DDQNb/fRZ/NK6QGg8/meQdLlgSHyoA7bL3yGsL0vk5fEUIpIfYp55
cwIDAQABMA0GCSqGSIb3DQEBBQUAA4GBAFhc477qkTMg3KOqfzRDb8s39i22x4RZ
6DdoLib9JyAtg8TYS4F+hutxi0kFJ1sz7LQ0vUp2QuKBrLwWyOXbLzmZXtfTX//P
0rHLhgjPxPtQo9DYQI8NPqSrDM+dX4TflmaBBkfAkFbV3PVVp5NUqCLtUdcuLNhk
BNH/pCxU55Kz
-----END CERTIFICATE-----

openssl x509 -noout -in cert.pem -fingerprint -sha1

SHA1 Fingerprint=21:09:F5:1A:7F:05:E5:A0:82:5B:E6:DC:23:EB:BC:D8:B9:7A:B4:DE

Use it in code:

HTTPClient http;
http.begin("https://kotopeky.cz/api/kotinode/heating/schedule/raw","21:09:F5:1A:7F:05:E5:A0:82:5B:E6:DC:23:EB:BC:D8:B9:7A:B4:DE");