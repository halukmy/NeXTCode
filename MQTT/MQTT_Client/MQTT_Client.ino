 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <dht11.h>

#include <ESP8266WebServer.h>

#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN  4
 

#include "floatToString.h" 

// Update these with values suitable for your network.


const char* ssid = "sungerbob";
const char* password = "halukbinreyhan";
const char* mqtt_server = "192.168.0.12";

//dht11 DHT12;

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266
 
float humidity, temp_f;  // Values read from sensor
String webString="";     // String to display
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

 void handle_root() {
  server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
  delay(100);
}


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

   
 
  
  server.begin();
  Serial.println("HTTP server started");
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...


      
      client.publish("outTopic", "Bağlandım :)"); //"hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}




void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
   dht.begin(); 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);



 Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  
     server.on("/", handle_root);


  server.on("/temp", [](){  // if you add this subdirectory to your webserver call, you get text below :)




  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
 
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    
    float bd = dht.readTemperature(true);     // Read temperature as Fahrenheit

    temp_f =  (bd-32) * 5/9;  // (5/9) * (bd-32);
    
   // temp_f = bd;
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }

    webString="Temperature: "+String((int)temp_f)+" C";   // Arduino has a hard time with float to string
    server.send(200, "text/plain", webString);            // send to someones browser when asked
  });
}


#include "DHT.h"

#define DHTPIN 4     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

  dht.begin();

void loop() {

    

   
  server.handleClient();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;





  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0); 
  lcd.print("Temp = ");
  lcd.print(DHT.temperature);
  lcd.setCursor(0,1);
  lcd.print("Humidity = ");
  lcd.print(DHT.humidity);
  delay(1000);



 

    
    snprintf (msg, 75, "Derece :  #%ld", temp_f);// (float)DHT12.humidity, 2);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);

  }
}
