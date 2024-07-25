#include <Arduino.h>
#include "TFTDisplay.h"
#include "BluetoothControl.h"
#include "GifPlayer.h"
#include "Globals.h"

bool isGifRunning = true;
void setup() {
  Serial.begin(115200);
  setupGifPlayer();
  // Initialize Bluetooth
  initBluetooth();

  // Initialize SPI bus for SD card
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card mount failed");
    return;
  }
  Serial.println("SD card initialized");

  // Initialize TFT display
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  // Set callback for JPEG decoding
  // TJpgDec.setCallback(tft_output);

  // List JPG files in the root directory
  listJpgFiles(folder);
}

void loop() {
  if(isGifRunning){
    loopGifPlayer();
  }
  
  // for (int i = 0; i < fileCount; i++) {
  //   if (folder == "/") {
  //     scalePic("/" + jpgFiles[i]);
  //   } else {
  //     scalePic(folder + "/" + jpgFiles[i]);
  //   }
  //   delay(5000); // Switch images every 5 seconds
  // }
}
