//Library
#include <ESP8266WiFi.h>
#include <DHT.h>
#include "DHT.h"
#include "FirebaseESP8266.h"

//Integrasi Firebase
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

//Integrasi Jaringan WIFI
#define WIFI_SSID ""                                
#define WIFI_PASSWORD ""

//DHT 11
#define DHTPIN 4  //D2
#define DHTTYPE DHT11 

//Library LDR
#define pinSensor 3 //D7

 
DHT dht (DHTPIN,DHTTYPE);
float t; 
float a;
float tf;
int sensorValue = 0;

#define led1 5  //D2
#define led2 6  //D3
#define ldr 0

FirebaseData firebaseData;

String path = "/Node1";
int oldDistance;
int newDistance;

int oldAdcLdr;
int newAdcLdr;               

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(led1,OUTPUT);
  pinMode(ldr,INPUT);

  dht.begin();    
  initWifi();
}

void loop() {
  
//  LAMPU
  if(Firebase.getString(firebaseData, path + "/lampu1")){
    if(firebaseData.stringData() == "0")
      digitalWrite(led1,0);
    else
      digitalWrite(led1,1); 
  }

// POMPA
  if(Firebase.getString(firebaseData, path + "/lampu2")){
    if(firebaseData.stringData() == "0")
      digitalWrite(led2,0);
    else
      digitalWrite(led2,1);   
  }

//Sensor Suhu
  t = dht.readTemperature();    //Baca suhu
  a = dht.readHumidity();
  tf = (((9/5)*t) + 32);   
  Serial.println("Suhu Celcius : ");
  Serial.print(t);              //Tampilkan suhu
  Serial.println(" *C");
  Serial.print("Humidity Fahrenheit= ");
  Serial.print(a);
  Serial.println(" *F");
  Serial.print("Humidity = ");
  Serial.print(tf);
  Serial.println(" *RH");

  Firebase.setInt(firebaseData, path + "/suhuc", t);
  Firebase.setInt(firebaseData, path + "/suhuf", tf);

  Firebase.setInt(firebaseData, path + "/udara", a);
  delay(1000);                  //Jeda pembacaan 1000ms

//Sensor LDR
  sensorValue = analogRead(pinSensor);
  Serial.print("Nilai Sensor : ");
  Serial.println(sensorValue);
  Firebase.setInt(firebaseData, path + "/cahaya", sensorValue);
  delay(500);
}
void initWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
