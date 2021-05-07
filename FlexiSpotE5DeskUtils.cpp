#include "Arduino.h"
#include "FlexiSpotE5DeskUtils.h"

DeskDisplay::DeskDisplay() {}

void DeskDisplay::setDebug(Serial_ *debug) {
    this->_debug = debug;
}

String DeskDisplay::byteToString(byte dig) {
    String dp = "";
    String v = "";
    if (dig > _dp_value) {
        dp = ".";
        dig -= _dp_value;
    }

    for (int i = 0; i < _len; i++) {
        if (_k[i] == byte(dig)) {
            v = _v[i];
            break;
        }
    }

    if (_debug && v == "") {
        _debug->println("Fail to decode: ");
        _debug->print("BIN: ");
        _debug->println(dig, BIN);
        _debug->print("HEX: ");
        _debug->println(dig, HEX);
        _debug->println("-----------------");
    }
    return v + dp;
}

String DeskDisplay::bytesToString(byte *digs) { // Define this limit where?
    String v = "";
    for (int i = 3; i < 3 + this->numDigits; i++) {
        v += this->byteToString(digs[i]);
    }

    if (_debug) _debug->print("Result: ");
    if (_debug) _debug->println(v);

    return v;
}

Desk::Desk(Uart *uart, byte oin20Pin, byte ledPin) {
    this->_uart = uart;
    this->_oin20Pin = oin20Pin;
    this->_ledPin = ledPin;
    init();
}

void Desk::setDebug(Serial_ *debug) {
    this->_debug = debug;
}

void Desk::init() {
    pinMode(_oin20Pin, OUTPUT);
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_oin20Pin, HIGH);
    digitalWrite(_ledPin, HIGH);
}

void Desk::standBy() {
    _standByStatus = true;
    digitalWrite(_oin20Pin, LOW);
    digitalWrite(_ledPin, LOW);
}

void Desk::sendCommand(DeskCommand c) {
    if (_standByStatus) {
        _standByStatus = false;
        digitalWrite(_oin20Pin, HIGH);
        digitalWrite(_ledPin, HIGH);
        delay(500);
    }

    if (_debug) _debug->print("Sending command: ");
    if (_debug) _debug->println(deskCommandsNames[c]);
    if (_debug) _debug->print("TX: ");
    for (int i = 0; i < commandLength; i++) {
        _uart->write(deskCommands[c][i]);
        if (_debug) _debug->print(deskCommands[c][i], HEX);
        if (_debug) _debug->print(" ");
    }
    if (_debug) _debug->println("");
}

void Desk::sendIdleCommand() {
    sendCommand(IDLE);
}

void Desk::sendUpCommand() {
    sendCommand(UP);
}

void Desk::sendDownCommand() {
    sendCommand(DOWN);
}

void Desk::sendOneCommand() {
    sendCommand(ONE);
}

void Desk::sendTwoCommand() {
    sendCommand(TWO);
}

void Desk::sendThreeCommand() {
    sendCommand(THREE);
}

void Desk::sendMemoryCommand() {
    sendCommand(MEMORY);
}

void Desk::sendAlarmCommand() {
    sendCommand(ALARM);
}
