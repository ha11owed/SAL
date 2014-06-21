#include "preheader.h"
#include "renderer.h"

#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Renderer Renderer::g_instance;

inline void translate(const Point2D& point)
{
	glTranslated(point.x(), point.y(), 0.0);
}

inline void translate(const Point3D& point)
{
	glTranslated(point.x(), point.y(), point.z());
}

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "GL error 0x%X detected in %s\n", error, functionName);
	}
}

/*
* These global variables control which object is drawn,
* and how it is drawn.  No object uses all of these
* variables.
*/
static float ar;
static GLboolean persProject = GL_TRUE;
static GLboolean animateXRot = GL_FALSE;
static GLdouble zCamera = -3.0;

/*!
Does printf()-like work using freeglut
glutBitmapString().  Uses a fixed font.  Prints
at the indicated row/column position.

Limitation: Cannot address pixels.
Limitation: Renders in screen coords, not model coords.
*/
void Renderer::shapesPrintf(int row, int col, const char *fmt, ...)
{
	static char buf[256];
	int viewport[4];
	void *font = GLUT_BITMAP_9_BY_15;
	va_list args;

	va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
	(void)_vsnprintf(buf, sizeof(buf), fmt, args);
#else
	(void)vsnprintf(buf, sizeof(buf), fmt, args);
#endif
	va_end(args);

	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -1, 1);

	glRasterPos2i
		(
		glutBitmapWidth(font, ' ') * col,
		-glutBitmapHeight(font) * row + viewport[3]
		);
	glutBitmapString(font, (unsigned char*)buf);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

Vector3D Renderer::mouseToCoord(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble winX = 0.0, winY = 0.0, winZ = 0.0;
	winX = (double)x;
	winY = (double)(viewport[3] - y); // invert winY so that down lowers value

	glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &winZ);

	GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	GLdouble fromX = 0.0, fromY = 0.0, fromZ = 0.0;
	gluUnProject(winX, winY, 2.0, modelview, projection, viewport, &fromX, &fromY, &fromZ);
	return Vector3D(posX - fromX, posY - fromY, -posZ); // invert z value

	//return Vector3D(posX, posY, -posZ); // invert z value
}

// ---- GLUT callback Handlers ----
static void renderer_resize(int width, int height)
{
	ar = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

static void renderer_display()
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*89.0;
	const double b = (animateXRot ? t : 1)*67.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double zNear = 2.0;
	if (persProject)
		glFrustum(-ar, ar, -1.0, 1.0, zNear, 100.0);
	else
		glOrtho(-ar * 3, ar * 3, -3.0, 3.0, zNear, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, -zCamera, 0, 0, 0, 0, 1, 0);
	glTranslated(0.0, 0.0, zCamera);

	glEnable(GL_LIGHTING);
	glColor3d(1, 0, 0);

	Renderer& renderer = Renderer::instance();
	Scene* scene = renderer.scene;
	if (scene != nullptr)
	{
		scene->onDraw(&renderer);
	}

	glDisable(GL_LIGHTING);
	glColor3d(0.1, 0.1, 0.4);

	if (scene != nullptr)
	{
		scene->onDrawText(&renderer);
	}

	glutSwapBuffers();
}

static void renderer_key(unsigned char key, int x, int y)
{
	Renderer& renderer = Renderer::instance();
	Scene* scene = renderer.scene;

	switch (key)
	{
	case 27:
	case 'Q':
	case 'q': glutLeaveMainLoop();      break;

	default:
		if (scene != nullptr)
		{
			scene->onInput(&renderer, MouseKeyboardInput(key, x, y));
		}
		break;
	}

	glutPostRedisplay();
}

static void renderer_mouse(int button, int state, int x, int y)
{
	MouseKeyboardInput::KeyType mb = MouseKeyboardInput::None;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mb = (state == GLUT_DOWN) ? MouseKeyboardInput::MouseLeftPressed : MouseKeyboardInput::MouseLeftReleased;
		break;
	case GLUT_MIDDLE_BUTTON:
		mb = (state == GLUT_DOWN) ? MouseKeyboardInput::MouseMiddlePressed : MouseKeyboardInput::MouseMiddleReleased;
		break;
	case GLUT_RIGHT_BUTTON:
		mb = (state == GLUT_DOWN) ? MouseKeyboardInput::MouseRightPressed : MouseKeyboardInput::MouseRightReleased;
		break;
	}

	Renderer& renderer = Renderer::instance();
	Scene* scene = renderer.scene;
	if (scene != nullptr)
	{
		scene->onInput(&renderer, MouseKeyboardInput(mb, x, y));
	}

	if (mb != MouseKeyboardInput::None)
	{
		glutPostRedisplay();
	}
}

static void renderer_special(int key, int x, int y)
{
	MouseKeyboardInput::KeyType kt = MouseKeyboardInput::None;

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		kt = MouseKeyboardInput::ArrowRight;
		break;
	case GLUT_KEY_LEFT:
		kt = MouseKeyboardInput::ArrowLeft;
		break;
	case GLUT_KEY_UP:
		kt = MouseKeyboardInput::ArrowUp;
		break;
	case GLUT_KEY_DOWN:
		kt = MouseKeyboardInput::ArrowDown;
		break;
	default:
		break;
	}

	Renderer& renderer = Renderer::instance();
	Scene* scene = renderer.scene;
	if (scene != nullptr) {
		scene->onInput(&renderer, MouseKeyboardInput(kt, x, y));
	}

	glutPostRedisplay();
}

static void renderer_idle()
{
	// Do some framerate limiting to prevent CPU usage of 100%
	const int TICKS_PER_SECOND = 25;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;

	static int lastTime = glutGet(GLUT_ELAPSED_TIME);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = std::abs(currentTime - lastTime);

	if (deltaTime >= SKIP_TICKS)
	{
		lastTime = currentTime;
		glutPostRedisplay();
	}
	else
	{
		Sleep(1);
	}
}

Renderer& Renderer::instance()
{
	return g_instance;
}

Renderer::Renderer() : scene(nullptr)
{
}


Renderer::~Renderer()
{
}


void Renderer::mainLoop()
{
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(40, 40);
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow("SAL Algorithms");

	// --- Initialize ---
	glPointSize(6.0);
	// --- End Initialize ---

	glutReshapeFunc(renderer_resize);
	glutDisplayFunc(renderer_display);
	glutKeyboardFunc(renderer_key);
	glutMouseFunc(renderer_mouse);
	glutSpecialFunc(renderer_special);
	glutIdleFunc(renderer_idle);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glClearColor(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
}

void Renderer::drawPoint(const Point3D& point, const Color& color)
{
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_POINTS);
	glVertex3d(point.x(), point.y(), point.z());
	glEnd();
}

void Renderer::drawLine(const Point3D& p1, const Point3D& p2, const Color& color)
{
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINES);
	glVertex3d(p1.x(), p1.y(), p1.z());
	glVertex3d(p2.x(), p2.y(), p2.z());
	glEnd();
}

void Renderer::drawVector(const Point3D& point, const Vector3D& vector, const Color& color)
{
	glColor3f(color.r, color.g, color.b);

	double tx = vector.x() + point.x();
	double ty = vector.y() + point.y();
	double tz = vector.z() + point.z();

	glBegin(GL_LINES);
	glVertex3d(point.x(), point.y(), point.z());
	glVertex3d(tx, ty, tz);
	glEnd();
}

