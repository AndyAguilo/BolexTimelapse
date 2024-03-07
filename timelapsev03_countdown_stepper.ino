
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Stepper.h>

const int in1 = A0;
const int in2 = A1;
const int in3 = A2;
const int in4 = A3;

const int stepsPerRevolution = 2048; // Steps per revolution for the 28BYJ-48 motor
Stepper myStepper(stepsPerRevolution, in4, in2, in3, in1);


int getUserInput(const char* prompt);
void triggerSolenoid();
void displayCountdownAndFrameCount(int currentFrame, int totalFrames, int interval);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Define the keypad
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 2, A4, A5}; // connect to the column pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Time-lapse Setup");
  delay(3000);
}

void loop() {
  // Ask the user for the number of frames
  int numFrames = getUserInput("Enter frames: #");

  // Ask the user for the interval between frames
  int interval = getUserInput("Interval (s):");

  // Perform the time-lapse
  for (int i = 0; i < numFrames; i++) {
    // Display countdown
    displayCountdown(interval);
    
    triggerStepper();
    delay(50); // Adjust the delay according to your solenoid's requirements
  }

  lcd.clear();
  lcd.print("Time-lapse complete");
  delay(5000); // Display the message for 5 seconds, adjust as needed
}

// Function to get user input from the keypad
int getUserInput(const char* prompt) {
  lcd.clear();
  lcd.print(prompt);
  lcd.setCursor(0, 1);

  String input = "";
  char key;

  while (true) {
    key = customKeypad.getKey();

    if (key) {
      if (key == '#') {
        // User pressed enter, exit the loop
        break;
      } else {
        // Append the key to the input string
        input += key;
        lcd.print(key);
      }
    }
  }

  // Convert the input string to an integer
  int userInput = input.toInt();
  return userInput;
}

void triggerStepper() {
  // Rotate stepper motor 45 degrees clockwise
  myStepper.setSpeed(17); // Adjust the speed as needed
  myStepper.step(stepsPerRevolution *8);
}

// Function to display countdown
void displayCountdown(int duration) {
  for (int i = duration; i > 0; i--) {
    lcd.clear();
    lcd.print("Countdown: " + String(i));
    
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
      // Wait for 1 second using millis() for a non-blocking delay
    }
  }
}
