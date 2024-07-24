#include "TFTDisplay.h"

// Initialize TFT display
TFT_eSPI tft = TFT_eSPI();
String jpgFiles[maxFiles];
int fileCount = 0;
String folder = "/";

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  tft.pushImage(x, y, w, h, bitmap);
  delay(5); // Delay for line-by-line display effect
  return 1;
}

void listJpgFiles(String dir) {
  File root = SD.open(dir);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  Serial.println("Listing JPG files in the directory:");
  while (true) {
    File file = root.openNextFile();
    if (!file) {
      break;
    }
    if (!file.isDirectory() && isJpgFile(file.name())) {
      Serial.print("File: ");
      Serial.println(file.name());
      if (fileCount < maxFiles) {
        jpgFiles[fileCount] = file.name();
        fileCount++;
      } else {
        Serial.println("Reached maximum file limit");
        break;
      }
    }
    file.close();
  }
  root.close();
}

void scalePic(String fileName) {
  uint16_t w = 0, h = 0;
  TJpgDec.getSdJpgSize(&w, &h, fileName);
  Serial.print("Width = ");
  Serial.print(w);
  Serial.print(", Height = ");
  Serial.println(h);
  float a = static_cast<float>(h) / tft.height();
  if (a > 4) {
    TJpgDec.setJpgScale(8);
  } else if (a > 2) {
    TJpgDec.setJpgScale(4);
  } else if (a > 1) {
    TJpgDec.setJpgScale(2);
  } else {
    TJpgDec.setJpgScale(1);
  }
  Serial.println(a);
  TJpgDec.drawSdJpg(0, 0, fileName);
}

bool isJpgFile(String name) {
  return !name.startsWith(".") && (name.endsWith(".jpg") || name.endsWith(".JPG") || name.endsWith(".jpeg") || name.endsWith(".JPEG"));
}
