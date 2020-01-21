/*
*Coded By:
*Muktadir Rahman
*Student id: 13 05 097
*Sec: A2
*/
#include "iGraphics.h"
#include <stdio.h>
#include <math.h>

//size of window
#define HEIGHT 600
#define WIDTH  1200
#define HEIGHT_OF_BOARD 600
#define WIDTH_OF_BOARD  1200

#define N 16//no of balls
#define h_R 25 //radius of holes
#define R 20 //radius of the balls
#define DAMPING_FACTOR .99


/*stuctures*/
struct point{
	int x;
	int y;
};
struct holes{
	int x;
	int y;
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
velocity v[N];
color col[N];
holes h[2][3];
int lx = 0, ly = 300;
char no[] = "1"; //to print a number on every ball

/*end*/

/*prototypes of the funtions*/
void iDraw();
void iKeyboard(unsigned char key);
void iMouse(int button, int state, int mx, int my);
void iMouseMove(int mx, int my);
void move();	//this will change some thing every millisecond
void collision(int ball1, int ball2); //this will change the velocity of two balls after collision.
/*end*/



int main()
{ 
	h[0][0].x = h_R, h[0][0].y = h_R, h[0][1].x = 600, h[0][1].y = h_R, h[0][2].x = 1200-h_R, h[0][2].y = h_R;
	h[1][0].x = h_R, h[1][0].y = 600 - h_R, h[1][1].x = 600, h[1][1].y = 600 - h_R, h[1][2].x = 1200 - h_R, h[1][2].y = 600 - h_R;

	c[0].x = 200, c[0].y = 300, v[0].x = 0, v[0].y = 0, col[0].r = 255, col[0].g = 255, col[0].b = 255;

	c[1].x = 800, c[1].y = 300, v[1].x = 0, v[1].y = 0, col[1].r = 255, col[1].g = 255, col[1].b = 0;

	c[2].x = 800 + 2 * R, c[2].y = 300 + R, v[2].x = 0, v[2].y = 0, col[2].r = 0, col[2].g = 0, col[2].b = 255;
	c[3].x = 800 + 2 * R, c[3].y = 300 - R, v[3].x = 0, v[3].y = 0, col[3].r = 255, col[3].g = 0, col[3].b = 0;

	c[4].x = 800 + 4 * R, c[4].y = 300, v[4].x = 0, v[4].y = 0, col[4].r = 102, col[4].g = 0, col[4].b = 102;
	c[5].x = 800 + 4 * R, c[5].y = 300 - 2 * R, v[5].x = 0, v[5].y = 0, col[5].r = 0, col[5].g = 153, col[5].b = 153;
	c[6].x = 800 + 4 * R, c[6].y = 300 + 2 * R, v[6].x = 0, v[6].y = 0, col[6].r = 139, col[6].g = 0, col[6].b = 0;

	c[7].x = 800 + 6 * R, c[7].y = 300 + R, v[7].x = 0, v[7].y = 0, col[7].r = 153, col[7].g = 0, col[7].b = 76;
	c[8].x = 800 + 6 * R, c[8].y = 300 - R, v[8].x = 0, v[8].y = 0, col[8].r = 0, col[8].g = 0, col[8].b = 0;
	c[9].x = 800 + 6 * R, c[9].y = 300 + 3 * R, v[9].x = 0, v[9].y = 0, col[9].r = 0, col[9].g = 102, col[9].b = 0;
	c[10].x = 800 + 6 * R, c[10].y = 300 - 3 * R, v[10].x = 0, v[10].y = 0, col[10].r = 102, col[10].g = 51, col[10].b = 0;

	c[11].x = 800 + 8 * R, c[11].y = 300, v[11].x = 0, v[11].y = 0, col[11].r = 0, col[11].g = 0, col[11].b = 102;
	c[12].x = 800 + 8 * R, c[12].y = 300 + 2 * R, v[12].x = 0, v[12].y = 0, col[12].r = 0, col[12].g = 0, col[12].b = 25;
	c[13].x = 800 + 8 * R, c[13].y = 300 - 2 * R, v[13].x = 0, v[13].y = 0, col[13].r = 25, col[13].g = 0, col[13].b = 0;
	c[14].x = 800 + 8 * R, c[14].y = 300 + 4 * R, v[14].x = 0, v[14].y = 0, col[14].r = 102, col[14].g = 0, col[14].b = 255;
	c[15].x = 800 + 8 * R, c[15].y = 300 - 4 * R, v[15].x = 0, v[15].y = 0, col[15].r = 255, col[15].g = 69, col[15].b = 0;

	iSettimer(1, move);	
	iInitialize(WIDTH, HEIGHT, "International Pool");
	
	return 0;
}

void iDraw()
{
	int flag = 0;
	iClear();
	iSetcolor(0, 128, 0);
	iFilledRectangle(0, 0, 1200, 600);
	iSetcolor(0, 0, 0);
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 3; j++)
			iFilledCircle(h[i][j].x, h[i][j].y, h_R);
	}

	//int i = 2;
	/*iSetcolor(col[i].r, col[i].g, col[i].b);
	iFilledCircle(c[i].x, c[i].y, R);*/
	for (int i = 0; i < N; i++){
		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 3; k++){
				int a_x = (c[i].x - h[j][k].x) * (c[i].x - h[j][k].x);
				int a_y = (c[i].y - h[j][k].y) * (c[i].y - h[j][k].y);
				if ((a_x + a_y) < ((R + h_R) * (R + h_R))){
					c[i].x = -100; c[i].y = -100;
					v[i].x = 0; v[i].y = 0;
				}


				else{
					iSetcolor(col[i].r, col[i].g, col[i].b);
					iFilledCircle(c[i].x, c[i].y, R);
					iSetcolor(0, 0, 0);

					no[0] = (char)(i + '0');
					
					for (int l = 1; l < N; l++)
						iSetcolor(255, 255, 255);
						iText(c[i].x - 6, c[i].y - 5, no, GLUT_BITMAP_TIMES_ROMAN_24);
				}
			}
		}
	}

	if (c[0].x < 0){
		c[0].x = 200;
		c[0].y = 300;
		iFilledCircle(c[0].x, c[0].y, 20);
	}

	for (int i = 0; i < N; i++){
		if (v[i].x != 0 || v[i].y != 0)
			break;
		else{
			iSetcolor(0, 0, 0);
			iLine(c[0].x, c[0].y, lx, ly);
		}
	}
	

}

