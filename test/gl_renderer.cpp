
#include "stdafx.h"
#include "gl_renderer.h"

#include "gl_model.h"
#include "high_res_timer.h"



//////////////////////////////////////////////////////////////////////
// Static object
//////////////////////////////////////////////////////////////////////

gl_renderer::model s_Model = gl_renderer::SKULL;

gl_model gl_renderer::m_TestModel;
size_t gl_renderer::m_NbFrames = 0;
high_res_timer gl_renderer::m_Timer;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

gl_renderer::gl_renderer()
{
	using namespace std;

	atexit(AtExit);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(320, 240);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("Triangle Stripper Test");

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << endl;

	InitStates();
	InitScene();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}



gl_renderer::~gl_renderer()
{

}



//////////////////////////////////////////////////////////////////////
// Members Functions
//////////////////////////////////////////////////////////////////////

void gl_renderer::Display()
{
	// Benchmark part
	if (m_NbFrames == 0)
		m_Timer.Reset();

	++m_NbFrames;

	// Draw the test model
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0, 0.5, -13.0);
	glRotatef((m_Timer.ElapsedTime<float>() * 1000.0 / 20.0), 0.0, 1.0, 0.0);
//	glRotatef(145.0, 0.0, 1.0, 0.0);

//	glRotatef(-45.0, 1.0, 0.0, 0.0);	// Starship model test
//	glScalef(0.05, 0.05, 0.05);			// Starship model test

	m_TestModel.ExecuteAllMeshes();

	glFlush();
	glutSwapBuffers();

	// Ask for a redraw
	glutPostRedisplay();
}



void gl_renderer::AtExit()
{
	using namespace std;

	const float Time = m_Timer.ElapsedTime<float>();

	// Check Display GL Error
	cout << "Display OpenGL Error: ";
	if (! PrintGLError(cout))
		cout << "-NONE-" << endl;

	cout << endl;

	cout << "Number of frames: " << static_cast<unsigned int>(m_NbFrames) << "." << endl;
	cout << "Time taken to render: " << Time << " seconds." << endl;
	cout << "Average speed: " << float(m_NbFrames) / Time << " fps." << endl;

	cout << endl;
}



void gl_renderer::Reshape(int Width, int Height)
{
	glViewport(0, 0, (GLsizei) Width, (GLsizei) Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((GLdouble) Width) / ((GLdouble) Height), 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}



void gl_renderer::InitStates()
{
	// Various GL setting
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

//	glPolygonMode(GL_FRONT, GL_LINE);
//	glPolygonMode(GL_BACK, GL_LINE);
//	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	// Hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Assume tightly packed textures
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Check Init GL Error
	std::cout << "Init State OpenGL Error: ";
	if (! PrintGLError(std::cout))
		std::cout << "-NONE-" << std::endl;
}



void gl_renderer::InitScene()
{
	const bool SimpleLight = gl_mesh::s_ViewStrips;

	std::cout << std::endl;

	if (SimpleLight) {

		const GLfloat LightAmbient[]= { 0.5, 0.5, 0.5, 0.0 };
		const GLfloat LightDiffuse[]= { 1.0, 1.0, 1.0, 1.0 };
		const GLfloat LightPosition[]= { 10.0, 0.0, 10.0, 0.0 };

		glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);	
		glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
		glEnable(GL_LIGHT0);

	} else {

		const float ModelAmbient[] = { 0.1, 0.1, 0.1, 1.0 };

		const float MaterialAmbient[] = { 0.0, 0.0, 0.0, 1.0 };
		const float MaterialDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		const float MaterialSpecular[] = { 0.35, 0.35, 0.35, 1.0 };
		const float MaterialEmission[] = { 0.0, 0.0, 0.0, 1.0 };

		const size_t NbLights = 3;

		const float LightsSpecular[NbLights][4] = {
			{ 1.0, 1.0, 1.0, 1.0 },
			{ 1.0, 1.0, 1.0, 1.0 },
			{ 1.0, 1.0, 0.5, 1.0 }
		};

		const float LightsDiffuse[NbLights][4] = {
			{ 1.0, 0.2, 0.2, 1.0 },
			{ 0.2, 1.0, 0.2, 1.0 },
			{ 0.2, 0.2, 1.0, 1.0 }
		};

		const float LightsPosition[NbLights][4] = {
			{   0.0, -40.0, 40.0, 1.0 },
			{ -40.0,  40.0,  0.0, 1.0 },
			{  40.0,   0.0, 40.0, 1.0 }
		};

		glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);
		glMaterialfv(GL_FRONT, GL_EMISSION, MaterialEmission);

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ModelAmbient);
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

		for (GLenum i = 0; i < NbLights; ++i) {
			glLightf((GL_LIGHT0 + i), GL_CONSTANT_ATTENUATION, 0.0);
			glLightf((GL_LIGHT0 + i), GL_LINEAR_ATTENUATION, 0.02);
			glLightf((GL_LIGHT0 + i), GL_QUADRATIC_ATTENUATION, 0.0);
			glLightfv((GL_LIGHT0 + i), GL_SPECULAR, LightsSpecular[i]);
			glLightfv((GL_LIGHT0 + i), GL_POSITION, LightsPosition[i]);
			glLightfv((GL_LIGHT0 + i), GL_DIFFUSE, LightsDiffuse[i]);
			glEnable(GL_LIGHT0 + i);
		}
	}

	// Load 3D Test Model
	std::cout << "Loading model... " << std::flush;

	if (! LoadTestModel())
		throw("Couldn't load test model");

	std::cout << "Model loaded." << std::endl;
	std::cout << std::endl;

	m_TestModel.CompileAllMeshes();

	// Check Init GL Error
	std::cout << "Init Scene OpenGL Error: ";
	if (! PrintGLError(std::cout))
		std::cout << "-NONE-" << std::endl;
}



bool gl_renderer::LoadTestModel()
{
	const char ModelPath[] = "models/skull.vxo";/*"models/starship.vxo";*/

	std::ifstream File(ModelPath, std::ios::binary);

	if (! File)
		return false;

	return (m_TestModel.LoadFromVXO(File));
}



bool gl_renderer::PrintGLError(std::ostream & Out)
{
	GLenum errCode;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		Out << gluErrorString(errCode) << std::endl;
		return true;
	} else
		return false;
}


