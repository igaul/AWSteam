/* ESP8266 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP8266 with AWS IoT.
 * No messing with openssl or spiffs just regular pubsub and certificates in string constants
 * 
 * This is working as at 3rd Aug 2019 with the current ESP8266 Arduino core release:
 * SDK:2.2.1(cfd48f3)/Core:2.5.2-56-g403001e3=20502056/lwIP:STABLE-2_1_2_RELEASE/glue:1.1-7-g82abda3/BearSSL:6b9587f
 * 
 * Author: Anthony Elder 
 * Modified by Stephen Borsay for Serverless AWS IoT on Udemy.com
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

const char* ssid = "iPhone";
const char* password = "burger888";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint 
const char* awsEndpoint = "a3arn02oc6oedk-ats.iot.us-west-2.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(

MIIDWTCCAkGgAwIBAgIUaW4uuZg5LHJQN8qARZMmQ7fpoxgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDcxMTE4MjUy
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAN7KRLMMELWeGgTFQyVP
jr1o95ybnFh6+E1Y04ihAPQDWcZ7PNDV5YX1sChejnQatn8DtV+89udgt6a8TJIy
6rBGn/TV2nHlZgfMLItsR7f0JQ6/5zPdw8PYORgCRt4YbcNmdqErQu/cHKIDg9uT
kk2bGDeCY9UlKFcC/WxywQ+xFrvztvWrYYxZ8ZJUubSiK3t4idKg+8I20VkzFOd0
Oh5Q037+7YbniMrvCis0Y74Y0GvDhTHtQzIbj/xBIWFYj+oa1M74NG0xszhEcO0A
687tJo+Zce8Os18o25a9Qo4Ae+oTKwtn7UhbDi/AFYgCwtkG+oB63Wg/zqhyPSmb
bGECAwEAAaNgMF4wHwYDVR0jBBgwFoAUr99IC1ZCWi3r4pkDetDoiyazAMkwHQYD
VR0OBBYEFDImjS8tFa8BRBSa/couG+mwhLW2MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQC+nMWXYmtQJllhZi/NxU9trYvq
MIBfIn36i/6gv2ThAaJ7JwXa96u4hC/C5oswMfc6QD5su1kA1Ew7ve5suAG8OvSW
rv4/PtfUKeV9ewi7T6zWEm6ZsN0zDM78e02z0am86eXLNysD4KWLie79SWSzyVsJ
6XXctqudReUQa3igZTtE+uUIsVN0fXsTomOpHIOt1auRRTizCf7ZJgXBign0rMGG
7DABiHQLXHgrkR5sowNR8gVZZP3MlHAd6Qb8iiBSwlsGef0gpMIZibZ3jmdulC+u
8lwl2gaKOEF1tmDZpInmirfFiPqaUZi0iDiBlx1F1WFN4au/heYXniXEnVeu

)EOF"; 
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(

MIIEpAIBAAKCAQEA3spEswwQtZ4aBMVDJU+OvWj3nJucWHr4TVjTiKEA9ANZxns8
0NXlhfWwKF6OdBq2fwO1X7z252C3prxMkjLqsEaf9NXaceVmB8wsi2xHt/QlDr/n
M93Dw9g5GAJG3hhtw2Z2oStC79wcogOD25OSTZsYN4Jj1SUoVwL9bHLBD7EWu/O2
9athjFnxklS5tKIre3iJ0qD7wjbRWTMU53Q6HlDTfv7thueIyu8KKzRjvhjQa8OF
Me1DMhuP/EEhYViP6hrUzvg0bTGzOERw7QDrzu0mj5lx7w6zXyjblr1CjgB76hMr
C2ftSFsOL8AViALC2Qb6gHrdaD/OqHI9KZtsYQIDAQABAoIBAQCD1LSqLi2FCrTi
tcdxwI36Tq6WoQ323W0od7NUWdEykvkAldP6z1ct2aAPg0RQN65dolgk+rAvYe/K
XgoBSNfOzzu/qPvGAbRKUOra5YvaskFrZOruqf85WC0Qo+hDa7knsu3nmK3XOSxI
QkQ4Hi6odWnzDtj8OyyPsFk9XvoWydyoQePw7k9i07U4Gdki7yFrguLPPwiwB2P9
CgdTtVX8sG6nouFznFKRMJD9fhE3dHaWW+V/Fw3Ir2IE+8qntP5kudTs/FFk5Gzy
TyEVo9OBQTK6X4W0Gq2BXuoPXN8Y8NhXrbhvj/5+jNmd8dEZitqYgA8l1HqXmZUn
bD1ATiXxAoGBAPV1m1L9WKePs09MY1SlG5u3n2yTejod0XyxKvzBsG0unRyYhzV5
kZVra1Im0vWyrK71Rh1bHAQuRWjdRvzFrtdQkVoHe51bAQQPMCqvq213jf9Ok3m0
jZf87S0Z54+n4WgG2aftyFctYvyrODAS15R62p9zdBRqhNO8fp78gerXAoGBAOhb
c+dRbSf78aa16GiliGGXTNwrd5wt3JPDKbt6ySZOHJMRVv7a1kQwn9N90zgWojIG
26W2rawfqPtWp3GcAjEMtzqswZkYpcDjBnfnKtCc/3ygGBwQJcleS8QmwqTx5gib
6qN67+Xb8ZkXPicGxKXvhGt9LCwdyQybTOqW9nOHAoGAKo4DKbYmgpZVPfA+YFRn
nu2MxCQlUqIMyZNlooBp9+PW9Tg0aL700HTZHFsCw/oAGi0HxNsg1lC3U+DZB8W1
9DpRmMwnUDaQzIVdkmj2TXomshaLVtRIoAsHi4QsMyml2IUngwekbgi+WpPMn/kE
Swepk29OtqNo99R8F1Arre0CgYAE+lCVWA06mg8nPyZnd4RgctlWrie11NWw5LF0
LsE4bNv32ps+gnmspE4IPd3UiceXwsXZ2vwFf/giykx0Bp+VZWxXeXP8T6bdZv+w
eedU6MggZ12c3Euy8K422Y258uS2soBoWC3jgr6+RN00ANUdqO/xTZEwsMKp3nnb
XLiS6QKBgQCghyPQq/xmYDkozrByXmeekuQHNJki6OlsoGrAs4/s3dL9qLCDV3dG
D6+o4g7oxhUUQ1ezjCm9P/TSBVJ91VIjhJLbU1Lj4zvnMD9m233gLeteDXTX/WS6
FitDvtp1Go2LixEbERk6+RC7HQfwmmfCUVnesvkvaun+iHi9VzWzLg==

)EOF";
//-----END RSA PRIVATE KEY-----

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy" \
"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

char fakeData[256];
char *Parks[] = {"Alberta Park", "Laurelhurst Park", "Japanese Gardens","Forest Park"};
// int myPins[] = {2, 4, 8, 3, 6};
const int Object_Id[] = {320, 91, 253, 5, 47, 59, 125, 28, 175, 298, 13,
                         175, 148, 217, 68, 224, 245, 168, 147, 177, 53,
                         223, 183, 216, 176, 107, 180};
                         
int parkNumber = random(0,25);  //number of park ID's
int population =  random(100,2000);  
int density = random(0,10); //density level

snprintf(fakeData, sizeof(fakeData),"{\"Object_Id\":%d,\"Density\":%d}", Object_Id[parkNumber], density);


  if (millis() - lastPublish > 10000) {
   // String msg = String("Hello from ESP8266: ") + ++msgCount;
    pubSubClient.publish("outTopic",fakeData); //shadow thing if needed = $aws/things/ParkDensity/shadow/update
    Serial.println(fakeData);
   // Serial.print("Published: "); Serial.println(msg);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}

int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
