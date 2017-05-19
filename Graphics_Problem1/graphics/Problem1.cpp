#include <windows.h>
#include <gl/glut.h>
#include <random>

//////////////////////////////////////////////////////////////////////////
//const UINT numOfRect = 4;
// GLfloat x1 = 0.0f;
// GLfloat y1 = 0.0f;

//std::mt19937 rd;

GLsizei rsize = 40.0f;
GLsizei srsize = 20.0f;

GLfloat rect_pos[4][2] = {-50.0f, 50.0f, -50.0f, -50.0f, 50.0f, 50.0f, 50.0f, -50.0f};

GLfloat x_step[4] = { 1.0f, 1.5f, 0.5f, 2.0f };//{ rd() % 10, rd() % 10, rd() % 10, rd() % 10 };
GLfloat y_step[4] = { 1.0f, 1.5f, 0.5f, 2.0f };//{ rd() % 10, rd() % 10, rd() % 10, rd() % 10 };

GLfloat window_width;
GLfloat window_height;
//////////////////////////////////////////////////////////////////////////

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	

	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(rect_pos[0][0], rect_pos[0][1], rect_pos[0][0] + rsize, rect_pos[0][1] + rsize);
	glColor3f(0.7f, 0.0f, 0.3f);
	glRectf(rect_pos[0][0] + rsize / 4, rect_pos[0][1] + rsize / 4, rect_pos[0][0] + rsize / 4 + srsize, rect_pos[0][1] + rsize / 4 + srsize);


	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf(rect_pos[1][0], rect_pos[1][1], rect_pos[1][0] + rsize, rect_pos[1][1] + rsize);
	glColor3f(0.2f, 0.0f, 0.6f);
	glRectf(rect_pos[1][0] + rsize / 4, rect_pos[1][1] + rsize / 4, rect_pos[1][0] + rsize / 4 + srsize, rect_pos[1][1] + rsize / 4 + srsize);

	
	glColor3f(0.0f, 0.0f, 1.0f);
	glRectf(rect_pos[2][0], rect_pos[2][1], rect_pos[2][0] + rsize, rect_pos[2][1] + rsize);
	glColor3f(0.0f, 0.9f, 0.3f);
	glRectf(rect_pos[2][0] + rsize / 4, rect_pos[2][1] + rsize / 4, rect_pos[2][0] + rsize / 4 + srsize, rect_pos[2][1] + rsize / 4 + srsize);


	glColor3f(1.0f, 0.0f, 1.0f);
	glRectf(rect_pos[3][0], rect_pos[3][1], rect_pos[3][0] + rsize, rect_pos[3][1] + rsize);
	glColor3f(0.7f, 0.3f, 0.9f);
	glRectf(rect_pos[3][0] + rsize / 4, rect_pos[3][1] + rsize / 4, rect_pos[3][0] + rsize / 4 + srsize, rect_pos[3][1] + rsize / 4 + srsize);



	glPopMatrix();


	//glFlush(); //버퍼에 처리될걸 저장해뒀다가 한번에 glFlush로 처리
	glutSwapBuffers();
}

void TimerFunction(int value)
{
	//태두리
	for (int i = 0; i < 4; ++i)
	{
		if (rect_pos[i][0] > window_width - rsize || rect_pos[i][0] < -window_width)
		{
			x_step[i] = -x_step[i];
		}

		if (rect_pos[i][1] > window_height - rsize || rect_pos[i][1] < -window_height)
		{
			y_step[i] = -y_step[i];
		}

		if (rect_pos[i][0] > window_width - rsize)
		{
			rect_pos[i][0] = window_width - rsize - 1;
		}

		if (rect_pos[i][1] > window_height - rsize)
		{
			rect_pos[i][1] = window_height - rsize - 1;
		}

		rect_pos[i][0] += x_step[i];
		rect_pos[i][1] += y_step[i];
	}

	//충돌
	for (int i = 0; i < 4; ++i)
	{
		for (int j = i + 1; j < 4; ++j)
		{
			if (rect_pos[i][0] + rsize >= rect_pos[j][0] && rect_pos[i] <= rect_pos[j] + rsize
				&& rect_pos[i][1] + rsize >= rect_pos[j][1] && rect_pos[i][1] <= rect_pos[j][1] + rsize)
			{
				x_step[j] = -x_step[j];
				y_step[j] = -y_step[j];

				x_step[i] = -x_step[i];
				y_step[i] = -y_step[i];

			}
		}
	}

	glutPostRedisplay();

	glutTimerFunc(33, TimerFunction, 1);
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	if (w <= h)
	{
		window_width = 100.0f;
		window_height = 100.0f * h / w;

		glOrtho(-100.0f, 100.0f, -window_height, window_height, 1.0f, -1.0f);
	}
	else
	{
		window_width = 100.0f * w / h;
		window_height = 100.0f;

		glOrtho(-window_width, window_width, -100.0f, 100.0f, 1.0f, -1.0f);
	}


	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //초기화 싱글버퍼 사용

	glutInitWindowSize(800, 600);

	glutCreateWindow("BounceRectangle"); //win title

	glutDisplayFunc(RenderScene); //콜백 함수 
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(1000, TimerFunction, 1);

	SetupRC(); //

	glutMainLoop(); // wm_exit 전까지 루프
}
