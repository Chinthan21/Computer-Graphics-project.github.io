#include<stdlib.h>
#include<stdio.h>
#include<glut.h>
#include<string.h>
#include<math.h>

int width = 2000, height = 1800;
void keyboard(unsigned char key, int a, int b);

void buttons1();
void delay();
void drawstring(GLfloat x, GLfloat y, const char* string);
void* currentfont;
float SCREEN_Y = 500, YD = SCREEN_Y / 12;

//

int ipflag = 0,n;
int pageArray[10] = { 0 }, pageOfBuffer[3] = { 0 }, fault[10] = { 0 };

	float pagePosition[10] = { -5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5 ,-5.5};


int hit = 0, step = -1, destination = 0, startAnimation = 0;
char res[] = "No. of Page Faults are :  ";

//Colours
float backgroundColor[][3] = { {1,0,0}, {0,1,0}, {0,0,1} }, tileColor[][3] = { {1,1,0}, {1,0.7,0.7}, {0,1,1} };
int bcPointer = 0, tcPointer = 0;

void setBGColor(int action)
{
    bcPointer = action;
    glutPostRedisplay();
}

void setTileColor(int action)
{
    tcPointer = action;
    glutPostRedisplay();
}

void menu(int action)
{
    if (action == 0)
        exit(0);
}

void addMenu()
{
    int submenu1, submenu2;

    submenu1 = glutCreateMenu(setBGColor);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);


    submenu2 = glutCreateMenu(setTileColor);
    glutAddMenuEntry("Yellow", 0);
    glutAddMenuEntry("Light Red", 1);
    glutAddMenuEntry("Light Blue", 2);


    glutCreateMenu(menu);
    glutAddSubMenu(" page No Title Colour", submenu1);
    glutAddSubMenu("Box No Colour", submenu2);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mouse(int btn, int state, int x, int y)
{
    printf("Mouse function called\n");
    int  i, j;

    if (startAnimation == 1) {
        printf("Animating");
        return;
    }
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (step < n)
            step++;
        else
            printf("%d\n", hit);


        for (i = 0; i < 3; i++)
        {
            if (pageArray[step] == pageOfBuffer[i])
            {
                hit++;
                fault[step] = 0;
                startAnimation = 1;
                destination = -5;
                glutPostRedisplay();
                return;
            }
        }
        destination = ((step - hit) % 3);
        printf("%d\n", destination);
        startAnimation = 1;
        fault[step] = 1;

        glutPostRedisplay();
    }
}

void tile(float x, float y, char n)
{

    float size = 20;
    //Colour of frame buffer
    // glColor3f(1,0,1);

    glBegin(GL_POLYGON);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y - size);
    glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size);
    glEnd();

    glColor3f(1.9, 0.3, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y - size);
    glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size);
    glEnd();
    glRasterPos2f(x - size / 2, y);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, n);
}

void setFrame()
{
    glPushMatrix();

    tile(0, 0, pageOfBuffer[0] == 0 ? ' ' : pageOfBuffer[0] + '0');
    glTranslatef(0, -45, 0);

    tile(0, 0, pageOfBuffer[1] == 0 ? ' ' : pageOfBuffer[1] + '0');
    glTranslatef(0, -45, 0);

    tile(0, 0, pageOfBuffer[2] == 0 ? ' ' : pageOfBuffer[2] + '0');

    glPopMatrix();
}

//for fault count
void draw_flag()
{
    glColor3fv(backgroundColor[bcPointer]);
    glBegin(GL_POLYGON);
    glVertex2f(-10, -10);
    glVertex2f(10, -10);
    glVertex2f(10, 10);
    glVertex2f(-10, 10);
    glEnd();
}

void setInput()
{
    glColor3fv(backgroundColor[bcPointer]);

    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(725, 0);
    glVertex2f(725, 75);
    glVertex2f(0, 75);
    glEnd();


    glPushMatrix();
    glTranslatef(-10, 40, 0);
    int i;
    for (i = 0; i < n; i++)
    {
        glColor3fv(tileColor[tcPointer]);
        glTranslatef(70, 0, 0);
        glPushMatrix();

        if (pagePosition[i] > -4.5)
        {
            //Moving right
            glTranslatef(70 * step - 70 * i, 0, 0);
        }
        //Input positions
        glTranslatef(0, -250 - (45 * pagePosition[i]), 0);

        if ((int)pagePosition[i] == destination && pagePosition[i] >= 0)
            glColor3f(1, 0.5, 0.8);
        else
            glColor3fv(tileColor[tcPointer]);
        //glColor3f(0.1,0.5,0.1);

        tile(11, 11, pageArray[i] + '0');
        glPopMatrix();

        if (fault[i])
        {
            glPushMatrix();
            glTranslatef(0, -385, 0);
            draw_flag();
            glPopMatrix();
        }

    }
    glPopMatrix();
}

