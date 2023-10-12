///////////////////////////////////////////////////////////////////////////////////
//         Change these variables directly in the code or use the config         //
//  form in the web-installer https://lnbits.github.io/bitcoinswitch/installer/  //
///////////////////////////////////////////////////////////////////////////////////

String ssid = "null"; // 'String ssid = "ssid";' / 'String ssid = "null";'
String wifiPassword = "null"; // 'String wifiPassword = "password";' / 'String wifiPassword = "null";'

// String from the lnurlDevice plugin in LNbits lnbits.com
String switchStr = "null"; // 'String switchStr = "ws url";' / 'String switchStr = "null";'

// Change for threshold trigger only
String wallet; // ID for the LNbits wallet you want to watch,  'String wallet = "walley ID";' / 'String wallet = "null";'
long threshold; // In sats, 'long threshold = 0;' / 'long threshold = 100;'
int thresholdPin; // GPIO pin, 'int thresholdPin = 16;' / 'int thresholdPin;'
long thresholdTime; // Time to turn pin on, 'long thresholdTime = 2000;' / 'long thresholdTime;'

///////////////////////////////////////////////////////////////////////////////////
//                                 END of variables                              //
///////////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>

fs::SPIFFSFS &FlashFS = SPIFFS;
#define FORMAT_ON_FAIL true
#define PARAM_FILE "/elements.json"

String urlPrefix = "ws://";
String apiUrl = "/api/v1/ws/";

// length of lnurldevice id
// 7dhdyJ9bbZNWNVPiFSdmb5
int uidLength = 22;

String payloadStr;
String lnbitsServer;
String deviceId;
String dataId;
bool paid;
bool down = false;
long thresholdSum = 0;
long payment_amount = 0;

// Serial config
int portalPin = 4;

WebSocketsClient webSocket;

struct KeyValue {
    String key;
    String value;
};

void setup()
{
    Serial.begin(115200);
    bool triggerConfig = false;
    pinMode (2, OUTPUT); // To blink on board LED
    FlashFS.begin(FORMAT_ON_FAIL);
    int timer = 0;
    while (timer < 2000)
    {
        digitalWrite(2, HIGH);
        Serial.println(touchRead(portalPin));
        if (touchRead(portalPin) < 60)
        {
            triggerConfig = true;
            timer = 5000;
        }

        timer = timer + 100;
        delay(150);
        digitalWrite(2, LOW);
        delay(150);
    }

    readFiles(); // get the saved details and store in global variables

    if(triggerConfig == true || ssid == "" || ssid == "null"){
        Serial.println("Launch serial config");
        configOverSerialPort();
    }
    else{
        WiFi.begin(ssid.c_str(), wifiPassword.c_str());
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
            digitalWrite(2, HIGH);
            Serial.print(".");
            delay(500);
            digitalWrite(2, LOW);
        }
    }

    if(threshold != 0){ // Use in threshold mode
        Serial.println("");
        Serial.println("Using threshold mode");
        Serial.println("Connecting to websocket: " + urlPrefix + lnbitsServer + apiUrl + wallet);
        webSocket.beginSSL(lnbitsServer, 443, apiUrl + wallet);
    }
    else{ // Use in normal mode
        Serial.println("");
        Serial.println("Using normal mode");
        Serial.println("Connecting to websocket: " + urlPrefix + lnbitsServer + apiUrl + deviceId);
        webSocket.beginSSL(lnbitsServer, 443, apiUrl + deviceId);
    }
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(1000);
}

