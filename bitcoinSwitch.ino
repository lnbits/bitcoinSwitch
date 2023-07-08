#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>

fs::SPIFFSFS &FlashFS = SPIFFS;
#define FORMAT_ON_FAIL true
#define PARAM_FILE "/elements.json"

////////////////////////////////

String wallet;
int threshold = 0;
int thresholdSum = 0;
int thresholdPin = 0;
int thresholdTime = 0;

/////////////////////////////////

// Access point variables
String payloadStr;
String password;
String serverFull;
String lnbitsServer;
String ssid;
String wifiPassword;
String deviceId;
String dataId;
String lnurl;

bool paid;
bool down = false;

WebSocketsClient webSocket;

struct KeyValue {
  String key;
  String value;
};

void setup()
{
  Serial.begin(115200);
  pinMode (2, OUTPUT);

  FlashFS.begin(FORMAT_ON_FAIL);

  // get the saved details and store in global variables
  readFiles();

  WiFi.begin(ssid.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED && timer) {
    Serial.println("Connecting to WiFi");
    delay(500);
    digitalWrite(2, HIGH);
    Serial.print(".");
    delay(500);
    digitalWrite(2, LOW);
  }
  if(threshold != 0){
    Serial.println(lnbitsServer + "/api/v1/ws/" + wallet);
    webSocket.beginSSL(lnbitsServer, 443, "/api/v1/ws/" + wallet);
  }
  else{
    Serial.println(lnbitsServer + "/api/v1/ws/" + deviceId);
    webSocket.beginSSL(lnbitsServer, 443, "/api/v1/ws/" + deviceId);
  }

  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(1000);
}

void loop() {
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed to connect");
    delay(500);
  }
  digitalWrite(2, LOW);
  payloadStr = "";
  delay(2000);
  while(paid == false){
    webSocket.loop();
    if(paid){
      if(threshold == 0){
        pinMode(getValue(payloadStr, '-', 0).toInt(), OUTPUT);
        digitalWrite(getValue(payloadStr, '-', 0).toInt(), HIGH);
        delay(getValue(payloadStr, '-', 1).toInt());
        digitalWrite(getValue(payloadStr, '-', 0).toInt(), LOW);
      }
      else{
        StaticJsonDocument<2000> doc;
        DeserializationError error = deserializeJson(doc, input);
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }
        int payment_amount = payment["amount"];
        thresholdSum = thresholdSum + payment_amount
        if(threshold >= thresholdSum){
          pinMode (thresholdPin, OUTPUT);
          digitalWrite(thresholdPin, HIGH);
          delay(thresholdTime);
          digitalWrite(thresholdPin, LOW);
        }
      }
    }
  }
  Serial.println("Paid");
  paid = false;
}

//////////////////HELPERS///////////////////


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void readFiles()
{
  File paramFile = FlashFS.open(PARAM_FILE, "r");
  if (paramFile)
  {
    StaticJsonDocument<1500> doc;
    DeserializationError error = deserializeJson(doc, paramFile.readString());

    const JsonObject maRoot0 = doc[0];
    const char *maRoot0Char = maRoot0["value"];
    password = maRoot0Char;
    Serial.println(password);

    const JsonObject maRoot1 = doc[1];
    const char *maRoot1Char = maRoot1["value"];
    ssid = maRoot1Char;
    Serial.println(ssid);

    const JsonObject maRoot2 = doc[2];
    const char *maRoot2Char = maRoot2["value"];
    wifiPassword = maRoot2Char;
    Serial.println(wifiPassword);

    const JsonObject maRoot3 = doc[3];
    const char *maRoot3Char = maRoot3["value"];
    serverFull = maRoot3Char;
    lnbitsServer = serverFull.substring(5, serverFull.length() - 33);
    deviceId = serverFull.substring(serverFull.length() - 22);

    const JsonObject maRoot4 = doc[4];
    const char *maRoot4Char = maRoot4["value"];
    lnurl = maRoot4Char;
    Serial.println(lnurl);
  }
  paramFile.close();
}

//////////////////NODE CALLS///////////////////

void checkConnection(){
  WiFiClientSecure client;
  client.setInsecure();
  const char* lnbitsserver = lnbitsServer.c_str();
  if (!client.connect(lnbitsserver, 443)){
    down = true;
    return;   
  }
}

//////////////////WEBSOCKET///////////////////

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                Serial.printf("[WSc] Connected to url: %s\n",  payload);

          // send message to server when Connected
        webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
            payloadStr = (char*)payload;
            if(threshold != 0){
              payloadStr.replace("'", '"')
            }
            paid = true;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }
}