void displayfifo()
{
	
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.5, 1);
    glLoadIdentity();

    glPushMatrix();
    //Frame Buffer position
    glTranslatef(120 + (70 * step), 195, 0);
    setFrame();
    glPopMatrix();

    glColor3f(1, 0, 0);
    glPushMatrix();
    //Input Red box position
    glTranslatef(50, 400, 0);
    setInput();
    glPopMatrix();

    glEnd();
    if (step == 9)
    {
        glColor3f(0, 0, 0);
        res[24] = (10 - hit) + '0';
        drawstring(50, 20, res);
    }
	glColor3f(1, -0, 0);
    drawstring(250, 550, "FIFO Page Replacement Algorithm");
    glFlush();
    glutSwapBuffers();
}

void idle()
{
    if (ipflag == 0)
    {
		printf("Enter the page for fifo operation\n");
        
            scanf_s("%d", &n);

        printf("Enter the page array of size n\n");
        for (int j = 0; j < n; j++)
            scanf_s("%d", &pageArray[j]);
        ipflag = 1;
    }
    if (!startAnimation)
        return;
    printf("Idle called\n");
    printf("%d, %f\n", destination, pagePosition[step]);
    if (destination > pagePosition[step])
        pagePosition[step] += 0.11;

    if (destination < pagePosition[step])//It has reached its location...So stop animation
    {
        if (fault[step])
            pageOfBuffer[destination] = pageArray[step];

        startAnimation = 0;
        destination = -11;
    }
    displayfifo();
}

void fifo()
{
	
    addMenu();
    glutDisplayFunc(displayfifo);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glClearColor(0.8, 1, -0.5, -1);
    glutPostRedisplay();
}

//


void setFont(void* font)
{
    currentfont = font;
}
void init(void)
{
    glClearColor(0, 0, 0, 0);
    glLineWidth(1);
    glColor3f(0.65, 0.65, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.1, 800, -100, 600);
    glMatrixMode(GL_MODELVIEW);
    setFont(GLUT_BITMAP_TIMES_ROMAN_24);
}
void delay()
{
    int i, j;
    for (i = 0; i < 32000; i++)
        for (j = 0; j < 15000; j++);
}
void drawstring(GLfloat x, GLfloat y, const char* string)
{
    const char* c;
    glRasterPos2f(x, y);
    for (c = string;
        *c != '\0';
        c++)
    {
        glutBitmapCharacter(currentfont, *c);
    }
}




void MOUSE2(int btn, int state, int x, int y)
{

}

