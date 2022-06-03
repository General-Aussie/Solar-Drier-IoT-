#define BLYNK_TEMPLATE_ID "TMPLPBJ3hBu5"
#define BLYNK_DEVICE_NAME "A Solar Drier IoT"

#include <ESP8266WiFi.h> //including the ESP8266WiFi library
#include <DHT.h> //including the DHT library for the DHT11 sensor
#include "HX711.h" //including the HX711 library for the HX711
#include <Wire.h> //including the Wire library for the I2C
#include <Blynk.h> //including the Blynk library for the Blynk cloud
#include <LiquidCrystal_I2C.h> //including the LiquidCrystal_I2C library for the lcd
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

#define DOUT D5
#define CLK D6
HX711 scale(DOUT, CLK); 

LiquidCrystal_I2C lcd(0x27, 16, 2); //creating an object lcd to initiallize the lcd


#define DHTTYPE DHT11 //defining the type of dht sensor
#define DHTPIN D3 //defining the pin for the dht11 sensor
DHT dht(DHTTYPE, DHTPIN); //creating an object dht to initiallize the dht11 sensor


const char *ssid = "Famz's iPhone";
const char *pass = "famzfa101";

WiFiClient client;

int resetButton = D4; //this button resets the scale to zero
float humidity;
float temperature;
float weight;
float calibration_factor =-101525;

void sendSensor() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  Blynk.virtualWrite (V0, humidity);
  Blynk.virtualWrite (V1, temperature);
}

void setup() {
  Serial.begin(115200);
  pinMode(resetButton, INPUT_PULLUP);
  dht.begin();
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.read_average();
  Blynk.begin(ssid, pass);
  Wire.begin(D2, D3);
  lcd.begin();
  lcd.setCursor(2,0);
  lcd.print("Tosin's Project");
  lcd.setCursor(2,1);
  lcd.print("Solar Drier IoT");
  delay(3000);
  lcd.clear();

  lcd.print("Connecting WiFi");
  timer.setInterval(1000L, sendSensor);

  WiFi.begin(ssid, pass);{
    delay(1000);
    Serial.print(".");
    lcd.clear();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  lcd.clear();
  lcd.print("WiFi connected");
  delay(2000);
}

void loop() {
  Blynk.run();
  timer.run();
  scale.set_scale(calibration_factor);

  weight = scale.get_units(5);

  lcd.setCursor(0,0);
  lcd.print("Measured Weight");
  lcd.setCursor(0,1);
  lcd.print(weight);
  lcd.print(" KG ");
  Blynk.virtualWrite(V2, weight);
  delay(2000);
  lcd.clear();

  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" KG");
  Serial.println();

  val = digitalRead(resetButton);
  if (val == LOW){
    scale.set_scale();
    scale.tare();
  }
}
