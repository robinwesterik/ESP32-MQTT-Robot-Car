#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// MQTT data
const char* mqttServer = "public.mqtthq.com";
const char* mqttUsername = "esp32robot";
const char* mqttMovTopic = "esp32robot/movement";

//Instantiation of clients and message arrays
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Right Motor
int motorRPin1 = 27; 
int motorRPin2 = 26; 

// Left Motor
int motorLPin1 = 33; 
int motorLPin2 = 32; 

void rStop()
{
  // Stop the right motor
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, LOW); 
}

void lStop()
{
  // Stop the left motor
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, LOW); 
}

void stop()
{
  //Stop both motors
  rStop();
  lStop();
}

void rForward()
{
  // Move the right motor forward at maximum speed
  rStop();
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH); 
}

void lForward()
{
  // Move the left motor forward at maximum speed
  lStop();
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH); 
}

void forward()
{
  //Move both motors forward
  rForward();
  lForward();
}

void rBackward()
{
  // Move the right motor backward at maximum speed
  rStop();
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW); 
}

void lBackward()
{
  // Move the left motor backward at maximum speed
  lStop();
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW); 
}

void backward()
{
  //Move both motors backward
  rBackward();
  lBackward();
}

void turnR()
{
  // Rotate to the right using both motors
  rStop();
  lForward();
  rBackward();
}

void turnL()
{
  // Rotate to the left using both motors
  rStop();
  rForward();
  lBackward();
}

//Connects to the wifi network using the credentials
void setupWifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//MQTT callback implementation if received a message
void mqttCallback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (String(topic) == mqttMovTopic) 
  {
    if(messageTemp == "forward") forward();
    else if(messageTemp == "backward") backward();
    else if(messageTemp == "left") turnL();
    else if(messageTemp == "right") turnR();
    else if(messageTemp == "stop") stop();
  }
}

//MQTT (re)connect implementation if connection is lost
void mqttConnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqttUsername)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(mqttMovTopic);
      turnR(); //Turn around to indicate that a connection has been made
      delay(2000);
      stop();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() 
{
  Serial.begin(9600);
  // sets the left motor pins as outputs
  pinMode(motorLPin1, OUTPUT);
  pinMode(motorLPin2, OUTPUT);

  // sets the right motor pins as outputs
  pinMode(motorRPin1, OUTPUT);
  pinMode(motorRPin2, OUTPUT);

  
  setupWifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(mqttCallback);
}

void loop() 
{
  if (!client.connected()) 
  {
    mqttConnect();
  }
  client.loop();
}