void display()
{
    glClearColor(1.0, -1.2, 0.2, 0.1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.5, 0.0, 0.5);
    drawstring(250, 560, "SDM INSTITUTE OF TECHNOLOGY");
    glColor3f(1.0, 0.5, 0.5);
    drawstring(182, 530, "DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
    glColor3f(1.0, 0.5, 0.5);
    drawstring(182, 490, "COMPUTER GRAPHICS LABORATORY WITH MINI PROJECT ");
    glColor3f(0.5, 0.0, 0.5);
    drawstring(275, 450, "A MINI PROJECT ON");
    glColor3f(1.0, 1.0, 1.0);
    drawstring(260, 420, "**FIRST IN FIRST OUT**");

    glColor3f(0.5, 0.0, 0.5);
    drawstring(45, 180, "SUBMITTED BY:");
    glColor3f(1.0, 0.5, 0.5);
    drawstring(45, 150, "CHINTHAN");
    drawstring(45, 125, "Linkedin:www.linkedin.com/in/chinthan-k-92a631224");
    drawstring(45, 100, "Email:chinthank25@gmail.com");
    glColor3f(0.5, 0.0, 0.5);
    drawstring(540, 195, "GUIDED BY:");
    glColor3f(1.0, 0.5, 0.5);
    drawstring(540, 170, "Prof.Arjun K");
    glColor3f(0.8, 0.7, 0.2);
    drawstring(280, 60, "[PRESS 'X' TO START]");
    glutSwapBuffers();
    glFlush();
}
//glutSwapBuffers();

//to display sorted array



void square(int x1, int y1, int x2, int y2)
{
	    glColor3f(1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
	glLineWidth(10.5f);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}
void drawoutline(int x1, int y1, int x2, int y2)
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glLineWidth(3.5f);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}
void buttons1()
{
    glClearColor(-0.5, 0.5, -0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.5, -0.7);
    drawstring(270.0, 430.0, "MINI PROJECT");
    drawstring(270.0, 390.0, "FIFO");
    glColor3f(1.0, 1.0, 0.5);
    square(350, (SCREEN_Y / 2) + 33, 270, SCREEN_Y / 2 + 30 + YD - 3);//box2
    glColor3f(0.0, 0.0, 0.0);
    drawstring(280.0, SCREEN_Y / 2 + 47, "NEW(n)");//label
    drawoutline(350, (SCREEN_Y / 2) + 34, 270, SCREEN_Y / 2 + 30 + YD - 3);
    glColor3f(0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.5);
    square(350, (SCREEN_Y / 2) - 47, 270, SCREEN_Y / 2 - 50 + YD - 3);//box2
    glColor3f(0.0, 0.0, 0.0);
    drawstring(280, (SCREEN_Y / 2) - 33, "HELP(h)");//label
    drawoutline(350, (SCREEN_Y / 2) - 47, 270, SCREEN_Y / 2 - 50 + YD - 3);
    glColor3f(0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.5);
    square(350, SCREEN_Y / 2 - 128, 270, SCREEN_Y / 2 - 131 + YD - 3);//box2
    glColor3f(0.0, 0.0, 0.0);
    drawstring(280, (SCREEN_Y / 2) - 114, "ABOUT(a)");//label
    drawoutline(350, (SCREEN_Y / 2) - 128, 270, SCREEN_Y / 2 - 131 + YD - 3);
    glColor3f(1.0, 1.0, 0.5);
    square(350, SCREEN_Y / 2 - 208, 270, SCREEN_Y / 2 - 211 + YD - 3);//box2
    glColor3f(0.0, 0.0, 0.0);
    drawstring(280, (SCREEN_Y / 2) - 194, "QUIT(q)");//label
    drawoutline(350, (SCREEN_Y / 2) - 209, 270, SCREEN_Y / 2 - 211 + YD - 3);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MOUSE2);
    glFlush();
    glutSwapBuffers();
}



void help() {
    glColor3f(1.0, 0.5, 1);
    drawstring(263, 415, "FIFO");
    glColor3f(1.5, 1.0, 1.0);
    drawstring(50, 375, "INSTRUCTIONS:");
    drawstring(50, 370, "__");
    glColor3f(1, 0.5, 0.5);
    drawstring(60, 348, "Keyboard Functions");
    drawstring(60, 342, " ------------- ");
    glColor3f(1.0, 1.0, 0.0);
    drawstring(60, 325, "*Press 'X' to start");
    drawstring(60, 307, "*Press 'n' to enter the values");
    drawstring(60, 289, "*Press 'h' for help window");
    drawstring(60, 271, "*Press 'q' for quit");
    drawstring(60, 253, "*Press 'm' for main menu");
    drawstring(60, 181, "*press any key to close the terminal");
    glColor3f(1, 0.5, 0.5);
    drawstring(60, 158, "Mouse Function");
    drawstring(60, 152, "------------ ");
    glColor3f(0.5, 1, 1);
    drawstring(60, 135, "*left click on mouse we can able to select what color you want in fifo window");
    glutKeyboardFunc(keyboard);
    glutSwapBuffers();
    glFlush();

}

