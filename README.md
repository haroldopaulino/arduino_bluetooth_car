# Arduino based Bluetooth Car
![PXL_20250519_012057705](https://github.com/user-attachments/assets/a7bd9953-cd6c-4ee4-ac14-45c4a5843ea4)

### This arduino project performs the following:

#### 1) Digitally controls 4 independent motors, to turn them ON or OFF;
#### 2) Control each motor speed independently;
#### 3) Read bluetooth via serial communication, at 9600 baud;
#### 4) Parse a basic JSON string from the bluetooth communication, enough to interpret commands;
#### 5) Use the received bluetooth commands to control the motors;
#### 6) Added a timer for each command, so it ends in 1 second.
#### 7) Download and use a generic bluetooth remote control app, and this should work.


### This project does have an ultrasonic sensor and a servo motor to rotate it, however that's not being used in this specific source code.

![PXL_20250519_012120126](https://github.com/user-attachments/assets/413c32bd-ec5d-49b7-880d-9b533699f507)
![PXL_20250519_012132951](https://github.com/user-attachments/assets/ef0ddb45-f065-4a41-b164-08b7ec630c0f)
![PXL_20250519_012153578](https://github.com/user-attachments/assets/cd424184-bfe8-4095-bf92-b64982fe9fe3)
