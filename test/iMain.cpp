/*
*Coded By:
*Muktadir Rahman
*Student id: 13 05 097
*Sec: A2
*/
#include "iGraphics.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//size of window
#define HEIGHT 600
#define WIDTH  1000
#define HEIGHT_OF_BOARD (HEIGHT - 100 - 40) 
#define WIDTH_OF_BOARD  (WIDTH - 40)

#define N 10 //no of balls
#define R 20 //radius of the balls
#define DAMPING_FACTOR .99


/*stuctures*/
struct point{
	int x;
	int y;
	bool visible;
};
struct velocity{
	double x;
	double y;
};
struct color{
	int r;
	int g;
	int b;
};

/*end*/

/*global Variables*/
point c[N];
point l;
velocity v[N];
color col[N];
char n[20] = "0"; //to print a number on every ball
int score[2] = {0 ,0}; 
int motion = 0, mouse_down = 0;
char player[2][100];
/*end*/

/*prototypes of the funtions*/
void iDraw();
void iKeyboard(unsigned char key);
void iMouse(int button, int state, int mx, int my);
void iMouseMove(int mx, int my);
void move();	//this will change some thing every millisecond
void collision(int ball1, int ball2); //this will change the velocity of two balls after collision.
void initialize();
/*end*/



int main()
{  
	initialize();
	printf("Enter the name of Player-1(max - 10 char): ");
	gets(player[0]);
	printf("Enter the name of Player-2(max - 10 char): ");
	gets(player[1]);


	iSettimer(1, move);
	
	iInitialize(WIDTH, HEIGHT, "Game");
	
	return 0;
}

