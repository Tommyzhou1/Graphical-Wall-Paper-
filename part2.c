volatile int pixel_buffer_start; // global variable
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x1, int y1, int x2, int y2, short int line_color);
void swap(int *x, int *y);
void clear_screen();
void wait_for_vsync();
int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    
    int y1 = 0;
    int y2 = 0;
    bool reverse = false;
    //draw_line(0, 0, 319, 0, 0x001F);   // this line is blue
    clear_screen();
 
    while(1){
    	
    	
    	
    	
    	if(reverse){
    	y1 --;
    	y2 = y1;
    	draw_line(0, y1, 319, y2, 0x001F);   // this line is blue
	    	if(y1==0){
	    		reverse = false;
	    	}
    	}
    	else if(!reverse){
    	y1 ++;
    	y2 = y1;
    	draw_line(0, y1, 319, y2, 0x001F);   // this line is blue
	    	if(y1 == 239){
	    		reverse = true;
	    	}
    	}

    	 wait_for_vsync(); // swap front and back buffers on VGA vertical sync
         pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    	draw_line(0, y1, 319, y2, 0x0000);
    	
    	
    }

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
void wait_for_vsync(){
	volatile int* pixel_ctrl_ptr = 0xFF203020; //pixel controller
	register int status;

	*pixel_ctrl_ptr = 1; //start the sync process
	status = *(pixel_ctrl_ptr + 3);
	while ((status & 0x01) != 0){
		status = *(pixel_ctrl_ptr + 3);
	}
} 

void swap(int *x, int *y)
{
	int z;
	z = *x;
	*x = *y;
	*y = z;
}