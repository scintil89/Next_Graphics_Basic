#pragma comment(lib, "glew32.lib")

#include <windows.h>
#include <math.h>
//#include <vector>
#include <iostream>
#include <fstream>

//#include <gl/glew.h>
#include "GL/glew.h"

#include <gl/glut.h>
#include <gl/GLU.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "gltools.h"


#define GL_PI 3.1415f


//////////////////////////////////////////////////////////////////////////
GLfloat window_width;
GLfloat window_height;

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zDistance = 0.0f;		///ADD

bool iCull = false;
bool iOutline = false;
bool iDepth = false;

// GLfloat amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
// GLfloat dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
// GLfloat lightPos[] = { -50.0f, 50.0f, 100.0f, 1.0f };


struct LightSource
{
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	glm::vec3 position;
};

struct Material
{
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	GLfloat specular_shininess;
};

LightSource light;
Material mat;

// Shaders
GLuint shaderProgram;

static unsigned int setup_shader(const char *vertex_shader, const char *fragment_shader)
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const GLchar**)&vertex_shader, nullptr);

	glCompileShader(vs);	//compile vertex shader

	int status, maxLength;
	char *infoLog = nullptr;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);		//get compile status
	if (status == GL_FALSE)								//if compile error
	{
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);	//get error message length

															/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];

		glGetShaderInfoLog(vs, maxLength, &maxLength, infoLog);		//get error message

		fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] infoLog;
		return 0;
	}
	//	for fragment shader --> same as vertex shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const GLchar**)&fragment_shader, nullptr);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];

		glGetShaderInfoLog(fs, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Fragment Shader Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] infoLog;
		return 0;
	}

	unsigned int program = glCreateProgram();
	// Attach our shaders to our program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);


		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, NULL, infoLog);

		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Link Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] infoLog;
		return 0;
	}
	return program;
}

