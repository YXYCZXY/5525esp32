#ifndef TFTDISPLAY_H
#define TFTDISPLAY_H

#include <SD.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

// Pin definitions for SD card
#define SD_MOSI 13
#define SD_MISO 19
#define SD_SCK 14
#define SD_CS 12

const int maxFiles = 100;  // Maximum number of JPG files

// Global variables
extern TFT_eSPI tft;
extern String jpgFiles[maxFiles];
extern int fileCount;
extern String folder;

// Function prototypes
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
void listJpgFiles(String dir);
void scalePic(String fileName);
bool isJpgFile(String name);

#endif
