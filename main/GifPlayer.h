// GifPlayer.h

#ifndef GIF_PLAYER_H
#define GIF_PLAYER_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>

// Function declarations
void setupGifPlayer();
void loopGifPlayer();
void GIFDraw(GIFDRAW *pDraw);

#endif
