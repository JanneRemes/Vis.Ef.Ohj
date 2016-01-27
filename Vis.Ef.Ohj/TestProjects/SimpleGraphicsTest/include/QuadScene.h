#include "scene.h"
#include <core/log.h>

class QuadScene : public Scene
{
public:
	QuadScene()
	{

		LOG("QuadScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_hShaderProgram = 0;

		const char* g_strVSProgram =
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"attribute vec2 g_vTexCoord;										 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVSTexCoord;                                       \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"    g_vVSTexCoord = g_vTexCoord;                                    \n"
			"}																	 \n";


		const char* g_strFSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVSTexCoord;                                       \n"
			"uniform float t;													 \n"	
			"																	 \n"
			"void main()														 \n"
			"{																	 \n" 
			" float x = g_vVSTexCoord.x * 500;									 \n"
			" float y = g_vVSTexCoord.y * 500;									 \n"	
			" float vv = 0.5 * tan((0.5 + (0.5 * sin(x / 5.0 + t * 10))) +		 \n"
			" (t * 0.5 + (0.5 * cos(y / 3.0 - t * 0.5))) +							 \n"
			" sin(0.5 + (0.1 * sin((x + y) / 6.0 - t))) +							 \n"
			" tan(0.5 + (0.15 * -cos(sqrt(float(x*x + y*y)) / 4.0 - t * 5))));		 \n"
			"																	 \n"	
			"    gl_FragColor = g_vVSColor * vv;								 \n"
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor",  1 },
			{ "g_vTexCoord", 2}
		};

		FrmCompileShaderProgram(g_strVSProgram, g_strFSProgram, &m_hShaderProgram, attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		checkOpenGL();
	}


	virtual ~QuadScene()
	{
		glDeleteProgram(m_hShaderProgram);
		LOG("QuadScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime;
		m_timer += deltaTime / 5;

		if (m_count > 1.0f)
			m_count = 0.0f;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();

		float aspectRatio = float(esContext->width) / float(esContext->height);
		float fSize = 0.98f;
		float VertexPositions[] =
		{
			-fSize,  +fSize, 0.0f, 1.0f,
			+fSize, +fSize, 0.0f, 1.0f,
			-fSize, -fSize, 0.0f, 1.0f,
			+fSize, -fSize, 0.0f, 1.0f,
		};

		float VertexColors[] = 
		{ 
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f
		};

		float TextureCoordinates[] = 
		{
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		// Clear the backbuffer and depth-buffer
		glClearColor(0.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		checkOpenGL();

		// Set the shader program and the texture
		glUseProgram(m_hShaderProgram);
		checkOpenGL();

		glUniform1f(glGetUniformLocation(m_hShaderProgram, "t"), m_timer);

		// Draw the colored triangle
		glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, VertexPositions);
		checkOpenGL();
		glEnableVertexAttribArray(0);
		checkOpenGL();

		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray(1);
		checkOpenGL();

		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, TextureCoordinates);
		checkOpenGL();
		glEnableVertexAttribArray(2);
		checkOpenGL();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		checkOpenGL();

		glDisableVertexAttribArray(0);
		checkOpenGL();
		glDisableVertexAttribArray(1);
		checkOpenGL();
	}
private:
	float m_count;
	float m_timer;

	GLuint       m_hShaderProgram;

};