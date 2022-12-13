#include <DHT.h>
#include <LiquidCrystal.h>
#include <Stepper.h> 
#include <Adafruit_Sensor.h>
#include <RTClib.h>
//********
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
DHT dht(6, DHT11);

//********
Stepper stepper(32, 9, 11, 10, 12);
int Pval = 0; // sets value to 0 so that change in pot makes stepper move
int potVal = 0; // value pulled from the analog / pot

//********
int waterLevel = 0; // sets place value for waterLevel
int anologPin = A5; // AnalogPin

//********
int motorSpeed = 225; // sets speed of the motor 250 is max around 100 it sorta works 

//********
int green = 38;
int red = 39;
int blue = 40;
int yellow = 41;
bool button = 43;

// Pointers
volatile unsigned char* port_c= (unsigned char*) 0x28;
volatile unsigned char* ddr_c= (unsigned char*) 0x27;
volatile unsigned char* pin_c= (unsigned char*) 0x26;

//********
RTC_DS1307 realTimeClock;

//********
String currentState;
bool start;


void setup(){
  Serial.begin(9600);
  //DHT
  lcd.begin(16, 2); // sets the location on the lcd
  dht.begin();
  //STEPPER
  stepper.setSpeed(200); 
  //fan
  pinMode (25, OUTPUT);
  pinMode (27, OUTPUT);
  pinMode (29, OUTPUT);
  *ddr_c &= 0b11110000;
  *port_c |= 0b00001111;
  realTimeClock.begin();
  realTimeClock.adjust(DateTime(F(__DATE__),F(__TIME__)));
  start = false;
  currentState = "disable";
}
float getHumidity(){
  return dht.readHumidity();
}
float getTemperature(){
  return dht.readTemperature(true);
}
void lcd_display(float fah, float humid){
  lcd.clear();
  delay(2000);
  lcd.setCursor(0,0); // set location of chars
  lcd.print("Temp: ");
  lcd.print(fah);
  lcd.print(" F");
  lcd.setCursor(0,1); // sets next col
  lcd.print("Humidity: ");
  lcd.print(humid);
  lcd.print("%");
  delay(1000);
}

void dht_function(){
  delay(2000);
  float humid = getHumidity(); // reads humid
  float fah = getTemperature(); // reads temp in F the true statment makes it such
  lcd_display(fah, humid);
  
}
void stepper_function(){
  potVal = map(analogRead(A0),0,1024,0,500);// 1024 stands for the anolog and the 500 is the stepper (this section needs a redo )
  if (potVal>Pval)
    stepper.step(20);
  if (potVal<Pval)
    stepper.step(-20);
  Pval = potVal; // creates new value so that when pot is move there will be a shift
}
int getWaterLevel(){
    unsigned int waterLevel = adc_Read(1);
  float fah = dht.readTemperature(true);

  if(waterLevel > 100 && fah <= 60)
  {
    idle();
  }
  else if( waterLevel > 100 && fah <= 60)
  {
    running();
  }
  else if (waterLevel <= 100)
  {
    error();
  }
  else {
  }
  Serial.println(analogRead(anologPin));
  return analogRead(anologPin); //Reads value from analog and assigns value to variable
}
void realTimeClockFunction(){
  Serial.print(realTimeClock.now().toString("hh:mm:ss ap"));
  Serial.write("\n");
}
void running(){
  turnBlue();
  delay(1000);
  float humid = getHumidity(); // reads humid
  float fah = getTemperature(); // reads temp in F the true statment makes it such
  analogWrite(25, 225);
  lcd_display(getTemperature(), getHumidity());
  digitalWrite(27, HIGH); // gets the fan to turn on 
  if(fah < 60 )
  {
     digitalWrite(27, LOW); // gets the fan to turn off
     currentState = "idle";
  }
  if(!start){
    currentState = "disable";
  }
  if(getWaterLevel() < 150){
    //int s = 1;
    currentState = "error";
  }

}

void turnGreen(){
  //digitalWrite(green, HIGH);
  *port_c &= 0b11110000;
  *port_c |= 0b00000001;
}
void turnRed(){
  //digitalWrite(red, HIGH);
  *port_c &= 0b11110000;
  *port_c |= 0b00001000;
}
void turnBlue(){
  //digitalWrite(blue, HIGH);
  *port_c &= 0b11110000;
  *port_c |= 0b00000010;
}
void turnYellow(){
  //digitalWrite(yellow, HIGH);
  *port_c &= 0b11110000;
  *port_c |= 0b00000100;
}
void disable(){
  turnYellow();
  delay(1000);
  if(!start){
    start = true;
  }
  if(start)
  {
    currentState = "idle";
  }
}
void idle(){
  turnGreen();
  delay(1000);
  lcd_display(getTemperature(), getHumidity());
  if(getTemperature() > 60){
    currentState = "running";
  }
  
  if (getWaterLevel() <= 150){
    //int s = 1;
    currentState = "error";
  }

}
void changeState()
{
  lcd.print("The Current State is: " + currentState);
}
void error(){
  turnRed();
  lcd.clear();
  lcd.print("ERROR");
  if(!start){
    currentState = "disable";
  }
  if( button = true)
  {
    idle();
    currentState = "Idle State";
  }
  else
  {
    error();
  }
  
}
void selectState()
{
  if(currentState == "disable")
  {
    disable();      
  }
  if(currentState == "running")
  {
    running();
  }
  if(currentState == "error")
  {
    error();
  }
  if(currentState == "idle")
  {
    idle();
  }
}
void loop(){
  //idle();
 //dht_function();
 stepper_function();
 //water_sensor();
 //fan();
 selectState();
}
