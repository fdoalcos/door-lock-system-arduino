<!-- PROJECT NAME -->
# Door Lock System

- Francis Alcos
- CS 341 Final Project
- University of Massachusetts Boston

<!-- ABOUT THE PROJECT -->
## About The Project

![Untitledvideo-MadewithClipchamp-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/4029a22e-1fd6-4c77-8d5b-15e952127992)

<!-- tell about the project -->

This project is about building a secure door lock system using an Arduino UNO as its microcontroller. The system activates when the user inputs a password, allowing for secure access control.

<!-- tell about the project features -->

## Project Features
This door lock system project provides the following features:

* **Secure Access Control**: The door unlocks only if the correct password is entered. Incorrect passwords do not unlock the door, ensuring security.
* **Lock/Unlock Mechanism**:A micro servo motor controls the door lock, moving to unlock when the correct password is entered and relocking when appropriate.
* **User Feedback**: The system displays the status of each input attempt, indicating whether access was successfully granted or denied.
* **Encryption and Decryption Support**: The system displays the status of each input attempt, indicating whether access was successfully granted or denied.


<!-- REQUIREMENTS OF THE PROJECTS -->
## Project Requirements

* Arduino UNO Board
* Micro Server Motor
* LCD Screen 
* 4x4 Keypad 
* Breadboard
* Jumper wires


<!-- GETTING STARTED -->
## Building the Project
![image1](https://github.com/user-attachments/assets/328175e5-4ca2-454a-b06c-81690a195469)
**Figure: Door Lock System Diagram (created in Tinkercad)**


The steps to make the project was followed by these steps:

1. **Brainstorm Ideas**
    - The task for this project in CS 341 was to design a system with both inputs and outputs, requiring user input and performing necessary outputs based on that input. I began brainstorming ways to fulfill these instructions and eventually developed the concept for a Door Lock System. In this system, the user input would be a password, and the output would control the servo motor to lock or unlock the door based on the input's validity.

2. **Libraries Used**
    - `Keypad.h` - Used for handling the 4x4 input keypad.
    - `LiquidCrystal_I2C.h` - Provides functionalities for the LCD display.
    - `Servo.h` - Manages the servo motor control.
    - `AESLib.h` - Supports encryption and decryption functionalities.

3. **Secure Access Control Functionality**
    - To implement secure access control, I designed logic in Arduino to verify whether the user’s input password matches the pre-set, encrypted password. The process requires the user to press `#` after entering the password, which triggers a function to check its validity.
    - Sample code (**note - this is a high-level overview, not the full code; see the main file for the complete implementation** [link here](https://github.com/fdoalcos/door-lock-system-arduino/blob/main/door-lock-system.cpp#L65)):
    ```cpp
    if (customKey == '#') {
        if (userInput == password) {
            lcd.print("Access Granted");
            if (isClosed) {
                openDoor();
            } else {
                closeDoor();
            }
        }
    }
    ```


4. **Lock/Unlock Mechanism**
    - To implement the lock/unlock mechanism, I created logic in Arduino with `openDoor` and `closeDoor` functions, which are called based on the validity of the entered password. This functionality is managed using the `servo motor`, where the door position is controlled by the `servo.write(position)` command.
    - [openDoor code](https://github.com/fdoalcos/door-lock-system-arduino/blob/main/door-lock-system.cpp#L106)
    - [closeDoor code](https://github.com/fdoalcos/door-lock-system-arduino/blob/main/door-lock-system.cpp#L114)


5. **User Feedback**
    - The User Feedback functionality displays user input and access status on the LCD, indicating whether the password was accepted (e.g., displaying `Access Granted` or `Access Denied`). This is managed by printing messages to the LCD using `lcd functionalities`.
    - Some code examples:
    ```cpp
    // Clear the LCD screen
    lcd.clear();
    // Set the cursor position on the LCD
    lcd.setCursor(0, 0);
    // Print "Access Granted" on the LCD
    lcd.print("Access Granted");
    // Print "Access Denied" on the LCD
    lcd.print("Access Denied");
    ```

6. **Encryption and Decryption Support**
    - This feature adds security to our project by encrypting and decrypting password data. We use the `AESlib` library, which provides AES encryption for the Arduino IDE, supporting encryption of data as byte arrays and strings.
    - The implementation involves encrypting the stored password using a specified key. When the user presses `#`, the system encrypts the input and compares it to the encrypted stored password to determine access. If they match, access is granted, otherwise, it is denied.
    - The `AESlib` documentation, with example code, was very helpful; you can check it [here](https://github.com/suculent/thinx-aes-lib).
    - [Encryption code](https://github.com/fdoalcos/door-lock-system-arduino/blob/main/door-lock-system.cpp#L44)
    - [Comparison code](https://github.com/fdoalcos/door-lock-system-arduino/blob/main/door-lock-system.cpp#L77)


<!-- DEMO -->
## Video Demo
[Video Link](https://drive.google.com/file/d/1r6c4meh_EuEtrr2BybjQ7lLKV6Unnu95/view?usp=drive_link)

<!-- RESOURCES -->
## Resources
* [Tinkercad](https://www.tinkercad.com/dashboard/)
* [AESlib](https://github.com/suculent/thinx-aes-lib)
