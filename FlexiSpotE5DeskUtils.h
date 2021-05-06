#ifndef E5WDeskUtils_h
#define E5WDeskUtils_h

#include "Arduino.h"

class DeskDisplay {
  private:
    inline static constexpr int _len = 20;
    inline static constexpr byte  _k[_len] = {0x00, 0x09, 0x40, 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x71, 0x31, 0x37};
    inline static constexpr char* _v[_len] = { " ",  "=",  "-",  "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "A",  "F",  "R",  "n"};
    inline static constexpr byte _dp_value = 0x80;
    Serial_* _debug = NULL;
    
  public:
    static const int numDigits = 3;
    static const byte displayMessageLength = 0x07;
    
    DeskDisplay();
    void setDebug(Serial_* debug);
    String byteToString(byte dig);
    String bytesToString(byte *digs);
    
};

class Desk {
  
  private:
    Serial_* _debug = NULL;
    Uart* _uart;
    byte _oin20Pin;
    byte _ledPin;
    bool _standByStatus = true;

  public:
    enum DeskCommand {IDLE, UP, DOWN, ONE, TWO, THREE, MEMORY, ALARM};

    inline static constexpr char deskCommandsNames[8][8] = {
      "IDLE",
      "UP",
      "DOWN",
      "ONE",
      "TWO",
      "THREE",
      "MEMORY",
      "ALARM",
    };

    static const int commandLength = 8;
    static const byte messageStart = 0x9B;
    static const byte messageEnd = 0x9D;
    
    inline static constexpr byte deskCommands[8][commandLength] = {
      {messageStart, 0x06, 0x02, 0x00, 0x00, 0x6C, 0xA1, messageEnd}, // IDLE
      {messageStart, 0x06, 0x02, 0x01, 0x00, 0xFC, 0xA0, messageEnd}, // UP
      {messageStart, 0x06, 0x02, 0x02, 0x00, 0x0C, 0xA0, messageEnd}, // DOWN
      {messageStart, 0x06, 0x02, 0x04, 0x00, 0xAC, 0xA3, messageEnd}, // 1
      {messageStart, 0x06, 0x02, 0x08, 0x00, 0xAC, 0xA6, messageEnd}, // 2
      {messageStart, 0x06, 0x02, 0x10, 0x00, 0xAC, 0xAC, messageEnd}, // 3
      {messageStart, 0x06, 0x02, 0x20, 0x00, 0xAC, 0xB8, messageEnd}, // Memory
      {messageStart, 0x06, 0x02, 0x40, 0x00, 0xAC, 0x90, messageEnd}, // Alarm
    };
    
    Desk(Uart* uart, byte oin20Pin, byte ledPin);
    void setDebug(Serial_* debug);
    void init();
    void standBy();
    void sendCommand(DeskCommand c);
    void sendIdleCommand();
    void sendUpCommand();
    void sendDownCommand();
    void sendOneCommand();
    void sendTwoCommand();
    void sendThreeCommand();
    void sendMemoryCommand();
    void sendAlarmCommand();
};

#endif