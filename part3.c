volatile int pixel_buffer_start; // global variable
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x1, int y1, int x2, int y2, short int line_color);
void swap(int *x, int *y);
void clear_screen();
void draw();
int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    int N = 6;
    int x_xs[N], y_xs[N],dx_xs[N],dy_xs[N],color[4],color_xs[N];
    color[4] = {0x001F,0x07E0,0xF800,0xF81F};
    int i;
    for (i = 0; i <N; i++){
        dx_xs[i] = rand()%2*2-1;
        dy_xs[i] = rand()%2*2-1;
        color_xs[i] = color[rand()%4];
        x_xs[i] = rand()%318;
        y_xs[i] = rand()%238;
    }
    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        draw();


        // code for drawing the boxes and lines (not shown)
        // code for updating the locations of boxes (not shown)

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}
void draw(){
    clear_screen();
    int i,j,k;
    bool bound = false;
    int N = 6;
    for (j=0; j<N; j++){

        plot_pixel(x_xs[j],y_xs[j],0x0000);
        plot_pixel(x_xs[j]+1,y_xs[j],0x0000);
        plot_pixel(x_xs[j],y_xs[j]-1,0x0000);
        plot_pixel(x_xs[j]+1,y_xs[j]-1,0x0000);
    }

    for (k=0; k<N; k++){
        if(!bound){
            x_xs[k]+=dx_xs[k];
            y_xs[k]+=dy_xs[k];
            if(x_xs[k] == 319||y_xs[k]==239||x_xs[k] == 0||y_xs[k]==0){
                bound = true;
            }
        }
        else if(bound){
            x_xs[k]-=dx_xs[k];
            y_xs[k]-=dy_xs[k];
            if(x_xs[k] == 319||y_xs[k]==239||x_xs[k] == 0||y_xs[k]==0){
                bound = false;
            }
        }
    }

    for (i=0; i<N; i++){
        draw_line(x_xs[i],y_xs[i],x_xs[(i+1)%N],y_xs[(i+1)%N],color_xs[i]);
    }
    for (j=0; j<N; j++){

        plot_pixel(x_xs[j],y_xs[j],color_xs[j]);
        plot_pixel(x_xs[j]+1,y_xs[j],color_xs[j]);
        plot_pixel(x_xs[j],y_xs[j]-1,color_xs[j]);
        plot_pixel(x_xs[j]+1,y_xs[j]-1,color_xs[j]);
    }

    

}
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


// code for subroutines (not shown)
