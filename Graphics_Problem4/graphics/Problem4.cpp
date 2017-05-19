#include <windows.h>
#include <gl/glut.h>

#include <math.h>
#include <vector>

#include "gltools.h"
#include "lodepng.h"


#define GL_PI 3.1415f


//////////////////////////////////////////////////////////////////////////
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

GLfloat Sr = 12.0f;
GLfloat Er = 8.0f;
GLfloat Mr = 4.0f;

GLfloat SEdist = 50.0f;
GLfloat EMdist = 15.0f;

GLfloat SEtheta = 0.0f;
GLfloat EMtheta = 0.0f;

GLuint Stex;
GLuint Etex;
GLuint Mtex;	
// GLfloat SEalpha = 0.005f;


GLfloat amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPos[] = { 1.0f, 0.0f, 0.0f, 0.0f };

// GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
// GLfloat spotDir[] = { 0.0f, 0.0f, -1.0f };

//////////////////////////////////////////////////////////////////////////

void LoadTexture()
{
	//////////////////////////////////////////////////////////////////////////
	//Sun
	std::vector<UCHAR> Sun;

	unsigned Swidth, Sheigh, Serror;

	Serror = lodepng::decode(Sun, Swidth, Sheigh, "sun.png");

	if (Serror)
	{
		printf_s("sun error %u: %s\n", Serror, lodepng_error_text(Serror));
	}

	printf_s("sun image size is %i\n", Sun.size());

	glGenTextures(1, &Stex);
	glBindTexture(GL_TEXTURE_2D, Stex);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, Swidth, Sheigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, Sun.data());

	//////////////////////////////////////////////////////////////////////////
	//Earth
	std::vector<UCHAR> Earth;

	unsigned Ewidth, Eheigh, Eerror;

	Eerror = lodepng::decode(Earth, Ewidth, Eheigh, "earth.png");

	if (Eerror)
	{
		printf_s("earth error %u: %s\n", Eerror, lodepng_error_text(Eerror));
	}

	printf_s("earth image size is %i\n", Earth.size());

	glGenTextures(1, &Etex);
	glBindTexture(GL_TEXTURE_2D, Etex);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, Ewidth, Eheigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, Earth.data());

	//////////////////////////////////////////////////////////////////////////
	//Moon
	std::vector<UCHAR> Moon;

	unsigned Mwidth, Mheigh, Merror;

	Merror = lodepng::decode(Moon, Mwidth, Mheigh, "moon.png");

	if (Merror)
	{
		printf_s("moon error %u: %s\n", Merror, lodepng_error_text(Merror));
	}

	printf_s("moon image size is %i\n", Moon.size());

	glGenTextures(1, &Mtex);
	glBindTexture(GL_TEXTURE_2D, Mtex);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/*glTexEnvi(GL_TEXTURE_2D, , );*/
	glTexImage2D(GL_TEXTURE_2D, 0, 4, Mwidth, Mheigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, Moon.data());

	//////////////////////////////////////////////////////////////////////////
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
}

void SetupRC(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

// 	GLfloat amb[] = { 0.3f,0.3f,0.3f };
// 	GLfloat diff[] = { 0.7f,0.7f,0.7f };

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);  // 내부 폴리곤 연산 off (backface culling)
	glFrontFace(GL_CCW);

	glEnable(GL_LIGHTING);

// 	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
// 	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
// 	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
// 	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
// // 	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
// // 	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
// //	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 360.0f);
// 	glEnable(GL_LIGHT0);
// 
// 	glEnable(GL_COLOR_MATERIAL);
// 	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

// 	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
// 	glMateriali(GL_FRONT, GL_SHININESS, 90);
}

void RenderScene(void)
{
	//static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

// 	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
// 	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glDisable(GL_LIGHTING); //태양 밝게 하기 위해


	//////////////////////////////////////////////////////////////////////////
	//SUN
	glPushMatrix();
		GLUquadric* sun = gluNewQuadric();

		gluQuadricDrawStyle(sun, GLU_FILL);
		glBindTexture(GL_TEXTURE_2D, Stex);
		gluQuadricTexture(sun, GL_TRUE);
		gluQuadricNormals(sun, GLU_SMOOTH);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_POLYGON);
		//glColor3f(1.0f, 0.0f, 0.0f);
// 		for (int i = 0; i < 360; i++)
// 		{
// 			glVertex2f(Sr*cosf(i * 180 / PI), Sr*sinf(i * 180 / PI));
// 		}
		gluSphere(sun, 8.0f, 30, 30);
	glEnd();

	//////////////////////////////////////////////////////////////////////////
	//EARTH

	//빛 방향 계산
	lightPos[0] = cosf(SEtheta * 180 / GL_PI);
	lightPos[1] = sinf(SEtheta * 180 / GL_PI);
	lightPos[2] = 0.0f;
	lightPos[3] = 1.0f;

	//lighting 적용
	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	// 	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	// 	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	//	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 360.0f);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	glPushMatrix();
		GLUquadric* earth = gluNewQuadric();

		gluQuadricDrawStyle(earth, GLU_FILL);
		glBindTexture(GL_TEXTURE_2D, Etex);
		gluQuadricTexture(earth, GL_TRUE);
		gluQuadricNormals(earth, GLU_SMOOTH);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTranslatef(SEdist*cosf(SEtheta * 180 / GL_PI), SEdist*sinf(SEtheta * 180 / GL_PI), 0);
		
		glBegin(GL_POLYGON);
		//glColor3f(0.0f, 1.0f, 0.0f);
// 		for (int i = 0; i < 360; i++)
// 		{
// 			glVertex2f(Er*cosf(i * 180 / PI), Er*sinf(i * 180 / PI));
// 		}
		gluSphere(earth, 6.0f, 30, 30);
	glEnd();

	//////////////////////////////////////////////////////////////////////////
	//MOON
	glPushMatrix();
		GLUquadric* moon = gluNewQuadric();

		gluQuadricDrawStyle(moon, GLU_FILL);
		glBindTexture(GL_TEXTURE_2D, Mtex);
		gluQuadricTexture(moon, GL_TRUE);
		gluQuadricNormals(moon, GLU_SMOOTH);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTranslatef(EMdist*cosf(EMtheta * 180 / GL_PI), EMdist*sinf(EMtheta * 180 / GL_PI), 0);


		glBegin(GL_POLYGON);
		//glColor3f(0.0f, 0.0f, 1.0f);
// 		for (int i = 0; i < 360; i++)
// 		{
// 			glVertex2f(Mr*cosf(i * 180 / PI), Mr*sinf(i * 180 / PI));
// 		}

		gluSphere(moon, 3.0f, 30, 30);
	glEnd();


	glPopMatrix();
	//END MOON
	//////////////////////////////////////////////////////////////////////////

	glPopMatrix();
	//END EARTH
	//////////////////////////////////////////////////////////////////////////
	
	glPopMatrix();
	//END SUN
	//////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();
}

void TimerFunction(int value)
{
	glutPostRedisplay();

	SEtheta += 0.0003f;
	EMtheta += 0.0020f;

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
	glutInitWindowSize(1200, 800);

	glutCreateWindow("Practicl Problem 4 "); //win title
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
	
	LoadTexture();

	glutMainLoop(); // wm_exit 전까지 루프
}