static std::string readfile(const char *filename)
{
	std::ifstream ifs(filename);
	if (!ifs)
		exit(EXIT_FAILURE);
	return std::string((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
}

//////////////////////////////////////////////////////////////////////////
void KeyControl(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		xRot -= 5.0f;
	}

	if (key == GLUT_KEY_DOWN)
	{
		xRot += 5.0f;
	}

	if (key == GLUT_KEY_LEFT)
	{
		yRot -= 5.0f;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		yRot += 5.0f;
	}

	glutPostRedisplay(); //변경점이 생겼으니 다시 그려라
}

void DrawJet()
{
	GLTVector3 vNormal;	// Storeage for calculated surface normal

	glColor3ub(128, 128, 128);
	glBegin(GL_TRIANGLES);

	//front bottom
	glNormal3f(0.0f, -1.0f, 0.0f);

	//		glVertex3f(0.0f, 0.0f, 60.0f);
	//		glVertex3f(-15.0f, 0.0f, 30.0f);
	//		glVertex3f(15.0f,0.0f,30.0f);
	//		//주석처리 제거   

	//Right Winng
	GLTVector3 vPoints[3] = { { 15.0f, 0.0f,  30.0f },
	{ 0.0f,  15.0f, 30.0f },
	{ 0.0f,  0.0f,  60.0f } };

	// Calculate the normal for the plane
	gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);

	glNormal3fv(vNormal);
	glVertex3fv(vPoints[0]);
	glVertex3fv(vPoints[1]);
	glVertex3fv(vPoints[2]);

	{
		GLTVector3 vPoints[3] = { { 0.0f, 0.0f, 60.0f },
		{ 0.0f, 15.0f, 30.0f },
		{ -15.0f, 0.0f, 30.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	// Body of the Plane ////////////////////////
	{
		GLTVector3 vPoints[3] = { { -15.0f, 0.0f, 30.0f },
		{ 0.0f, 15.0f, 30.0f },
		{ 0.0f, 0.0f, -56.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}

	{
		GLTVector3 vPoints[3] = { { 0.0f, 0.0f, -56.0f },
		{ 0.0f, 15.0f, 30.0f },
		{ 15.0f,0.0f,30.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(15.0f, 0.0f, 30.0f);
	glVertex3f(-15.0f, 0.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, -56.0f);

	///////////////////////////////////////////////
	// Left wing
	// Large triangle for bottom of wing
	{
		GLTVector3 vPoints[3] = { { 0.0f,2.0f,27.0f },
		{ -60.0f, 2.0f, -8.0f },
		{ 60.0f, 2.0f, -8.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	{
		GLTVector3 vPoints[3] = { { 60.0f, 2.0f, -8.0f },
		{ 0.0f, 7.0f, -8.0f },
		{ 0.0f,2.0f,27.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}

	{
		GLTVector3 vPoints[3] = { { 60.0f, 2.0f, -8.0f },
		{ -60.0f, 2.0f, -8.0f },
		{ 0.0f,7.0f,-8.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}

	{
		GLTVector3 vPoints[3] = { { 0.0f,2.0f,27.0f },
		{ 0.0f, 7.0f, -8.0f },
		{ -60.0f, 2.0f, -8.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	// Tail section///////////////////////////////
	// Bottom of back fin
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-30.0f, -0.50f, -57.0f);
	glVertex3f(30.0f, -0.50f, -57.0f);
	glVertex3f(0.0f, -0.50f, -40.0f);

	{
		GLTVector3 vPoints[3] = { { 0.0f,-0.5f,-40.0f },
		{ 30.0f, -0.5f, -57.0f },
		{ 0.0f, 4.0f, -57.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	{
		GLTVector3 vPoints[3] = { { 0.0f, 4.0f, -57.0f },
		{ -30.0f, -0.5f, -57.0f },
		{ 0.0f,-0.5f,-40.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	{
		GLTVector3 vPoints[3] = { { 30.0f,-0.5f,-57.0f },
		{ -30.0f, -0.5f, -57.0f },
		{ 0.0f, 4.0f, -57.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}

	{
		GLTVector3 vPoints[3] = { { 0.0f,0.5f,-40.0f },
		{ 3.0f, 0.5f, -57.0f },
		{ 0.0f, 25.0f, -65.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}

	{
		GLTVector3 vPoints[3] = { { 0.0f, 25.0f, -65.0f },
		{ -3.0f, 0.5f, -57.0f },
		{ 0.0f,0.5f,-40.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}

	{
		GLTVector3 vPoints[3] = { { 3.0f,0.5f,-57.0f },
		{ -3.0f, 0.5f, -57.0f },
		{ 0.0f, 25.0f, -65.0f } };

		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glVertex3fv(vPoints[0]);
		glVertex3fv(vPoints[1]);
		glVertex3fv(vPoints[2]);
	}


	glEnd();

}
//////////////////////////////////////////////////////////////////////////

void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Build and compile our shader program
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");

	mat.ambient_color = glm::vec3(0.1f, 0.1f, 0.1f);
	mat.diffuse_color = glm::vec3(1.0f, 0.0f, 0.0f);
	mat.specular_color = glm::vec3(1.0f, 1.0f, 1.0f);
	mat.specular_shininess = 15.0f;

	light.ambient_color = glm::vec3(1.0f, 0.0f, 0.0f);
	light.diffuse_color = glm::vec3(1.0f, 0.0f, 0.0f);
	light.specular_color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(4, -4, 4);

	shaderProgram = setup_shader(readfile("Phong.vertex").c_str(), readfile("Phong.fragment").c_str());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 	glFrontFace(GL_CCW);
	// 
	// 	glEnable(GL_LIGHTING);
	// 
	// 	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	// 	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	// 	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	// 	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	// 	
	// 	glEnable(GL_LIGHT0);
	// 
	// 	glEnable(GL_COLOR_MATERIAL);
	// 	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void RenderScene(void)
{
	//static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glMatrixMode(GL_MODELVIEW);
// 	
// 	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, ));


	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "light.ambient_color"), 1, false, glm::value_ptr(light.ambient_color));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "light.diffuse_color"), 1, false, glm::value_ptr(light.diffuse_color));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "light.specular_color"), 1, false, glm::value_ptr(light.specular_color));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, false, glm::value_ptr(light.position));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, false, glm::value_ptr(light.position));


	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "material.ambient_color"), 1, false, glm::value_ptr(mat.ambient_color));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "material.diffuse_color"), 1, false, glm::value_ptr(mat.diffuse_color));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "material.specular_color"), 1, false, glm::value_ptr(mat.specular_color));
	glUniform1f(glGetUniformLocation(shaderProgram, "material.specular_shininess"), mat.specular_shininess);

	glPushMatrix();

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glTranslatef(0.0f, 0.0f, zDistance);

	//glColor3f(1.0f, 0.0f, 0.0f);

	glUseProgram(shaderProgram);
	//DrawJet();

	glRectf(0.0f, 0.0f, 50.0f, 50.0f);
	glUseProgram(0);

	glPopMatrix();


	//glFlush(); //버퍼에 처리될걸 저장해뒀다가 한번에 glFlush로 처리
	glutSwapBuffers();
}

void TimerFunction(int value)
{
	glutPostRedisplay();

	glutTimerFunc(100, TimerFunction, 1);
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

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//light Position

	glTranslatef(0.0f, 0.0f, -150.0f);
}

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //초기화 싱글버퍼 사용

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Graphics_Problem5 "); //win title
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
			   //	LoadTexture();

	glutMainLoop(); // wm_exit 전까지 루프
}
