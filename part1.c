volatile int pixel_buffer_start; // global variable
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x1, int y1, int x2, int y2, short int line_color);
void swap(int *x, int *y);
void clear_screen();
int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
    draw_line(0, 0, 150, 150, 0x001F);   // this line is blue
    draw_line(150, 150, 319, 0, 0x07E0); // this line is green
    draw_line(0, 239, 319, 239, 0xF800); // this line is red
    draw_line(319, 0, 0, 239, 0xF81F);   // this line is a pink color

}

// code not shown for clear_screen() and draw_line() subroutines

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
void clear_screen(){
	int x;
	int y;
	for (x = 0; x <= 319; x ++){
		for (y = 0; y <= 239; y++){
			plot_pixel(x, y, 0x0000);
		}
	}
}
void draw_line(int x1, int y1, int x2, int y2, short int line_color)
{
	bool is_steep = abs(y2-y1) > abs(x2-x1);
	int y_step = 0;
	if(is_steep){
		swap(&x1, &y1);
		swap(&x2, &y2);
	}
	if(x1 > x2){
		swap(&x1,&x2);
		swap(&y1,&y2);
	}
	int deltax = x2 - x1;
	int deltay = abs(y2 - y1);
	int error = -(deltax/2);
	int y = y1;
	if(y1<y2){
		y_step = 1;
	}
	else
		y_step = -1;
	int x = 0;
	for(x = x1; x <= x2; x++){
		if(is_steep){
			plot_pixel(y,x, line_color);
		}
		else
			plot_pixel(x,y, line_color);
		error = error + deltay;
		if(error >= 0){
			y+=y_step;
			error -= deltax;
		}

	}

}

void swap(int *x, int *y)
{
	int z;
	z = *x;
	*x = *y;
	*y = z;
}