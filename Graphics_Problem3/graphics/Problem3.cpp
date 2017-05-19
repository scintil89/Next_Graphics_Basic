#include <windows.h>
#include <gl/glut.h>

#include <math.h>
#include <vector>


#include "gltools.h"


#define GL_PI 3.1415f


//////////////////////////////////////////////////////////////////////////
// GLfloat x1 = 0.0f;
// GLfloat y1 = 0.0f;
GLsizei rsize = 50.0f;

GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

GLfloat window_width;
GLfloat window_height;


static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zDistance = 0.0f;		///ADD

//////////////////////////////////////////////////////////////////////////

void DrawCube()
{

	glPushMatrix();
	glScalef(3.0f, 3.0f, 3.0f);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(60.0, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);

	//Top
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(10.0f, 10.0f, 10.0f);
	glColor3f(1.0f, 0.0f, 1.0f);    // 
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glColor3f(1.0f, 1.0f, 1.0f);     // 
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(10.0f, 10.0f, -10.0f);

	//Right Side
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(10.0f, 10.0f, -10.0f);
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(10.0f, 10.0f, 10.0f);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(10.0f, -10.0f, 10.0f);
	glColor3f(0.3f, 0.7f, 0.0f);     // 
	glVertex3f(10.0f, -10.0f, -10.0f);

	//Front Side
	glColor3f(0.3f, 0.7f, 0.0f);     // 
	glVertex3f(10.0f, -10.0f, -10.0f);
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(10.0f, 10.0f, -10.0f);
	glColor3f(1.0f, 1.0f, 1.0f);     // 
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glColor3f(1.0f, 0.5f, 0.0f);     // 
	glVertex3f(-10.0f, -10.0f, -10.0f);

	//Bottom
	glColor3f(1.0f, 0.5f, 0.0f);     // 
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glColor3f(0.3f, 0.7f, 0.0f);     // 
	glVertex3f(10.0f, -10.0f, -10.0f);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(10.0f, -10.0f, 10.0f);
	glColor3f(0.0f, 1.0f, 1.0f);     // 
	glVertex3f(-10.0f, -10.0f, 10.0f);

	//Left
	glColor3f(0.0f, 1.0f, 1.0f);     // 
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glColor3f(1.0f, 0.5f, 0.0f);     // 
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glColor3f(1.0f, 1.0f, 1.0f);     // 
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glColor3f(1.0f, 0.0f, 1.0f);    // 
	glVertex3f(-10.0f, 10.0f, 10.0f);

	//Back
	glColor3f(1.0f, 0.0f, 1.0f);    // 
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(10.0f, 10.0f, 10.0f);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(10.0f, -10.0f, 10.0f);
	glColor3f(0.0f, 1.0f, 1.0f);     // 
	glVertex3f(-10.0f, -10.0f, 10.0f);



	glEnd();  // End Cube

	glPopMatrix();
}

void SetupRC(void)
{
	//GLfloat amb[] = { 0.3f,0.3f,0.3f };


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderScene(void)
{
	//static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	DrawCube();

	glPopMatrix();

	//glFlush(); //버퍼에 처리될걸 저장해뒀다가 한번에 glFlush로 처리
	glutSwapBuffers();
}

void TimerFunction(int value)
{
	glutPostRedisplay();

	glutTimerFunc(100, TimerFunction, 1);
}


void KeyControl(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		xRot -= 15.0f;
		printf_s("press up\n");
	}

	if (key == GLUT_KEY_DOWN)
	{
		xRot += 15.0f;
		printf_s("press down\n");
	}

	if (key == GLUT_KEY_LEFT)
	{
		yRot -= 15.0f;
		printf_s("press left\n");
	}

	if (key == GLUT_KEY_RIGHT)
	{
		yRot += 15.0f;
		printf_s("press right\n");
	}

	glutPostRedisplay(); //변경점이 생겼으니 다시 그려라
}


void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	GLfloat lightPos[] = { -50.f, 50.0f, 100.0f, 1.0f };


	//light Position

	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h; // Window의 종횡비 계산
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, fAspect, 1.0f, 255.0f); //(FOV,종횡비,near,far)


	//////////////////////////////////////////////////////////////////////////

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//light Position

	glTranslatef(0.0f, 0.0f, -150.0f);
}

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //초기화 싱글버퍼 사용

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Practicl Problem 3 "); //win title
// 	
// 	glutCreateMenu(ProcessMenu);
// 	glutAddMenuEntry("깊이 테스트", 1);
// 	glutAddMenuEntry("은면 제거", 2);
// 	glutAddMenuEntry("뒷면 라인 그리기", 3);
// 	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutReshapeFunc(ChangeSize);

	glutTimerFunc(33, TimerFunction, 1);


	glutSpecialFunc(KeyControl);	

	glutDisplayFunc(RenderScene); //콜백 함수 

	SetupRC(); //
	//LoadTexture();

	
	glutMainLoop(); // wm_exit 전까지 루프
}
