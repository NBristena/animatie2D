#include <windows.h>
#include <gl/freeglut.h>
#include <math.h>
#include <stdlib.h>
#include <String>

/* Variabile necesare pentru desenarea textului:
*/
GLint x = 300;
GLint y = 200;
GLint xpos;
GLint ypos;

/* Variabile necesare pentru crearea pieselor de domino:
*  (valorile de pe axa Oy sunt la fel pentru toate piesele)
*/
#define NR_DOMINOS 6
GLint nr_dominos = NR_DOMINOS;
GLint yJos = 0;
GLint ySus = 150;
struct dominos {
	GLdouble xJos, xSus;
}dominos[NR_DOMINOS];

/* Variabile necesare pentru desenarea rotilor masinii:
*/
GLint raza = 35;
const GLdouble TWO_PI = 6.2831853;
GLuint regHex1;
GLuint regHex2;
static GLfloat rotTheta = 0.0;
class scrPt {
public:
	GLint x, y;
};

/* Variabile necesare pentru miscarea masinii:
*/
GLdouble x_tr = 1;					// miscare stanga - dreapta
GLdouble alpha = -0.01;				// unghiul de rotatie al rotilor
GLdouble tr_speed = 3;				// viteza masinii
GLint max_dr = 50, max_stg = -112;  // limitele miscarii

/* Variabile necesare pentru miscarea pieselor de domino:
*/
GLdouble rotate_angle_1 = 0.001;
GLdouble rotate_angle_2 = 0.001;
GLdouble rotate_angle_3 = 0.001;
GLdouble rotate_angle_4 = 0.001;
GLdouble rotate_angle_5 = 0.001;
GLdouble rotate_angle_6 = 0.001;
GLint start = 0; // indica inceperea miscarii


void init(void)				
{									  
	glClearColor(0.3, 1.0, 1.0, 0.0);				  // culoarea de fond a ferestrei
	glMatrixMode(GL_MODELVIEW);						  // reprezentare 2D realizata prin proiectie ortogonala
	glOrtho(-525.0, 500.0, -200.0, 500.0, -1.0, 1.0); // scoordonatele extreme ale ferestrei
}

/// MASA
void drawTable()
{
	glColor3f(0.5, 0.2, 0.0);	// culoarea interiorului mesei
		// piciorul stang
		glBegin(GL_POLYGON);
			glVertex2i(-400, -200);
			glVertex2i(-400, -1);
			glVertex2i(-360, -1);
			glVertex2i(-360, -200);
		glEnd();
		// piciorul drept
		glBegin(GL_POLYGON);
			glVertex2i(335, -200);
			glVertex2i(335, -1);
			glVertex2i(375, -1);
			glVertex2i(375, -200);
		glEnd();
		// placa
		glBegin(GL_POLYGON);
			glVertex2i(-400, -50);
			glVertex2i(-400, -1);
			glVertex2i(375, -1);
			glVertex2i(375, -50);
		glEnd();

	glColor3f(0.0, 0.0, 0.0);	// culoarea conturului mesei
		glBegin(GL_LINES);
			// piciorul stang
			glVertex2i(-400, -200);
			glVertex2i(-400, -1);
			glVertex2i(-360, -200);
			glVertex2i(-360, -50);
			// piciorul drept
			glVertex2i(335, -50);
			glVertex2i(335, -200);
			glVertex2i(375, -1);
			glVertex2i(375, -200);
			// placa
			glVertex2i(-400, -1);
			glVertex2i(375, -1);
			glVertex2i(-360, -50);
			glVertex2i(335, -50);
		glEnd();
}

/// PIESE DOMINO
void createDominos()
{
	for (int c = 1; c <= nr_dominos; c++)
	{
		if (c == 1)
		{
			dominos[c].xJos = 0;
			dominos[c].xSus = 30;
		}
		else
		{
			dominos[c].xJos = dominos[c - 1].xJos - 60;
			dominos[c].xSus = dominos[c - 1].xSus - 60;
		}
	}
}
void drawD1()
{
	glColor3f(0.1, 0.0, 0.1);
		glRecti(dominos[1].xJos, yJos, dominos[1].xSus, ySus);
}
void drawD2()
{
	glColor3f(0.2, 0.0, 0.2);
		glRecti(dominos[2].xJos, yJos, dominos[2].xSus, ySus);
}
void drawD3()
{
	glColor3f(0.3, 0.0, 0.3);
		glRecti(dominos[3].xJos, yJos, dominos[3].xSus, ySus);
}
void drawD4()
{
	glColor3f(0.4, 0.0, 0.4);
		glRecti(dominos[4].xJos, yJos, dominos[4].xSus, ySus);
}
void drawD5()
{
	glColor3f(0.5, 0.0, 0.5);
		glRecti(dominos[5].xJos, yJos, dominos[5].xSus, ySus);
}
void drawD6()
{
	glColor3f(0.6, 0.0, 0.6);
		glRecti(dominos[6].xJos, yJos, dominos[6].xSus, ySus);
}

