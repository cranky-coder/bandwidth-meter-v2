#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

const char* mqtt_server = "192.168.2.224";
const String topic = "bandwidth/#";    // rhis is the [root topic]

WiFiClient espClient;
PubSubClient client(espClient);

long target_up;
long target_down;
long tmp_up;
long tmp_down;
long dwbw_raw;
 long upbw_raw;
 long dwbw_mapped;
 long upbw_mapped;
//bandwidth in megabit
const long max_bw_down = 275.0 * 1024 * 1024;
//const long max_bw_up = 25.0 * 1024 * 1024;
long max_bw_up = max_bw_down;

unsigned long previousMillis = 0; 
//how long to wait between moving to the next iteration in smoothing
const long interval = 1000; 


int status = WL_IDLE_STATUS;     // the starting Wifi radio's status

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char buff_p[length];
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    buff_p[i] = (char)payload[i];
  }
  buff_p[length] = '\0';
  String s = String(buff_p);
  
  if(strcmp(topic,"bandwidth/up")==0)
  {
    // upload (lets put this on the first meter
    //0v  - 5v
    //0mb - 300mb
   
    upbw_raw = s.toInt();
    upbw_raw = constrain(upbw_raw,0,max_bw_up+1000000);
    
    
    
   
    upbw_mapped=mymap(upbw_raw,0,max_bw_up,0,1024);
    //upbw_mapped=constrain(upbw_mapped,0,max_bw+10000);
    Serial.print(upbw_raw);
    Serial.print("  ");
    Serial.println(upbw_mapped);
    analogWrite(D1,upbw_mapped);
  }

  if(strcmp(topic,"bandwidth/down")==0)
  {
    // upload (lets put this on the first meter
    //0v  - 5v
    //0mb - 300mb
    
    dwbw_raw = s.toInt();
    dwbw_raw = constrain(dwbw_raw,0,max_bw_down+1000000);
    
    dwbw_mapped=mymap(dwbw_raw,0,max_bw_down,0,1024);
    //dwbw_mapped=constrain(dwbw_mapped,0,max_bw+10000);
    Serial.print(dwbw_raw);
    Serial.print("  ");
    Serial.println(dwbw_mapped);
    analogWrite(D2,dwbw_mapped);
  }  


  
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

String composeClientID() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String clientId;
  clientId += "esp-";
  clientId += macToStr(mac);
  return clientId;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = composeClientID() ;
    clientId += "-";
    clientId += String(micros() & 0xff, 16); // to randomise. sort of

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      // topic + clientID + in
      client.subscribe(topic.c_str() );
      Serial.print("subscribed to : ");
      Serial.println(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" wifi=");
      Serial.print(WiFi.status());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long mymap(float x, float in_min, float in_max, float out_min, float out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // put your setup code here, to run once:

  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.print("Max Bandwidth_Up:");
  Serial.println(max_bw_up);
  Serial.print("Max Bandwidth_Down:");
  Serial.println(max_bw_down);
  
    
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //Serial.println(dwbw_mapped);
  //Serial.println(upbw_mapped);
  
  analogWrite(D2,dwbw_mapped);
  analogWrite(D1,upbw_mapped);
  
  
 
}
