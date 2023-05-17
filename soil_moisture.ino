
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL5BOeZ6dQ5"
#define BLYNK_TEMPLATE_NAME "soil moisture"

#define BLYNK_TEMPLATE_ID "TMPL5BOeZ6dQ5" // Blynk template ID
#define BLYNK_TEMPLATE_NAME "soil moisture"  // Blynk template name
#define BLYNK_AUTH_TOKEN "-y6vkcW02EoIp7xpk-FyxJ5KIn74AT7z"  // Blynk authorization token


#define AOUT_PIN 36 // analog output pin
#define DHT_PIN 4 // DHT11 sensor data pin
#define DHT_TYPE DHT11 // DHT sensor type

int sensorvalue = 0; // variable to store sensor value
int outputvalue = 0; // variable to store output value
char auth[] = "-y6vkcW02EoIp7xpk-FyxJ5KIn74AT7z"; // Blynk authorization token
const char* ssid = "SLT-Fiber-9987"; // Wi-Fi network SSID
const char* password = "ks2499881"; // Wi-Fi network password


WidgetLCD lcd(V0); // LCD widget on V0

DHT dht(DHT_PIN, DHT_TYPE); // DHT sensor object

void setup()
{
  Serial.begin(115200); // initialize serial communication
  WiFi.begin(ssid, password); // connect to Wi-Fi network

  while (WiFi.status() != WL_CONNECTED) { // wait for Wi-Fi connection
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  Blynk.begin(auth, ssid, password); // initialize Blynk connection
  dht.begin(); // initialize DHT sensor
  Serial.println("connected");
}

void loop()
{
  sensorvalue = analogRead(AOUT_PIN); // read sensor value
  outputvalue = map(sensorvalue, 0, 4095, 0, 100); // convert sensor value to percentage

  Serial.print(sensorvalue);
  Serial.println(outputvalue);

  float temperature = dht.readTemperature(); // read temperature from DHT sensor
  float humidity = dht.readHumidity(); // read humidity from DHT sensor

  Blynk.virtualWrite(V1, outputvalue); // send soil moisture value to Blynk app
  Blynk.virtualWrite(V2, temperature); // send temperature value to Blynk app
  Blynk.virtualWrite(V3, humidity); // send humidity value to Blynk app

  if (outputvalue > 74) { // if soil moisture is too low
    Serial.println("plant is sad..., water your plant");
    Serial.print(outputvalue);
    lcd.clear(); // clear LCD screen
    lcd.print(0, 0, "plant is sad...,"); // display the message on the first row of the LCD widget
    lcd.print(0, 1, "water your plant"); // display the message on the second row of the LCD widget
    Blynk.logEvent("water_your_plant", ""); // add this line to trigger an event in the Blynk app
    delay(1000);
  }
  else if (outputvalue < 45)
  {
    Serial.println("too much water..., maybe stop watering?");
    Serial.print(outputvalue);
    lcd.clear();
    lcd.print(0, 0, "too much water...,"); // display the message on the first row of the LCD widget
    lcd.print(0, 1, "stop watering"); // display the message on the second row of the LCD widget
    delay(1000);
  } else {
    Serial.println("plant is on a good soil moisture value");
    lcd.clear();
    lcd.print(0, 0, "moisture value");// display the message on the first row of the LCD widget
    lcd.print(0, 1, "is good");// display the message on the second row of the LCD widget
  }

  Blynk.run();
}
