#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int ultrasonicEcho = A4;  
int ultrasonicTrigger = A5;

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13

int rightDistance = 0, leftDistance = 0, middleDistance = 0;
unsigned char bluetoothCarSpeed = 250;
unsigned char avoidanceCarSpeed = 100;
unsigned char carSpeed = 100;
unsigned char turnSpeed = 250;
bool state = LOW;
int
  captureBluetoothCommand = 0,
  captureCommand = 0,
  movingFlag = 0,
  movingValue = 0,
  ignoreBluetoothCommands = 1;
char bluetoothCommandChar;
String
  bluetoothCommandString = "",
  firstCommand = "",
  firstValue = "",
  secondCommand = "",
  secondValue = "",
  currentCommand = "";

void forward(){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);
  //digitalWrite(ENA,HIGH);
  //digitalWrite(ENB,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  //Serial.println("Forward");
}

void back(){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);
  //digitalWrite(ENA,HIGH);
  //digitalWrite(ENB,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  //Serial.println("Back");
}

void left(){
  analogWrite(ENA,turnSpeed);
  analogWrite(ENB,turnSpeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  //Serial.println("Left");
}

void right(){
  analogWrite(ENA,turnSpeed);
  analogWrite(ENB,turnSpeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  //Serial.println("Right");
}

void stop(){
  movingValue = 0;
  movingFlag = 0;
  currentCommand = "";
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  //Serial.println("Stop!");
}

void stateChange(){
  state = !state;
  digitalWrite(LED, state);
  //Serial.println("Light");  
}

void setup() { 
  Serial.begin(9600);

  myservo.attach(3,700,2400);  // attach servo on pin 3 to servo object
  pinMode(ultrasonicEcho, INPUT);
  pinMode(ultrasonicTrigger, OUTPUT);
  
  pinMode(LED,OUTPUT); 
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  stop();
}

void loop() {
  if (ignoreBluetoothCommands == 1) {
    carSpeed = avoidanceCarSpeed;
  } else {
    carSpeed = bluetoothCarSpeed;
  }
  /*Serial.print("carSpeed: ");
  Serial.print(carSpeed);
  Serial.print(" - ignoreBluetoothCommands: ");
  Serial.println(ignoreBluetoothCommands);*/
  checkUltrasonicActivity();
  checkBluetoothActivity();
}

void checkBluetoothActivity() {
  if (ignoreBluetoothCommands == 1) {
    return;
  }
  bluetoothCommandChar = Serial.read();
  Serial.print("Char command: '");
  Serial.print(bluetoothCommandChar);
  Serial.println("'");
  Serial.print("bluetoothCommandString: '");
  Serial.print(bluetoothCommandString);
  Serial.println("'");

  if (bluetoothCommandChar == '{') {
    captureBluetoothCommand = 1;
    bluetoothCommandString = "";
    firstCommand = "";
    firstValue = "";
    secondCommand = "";
    secondValue = "";
    captureCommand = 1;
  }
  if (captureBluetoothCommand == 1) {
    if (bluetoothCommandChar != '{' &&
       bluetoothCommandChar != '"' &&
       bluetoothCommandChar != '}') {
       if (bluetoothCommandChar == 'D' || bluetoothCommandChar == 'N' ||
           bluetoothCommandChar == '1' || bluetoothCommandChar == '2' || bluetoothCommandChar == '3' || bluetoothCommandChar == '4' ||
           bluetoothCommandChar == '5' || bluetoothCommandChar == '6' || bluetoothCommandChar == '7' || bluetoothCommandChar == '8' ||
           bluetoothCommandChar == '9' || bluetoothCommandChar == '0' || bluetoothCommandChar == ':' || bluetoothCommandChar == ',') {
         bluetoothCommandString += bluetoothCommandChar;
         if (bluetoothCommandChar != ':' &&
             bluetoothCommandChar != ',') {
            switch(captureCommand){
              case 1: firstCommand += bluetoothCommandChar; break;
              case 2: firstValue += bluetoothCommandChar;   break;
              case 3: secondCommand += bluetoothCommandChar;   break;
              case 4: secondValue += bluetoothCommandChar;  break;
              default:  break;
            } 
         } else {
          captureCommand++;
         }
       }
    }
    
    //Serial.print("Building command: ");
    //Serial.println(bluetoothCommandString);
  }

  if (bluetoothCommandChar == '}') {
    captureBluetoothCommand = 0;
    Serial.print("COMMAND: ");
    Serial.println(bluetoothCommandString);
    /*Serial.print("RECEIVED firstCommand: ");
    Serial.println(firstCommand);
    Serial.print("RECEIVED firstValue: ");
    Serial.println(firstValue);
    Serial.print("RECEIVED secondCommand: ");
    Serial.println(secondCommand);
    Serial.print("RECEIVED secondValue: ");
    Serial.println(secondValue);*/
    bluetoothCommandString = "";
    
    if (firstCommand == "D1" && (firstValue == "5" || firstValue == "3")) {
      if (currentCommand != "" && currentCommand != "forward") {
        currentCommand = "forward";
        return;
      }
      currentCommand = "forward";
      movingFlag = 1;
      movingValue = 0;
      forward();
    }

    if (firstCommand == "D1" && firstValue == "4") {
      if (currentCommand != "" && currentCommand != "back") {
        currentCommand = "back";
        return;
      }
      currentCommand = "back";
      movingFlag = 1;
      movingValue = 0;
      back();
    }

    if (firstCommand == "D1" && (firstValue == "2")) {
      if (currentCommand != "" && currentCommand != "right") {
        currentCommand = "right";
        return;
      }
      currentCommand = "right";
      movingFlag = 1;
      movingValue = 0;
      right();
    }

    if (firstCommand == "D1" && (firstValue == "1")) {
      if (currentCommand != "" && currentCommand != "left") {
        currentCommand = "left";
        return;
      }
      currentCommand = "left";
      movingFlag = 1;
      movingValue = 0;
      left();
    }
    bluetoothCommandString = "";
  }
  
  delay(1);
  if (movingFlag == 1) {
    movingValue++;
    if (movingValue > 200) {
      stop();
    }
  }
}

int getUltrasonicDistance() {
  digitalWrite(ultrasonicTrigger, LOW);   
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigger, HIGH);  
  delayMicroseconds(20);
  digitalWrite(ultrasonicTrigger, LOW);   
  float Fdistance = pulseIn(ultrasonicEcho, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
}

void checkUltrasonicActivity() {
  if (ignoreBluetoothCommands == 0) {
    return;
  }
  myservo.write(90);  //setservo position according to scaled value
  //delay(500); 
  middleDistance = getUltrasonicDistance();

  if(middleDistance <= 40) {
    stop();
    delay(500);                         
    myservo.write(10);          
    delay(1000);      
    rightDistance = getUltrasonicDistance();
    
    delay(500);
    myservo.write(90);              
    delay(1000);                                                  
    myservo.write(180);              
    delay(1000); 
    leftDistance = getUltrasonicDistance();
    
    delay(500);
    myservo.write(90);              
    delay(1000);
    if(rightDistance > leftDistance) {
      right();
      delay(360);
    } else if(rightDistance < leftDistance) {
      left();
      delay(360);
    } else if((rightDistance <= 40) || (leftDistance <= 40)) {
      back();
      delay(180);
    } else {
      forward();
    }
  } else {
      forward();
  }
}
