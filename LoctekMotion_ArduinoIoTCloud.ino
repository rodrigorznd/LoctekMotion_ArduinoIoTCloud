#include "arduino_secrets.h"
/*
  Sketch generated by the Arduino IoT Cloud Thing "Desk"
  https://create.arduino.cc/cloud/things/0852aa63-39c1-4b6a-9782-a090ccea1a68

  Arduino IoT Cloud Properties description

  The following variables are automatically generated and updated when changes are made to the Thing

  bool down;
  bool up;
  bool three;
  bool one;
  bool two;
  String display;
  bool memory;
  bool alarm;

  Properties which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

#include "FlexiSpotE5DeskUtils.h"

const byte OIN20_PIN = 7;
const byte LED_PIN = 6;

const byte DOWN_PIN = 0;
const byte UP_PIN = 1;
const byte ONE_PIN = 2;
const byte TWO_PIN = 3;
const byte THREE_PIN = 4;
const byte MEMORY_PIN = 5;
const byte ALARM_PIN = 8;

const int MAX_CICLES = 200;

bool first = true;
int incomingByte = 0;
byte buffer[100];
int bufferPosition = 0;
int cicles = MAX_CICLES;

Desk::DeskCommand cmd = Desk::IDLE;
Desk::DeskCommand btn_cmd = Desk::IDLE;

Desk desk = Desk(&Serial1, OIN20_PIN, LED_PIN);
DeskDisplay deskDisplay = DeskDisplay();


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);

  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(6000);
  
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();
  
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(ONE_PIN, INPUT_PULLUP);
  pinMode(TWO_PIN, INPUT_PULLUP);
  pinMode(THREE_PIN, INPUT_PULLUP);
  pinMode(MEMORY_PIN, INPUT_PULLUP);
  pinMode(ALARM_PIN, INPUT_PULLUP);
  
  desk.setDebug(&Serial);
  deskDisplay.setDebug(&Serial);
}

void loop() {
  ArduinoCloud.update();
  
  delay(100);
  while (Serial1.available() > 0) {
    readControlBoxMessage();
  }
  
  readButtonCommand();
  
  if (cmd != Desk::IDLE || btn_cmd != Desk::IDLE) {
    cicles = MAX_CICLES;
    if (btn_cmd) {
      desk.sendCommand(btn_cmd);
    } else {
      desk.sendCommand(cmd);
    }
  } else {
    if (cicles > 0) {
      desk.sendIdleCommand();
      cicles--;
    } else {
      desk.standBy();
    }
  }
}


void readControlBoxMessage() {
  // read the incoming byte:
  incomingByte = Serial1.read();

  buffer[bufferPosition] = incomingByte;
  if (incomingByte == Desk::messageEnd) {
    if (buffer[1] == DeskDisplay::displayMessageLength) {
      display = deskDisplay.bytesToString(buffer);
    }
    bufferPosition = 0;
  } else {
    bufferPosition++;
  }
}

void readButtonCommand() {
  if (!digitalRead(UP_PIN)) {
    btn_cmd = Desk::UP;
  } else if (!digitalRead(DOWN_PIN)) {
    btn_cmd = Desk::DOWN;
  } else if (!digitalRead(ONE_PIN)) {
    btn_cmd = Desk::ONE;
  } else if (!digitalRead(TWO_PIN)) {
    btn_cmd = Desk::TWO;
  } else if (!digitalRead(THREE_PIN)) {
    btn_cmd = Desk::THREE;
  } else if (!digitalRead(MEMORY_PIN)) {
    btn_cmd = Desk::MEMORY;
  } else if (!digitalRead(ALARM_PIN)) {
    btn_cmd = Desk::ALARM;
  } else {
    btn_cmd = Desk::IDLE;
  }
}


void onDownChange() {
  Serial.print("Value of down changed to: ");
  Serial.println(down);
  if (down) {
    cmd = Desk::DOWN;
  } else {
    cmd = Desk::IDLE;
  }
}

void onOneChange() {
  Serial.print("Value of one changed to: ");
  Serial.println(one);
  if (one) {
    cmd = Desk::ONE;
  } else {
    cmd = Desk::IDLE;
  }
}

void onThreeChange() {
  Serial.print("Value of three changed to: ");
  Serial.println(three);
  if (three) {
    cmd = Desk::THREE;
  } else {
    cmd = Desk::IDLE;
  }
}

void onTwoChange() {
  Serial.print("Value of down changed two: ");
  Serial.println(two);
  if (two) {
    cmd = Desk::TWO;
  } else {
    cmd = Desk::IDLE;
  }
}

void onUpChange() {
  Serial.print("Value of down changed up: ");
  Serial.println(up);
  if (up) {
    cmd = Desk::UP;
  } else {
    cmd = Desk::IDLE;
  }
}


void onMemoryChange() {
  Serial.print("Value of memory changed up: ");
  Serial.println(memory);
  if (memory) {
    cmd = Desk::MEMORY;
  } else {
    cmd = Desk::IDLE;
  }
}


void onAlarmChange() {
  Serial.print("Value of alarm changed up: ");
  Serial.println(alarm);
  if (alarm) {
    cmd = Desk::ALARM;
  } else {
    cmd = Desk::IDLE;
  }
}