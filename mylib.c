#include "mylib.h"
unsigned short *videoBuffer = (unsigned short*) 0x6000000;
extern const unsigned char fontdata_6x8[12288]; 

void setPixel(int r, int c, unsigned short color)
{
	videoBuffer[r * 240 + c] = color;
}


void drawRect(int left, int top, int length, int height, unsigned short color)
{
	int x, y;
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < length; x++)
		{
			setPixel(left+x, top+y, color);
		}
	}
}


void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawImage3(int x, int y, int width, int height, const unsigned short *image) {

	for (int i = 0; i < height; i++) {
	DMA[3].src = &image[OFFSET(i,0,width)];
	DMA[3].dst = videoBuffer + OFFSET(x+i, y, 240);
	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT;
	 }


}


void drawChar(int row, int col, char ch, u16 color)
{
	int r,c;
	for(r = 0; r < 8; r++)
	{
		for(c = 0; c < 6; c++)
		{
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48])
			{
				setPixel(r+row, c+col, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, u16 color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color );
		col += 6;
	}
}


int collisionCheck(TONGUE* tonguep, ENEMY* enemyp) {
	int ecol=  enemyp->col;
	int erow = enemyp->row;
	int ewidth = enemyp->length;
	int eheight = enemyp->height;

	int tcol = tonguep->col;
	int trow = tonguep->row;
	int twidth = tonguep->length;
	int theight = tonguep->height;


	if( (ecol > tcol) && (ecol < tcol + twidth) && (erow < trow) && (erow < trow + theight)) {
		return 1;
	} 

	if ((ecol > tcol) && (ecol < tcol +twidth) && (erow + eheight > trow) && (erow +eheight < trow + theight)) {
		return 1;
	}

	if ((ecol + ewidth > tcol) && (ecol +ewidth < tcol + twidth) && (erow > trow) && (erow < trow + theight)) {
		return 1;
	}

	if ((ecol + ewidth > tcol) && (ecol + ewidth <tcol + twidth) && (erow +eheight > trow) && (erow +eheight < trow +theight)) {
		return 1;
	}


	return 0;

}


void fillScreen(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240 * 160) | DMA_ON | DMA_SOURCE_FIXED;
}
