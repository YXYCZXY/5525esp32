#include "BluetoothControl.h"
#include "GifPlayer.h"
#include "TFTDisplay.h"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

String chipId;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// 生成一个随机的4位字符串
String generateRandomString() {
  String randomString = "";
  for (int i = 0; i < 4; i++) {
    char randomChar = random(0, 36); // 0-9和a-z，共36个字符
    if (randomChar < 10) {
      randomChar += '0'; // 0-9
    } else {
      randomChar += 'a' - 10; // a-z
    }
    randomString += randomChar;
  }
  return randomString;
}

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
        if(isGifRunning){
            isGifRunning = false;
            closeGif();
            delay(1000);
            Serial.printf("结束2\n");
        };
        
        String resStr;
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++){
          Serial.print(rxValue[i]);
          resStr += rxValue[i];
        }
        Serial.println();
        // //调用图片切换
        Serial.println("*********");
        Serial.println(resStr);
        scalePic(resStr);
      }
    }
};



void initBluetooth() {
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(14, 0);
  digitalWrite(12, 0);
  chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
  chipId.toUpperCase();
  
  Serial.println(chipId);

  String deviceName = "5525!" + generateRandomString();
  BLEDevice::init(deviceName.c_str());

  Serial.println("Device initialized with name: " + deviceName);

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
