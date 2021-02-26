/*
   Code permettant d'afficher l'humidité et / ou la température d'une pièce.
   La température et l'humidité sont mesuré à partir d'un DHT11
   L'affichage se fait à partir d'un LCD 1602.
            Thomas DUCHATEAU
*/
//Required libraries
#include <DHT.h>
#include <LiquidCrystal.h>

//define I/O name
#define button 2
#define DHTPIN 12

//Necessary to DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Necessary to LCD
LiquidCrystal lcd(9, 8, 3, 4, 6, 7);

//Global variable use in this code
//tempsA, CD and mode can be transform to delete the "global effect" and replace by pointer
unsigned long int tempsA = millis();
int CD;
bool mode = 0;
//timing is the time between two measure of values so it's necessary to keep in global variable.
//Unit is in millisecond
unsigned long timing = 128000;


void setup() {
  //Start the communication with serial port (no need)
  Serial.begin(9600);
  Serial.println("Hello World");

  //Start the communication with the DHT11 sensor
  dht.begin();

  //Start the communication with the LCD screen
  //Display a message that remains throughout the execution
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  lcd.print("Measure in : ");

  //Set to INPUT_PULLUP the I/O
  pinMode(button, INPUT_PULLUP);

  //Set up the Interruption to work with button
  attachInterrupt(digitalPinToInterrupt(button), SwitchMode, RISING);
}


void loop() {
  changeAffich();

}

//This function display the countdown (with call of countdown())
//And change the displayed message "humid : " or "tempe : ".
void changeAffich() {

  countdown();
  float t, h;

  if (tempo()) {
    t = read_temp();
    h = read_hygro();
  }

  if (mode == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("Temp : "));
    lcd.print(t);
    lcd.print(F(" c  "));
  }
  else if (mode == 1) {
    lcd.setCursor(0, 0);
    lcd.print(F("Humid : "));
    lcd.print(h);
    lcd.print(F(" % "));
  }
}


//with the interrupt, switch the mode between humidity and temperature
bool SwitchMode() {
  if (mode == 0) {
    mode = 1;
  }
  else if (mode == 1) {
    mode = 0;
  }
}

//Display a countdown before next measure of values.
void countdown() {
  lcd.setCursor(13, 1);
  lcd.print(CD);
  //if the countdown is a 2 or 3 number, the code erase the parasite "0" who stays on the screen
  if (CD / 10 >= 1) {
    if (CD / 100 >= 1) {
      lcd.print(CD);
    }
    else {
      //lcd.setCursor();
      lcd.print(F("   "));
      lcd.print(CD);
    }
  }
  else {
    lcd.print(F("   "));
    lcd.print(CD);
  }
}

//Return 1 if the time between one instant and another has passed
bool tempo() {
  CD = (timing - (millis() - tempsA)) / 1000;
  if (millis() - tempsA >= timing) {
    tempsA = millis();
    return 1;
  }
  else {
    return 0;
  }
}

//Read the humidity and return the values
float read_hygro() {
  // Lecture de l'humidité
  const float h = dht.readHumidity();

  // si la lecture est invalide, on renvoie 0
  if (isnan(h)) {
    return 0;
  }
  return h;
}

//Read the temperature and return the values
float read_temp() {
  // lecture de la température
  const float t = dht.readTemperature();

  // si la lecture est invalide, return 0
  if (isnan(t)) {
    return 0;
  }
  return t;
}
