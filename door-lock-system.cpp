#include <AESLib.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Keypad and LCD Setup
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// AES Setup
AESLib aesLib;
byte aes_key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
byte aes_iv[N_BLOCK] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

// Password Setup
char plainPassword[] = "1234";  // Plaintext password
char encryptedPassword[64];     // Store encrypted password
char userInput[32];             // Array to store up to 32 characters (adjust as needed)
byte inputIndex = 0;            // Index to keep track of input length
int isClosed = 1;               // Door is closed by default

// Servo Setup
Servo servo;
int pos = 0;

// Reset IV for each encryption attempt
void resetIV(byte iv[]) {
  byte initial_iv[N_BLOCK] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
  memcpy(iv, initial_iv, N_BLOCK);
}

// Encrypt the stored password for comparison
void encryptPassword() {
  resetIV(aes_iv);  // Reset IV before encryption
  uint16_t encLen = aesLib.encrypt((byte*)plainPassword, strlen(plainPassword), (byte*)encryptedPassword, aes_key, sizeof(aes_key), aes_iv);
  encryptedPassword[encLen] = '\0';  // Null-terminate the encrypted password
}

void setup() {
  Serial.begin(9600);
  lcd.init();                      // Initialize the LCD
  lcd.setBacklight(HIGH);          // Turn on the backlight
  lcd.print("Enter Password:");    // Initial prompt on the LCD
  lcd.setCursor(0, 1);
  servo.attach(10, 500, 2500);     // Attach servo on pin 10 with min/max pulse width
  servo.write(90);                 // Move servo to closed position
  
  encryptPassword();  // Encrypt the stored password at setup
}

void loop() {
  char customKey = customKeypad.getKey();
  
  if (customKey == '#') {
    lcd.clear();
    lcd.setCursor(0, 0);

    // Encrypt the user input for comparison
    char encryptedInput[64];
    resetIV(aes_iv);  // Reset IV before encrypting user input
    uint16_t encLen = aesLib.encrypt((byte*)userInput, strlen(userInput), (byte*)encryptedInput, aes_key, sizeof(aes_key), aes_iv);
    encryptedInput[encLen] = '\0';

    Serial.println(encryptedInput);
    Serial.println(encryptedPassword);
    if (strcmp(encryptedInput, encryptedPassword) == 0) {  // Compare encrypted values
      lcd.print("Access Granted");
      if (isClosed) {
        openDoor();
      } else {
        closeDoor();
      }
      isClosed = 0;  // Set door state to open
    } else {
      lcd.print("Access Denied");
      if (isClosed != 1) {
        closeDoor();
        isClosed = 1;
      }
    }

    delay(1000);  // Wait 1 second before clearing input
    clearInput(); // Clear the input for the next attempt
  } 
  else if (customKey) {
    Serial.println(customKey);
    lcd.print('*');                // Mask the input on the LCD
    userInput[inputIndex] = customKey;
    inputIndex++;                  // Move to the next index
    userInput[inputIndex] = '\0';  // Null-terminate the input string
  }
}

// Function to open the door (move servo to 0 degrees)
void openDoor() {
  for (pos = 90; pos >= 0; pos -= 1) {
    servo.write(pos);      // Move servo to position
    delay(15);             // Wait for 15 milliseconds for the servo to move
  }
}

// Function to close the door (move servo back to 90 degrees)
void closeDoor() {
  for (pos = 0; pos <= 90; pos += 1) {
    servo.write(pos);      // Move servo to position
    delay(15);             // Wait for 15 milliseconds for the servo to move
  }
}

// Function to clear the input array
void clearInput() {
  memset(userInput, 0, sizeof(userInput));  // Clear the array
  inputIndex = 0;                           // Reset the input index
  lcd.clear();                              // Clear the LCD screen
  lcd.print("Enter Password:");             // Prompt the user again
  lcd.setCursor(0, 1);
}
