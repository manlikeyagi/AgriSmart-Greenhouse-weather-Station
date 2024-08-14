/*Weather monitoring system with the New Blynk app
   https://srituhobby.com
*/
#define BLYNK_TEMPLATE_ID "TMPL2RoAknHMk"
#define BLYNK_TEMPLATE_NAME "AgriSmart Weather Station System"
#define BLYNK_AUTH_TOKEN "8MNX48igL1OMyAIvXIReMNXHWK9cs7e3"
//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SFE_BMP180.h>

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Create an object for the BMP180 sensor
SFE_BMP180 bmp;

char auth[] = "8MNX48igL1OMyAIvXIReMNXHWK9cs7e3";//Enter your Auth token
char ssid[] = "Tope";//Enter your WIFI name
char pass[] = "buydata000";//Enter your WIFI password

DHT dht(D3, DHT11);//(sensor pin,sensor type)
BlynkTimer timer;

//Define component pins
#define rain A0
#define light D0

//Create three variables for pressure
double T, P;
char status;

void setup() {
  Serial.begin(9600);
  bmp.begin();
  lcd.init();
  lcd.backlight();
  pinMode(light, INPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("AgriSmart Weather  ");

  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(4000);
  lcd.clear();

  //Call the functions
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, rainSensor);
  timer.setInterval(100L, pressure);
  timer.setInterval(100L, LDRsensor);
}

//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);

}

//Get the rain sensor values
void rainSensor() {
  int value = analogRead(rain);
  value = map(value,  0, 1023, 100, 0);
  Blynk.virtualWrite(V2, value);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(value);
  lcd.print(" ");
}

//Get the pressure values
void pressure() {
  status =  bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3);// 0 to 3
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
      if (status != 0) {

      }
    }
  }

  Blynk.virtualWrite(V3, P);
  //lcd.setCursor(8, 1);
  //lcd.print("P:");
  //lcd.print(P);
}

//Get the LDR sensor values
void LDRsensor() {
  bool value = digitalRead(light);
  if (value == 0) {
    WidgetLED LED(V4);
    LED.on();
  } else {
    WidgetLED LED(V4);
    LED.off();
  }

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
