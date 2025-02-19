//Instalar librería: PubSubClient
//El autor de la librería PubSubClient es Nick O'Leary
//https://c2plabs.com/blog/2022/01/02/esp8266-mqtt-client-tutorial-using-pubsubclient-library/
//https://www.youtube.com/watch?v=WQURoN5IXf0

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>


char *server = "192.168.18.149";
int port = 1883;
//content of: ca.crt
static const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDuzCCAqOgAwIBAgIUU+tRYtIX4/jLFuMagk5wupreKjkwDQYJKoZIhvcNAQEL
BQAwbTELMAkGA1UEBhMCU0UxEjAQBgNVBAgMCVN0b2NraG9sbTESMBAGA1UEBwwJ
U3RvY2tob2xtMRAwDgYDVQQKDAdoaW1pbmRzMQswCQYDVQQLDAJDQTEXMBUGA1UE
AwwOMTkyLjE2OC4xOC4xNDkwHhcNMjUwMjE4MjMyMzE4WhcNMjUwMjE5MjMyMzE4
WjBtMQswCQYDVQQGEwJTRTESMBAGA1UECAwJU3RvY2tob2xtMRIwEAYDVQQHDAlT
dG9ja2hvbG0xEDAOBgNVBAoMB2hpbWluZHMxCzAJBgNVBAsMAkNBMRcwFQYDVQQD
DA4xOTIuMTY4LjE4LjE0OTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
ANFvAWOG9k63qV5jh6CGTrkQhHk3AY0mwDlWV2xGr9wXu1RM7VNIBlNFFpY3rSku
oSvclEMJOsfMDYDVkY2bqb0cXcDA9zv5xLpgWGl2dBdr1KoCgFV1rncCSn2IMnTk
xMIsgXH3KmK1OXTQlsB3Fd0JQLBpCFxkeJyZI+HAXTcU1aogslgBBuRo3SPc6yLZ
xn47Wt+0Gdd+IQeIa2lk/Jx453C2w0gfVY0kSlJbmbFiePvBjIMGJPlBs1v4co29
LkEm/HvL2+JeaSAjuJ7PPLe7Qadb9g4Dzg7NWQk4xYhPZ/xm5gs4W+4GEkNz5OfR
sYjruLGrPKTyVsJPA6VAZ90CAwEAAaNTMFEwHQYDVR0OBBYEFJovWLV1AR4YovUc
vlrOUXLoxbBLMB8GA1UdIwQYMBaAFJovWLV1AR4YovUcvlrOUXLoxbBLMA8GA1Ud
EwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAFfckifhr6ateW3qNC1gVd1Q
WbLDwXN8qfgtPZT2tj2bHsUBNCmBoccp13iustTJlSPlSVySOuvFOfIOqy52/BLW
xJLAwkFgXm6YHaJckmt+HRPHJCoHNh+094r3LugBRaA/qKq/22fCLwiXbpD44EE6
54IBl6J95ptg+dO3LXxRGXJDUfLpk68+0cC9zbRaSmF/cyT8O53RivpdIGrqA7xH
eR18JyvJ5pYlGis7Z6lrOwRdOwdXUnsuaD+tERXISV+RNqsG1HbpcaoiYGEGhNIa
zoQ08WEHOXHat9hamskpbUa3v0REZ+x7akkjU4FiSDTFITAgOO0mIz1zGcuooxw=
-----END CERTIFICATE-----
)EOF";

