
#ifndef HEADER_GUARD_GL_RENDERER_H
#define HEADER_GUARD_GL_RENDERER_H



class gl_model;
class high_res_timer;



// OpenGL Window/Renderer
class gl_renderer
{
public:
	gl_renderer();
	~gl_renderer();

	static enum model {
		SKULL,
		STARSHIP
	} s_Model;

private:
	static void Display();
	static void Reshape(int Width, int Height);
	static bool PrintGLError(std::ostream & Out);
	static void AtExit();

	void InitStates();
	void InitScene();

	bool LoadTestModel();

	static gl_model			m_TestModel;
	static high_res_timer	m_Timer;
	static size_t			m_NbFrames;
};



#endif // HEADER_GUARD_GL_RENDERER_H
