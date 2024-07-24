#include "BluetoothControl.h"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
String resStr;
String chipId;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++){
          Serial.print(rxValue[i]);
          resStr += rxValue[i];
        }
        Serial.println();
        Serial.println("*********");
        if(resStr == "getid"){
          pTxCharacteristic->setValue(chipId.c_str());
          pTxCharacteristic->notify();
        }else if(resStr == "light1on"){
          digitalWrite(14, 1);
        }else if(resStr == "light1off"){
          digitalWrite(14, 0);
        }else if(resStr == "light2on"){
          digitalWrite(12, 1);
        }else if(resStr == "light2off"){
          digitalWrite(12, 0);
        }
        resStr = "";
      }
    }
};

// 生成一个四位随机数，并拼接到 "5525" 后面
String generateBLEName() {
  String baseName = "5525";
  String randomSuffix = String(random(1000, 9999));  // 生成四位随机数
  return baseName + '_' + randomSuffix;
}

void initBluetooth() {
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(14, 0);
  digitalWrite(12, 0);
  chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
  chipId.toUpperCase();
  
  Serial.println(chipId);
  String bleName = generateBLEName();
  // Create the BLE Device
  BLEDevice::init(bleName.c_str());

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE
                                          );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void handleBluetooth() {
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}
