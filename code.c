#include <max6675.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SoftwareSerial.h>

const int flameSensorPin = 8; //flame Sensor Value;
const int thermoCLK = 4; // Connect MAX6675 SCK pin to Arduino digital pin 4
const int thermoCS = 3;  // Connect MAX6675 CS pin to Arduino digital pin 3
const int thermoSO = 2;  // Connect MAX6675 SO pin to Arduino digital pin 2
const int DHTPIN = 9;
const int Buzzer=7;
const int green_led=10;
const int red_led=11;
const int relay=6;

int Threshold=50;

#define RX 12                      // TX of esp8266 in connected with Arduino pin 2
#define TX 13                      // RX of esp8266 in connected with Arduino pin 3
String WIFI_SSID = "";    // WIFI NAME
String WIFI_PASS = "";    // WIFI PASSWORD
String API = "";  // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
SoftwareSerial esp8266(RX, TX);

#define DHTTYPE DHT11   // Type of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(9600);
  dht.begin();
  dht.begin();
  pinMode(flameSensorPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(relay, OUTPUT);

   Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + WIFI_SSID + "\",\"" + WIFI_PASS + "\"", 20, "OK");
  
}

void loop() {
 double CoupleTemperature_C = thermocouple.readCelsius();  //Thermocouple Temp value

  float humidity = dht.readHumidity();   //DHT Humidity value

  int flameState = digitalRead(flameSensorPin); //Flame Detecion [On or Off]
  Serial.println(flameState);

  lcd.init();
lcd.setBacklight(HIGH);

  lcd.setCursor(0, 0); //starting value of x and y

  lcd.print("Temperature ");
  lcd.print(CoupleTemperature_C);

  lcd.setCursor(0, 1); //starting value of x but second(2nd row) of y
  lcd.print("Humidity ");
  lcd.print(humidity);

 if (flameState == LOW) {
  while(flameState == LOW){
    Serial.println("Fire Detected");
    digitalWrite(Buzzer, HIGH);
    digitalWrite(red_led, HIGH);
    digitalWrite(green_led, LOW);
  }
    
    // Add any additional actions or code you want to perform when fire is detected
  } else {
    Serial.println("Fire not Detected");
    digitalWrite(Buzzer, LOW);
    digitalWrite(green_led, HIGH);
  }


  if(CoupleTemperature_C>Threshold){
    Serial.println("Threshould");
    digitalWrite(relay, LOW);
    digitalWrite(red_led, HIGH);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(green_led, LOW);
  }
  else{
    digitalWrite(green_led, HIGH);
    digitalWrite(relay, HIGH);
    digitalWrite(red_led, LOW);
    digitalWrite(Buzzer, LOW);
  }

  String getData = "GET /update?api_key=" + API + "&field1=" + CoupleTemperature_C+
 "&field2="+humidity;
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(1000);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1)) {
    esp8266.println(command);      //at+cipsend
    if (esp8266.find(readReplay))  //ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if (found == true) {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if (found == false) {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false;

}