void loop() {
    while(WiFi.status() != WL_CONNECTED){ // check wifi again
        Serial.println("Failed to connect");
        delay(500);
    }
    digitalWrite(2, LOW);
    payloadStr = "";
    delay(2000);
    while(paid == false){ // loop and wait for payment
        webSocket.loop();
        if(paid){
            if(threshold != 0){ // If in threshold mode we check the "balance" pushed by the websocket and use the pin/time preset
                StaticJsonDocument<1900> doc;
                DeserializationError error = deserializeJson(doc, payloadStr);
                if (error) {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.c_str());
                    return;
                }
                JsonObject payment = doc["payment"];
                payment_amount = payment["amount"];
                thresholdSum = thresholdSum + payment_amount;
                Serial.println("thresholdSum: " + String(thresholdSum));
                Serial.println("threshold: " + String((threshold * 1000)));
                Serial.println("thresholdPin: " + String(thresholdPin));
                if(thresholdSum >= (threshold * 1000)){
                    pinMode (thresholdPin, OUTPUT);
                    digitalWrite(thresholdPin, HIGH);
                    delay(thresholdTime);
                    digitalWrite(thresholdPin, LOW);
                    thresholdSum = 0;
                }
            }
            else{ // If in normal mode we use the pin/time pushed by the websocket
                pinMode(getValue(payloadStr, '-', 0).toInt(), OUTPUT);
                digitalWrite(getValue(payloadStr, '-', 0).toInt(), HIGH);
                delay(getValue(payloadStr, '-', 1).toInt());
                digitalWrite(getValue(payloadStr, '-', 0).toInt(), LOW);
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

String getJsonValue(JsonDocument &doc, const char* name)
{
    for (JsonObject elem : doc.as<JsonArray>()) {
        if (strcmp(elem["name"], name) == 0) {
            String value = elem["value"].as<String>();
            return value;
        }
    }
    return "";  // return empty string if not found
}

void readFiles()
{
    File paramFile = FlashFS.open(PARAM_FILE, "r");
    if (paramFile)
    {
        StaticJsonDocument<2500> doc;
        DeserializationError error = deserializeJson(doc, paramFile.readString());
        if(error){
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }
        if(ssid == "null"){ // check ssid is not set above
            ssid = getJsonValue(doc, "ssid");
            Serial.println("");
            Serial.println("ssid used from memory");
            Serial.println("SSID: " + ssid);
        }
        else{
            Serial.println("");
            Serial.println("ssid hardcoded");
            Serial.println("SSID: " + ssid);
        }
        if(wifiPassword == "null"){ // check wifiPassword is not set above
            wifiPassword = getJsonValue(doc, "wifipassword");
            Serial.println("");
            Serial.println("ssid password used from memory");
            Serial.println("SSID password: " + wifiPassword);
        }
        else{
            Serial.println("");
            Serial.println("ssid password hardcoded");
            Serial.println("SSID password: " + wifiPassword);
        }
        if(switchStr == "null"){ // check switchStr is not set above
            switchStr = getJsonValue(doc, "socket");
            Serial.println("");
            Serial.println("switchStr used from memory");
            Serial.println("switchStr: " + switchStr);
        }
        else{
            Serial.println("");
            Serial.println("switchStr hardcoded");
            Serial.println("switchStr: " + switchStr);
        }

        int protocolIndex = switchStr.indexOf("://");
        if (protocolIndex == -1) {
            Serial.println("Invalid switchStr: " + switchStr);
            return;
        }
        urlPrefix = switchStr.substring(0, protocolIndex + 3);

        int domainIndex = switchStr.indexOf("/", protocolIndex + 3);
        if (domainIndex == -1) {
            Serial.println("Invalid switchStr: " + switchStr);
            return;
        }

        lnbitsServer = switchStr.substring(protocolIndex + 3, domainIndex);
        apiUrl = switchStr.substring(domainIndex, switchStr.length() - uidLength);
        deviceId = switchStr.substring(switchStr.length() - uidLength);

        Serial.println("LNbits ws prefix: " + urlPrefix);
        Serial.println("LNbits server: " + lnbitsServer);
        Serial.println("LNbits api url: " + apiUrl);
        Serial.println("Switch device ID: " + deviceId);
    }
    paramFile.close();
}

//////////////////WEBSOCKET///////////////////

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            Serial.printf("[WSc] Connected to url: %s\n",  payload);
            webSocket.sendTXT("Connected"); // send message to server when Connected
            break;
        case WStype_TEXT:
            payloadStr = (char*)payload;
            payloadStr.replace(String("'"), String('"'));
            payloadStr.toLowerCase();
            Serial.println("Received data from socket: " + payloadStr);
            paid = true;
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}
