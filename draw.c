/******************************************************************
 * This program illustrates the fundamental instructions for handling
 * mouse and keyboeard events as well as menu buttons.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define    SIZEX   1000
#define    SIZEY   1000

#define    MY_QUIT  5
#define    MY_CLEAR 4
#define    MY_SAVE  1
#define    MY_BLEND 3
#define    MY_LOAD  2


#define    RED     1
#define    GREEN   2
#define    BLUE    3
#define    WHITE   4
#define    BLACK   5 

#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CURVE   4
#define    CIRCLE  5
#define    STICKER 6
typedef    int   menu_t;
menu_t     top_m, color_m, file_m, type_m;

int        height = 600, width = 900;
unsigned char  image[SIZEX * SIZEY][4];  /* Image data in main memory */

int        pos_x = -1, pos_y = -1;
float      myColor[3] = { 0.0,0.0,0.0 };
int        obj_type = -1;
int        first = 0;      /* flag of initial points for lines and curve,..*/
int        vertex[128][2]; /*coords of vertices */
int        side = 0;         /*num of sides of polygon */
float      pnt_size = 1.0;
float      circle_size = 10.0;
//宣告fun
void  change_color(int value);
void  change_type(int value);
void  change_size(int value);
void choose_file(int value); 

/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * Just clear the window again
 */
#define color_num 5
int ttt = 0;
float my_color[color_num][3] = { {1.0,0.0,0.0} ,{0.0,1.0,0.0}, {0.0,0.0,1.0},{0.0,0.0,0.0},{1.0,1.0,1.0} };
int color_btn[color_num][4] = {
      {55, 85,580,590}
     ,{95, 125,580,590}
     ,{135, 165,580,590}
     ,{175, 205,580,590}
     ,{215, 245,580,590} };

int type_btn[5][4] = {
  {55, 105,557,572}
 ,{110, 160,557,572}
 ,{165, 215,557,572}
 ,{220, 270,557,572}
 ,{275, 325,557,572} };
int size_btn[2][4] = {
  {55, 105,557,572}
 ,{110, 167,557,572} };

int file_btn[5][4] = {
  {55, 100,517,532}
 ,{110, 155,517,532}
 ,{165, 210,517,532}
 ,{220, 265,517,532}
 ,{275, 320,517,532} };
int orange_btn[2][4] = { 
    {0,0,0,0},{848, 857,579,590} };//x range y range 圓,樹枝
