
// Name: Tiera Lee

// Useful defines
#include <stdio.h>
#include "mylib.h"
#include "frog.h"
#include "fly.h"


int main() {

	REG_DISPCNT = MODE3 | BG2_ENABLE;

	enum GBAState state = TITLE_SCREEN;


// variables to change state
	u16 select = 0;
	u16 start = 0;

//background colors for states
	u16 titlecolor = TEAL;
	u16 bgcolor = GREEN;
	u16 endcolor = ORANGE;

//player controls

	PLAYER player;
	PLAYER *playerp;
	player.col = 150;
	player.row = 120;
	player.length = FROG_WIDTH;
	player.height = FROG_HEIGHT;
	player.color = BLUE;

// setting up the enemies
	int start_r = 50;
	int start_c = 80;
	int enemynum = 5;

	ENEMY enemies[enemynum];
	ENEMY *enemyp;

	int multi = 0;
	for (int i = 0; i < enemynum; i++ ) {
		enemies[i].row = (start_r);
		enemies[i].col = (start_c + multi);
		enemies[i].length = FLY_WIDTH;
		enemies[i].height = FLY_HEIGHT;
		enemies[i].color = RED;
		enemies[i].deltaCol = 1;
		enemies[i].deltaRow = 3;

		multi +=20; //spacing for enemies
	}
	//variables to control enemy movement
	int counter = 0;
	int moveRight = 0;
	int moveUp = 0;

	//sets up frog tongue
	TONGUE tongue;
	TONGUE *tonguep;

	tongue.col = 160;
	tongue.row = 150;
	tongue.length = 1;
	tongue.height = 70;
	tongue.color = MAGENTA;

////// score variablles
	int score = 0;
	char buffer[41];


/////timer
	int timer = 30;
	char t_buffer[41];
	int timeCounter = 0;

	while(1) {

		

		waitForVBlank();
		switch(state) {

		case TITLE_SCREEN:

		fillScreen(titlecolor);

		drawString(130,50,"Welcome to FLY_EATER!",WHITE);
		drawString(140,50,"Press START to begin!", WHITE);
	

		if(KEY_DOWN_NOW(BUTTON_START)) {
				state = GAME_SCREEN;
			}
		break;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////


		case GAME_SCREEN:

		// DMA[3].src = &bgcolor;
		// DMA[3].dst = videoBuffer;
		// DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
		// playerp = &player;

		fillScreen(bgcolor);



		if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			playerp->col--;
		}

		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			playerp->col++;
		}

		//drawRect(playerp->row, playerp->col, playerp->length, playerp->height, playerp->color);
		drawImage3(playerp->row, playerp->col, FROG_WIDTH, FROG_HEIGHT, frog);


		//automates movement of enemies		
		for(int i = 0; i < enemynum; i++) {
			enemyp = &enemies[i];
			if(moveRight) {
			enemyp-> col += enemyp-> deltaCol; //enemies right

			}else {
			enemyp-> col -= enemyp-> deltaCol; //enemies left
			}
			counter++;
			if(counter == 400) {
				moveUp = 1;
				moveRight = 0;

			}
		}

		if(counter == 200 || counter == 600) {
			for(int i = 0; i <enemynum; i++) {
				enemyp = &enemies[i];
				if(moveUp) {
					enemyp->row -= enemyp->deltaRow;
				} else{
				enemyp->row += enemyp->deltaRow;
				} //enemies down
			}
			if (counter == 200) {
				moveRight = 1; //resets so enemies go back right
			}
			if (counter == 600) {
				moveRight = 0;
				moveUp = 0; 
				counter = 0;
			}
		}
   		//draws enemies
		for (int i = 0; i < enemynum; i++) {
			enemyp = &enemies[i];
			//drawRect(enemyp->row, enemyp->col, enemyp->length, enemyp->height, enemyp->color);
			drawImage3(enemyp->row, enemyp->col, FLY_WIDTH, FLY_HEIGHT, fly);
		}
		//if A key is held, stick out tongue
		if(KEY_DOWN_NOW(BUTTON_A)) {
			tonguep = &tongue;
			int tx = playerp->row;
			tx = tx - tonguep->height;
			int ty = playerp->col;

			ty = ty + (playerp->length / 2);
			drawRect(tx, ty, tonguep->height, tonguep->length, tonguep->color);
		}

		for(int i = 0; i < enemynum; i++) {
			playerp = &player;
			enemyp = &enemies[i];
			if(collisionCheck(tonguep, enemyp)) {
				score++;
			}
		}

		// prints score to screen
		sprintf(buffer, "score: %d", score);
		drawRect(150,5,10,72,GREEN);
		drawString(150,5, buffer, RED);



		//prints timer
		 timeCounter++;
		if(timeCounter % 60 == 0) {
			timer--;
		}

		sprintf(t_buffer, "TIME: %d", timer);
		drawRect(140,5,10,72,GREEN);
		drawString(140,5, t_buffer, RED);

		if(timer == 0) {
			state = GAME_OVER;
		}
		
	

		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = TITLE_SCREEN;
		}

		break;

		case GAME_OVER:



		// DMA[3].src = &endcolor;
		// DMA[3].dst = videoBuffer;
		// DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
		fillScreen(endcolor);

		drawString(50,50,"GAME OVER!!!",WHITE);
		drawString(60,50, buffer,WHITE);

		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = TITLE_SCREEN;
		}
		}

		// TODO you should probably do something here with one of your variables
		select = ~BUTTONS; // hold whatever button is held
		start = ~BUTTONS;
	}

	return 0;
}


