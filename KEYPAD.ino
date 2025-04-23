// Keypad
// By: Diego J. Graham
// Date: 04/15/2025
// Description: This is the code for a working door lock with a keypad. The user can make the
//              The password in the array named password, and once uploaded to an arduino it can
//              take the users password with a keypad, display it on the lcd as * symbols. If the
//              password is correct the relay with switch to HIGH opening the lock, the lcd will
//              print "access granted", the buzzer will make an acces granded noise and countdown 
//              the time until the lock closes. If the password is Incorrect, the lcd will display 
//              "incorrect", the buzzer will make an error noise, after three seconds the user can 
//              enter another password. If there are 3 wrong passwords in a row, the lock with be 
//              disabled, the buzzer will sound, the lcd will print "Break in Detected" and the 
//              user must reset the device. 
// Components: 1 4x4 membrane switch module, 1 lcd1602, 1 active buzzer, 1 10k ohm potentiometer, 
//             1 Arduino MEGA, 1 SRD-05VDC-SL-C Relay, 3 F-M jumper wires, 30 M-M jumper wires,
//             and 1 Bread Board.
///////////////////////////////////////////////////////////////////////////////////////////////////////


// Libraries
#include <LiquidCrystal.h>
#include <Keypad.h>


// Pins for the lcd, relay and buzzer
const int buzzer = 5;
const int relay = 6;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


// Defining the keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},  // A = addition
  {'4','5','6','B'},  // B = subtraction
  {'7','8','9','C'},  // C = clear
  {'*','0','#','D'}   // D = equals
};

byte rowPins[ROWS] = {25, 26, 29, 30}; 
byte colPins[COLS] = {33, 34, 37, 38}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


// Variables for the program
const char PASSWORD[5] = {'1','2','3','4','5'}; // Change to your own password
char attempt[5];
int index = 0;
int attempts;
const int MAXINCORRECT = 3; // Change to adjust number of incorrect responces before lockout
const int LOCKTIME = 5; // Change to adjust the time the lock is oppened
bool hidePassword = true; // Change the default setting for hidePassword
///////////////////////////////////////////////////////////////////////////////////////////


// This function sets up the lock
void setup() {
  pinMode(buzzer, OUTPUT); 
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This is the main function
void loop() {
  char key = customKeypad.getKey();
   
  if (attempts < MAXINCORRECT){
    getPassword(key);
  }
  else{
    breakIn();
  }

}
///////////////////////////////////////////////////////////////////////////////////////////


// This function checks if the attempted password matches
bool checkPassword() {
  for (int i = 0; i < sizeof(PASSWORD); i++) {
    if (attempt[i] != PASSWORD[i]) {
      return false;
    }
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function opens and closes the lock 
void openLock(){
  digitalWrite(relay, HIGH);
  displayTime();
  digitalWrite(relay, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function displays a countdown for how long the lock is oppen for
void displayTime()  {
  if(digitalRead(relay)== HIGH){
    for (int i = LOCKTIME; i >= 0; i--) {
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(i);
      lcd.print("s   "); // extra spaces to clear old digits
      delay(1000);
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function defines the error beef for when an incorect password is entered
void errorBeep() {
  for (int i = 0; i < 3; i++) { // Three short beeps
    tone(buzzer, 500, 200); // Beep at 500Hz for 200ms
    delay(300); // Wait 300ms between beeps
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function defines the beep for when a button is pressed
void button_beep(){
  tone(buzzer, 700);
  delay(250);
  noTone(buzzer);
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function defines the beep for when the correct password is entered
void accessBeep(){
  tone(buzzer, 1000, 500);
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function restarts the lock
void clear(){
  index = 0;
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function displays access granted, gives the access beep, opens the lock, and resets attempts
void accessGranted(){
  lcd.clear();
  lcd.print("Access Granted");
  accessBeep();
  openLock();
  attempts == 0;
  hidePassword = true;
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function prints incorect, gives the access denied beep, and adds an attempt
void accessDenied(){
  lcd.clear();
  lcd.print("Incorrect");
  errorBeep();
  delay(3000);
  attempts ++;
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function hides the user input with '*'
void hideUserInput(char key){
  attempt[index] = key;
  button_beep();
  lcd.print("*"); 
  index++;
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function shows the user input 
void showUserInput(char key){
  attempt[index] = key;
  button_beep();
  lcd.print(key); 
  index++;
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function defines 'A' as the button to toggle hidePassword, and when it is pressed it will
// show the password on the lcd with the input or '*' depending on the weather hidePassword is T/F
void showPassword(char key){
  if (key == 'A' ){
    button_beep();
    hidePassword = !hidePassword;
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0, 1);
    for (int i =0; i < index; i++){
      if (hidePassword){
        lcd.print("*");
      }
      else{
        lcd.print(attempt[i]);
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This is the function activated when a break in is detected, and you must reset the Arduino
// for the lock to work again. 
void breakIn(){
  lcd.clear();
  lcd.print("BREAK IN");
  lcd.setCursor(0,1);
  lcd.print("DETECTED!");
  while (true) { // Three short beeps
    tone(buzzer, 500, 200); // Beep at 500Hz for 200ms
    delay(300); // Wait 300ms between beeps
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////


// This function brings it all together to get the password
void getPassword(char key){
    if (key) {
    showPassword(key);
      if (key == 'C') {
        button_beep();
        clear();
      }
      else if (key == '#' || key == '*') {
        // Check password if 5 digits entered
      button_beep();
      if (index == 5) {
        if (checkPassword()) {
          accessGranted();
        } else {
          accessDenied();
        }
      clear();
    } else {
        accessDenied();
        clear();
      }
    }
    else if (index < sizeof(PASSWORD) && isDigit(key)) {
      if (hidePassword){
        hideUserInput(key);
      }
      else {
        showUserInput(key);
      }
    }
  }
}