void background_color() {//背景
    glClearColor(1.0, 0.92, 0.85, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}
static GLUquadricObj* myOrange = NULL;
static GLUquadricObj* userOrange = NULL;
void color_bar() {
    glColor3f(0.0, 0.0, 0.0);
    char* col = "color";
    glRasterPos2f(5, height-20);

    for(char* i=col;*i!='\0';i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
   // float color_x = 55;// , color_y = 580;
    for (int i = 0; i < color_num; i++)
    {
        glBegin(GL_POLYGON);
        glColor3f(my_color[i][0], my_color[i][1], my_color[i][2]);
        glVertex3f(color_btn[i][0], height - 20, 0.0);
        glVertex3f(color_btn[i][1], height - 20, 0.0);
        glVertex3f(color_btn[i][1], height - 10, 0.0);
        glVertex3f(color_btn[i][0], height - 10, 0.0);
      //  color_btn[i][0] = color_x, color_btn[i][1] = color_x+30;
        color_btn[i][2] = height - 20, color_btn[i][3] = height - 10;
        //printf(" ,{%d, %d,%d,%d}\n", color_btn[i][0],color_btn[i][1], color_btn[i][2], color_btn[i][3]);
       // color_x += 40;
        glEnd();
    }
    glFlush();
}
void type_bar() {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "type";
    glRasterPos2f(5, height - 40);
    for (char* i = c; *i != '\0'; i++)
        // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    char* type[5] = {"Point","Line","Poly","Curve","Cricle"};

    for (int j = 0; j < 5; j++)
    {
        float temp =(j+1)*55;
        glBegin(GL_POLYGON);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(temp, height-43, 0.0);//557
        glVertex3f(temp + 50, height - 43, 0.0);
        glVertex3f(temp + 50, height - 28, 0.0);
        glVertex3f(temp, height-28, 0.0);
        //
           type_btn[j][0] = temp, type_btn[j][1] = temp+50;
          type_btn[j][2] = height - 43, type_btn[j][3] = height - 28;
       //   printf(" ,{%d, %d,%d,%d}\n", type_btn[j][0], type_btn[j][1], type_btn[j][2], type_btn[j][3]);
        //
        glEnd();
    }
    int s[5] = { 55,55,55,53,0 };
    float ini_x = 57;
    for (int j = 0; j < 5; j++)
    {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(ini_x, height - 40);
        for (char* i = type[j]; *i != '\0'; i++)
            // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        ini_x += s[j];
    }
    //color red
    glFlush();
}
void size_bar() {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Size";
    glRasterPos2f(5, height-60);
    for (char* i = c; *i != '\0'; i++)
        // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    char* type[2] = { "Bigger","Smaller" };

    for (int j = 0; j < 2; j++)
    {
        int temp = (j + 1) * 55;
        glBegin(GL_POLYGON);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(temp, height-63, 0.0);//537
        glVertex3f(temp + 50+j*7,height-63, 0.0);
        glVertex3f(temp + 50+j*7, height - 48, 0.0);
        glVertex3f(temp, height - 48, 0.0);
        //
        size_btn[j][0] = temp, size_btn[j][1] = temp + 50+j*7;
        size_btn[j][2] = height - 63, size_btn[j][3] = height - 48;
       //    printf(" ,{%d, %d,%d,%d}\n", size_btn[j][0], size_btn[j][1], size_btn[j][2], size_btn[j][3]);
         //
        glEnd();
    }
    int s[5] = { 55,55,55,53,0 };
    float ini_x = 57;
    for (int j = 0; j < 2; j++)
    {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(ini_x, height-60);
        for (char* i = type[j]; *i != '\0'; i++)
            // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        ini_x += s[j];
    }
    glFlush();
}
void flie_bar() {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "type";
    glRasterPos2f(5, height-80);
    for (char* i = c; *i != '\0'; i++)
        // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    char* type[5] = { "save","load","blend","clear","quit" };

    for (int j = 0; j < 5; j++)
    {
        float temp = (j + 1) * 55;
        glBegin(GL_POLYGON);
        glColor3f(0.5, 0.5, 0.5);
        glVertex2f(temp, height - 83);// , 0.0);
        glVertex2f(temp + 45, height - 83);// , 0.0);
        glVertex2f(temp + 45, height - 68);// , 0.0);
        glVertex2f(temp, height - 68);// , 0.0);
        //
        file_btn[j][0] = temp, file_btn[j][1] = temp + 45;
        file_btn[j][2] = height - 83, file_btn[j][3] = height - 68;
     //      printf(" ,{%d, %d,%d,%d}\n", file_btn[j][0], file_btn[j][1], file_btn[j][2], file_btn[j][3]);
         //

        glEnd();
    }
    int s[5] = { 55,55,55,53,0 };
    float ini_x = 57;
    for (int j = 0; j < 5; j++)
    {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(ini_x, height - 80);
        for (char* i = type[j]; *i != '\0'; i++)
            // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        ini_x += s[j];
    }
    //color red
    glFlush();
}
void mode_bar() {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "mode";
    glRasterPos2f(350, height - 20);
    for (char* i = c; *i != '\0'; i++)
        // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    char* type[2] = { "Text","" };

    for (int j = 0; j < 2; j++)
    {
        float temp = (j + 1) * 55;
        glBegin(GL_POLYGON);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(temp, 537, 0.0);
        glVertex3f(temp + 50 + j * 7, 537, 0.0);
        glVertex3f(temp + 50 + j * 7, 552, 0.0);
        glVertex3f(temp, 552, 0.0);
        glEnd();
    }
    int s[5] = { 55,55,55,53,0 };
    float ini_x = 57;
    for (int j = 0; j < 2; j++)
    {
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(ini_x, 540);
        for (char* i = type[j]; *i != '\0'; i++)
            // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        ini_x += s[j];
    }
    glFlush();
}
void sticker_orange()
{

    if (myOrange == NULL) {
        myOrange = gluNewQuadric();
        gluQuadricDrawStyle(myOrange, GLU_FILL);
    }
    glColor3f(1.0, 0.5, 0.0);
    glPushMatrix();
    int posx = width - 50, posy = height - 50;
    glTranslatef(posx, posy, 0.0);
    orange_btn[0][0] = posx;
    orange_btn[0][1] = posy;
    printf("%d %d\n", orange_btn[0][0], orange_btn[0][1]);
    gluDisk(myOrange,
        0.0,
        30,
        16,
        3);
    glPopMatrix();
    //光澤
    glColor3f(1.0, 0.74, 0.61);
    glPushMatrix();
    glTranslatef(posx+10, posy+20 , 0.0);
    gluDisk(myOrange,
        0.0,
        4,
        16,
        3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posx + 18, posy + 13, 0.0);
    gluDisk(myOrange,
        0.0,           // inner radius=0.0 
        3,          // outer radius=10.0 
        16,            // 16-side polygon 
        3);
    glPopMatrix();
    //樹枝
    for (int i = 0; i < 5; i++) {
        glColor3f(0.52, 0.28, 0.0);
        glBegin(GL_LINES);
      //  glVertex3f(width - 52 + i, height - 21, 0.0);
        glVertex3f(posx-2 + i, posy+29, 0.0);
        glVertex3f(posx +3 + i, posy + 40, 0.0);
      //  glVertex3f(width - 47 + i, height - 10, 0.0);
        if (!i)
        {
            orange_btn[1][0] = posx - 2 ;
            orange_btn[1][1] = posx + 3 + 4;
            orange_btn[1][2] = posy + 29;
            orange_btn[1][3] = posy + 40;
       printf(" ,{%d, %d,%d,%d}\n", orange_btn[1][0], orange_btn[1][1], orange_btn[1][2], orange_btn[1][3]);

        }
        glEnd();
    }
    glColor3f(0.0, 0.0, 0.0);
    char* c = "sticker";
    glRasterPos2f(width - 80, height - 92);
    for (char* i = c; *i != '\0'; i++)
        // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);

    glColor3f(myColor[0], myColor[1], myColor[2]);
    glFinish();//!!!

}

void draw_menu() {
    //菜單背景
    glColor3f(0.6, 0.77, 1.0);
    glBegin(GL_POLYGON);
    //glVertex3f(0, 900, 0.0);
    glVertex3f(0, height, 0.0);
    //glVertex3f(900, 900, 0.0);
    glVertex3f(width, height, 0.0);
    //glVertex3f(900, 505, 0.0);
    glVertex3f(width, height-100, 0.0);
    glVertex3f(0, height-100, 0.0);
    glEnd();
    ////
    color_bar();
    type_bar();
    size_bar();
    flie_bar();
    sticker_orange();
    ///
    glColor3f(myColor[0], myColor[1], myColor[2]);
}
void display_func(void)
{//每次渲染
    /* define window background color */
  //  printf("now x,y: %d , %d\n", x, y);

   printf( "display init\n");
    //不能在這改背景
   draw_menu();

    printf("display end\n");
    printf("--------------------------\n");
}


/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void my_reshape(int new_w, int new_h)
{
    printf("reshape init \n");
    height = new_h;
    width = new_w;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();  /*---Trigger Display event for redisplay window*/
    printf("reshape end \n");

}


/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'Q' || key == 'q') exit(0);
    if (key == 'A'||key=='a')
    {
        glRasterPos2f(5, 350);
        // Use GLUT_BITMAP_9_BY_15 font to display character 'A'
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
    }
}