/// MASINA
void drawCar()
{
	scrPt hexVertex;
	GLdouble hexTheta;
	GLint k;

	// Corp (triunghi)
	glColor3f(0.0, 0.2, 0.2);
		glBegin(GL_TRIANGLES);
			glVertex2f(245, 25);
			glVertex2f(140, 130);
			glVertex2f(350, 130);
		glEnd();

	// Roata 1
	regHex1 = glGenLists(1);
	glNewList(regHex1, GL_COMPILE);
		glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
				for (k = 0; k < 100; k++)
				{
					hexTheta = TWO_PI * k / 100;
					hexVertex.x = 195 + raza * cos(hexTheta);
					hexVertex.y = 35 + raza * sin(hexTheta);
					glVertex2i(hexVertex.x, hexVertex.y);
				}
			glEnd();
		glColor3f(0.6, 0.6, 0.6);
			glRecti(165, 33, 225, 37);
			glRecti(193, 5, 197, 65);
	glEndList();

	// Roata 2
	regHex2 = glGenLists(2);
	glNewList(regHex2, GL_COMPILE);
		glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
				for (k = 0; k < 100; k++)
				{
					hexTheta = TWO_PI * k / 100;
					hexVertex.x = 295 + raza * cos(hexTheta);
					hexVertex.y = 35 + raza * sin(hexTheta);
					glVertex2i(hexVertex.x, hexVertex.y);
				}
			glEnd();
		glColor3f(0.6, 0.6, 0.6);
			glRecti(265, 33, 325, 37);
			glRecti(293, 5, 297, 65);
	glEndList();

		/*   Miscarea rotilor:
		* 3. se muta obiectul in pozitia sa initiala
		* 2. se roteste obiectul
		* 1. se muta obiectul in origine
		*/
		glPushMatrix();
			glTranslatef(195, 35, 0.0);
			glRotatef(alpha, 0.0, 0.0, 1.0);
			glTranslatef(-195, -35, 0.0);
			glCallList(regHex1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(295, 35, 0.0);
			glRotatef(alpha, 0.0, 0.0, 1.0);
			glTranslatef(-295, -35, 0.0);
			glCallList(regHex2);
		glPopMatrix();

	glPopMatrix();
}

/// SCENA COMPLETA
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	std::string text1 = "Instructiuni:";
	std::string text2 = "1. Click dreapta pentru a misca masina";
	std::string text3 = "2. Click stanga pentru a o trimite in directa pieselor";
	std::string text4 = "3. Bucurati-va de cele 5 secunde de domino :)";
	// Afisare instructiuni
	if (nr_dominos == 6)
	{
		xpos = -300; ypos = 460;
		for (int i = 0; i < text1.length(); i++)
		{
			glRasterPos2f(xpos + i * 15, ypos);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
		}

		xpos = -450;  ypos = 425;
		for (int i = 0; i < text2.length(); i++)
		{
			glRasterPos2f(xpos + i * 16, ypos);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
		}

		xpos = -450;  ypos = 400;
		for (int i = 0; i < text3.length(); i++)
		{
			glRasterPos2f(xpos + i * 16, ypos);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text3[i]);
		}

		xpos = -450;  ypos = 375;
		for (int i = 0; i < text4.length(); i++)
		{
			glRasterPos2f(xpos + i * 16, ypos);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text4[i]);
		}
	}

	drawTable();

	// Creerea, desenarea si miscarea pieselor de domino:
	createDominos();
		// piesa 1
		glPushMatrix();
			glRotated(rotate_angle_1, 0.0, 0.0, 1.0);
			drawD1();
		glPopMatrix();
		// piesa 2
		glPushMatrix();
			glTranslatef(-60.0, 0.0, 0.0);
			glRotatef(rotate_angle_2, 0.0, 0.0, 1.0);
			glTranslatef(60.0, 0.0, 0.0);
		drawD2();
		glPopMatrix();
		// piesa 3
		glPushMatrix();
			glTranslatef(-120.0, 0.0, 0.0);
			glRotatef(rotate_angle_3, 0.0, 0.0, 1.0);
			glTranslatef(120.0, 0.0, 0.0);
		drawD3();
		glPopMatrix();
		// piesa 4
		glPushMatrix();
			glTranslatef(-180.0, 0.0, 0.0);
			glRotatef(rotate_angle_4, 0.0, 0.0, 1.0);
			glTranslatef(180.0, 0.0, 0.0);
		drawD4();
		glPopMatrix();
		// piesa 5
		glPushMatrix();
			glTranslatef(-240.0, 0.0, 0.0);
			glRotatef(rotate_angle_5, 0.0, 0.0, 1.0);
			glTranslatef(240.0, 0.0, 0.0);
		drawD5();
		glPopMatrix();
		// piesa 6
		glPushMatrix();
			glTranslatef(-300.0, 0.0, 0.0);
			glRotatef(rotate_angle_6, 0.0, 0.0, 1.0);
			glTranslatef(300.0, 0.0, 0.0);
		drawD6();
		glPopMatrix();

	glPushMatrix();
		glTranslated(x_tr, 0, 0);
		drawCar();
	glPopMatrix();

	/*		COLIZIUNE
	* - atunci cand varful masinii loveste prima piesa de domino aceasta incepe rotatia
	* - limita stanga de miscare a masinii se modifica deoarece prima piesa se muta
	* - fiecare piesa isi incepe miscarea atunci cand este impinsa de o alta piesa
	*/

	if (x_tr <= max_stg)
	{
		start = 1;
		if (max_stg > -148)
			max_stg -= 1;
	}
	if (rotate_angle_1 < 60 && start == 1)
		rotate_angle_1 += 3;
	if (rotate_angle_2 < 60 && rotate_angle_1 >= 11.5)
		rotate_angle_2 += 3;
	if (rotate_angle_3 < 60 && rotate_angle_2 >= 11.5)
		rotate_angle_3 += 3;
	if (rotate_angle_4 < 60 && rotate_angle_3 >= 11.5)
		rotate_angle_4 += 3;
	if (rotate_angle_5 < 62 && rotate_angle_4 >= 11.5)
		rotate_angle_5 += 3;
	if (rotate_angle_6 < 90 && rotate_angle_5 >= 11.5)
		rotate_angle_6 += 3;

	glPopMatrix();

	// Text miscare max dreapta a masinii
	if (x_tr >= max_dr)
	{
		glColor3f(1.0, 0.0, 0.0);
			std::string text1Dr = "  STOP!  ";
			std::string text2Dr = "O SA CAZI";
			for (int i = 0; i < text1Dr.length(); i++)
			{
				glRasterPos2f(x + i * 17, y);
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1Dr[i]);
				glRasterPos2f(x + i * 17, y - 30);
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2Dr[i]);
			}
	}
	// Text miscare max stanga a masinii
	if (x_tr <= max_stg)
	{
		glColor3f(0.0, 0.3, 0.0);
			std::string textStg = "Nu ai unde sa mai mergi...";
			for (int i = 0; i < textStg.length(); i++)
			{
				glRasterPos2f(x - 600 + i * 17, y + 50);
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, textStg[i]);
			}
	}

	glutSwapBuffers();
	glFlush();
}

/// MOUSE INPUT
void moveLeft(void)
{

	if (x_tr > max_stg)
	{
		x_tr += -tr_speed;
		alpha += tr_speed;
	}
	glutPostRedisplay();
}
void moveRight(void)
{
	if (x_tr < max_dr)
	{
		x_tr += tr_speed;
		alpha -= tr_speed;
	}
	glutPostRedisplay();
}
void mouse(int key, int state, int x, int y)
{
	switch (key)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(moveLeft);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(moveRight);
		break;
	default:
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 0); // pozitia ferestrei de vizualizare (in coordonate ecran)
	glutInitWindowSize(1000, 700);  // dimensiunile ferestrei
	glutCreateWindow("Domino");		// creeaza fereastra cu numele ales

	init();
	glClear(GL_COLOR_BUFFER_BIT);
	glutDisplayFunc(drawScene);
	glutMouseFunc(mouse);
	glutMainLoop();
}