//content of: client.crt
static const char client_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDZTCCAk0CFBRFrcfYd+asUcSl05Wr6YXyEqU4MA0GCSqGSIb3DQEBCwUAMG0x
CzAJBgNVBAYTAlNFMRIwEAYDVQQIDAlTdG9ja2hvbG0xEjAQBgNVBAcMCVN0b2Nr
aG9sbTEQMA4GA1UECgwHaGltaW5kczELMAkGA1UECwwCQ0ExFzAVBgNVBAMMDjE5
Mi4xNjguMTguMTQ5MB4XDTI1MDIxODIzMjMyNloXDTI1MDIxOTIzMjMyNlowcTEL
MAkGA1UEBhMCU0UxEjAQBgNVBAgMCVN0b2NraG9sbTESMBAGA1UEBwwJU3RvY2to
b2xtMRAwDgYDVQQKDAdoaW1pbmRzMQ8wDQYDVQQLDAZDbGllbnQxFzAVBgNVBAMM
DjE5Mi4xNjguMTguMTQ5MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA
woCGHkyJdhys7boMnM95LQIOrMsd6Lnw+0RHN9AJVw7wecDPiXA55C2gi8FqboiY
t+26Y+/6E3wUGkFweQh7v7NuCC8kpPRUn/IouTKs3vpXP20gX2+3Yd21kBVUyBeP
Ci+GqYaK+3xYRLu95aePK5nurTwrcXQxdkImC/mXk/JQixaNPpSbaMufrUn+lZHe
LL/CXxZJyfWTK2B5doJMqU8nyYl3gt5ucnnYZ86KlpGvbAExO579pxOtR+b8GsUT
WaOAXt35j3etZdqSAZddAmmg7F5TPCbed1hx0jBY7iRxn1QGClBfQanYJ4dumpt/
QP43/d2VzhTgUqHla/zG2QIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQBRczuQWj87
JV5Ey+1VWC7y+oiWdzdM6rx+W4Y9IA9kKJYFmDMVwzJlSZo2Z7vMJ+rLCKL7seqO
XrqcyN08nlfTCYW3LEehX2pmUyR+Exi5NneV4ETbWB8hH8Ddbne4ydsPvHP7J6RE
yQuuVG7mIqF8SplEiKscwgXQ8VczW7428wWt6Othpik7Z1GJghIRjUEHwnIUP1pn
papFH8sEUhM/hVo3o+TTv9yJKIHDvcdnZNRklwxWyWdT5AbkcJjiW2kOEaOK+WcT
3h/W6A71+wKCfQjcczQj1c/G6HFHvJ1x8528NQmdXAGNrfRjvuMmLc4AoU8L4wcU
C5Vc/v/z7kgu
-----END CERTIFICATE-----
)EOF";


//content of: client.key
static const char client_key[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDCgIYeTIl2HKzt
ugycz3ktAg6syx3oufD7REc30AlXDvB5wM+JcDnkLaCLwWpuiJi37bpj7/oTfBQa
QXB5CHu/s24ILySk9FSf8ii5Mqze+lc/bSBfb7dh3bWQFVTIF48KL4aphor7fFhE
u73lp48rme6tPCtxdDF2QiYL+ZeT8lCLFo0+lJtoy5+tSf6Vkd4sv8JfFknJ9ZMr
YHl2gkypTyfJiXeC3m5yedhnzoqWka9sATE7nv2nE61H5vwaxRNZo4Be3fmPd61l
2pIBl10CaaDsXlM8Jt53WHHSMFjuJHGfVAYKUF9Bqdgnh26am39A/jf93ZXOFOBS
oeVr/MbZAgMBAAECggEAAJAqMz7B/CFPn92UyURxg5+YpsfsxX56U+Pp99+mk0W0
DQoxn6J/TGmUaCsucXT8SjYKPt1UQkeu8DeFyZfscczlcekfCEo7z32+P/EFXpzn
cUPR3uzeIU6oxqrFJNIqpBsonueIkftVbxeDEH3odHYc4owEzS6KsR62aPz3QyKk
bawTNDHv/7atSuboVNWQyhghbC135W33U6RxhHquxOfJudKCvM6V4aPPiNL+Hf+c
14OsOQcY5EKJHdTq1q8Z25jtU5lCIQzcREpSNwJzIDSt6WqUJw4X2sBg9ZOFevw1
5K0MP4qzO05N2rGYKdBXCXJx3p3Fr++rd9Cc2N1mXQKBgQDkhlt+88D9bxWctP3A
s8wkdBSMQgcVfaviPbtm+72fHaiJVueINkGPkdubquetHy1TZvYRTuzFwoShjBap
+yVQfILbbQQ1Yxxc5ClJ9y3zbdTpQCV7syobjpYAZNI9GXLxpUMW1Th5HnNBL6hj
MDTiGUfr560N4QOMGSUxdkaydQKBgQDZ4v9a1D+fy5pYiLk1H6J7G5n8skl0hSM7
aA1qRzsRQ2UCIrbECQ6efTJoZzHSWRBA3Y+yCUViPwyu0l3AR4FZXOlum2QVYVMh
FU4cwlkKQYjA/NifXXuz8OCDs4yT5UzQvcV8EmwwVRVR09nXQpi7zFURp15UYJzB
3PY+nViuVQKBgE/6FZtgg3IupkmprgT65776QRa1JPWIfdc385YfFEXTg2BNYcpx
8o/kZA8eyOX1u08tAYEdNhAf+3ruJCBpHK4GZWWVyZtFjG+WCj0wnIxNK+5V3Mhl
856hUC3jGxD3Dxsy0z/yb0k0NvHhU/Uisd9PKbSN6CqrTSqCwlmFI0jhAoGBAMlz
H+cpkD288FJk7uAYBJDP/zps2I9bvSxi0uxtOUaHyi8ES3jKavLMDDwNFTpHUEi4
0O+ptLXWV3KXW0i4k/Y8ejLihtFhr4tJ+HOMmOhrmPXdkKcftoyy69G4wwPGoJPq
sVk6az7wm4qRVbq5fx87F3gAB635uVVgZ2eS8PgVAoGBANyUQuAFTHdw2h4PDCFK
kHXAoMcdf+XsqAHTvI9k4n1YMM0dGIDFBazsoqqAL8jBJcpx0qUJRVH2g6YOx8mN
7CVcHfTaKZ8IkVgXjrBKm8fJS8tI6PD26m5RkA+Dsj5ooFVL/uLzK8mnvdkLHtZO
In2nOojDc5zpAxiuIWr7OpZe
-----END PRIVATE KEY-----
)EOF";