/*---------------------------------------------------------
 * Procedure to draw a polygon
 */
void draw_polygon()
{
    int  i;

    glBegin(GL_POLYGON);
    for (i = 0; i < side; i++)
        glVertex2f(vertex[i][0], height - vertex[i][1]);
    glEnd();
    glFinish();
    side = 0;    /* set side=0 for next polygon */
}



/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle()
{
    static GLUquadricObj* mycircle = NULL;

    if (mycircle == NULL) {
        mycircle = gluNewQuadric();
        gluQuadricDrawStyle(mycircle, GLU_FILL);
    }
    glPushMatrix();
    glTranslatef(pos_x, height - pos_y, 0.0);
    gluDisk(mycircle,
        0.0,           /* inner radius=0.0 */
        circle_size,          /* outer radius=10.0 */
        16,            /* 16-side polygon */
        3);
    glPopMatrix();
}

void draw_orange(int posx, int posy)
{

    if (myOrange == NULL) {
        myOrange = gluNewQuadric();
        gluQuadricDrawStyle(myOrange, GLU_FILL);
    }
    glColor3f(1.0, 0.5, 0.0);
    glPushMatrix();
    posy = height - posy;
    glTranslatef(posx, posy, 0.0);
    gluDisk(myOrange,
        0.0,
        30,
        16,
        3);
    glPopMatrix();
    //光澤
    glColor3f(1.0, 0.74, 0.61);
    glPushMatrix();
    glTranslatef(posx + 10, posy + 20, 0.0);
    gluDisk(myOrange,
        0.0,
        4,
        16,
        3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posx + 18, posy + 13, 0.0);
    gluDisk(myOrange,
        0.0,           // inner radius=0.0 
        3,          // outer radius=10.0 
        16,            // 16-side polygon 
        3);
    glPopMatrix();
    //樹枝
    for (int i = 0; i < 5; i++) {
        glColor3f(0.52, 0.28, 0.0);
        glBegin(GL_LINES);
        glVertex3f(posx - 2 + i, posy + 29, 0.0);
        glVertex3f(posx + 3 + i, posy + 40, 0.0);
        glEnd();
    }


    glColor3f(myColor[0], myColor[1], myColor[2]);
    glFinish();//!!!
}