void iDraw()
{
	iClear();
	
	/*menu*/
	iSetcolor(255, 255,0);
	iFilledRectangle(0, 502, WIDTH, 100);
	iSetcolor(0, 0, 0);
	iText(20, 550, player[0], GLUT_BITMAP_TIMES_ROMAN_24);
	iText(480, 550, player[1], GLUT_BITMAP_TIMES_ROMAN_24);
	iText(800, 550, "Turn: ", GLUT_BITMAP_TIMES_ROMAN_24);
	/*end*/


	/*board*/
	iSetcolor(1,0,0);
	iFilledRectangle(0, 0, WIDTH, HEIGHT - 100); //BORDER OF THE BOARD
	iSetcolor(0, 1, 0);
	iFilledRectangle(20, 20,  WIDTH_OF_BOARD, HEIGHT_OF_BOARD);  // the main board
	iSetcolor(0, 0, 0);
	iFilledCircle(40, 40, R * 1.5); //bottom-left hole
	iFilledCircle(WIDTH - 40, 40, R * 1.5); //bottom-right hole
	iFilledCircle(40, HEIGHT_OF_BOARD, R * 1.5); //top-left hole
	iFilledCircle(WIDTH - 40, HEIGHT_OF_BOARD, R * 1.5);// top-right hole
	iSetcolor(255, 255, 255);
	iLine(125, 20, 125, 480);
	/*end*/

	/*balls*/
	for(int i = 0; i < N; i++){
		if(!c[i].visible) continue;
		iSetcolor(col[i].r, col[i].g, col[i].b);
		iFilledCircle(c[i].x, c[i].y, R);
		iSetcolor(0, 0,0);
		iCircle(c[i].x, c[i].y, R); //border of circle
		iSetcolor(0,0,0);
		
		n[0] =  (char)(i   + '0');
		n[1] = '\0';

		iText(c[i].x - 6, c[i].y - 5, n, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	/*end*/


	/*line */
	if( !motion && mouse_down) iLine(c[0].x, c[0].y, l.x, l.y);
	/*end*/

}

void iKeyboard(unsigned char key)
{

}

void iMouse(int button, int state, int mx, int my)
{
	if(motion) return;

	if(state == GLUT_DOWN){
		mouse_down = 1;
		l.x = mx;
		l.y = my;
	}
	else{
		mouse_down = 0;
		v[0].x = (c[0].x - mx) / 20;
		v[0].y = (c[0].y - my) / 20;
	}
	
}

void iMouseMove(int mx, int my)
{
	if(motion) return;
	l.x = mx;
	l.y = my;
}

void move() 
{

	//check the collision and change the velocity
	for(int i = 0; i < N - 1; i++){
		if(!c[i].visible) continue;
		for(int j = i + 1; j < N; j++){
			int d = (c[i].x - c[j].x) * (c[i].x - c[j].x) +  (c[i].y - c[j].y) * (c[i].y - c[j].y);
			
			if( d <= 4 * R * R) {
				collision(i, j);
				while( d < 4 * R * R){					
					c[i].x += v[i].x ;
					c[i].y += v[i].y ;
					c[j].x += v[j].x  ;
					c[j].y += v[j].y;
					d = (c[i].x - c[j].x) * (c[i].x - c[j].x) +  (c[i].y - c[j].y) * (c[i].y - c[j].y);
				}		
			}
		}
	}
	
	//increase the velocity 
	motion = 0;
	for(int i = 0; i < N; i++)
	{ 
		
		if(!c[i].visible) continue;

		if(v[i].x > 1  || v[i].y > 1 ) motion = 1;

		printf("v[%d].x = %4.2lf  v[%d].y = %4.2lf motion: %d\n", i, v[i].x, i, v[i].y, motion);

		c[i].x += (int)v[i].x;
		c[i].y += (int)v[i].y;

		/*if(c[i].x > 0 && c[i].x < 1) c[i].x ++;
		if(c[i].y > 0 && c[i].y < 1) c[i].y ++;*/

		//change the velocity if any collision with the wall.
		if(c[i].x + R > 980 || (v[i].x < 0 && c[i].x  < R + 20 )) v[i].x = -v[i].x;
		if(c[i].y + R > 420 || (v[i].y < 0 && c[i].y  < R + 20 )) v[i].y = -v[i].y;

		//check whether any ball has fallen into the hole.
		//HERE YOU HAVE TO MAKE SCORING
		if((c[i].x < 70 || c[i].x > 930) && (c[i].y < 70 || c[i].y > 370) ){
			//HERE SCORING
			c[i].visible = false;
		}
	
		
		//damping
		v[i].x *= DAMPING_FACTOR;
		v[i].y *= DAMPING_FACTOR;
	}
	printf("\n\n\n");
}

void collision(int ball1, int ball2)
{
	
	//determination of angle 
	int b = ball1;
	int d = ball2;
	
	double m = (double) (c[ball1].y - c[ball2].y )/((double)(c[ball1].x- c[ball2].x )); 
	double a = -  atan( m);

	double sin_a = sin(a);
	double cos_a = cos(a);
	double u1x = v[ball1].x, u1y = v[ball1].y;
	double u2x = v[ball2].x, u2y = v[ball2].y;

	double u1x_ = u1x * cos_a - u1y * sin_a;
	double u1y_ = u1x * sin_a + u1y * cos_a;
	double u2x_ = u2x * cos_a - u2y * sin_a;
	double u2y_ = u2x * sin_a + u2y * cos_a;

	double v1x_ = u2x_, v1y_ = u1y_;
	double v2x_ = u1x_, v2y_ = u2y_;

	double v1x = v1x_ * cos_a + v1y_*  sin_a;
	double v1y = - v1x_ * sin_a + v1y_ * cos_a;
	double v2x = v2x_ * cos_a + v2y_ *  sin_a;
	double v2y = - v2x_ * sin_a + v2y_ * cos_a;

	v[ball1].x = v1x;
	v[ball1].y = v1y;
	v[ball2].x = v2x;
	v[ball2].y = v2y;
}

void initialize()
{
	//position of the centers initialization
	int x = 500, y = 220; //co-ordinate of the center ball
	c[0].x = 125, c[0].y = y; //white ball
	c[1].x = x - 4 * R + 4, c[1].y = y;
	c[2].x = x - 2 * R + 2, c[2].y = y - R;
	c[3].x = x - 2 * R + 2, c[3].y = y + R;
	c[4].x = x + 4 * R - 4, c[4].y = y; 
	c[5].x = x + 2 * R - 2, c[5].y = y + R;
	c[6].x = x + 2 * R - 2,  c[6].y = y - R;
	c[7].x = x, c[7].y = y + 2 * R;
	c[8].x = x, c[8].y =  y - 2 * R; //center ball
	c[9].x = x, c[9].y = y;

	for(int i = 0; i < 10; i++) c[i].visible = true;

	//velocity initialization
	v[0].x = 0, v[0].y = 0;
	v[1].x = 0, v[1].y = 0;
	v[2].x = 0, v[2].y = 0;
	v[3].x = 0, v[3].y = 0;
	v[4].x = 0, v[4].y = 0;
	v[5].x = 0, v[5].y = 0;
	v[6].x = 0, v[6].y = 0;
	v[7].x = 0, v[7].y = 0;
	v[8].x = 0, v[8].y = 0;
	v[9].x = 0, v[9].y = 0;

	//color initialization
	col[0].r = 255, col[0].g = 255, col[0].b = 255;
	col[1].r = 255, col[1].g = 255, col[1].b = 0;
	col[2].r = 0, col[2].g = 0, col[2].b = 255;
	col[3].r = 255, col[3].g = 0, col[3].b = 0;
	col[4].r = 0, col[4].g = 0, col[4].b = 255;
	col[5].r = 100, col[5].g = 0, col[5].b = 100;
	col[6].r = 0, col[6].g = 100, col[6].b = 200;
	col[7].r = 255, col[7].g = 0, col[7].b = 0;
	col[8].r = 0, col[8].g = 255, col[8].b = 255;
	col[9].r = 255, col[9].g = 255, col[9].b = 0;

}