void about()
{
    //glClearColor(0.5, 0.5, 0.6, -1.0);
    
   
	glLineWidth(2);
    
    
   
    glBegin(GL_POLYGON);
    glColor3f(0.65, -0.65, 0.65);
    glVertex2d(0, 0);
    glColor3f(0.0, 0.138, 0.96);
    glVertex2d(0, height);
    glColor3f(0.0, 0.138, 0.96);
    glVertex2d(width, height);
    glColor3f(0.65, 0.65, 0.65);
    glVertex2d(width, 0);
    glEnd();
	// some information
	
	/* glColor3f(-1.5, 1.0, 1.0);
	 drawstring(50, 450, "->In a computer operating system  uses paging for virtual memory management, page replacement algorithms.");
	 drawstring(50, 425, "-> It decide which memory pages to page out, sometimes called swap out, or write to disk when a page of memory needs to be allocated.");
	 drawstring(50, 400, "-> Page replacement happens when a requested page is not in memory (page fault) and a free page cannot be used to satisfy the allocation. ");
	 drawstring(50, 375, "->Either because there are none, or because the number of free pages is lower than some threshold.");
    drawstring(50, 350, "->In this project, we created a Graphics animation to demonstrate FIFO Page Replacement Algorithm.");
    drawstring(50, 325, "->This is the simplest page replacement algorithm.");
    
    drawstring(50, 300, "->In this algorithm, the operating system keeps track of all pages in the memory in a queue, the oldest page is in the front of the queue.");
    drawstring(50, 275, " -> When a page needs to be replaced page in the front of the queue is selected for removal. ");*/
    //heading
    glColor3f(0.0, 0.0, 0.0);
    drawstring(290,560,"*WORKING FLOW CHART*");
    glColor3f(1.0, 1.0, 1.0);
   drawstring(260, 530, "-------------------------------");


    //introduction polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(350, 500);
    glVertex2i(555, 500);
    glVertex2i(555, 460);
    glVertex2i(350, 460);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(380, 475, "Introduction page");
    //intro-instruction
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(450, 460);
    glVertex2i(450, 425);
    glEnd();

    //instruction polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(350, 425);
    glVertex2i(555, 425);
    glVertex2i(555, 385);
    glVertex2i(350, 385);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(387, 400, "Instruction page");
    //horizontal
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(190, 365);
    glVertex2i(690, 365);
    glEnd();
    //instruction-horizontal
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(450, 385);
    glVertex2i(450, 365);
    glEnd();

    //instruction-algo
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(190, 365);
    glVertex2i(190, 350);
    glEnd();
    //instruction-end
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(690, 365);
    glVertex2i(690, 350);
    glEnd();
    //instruction-help
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(360, 365);
    glVertex2i(360, 350);
    glEnd();
    //instruction-simulation start
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(540, 365);
    glVertex2i(540, 350);
    glEnd();

    //algorithm polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(110, 350);
    glVertex2i(270, 350);
    glVertex2i(270, 310);
    glVertex2i(110, 310);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(125, 325, "Algorithm page");


    //help polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(290, 350);
    glVertex2i(430, 350);
    glVertex2i(430, 310);
    glVertex2i(290, 310);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(315, 325, "Help page");

    //start simulation polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(450, 350);
    glVertex2i(630, 350);
    glVertex2i(630, 310);
    glVertex2i(450, 310);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(470, 325, "Start Simulation");
    //simulation start-draw graph
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(540, 310);
    glVertex2i(540, 275);
    glEnd();

    //End polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(650, 350);
    glVertex2i(730, 350);
    glVertex2i(730, 310);
    glVertex2i(650, 310);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(670, 325, "Quit");
    // end-exit
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(690, 310);
    glVertex2i(690, 55);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(690, 55);
    glVertex2i(450, 55);
    glEnd();

    //simulation polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(450, 275);
    glVertex2i(630, 275);
    glVertex2i(630, 175);
    glVertex2i(450, 175);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(490, 250, "Enter the size of n");
    drawstring(490, 220, "Enter the elements");
    drawstring(490, 190, "Fifo blocks");
    //draw graph-output
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(540, 175);
    glVertex2i(540, 145);
    glEnd();

    //start simulation polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(450, 145);
    glVertex2i(630, 145);
    glVertex2i(630, 105);
    glVertex2i(450, 105);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(510, 120, "Output");

    //draw graph-output
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(540, 105);
    glVertex2i(540, 75);
    glEnd();

    //Exit polygon
    glColor3f(0.0, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(450, 75);
    glVertex2i(630, 75);
    glVertex2i(630, 35);
    glVertex2i(450, 35);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    drawstring(520, 50, "Exit");

	glFlush();
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(MOUSE2);
    
    glutSwapBuffers();


}


void keyboard(unsigned char key, int a, int b) {
    if (key == 'X' || key == 'x') {

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        buttons1();

    }
    if (key == 'h' || key == 'H') {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        help();
    }
    if (key == 'n' || key == 'N') {
        glClear(GL_COLOR_BUFFER_BIT);
        fifo();

    }
    if (key == 'q' || key == 'Q')
    {
        exit(0);
    }
    if (key == 'm' || key == 'M') {
        buttons1();
    }
    if (key == 'a' || key == 'A') {
        about();
    }

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1800, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("First in First out");
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}