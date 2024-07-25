// GifPlayer.h

#ifndef GIF_PLAYER_H
#define GIF_PLAYER_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>

extern bool isGifRunning;

// Function declarations
void setupGifPlayer();
void loopGifPlayer();
void closeGif();
void GIFDraw(GIFDRAW *pDraw);

#endif
