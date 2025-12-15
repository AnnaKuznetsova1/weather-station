#include <LiquidCrystal.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
 
constexpr uint8_t PIN_RS = 6;
constexpr uint8_t PIN_EN = 7;
constexpr uint8_t PIN_DB4 = 8;
constexpr uint8_t PIN_DB5 = 9;
constexpr uint8_t PIN_DB6 = 10;
constexpr uint8_t PIN_DB7 = 11;
float humidity;
float temperature;
float wind_speed;
bool Takeoff_clearance;
String str_humidity;
String str_temperature;
 
LiquidCrystal lcd(PIN_RS, PIN_EN, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  dht.begin();

  lcd.begin(16, 2);

  randomSeed(analogRead(A0));
  wind_speed = random(0, 3001) / 100.0;
}
 
void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));

    lcd.setCursor(0, 0);
    lcd.print("Humidity: ERROR");
    lcd.setCursor(0, 1);
    lcd.print("C: ERROR");
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C "));

  str_humidity = String(humidity);
  str_temperature = String(temperature);
  str_humidity.remove(str_humidity.length() - 1);
  str_temperature.remove(str_temperature.length() - 1);
  lcd.setCursor(0, 0);
  lcd.print("Humidity: " + str_humidity + "%");
  lcd.setCursor(0, 1);
  lcd.print("C: " + str_temperature + "\x99");
  delay(3000);
  lcd.clear();

  if (random(0, 2) == 1) {
    wind_speed += (random(0, 101) / 100.0);
  } else {
    wind_speed -= (random(0, 101) / 100.0);
  }
  if (wind_speed < 0) {
    wind_speed = 0;
  }
  if (temperature < -20 || temperature > 45 || humidity > 80 || wind_speed > 15) {
    Takeoff_clearance = false;
  } else {
    Takeoff_clearance = true;
  }

  Serial.print(F("Wind speed: "));
  Serial.print(wind_speed);
  Serial.print(F("m/s  Takeoff clearance: "));
  if (Takeoff_clearance) {
    Serial.println("Takeoff: Ready!");
  } else {
    Serial.println("Takeoff: WARNING!");
  }

  lcd.setCursor(0, 0);
  lcd.print("Wind: " + String(wind_speed) + "m/s");
  lcd.setCursor(0, 1);
  if (Takeoff_clearance) {
    lcd.print("Takeoff: Ready!");
  } else {
    lcd.print("Takeoff:WARNING!");
  }
  delay(3000);
  lcd.clear();
}