// Credenciales MQTT
const char *mqtt_user = "admin";      // Reemplaza con tu usuario MQTT
const char *mqtt_password = "123456";    // Reemplaza con tu contraseña MQTT

const char *deviceID = "ESP32-Client";

const char *ssid="WILLIAN-2.4G-pJAt";
const char *passwd = "942900777";

char serial_command = -1;
unsigned long previousMillis = 0;
unsigned long interval = 30000;


WiFiClientSecure mqttClient;
PubSubClient client(mqttClient);


void mqttCallback(char *topic, byte *payload, unsigned int length) {
  Serial.println ("Message arrived on Topic:");
  Serial.println (topic);

  char message[5]={0x00};

  for(int i=0;i<length;i++)
  message[i]=(char)payload[i];

  message[length]=0x00;
  Serial.println (message);
 String str_msg = String(message);
 if(str_msg.equals ("ON")) {
  
   Serial.println(str_msg);
   
 }else if (str_msg.equals("OFF")) {
  
   Serial.println(str_msg);
   
 }

}

void setup() {
  Serial.begin (115200);
  WiFi.begin(ssid,passwd);

  Serial.print ("Connecting to AP");
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println ("Connected to WiFi AP, Got an IP address :");
  Serial.println (WiFi.localIP());
  //Set The ESP8266 tries to reconnect automatically when the connection is lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

    // Setear certificados
    mqttClient.setCACert(ca_cert);
    mqttClient.setCertificate(client_cert);
    mqttClient.setPrivateKey(client_key);

  // Configuración del servidor MQTT
  client.setServer(server, port);
  client.setCallback(mqttCallback);

  // Conectar al broker MQTT con usuario y contraseña
  if (client.connect(deviceID)) {
    Serial.println("Connected to MQTT Broker");
  } else {
    Serial.println("MQTT Broker connection failed");
    Serial.println(client.state());
    delay(200);
  }
 
  //suscribirse a los temas.
  client.subscribe("/casa/foco");
  client.subscribe("/galpon/foco");

}

void loop() {

 
  printWifiStatus();

  if(Serial.available()>0){

     serial_command = Serial.read();
     if(serial_command == '1'){
        //Publicar mensaje en un topico:
        //QOS del PubSubClient es = 0
        client.publish("/casa/foco", "El foco se prendio");
     }
  }

  //verificar si no hay conexión con el broker, si es así reconectarse:
   if(!client.connected()) {
      reconnect();
  }


  
  client.loop();
}


void reconnect() {
  while (!client.connected()) {
    Serial.println("Trying to connect to the MQTT broker...");

    if (client.connect(deviceID)) {
      Serial.println("Connected to MQTT Broker");

      //suscribirse a los temas.

       client.subscribe("/casa/foco");
       client.subscribe("/galpon/foco");

      
    } else {
      Serial.print("Fallo, rc=");
      Serial.println(client.state());
      Serial.println("Trying to connect each 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    printWifiStatus();
  }
}


void printWifiStatus(){
      //print the Wi-Fi status every 30 seconds
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >=interval){
      switch (WiFi.status()){
        case WL_NO_SSID_AVAIL:
          Serial.println("Wifi Configured SSID cannot be reached");
          break;
        case WL_CONNECTED:
          Serial.println("Connection Wifi successfully established");
          break;
        case WL_CONNECT_FAILED:
          Serial.println("Wifi Connection failed");
          break;
      }
      Serial.printf("Connection status: %d\n", WiFi.status());
      Serial.print("RRSI: ");
      Serial.println(WiFi.RSSI());
      previousMillis = currentMillis;
  }
  }