/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */

void mouse_func(int button, int state, int x, int y)
{   //drwa_my_screen();
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;
      printf("now x,y: %d , %d\n", x, y);

    if ((x - (double)orange_btn[0][0]) * (x - orange_btn[0][0]) + ((double)height - y - orange_btn[0][1]) * (height - y - orange_btn[0][1]) <= 900)// detect orange
    {
       // printf("orange detect\n");
        obj_type = STICKER;
        return;
    }
    if (x >= orange_btn[1][0] && x <= orange_btn[1][1] && orange_btn[1][2] <= height-y && orange_btn[1][3] >=height- y)//樹枝
    {
      //  printf("orange detect22\n");
        obj_type = STICKER;
        return;
    }
    if (height - y <= height-10 && height - y >= height-20)//color choose 560 580
    {
       // printf("press color bar\n");
        for (int i = 0; i < color_num; i++)
        {
            if (x >= color_btn[i][0] && x <= color_btn[i][1])
            {
               change_color(i+1);
            }
        }
        return;
    }
    else if (height - y <= height - 30 && height - y >= height - 40)
    {
        for (int i = 0; i < 5; i++)
        {
            if (x >= type_btn[i][0] && x <= type_btn[i][1])
            {
                printf("hahhaa\n");
                change_type(i + 1);
                printf("fun type %d\n", obj_type);

            }
        }
        return;
    }
    else if (height - y <= height - 50 && height - y >= height - 60)//SIZE
    {
        for (int i = 0; i < 2; i++)
        {
            if (x >= size_btn[i][0] && x <= size_btn[i][1])
            {
                change_size(i + 1);

            }
        }
        return;
    }
    else if (height - y <= height - 70 && height - y >= height - 80)//File
    {
        for (int i = 0; i < 5; i++)
        {
            if (x >= file_btn[i][0] && x <= file_btn[i][1])
            {
                choose_file(i + 1);

            }
        }
        return;
    }
    //if (height - y <= height - 405 && height - y >= 0)return;
  //  printf("now x,y: %d , %d\n", x, y);
    printf("mouse_fun ini\n");
    switch (obj_type) {
    case POINT:
        glPointSize(pnt_size);     /*  Define point size */
        glBegin(GL_POINTS);     /*  Draw a point */
        glVertex2f(x, height - y);
        glEnd();
        break;
    case LINE:
        if (first == 0) {
         //左端點
            first = 1;
            pos_x = x; pos_y = y;
            glPointSize(pnt_size);
            glBegin(GL_POINTS);   /*  Draw the 1st point */
             glVertex3f(x, height - y, 0);
            // glVertex3f(x, height - y, 24);
            glEnd();
        }
        else {
           //右端點
            first = 0;
            glLineWidth(pnt_size);     /* Define line width */
            glBegin(GL_LINES);    /* Draw the line */
            glVertex2f(pos_x, height - pos_y);
            glVertex2f(x, height - y);
            glEnd();
        }
        break;
    case POLYGON:  /* Define vertices of poly */
        if (side == 0) {
            vertex[side][0] = x; vertex[side][1] = y;
            side++;
        }
        else {
            if (fabs(vertex[side - 1][0] - x) + fabs(vertex[side - 1][1] - y) < 2)
                draw_polygon();
            else {
                glBegin(GL_LINES);
                glVertex2f(vertex[side - 1][0], height - vertex[side - 1][1]);
                glVertex2f(x, height - y);
                glEnd();
                vertex[side][0] = x;
                vertex[side][1] = y;
                side++;
            }
        }
        break;
    case CIRCLE:
        pos_x = x; pos_y = y;
        draw_circle();
        printf("rwweweew");
        break;
    case STICKER:
        draw_orange(x,y);
        printf("success\n");
        break;
    default:
        break;
    }
    glFinish();
    printf("mouse_fun end\n");

}

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
    printf("motion ini\n");
    if (obj_type != CURVE) return;
    if (first == 0) {
        first = 1;
        pos_x = x; pos_y = y;
    }
    else {
        printf("second");
        glBegin(GL_LINES);
        glVertex3f(pos_x, height - pos_y, 0.0);
        glVertex3f(x, height - y, 0.0);
        glEnd();
        pos_x = x; pos_y = y;
    }
    glFinish();
    printf("motion end\n");

}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void)
{
    /*Do nothing else but clear window to black*/
    printf("init_window ini\n");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //變黑色
    glClearColor(1.0, 0.92, 0.85, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    printf("init_window end\n");

}


/*------------------------------------------------------
 * Procedure to initialize data alighment and other stuff
 */
void init_func()
{
    glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*-----------------------------------------------------------------
 * Callback function for color menu
 */
void  change_color(int value)
{/*#define    RED     1
#define    GREEN   2
#define    BLUE    3
#define    WHITE   4
#define    BLACK   5 
*/
    switch (value) {
    case WHITE:
        myColor[0] = myColor[1] = myColor[2] = 0.0;
        break;

    case RED:
        myColor[0] = 1.0;
        myColor[1] = myColor[2] = 0.0;
        break;

    case GREEN:
        myColor[0] = myColor[2] = 0.0;
        myColor[1] = 1.0;
        break;

    case BLUE:
        myColor[0] = myColor[1] = 0.0;
        myColor[2] = 1.0;
        break;
    case BLACK:
        myColor[0] = myColor[1] = myColor[2] = 1.0;
        break;

    default:
        break;
    }
    glColor3f(myColor[0], myColor[1], myColor[2]);
}
void change_type(int value) {
    obj_type = value; 
    printf("fun type %d\n",obj_type);
}
void change_size(int value)
{
    if (value == 1) {
        if (pnt_size < 10.0) pnt_size += 1.0;
        if (circle_size < 60)circle_size += 1.0;
    }
    else {
        if (pnt_size > 1.0) pnt_size = pnt_size - 1.0;
        if (circle_size >1.0)circle_size -= 1.0;
    }
}
void choose_file(int value)
{
    int i, j;

    if (value == MY_QUIT) exit(0);
    else if (value == MY_CLEAR) init_window();
    else if (value == MY_SAVE) { /* Save current window */
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
            image);
        for (i = 0; i < width; i++)   /* Assign 0 opacity to black pixels */
            for (j = 0; j < height; j++)
                if (image[i * width + j][0] == 0 &&
                    image[i * width + j][1] == 0 &&
                    image[i * width + j][2] == 0) {
                    image[i * width + j][3] = 0;
                }
                else image[i * width + j][3] = 127; /* Other pixels have A=127*/
    }
    else if (value == MY_LOAD) { /* Restore the saved image */
        glRasterPos2i(0, 0);
        glDrawPixels(width, height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            image);
    }
    else if (value == MY_BLEND) { /* Blending current image with the saved image */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(0, 0);
        glDrawPixels(width, height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            image);
        glDisable(GL_BLEND);
    }

    glFlush();
}
/*------------------------------------------------------------
 * Callback function for top menu.
 */

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
}


/*-------------------------------------------------------------
 * Callback Func for type_m, define drawing object
 */
void draw_type(int value)
{
    obj_type = value;
    if (value == LINE || value == CURVE)
        first = 0;
    else if (value == POLYGON) side = 0;
}


/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
void main(int argc, char** argv)
{
    int  size_menu;

    glutInit(&argc, argv);    /*---Make connection with server---*/

    glutInitWindowPosition(0, 0);  /*---Specify window position ---*/
    glutInitWindowSize(width, height); /*--Define window's height and width--*/

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); /*---set display mode---*/
    init_func();

    /* Create parent window */
    glutCreateWindow("Paint");
    background_color();
    draw_menu();
    glutDisplayFunc(display_func); /* Associate display event callback func */
    glutReshapeFunc(my_reshape);  /* Associate reshape event callback func */

    glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    
  

    /*---Test whether overlay support is available --*/
    if (glutLayerGet(GLUT_OVERLAY_POSSIBLE)) {
        fprintf(stderr, "Overlay is available\n");
    }
    else {
        fprintf(stderr, "Overlay is NOT available, May encounter problems for menu\n");
    }
    /*---Enter the event loop ----*/
    glutMainLoop();
}
