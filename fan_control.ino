#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial
#define FAN 2
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

char auth[] = BLYNK_AUTH_TOKEN ;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

const int DHT_PIN = 15;
DHTesp dht;

BlynkTimer timer;

int LED_SIGNAL=0;
int humidity =0;
int temperature =0 ;

BLYNK_WRITE (V2){
  humidity = param.asInt();
  sendSensor();
}
BLYNK_WRITE (V3){
  temperature = param.asInt();
  sendSensor();
}
BLYNK_WRITE (V4)
{
  LED_SIGNAL = param.asInt();
  if (LED_SIGNAL == 1){
    humidity =  humidity - humidity/2;
    temperature = temperature - temperature/2;
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, temperature);
  }
  else if(LED_SIGNAL ==0){
    humidity = humidity + humidity/2;
    temperature = temperature + temperature/2;
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, temperature);
  }
}
void sendSensor()
{
  TempAndHumidity  data = dht.getTempAndHumidity();
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.println(" C ");
  Serial.print("Humidity: ");
  Serial.print(data.humidity);
  Serial.println(" % ");

  Blynk.virtualWrite(V0, data.humidity);
  Blynk.virtualWrite(V1, data.temperature);

  if( humidity <= data.humidity && temperature <= data.temperature ){
    digitalWrite(FAN , HIGH);
    Blynk.virtualWrite(V4, 1);
  }
  else{
    digitalWrite(FAN , LOW);
    Blynk.virtualWrite(V4, 0);
  }
}


void setup()
{
// Debug console
Serial.begin(115200);
dht.setup(DHT_PIN, DHTesp::DHT22);

Blynk.begin(auth, ssid, pass);
timer.setInterval(1000, sendSensor);
pinMode(FAN, OUTPUT);
}

void loop()
{

Blynk.run();
timer.run();
}