void iKeyboard(unsigned char key)
{

}

void iMouse(int button, int state, int mx, int my)
{
	v[0].x = (c[0].x - mx) / 20;
	v[0].y = (c[0].y - my) / 20;
}

void iMouseMove(int mx, int my)
{
	printf("%d %d\n", mx, my);
	lx = mx;
	ly = my;
}

void move() 
{

	for(int i = 0; i < N - 1; i++){
		for(int j = i + 1; j < N; j++){
			int d = (c[i].x - c[j].x) * (c[i].x - c[j].x) +  (c[i].y - c[j].y) * (c[i].y - c[j].y);
			
			if( d <= 4 * R * R) {
				collision(i, j);

				while( d < 4 * R * R){
					printf("i:%d j:%d\n", i, j );
					c[i].x += v[i].x;
					c[i].y += v[i].y;
					c[j].x += v[j].x;
					c[j].y += v[j].y;
					d = (c[i].x - c[j].x) * (c[i].x - c[j].x) +  (c[i].y - c[j].y) * (c[i].y - c[j].y);
					/*double m = ( (double)(c[i].y - c[j].y) ) /(c[i].x - c[j].x);
					if(c[i].y > c[j].y)
						 c[i].y = c[j].y + ( 2 * R + 1.0) / sqrt(m * m + 1 );
					else
						 c[i].y = c[j].y - ( 2 * R + 1.0) / sqrt(m * m + 1 );

					if(c[i].x > c[j].x)
						c[i].x = c[j].x + m * (c[i].y - c[j].y);
					else 
						c[i].x = c[j].x - m * (c[i].y - c[j].y);*/
				}
				
				
			}
		}
	}
	
	for(int i = 0; i < N; i++)
	{ 
		
		c[i].x +=  v[i].x;
		c[i].y +=  v[i].y;

		//printf("%d %d %lf %lf\n", c[0].x, c[0].y, v[0].x, v[0].y);
		//change the velocity if any collision with the wall.
		if(c[i].x + R > WIDTH_OF_BOARD || (c[i].x  < R && v[i].x < 0) ) v[i].x = -v[i].x;
		if(c[i].y + R > HEIGHT_OF_BOARD || (c[i].y  < R && v[i].y < 0)) v[i].y = -v[i].y;
		
		//damping
		v[i].x *= DAMPING_FACTOR;
		v[i].y *= DAMPING_FACTOR;
	}
	
}

void collision(int ball1, int ball2)
{

	//determination of angle 
	int b = ball1;
	int d = ball2;

	double m = (double)(c[ball1].y - c[ball2].y) / ((double)(c[ball1].x - c[ball2].x));
	double a = -atan(m);

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
	double v1y = -v1x_ * sin_a + v1y_ * cos_a;
	double v2x = v2x_ * cos_a + v2y_ *  sin_a;
	double v2y = -v2x_ * sin_a + v2y_ * cos_a;

	v[ball1].x = v1x;
	v[ball1].y = v1y;
	v[ball2].x = v2x;
	v[ball2].y = v2y;
}
