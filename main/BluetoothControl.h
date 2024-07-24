#ifndef BLUETOOTHCONTROL_H
#define BLUETOOTHCONTROL_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

void initBluetooth();
void handleBluetooth();

extern BLECharacteristic * pTxCharacteristic;
extern bool deviceConnected;

#endif
