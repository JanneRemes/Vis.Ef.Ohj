#include "scene.h"
#include <core/log.h>
#include <graphics/Shader.h>
#include <MyMaterials.h>

class QuadShaderScene : public Scene
{
public:
	QuadShaderScene()
	{

		LOG("QuadShaderScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_sharedValues.totalTime = 0.0f;

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor",  1 },
			{ "g_vTexCoord", 2 }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

		m_shader = new graphics::Shader("assets/FullScreenQuad.vs.txt", "assets/PlasmaEffect.fs.txt", attributes, numAttributes);
		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);

		checkOpenGL();
	}


	virtual ~QuadShaderScene()
	{
		glDeleteProgram(m_shader->getProgram());
		LOG("QuadShaderScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime;
		m_sharedValues.totalTime += deltaTime / 3;

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
		m_material->bind();
		checkOpenGL();

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

	SharedShaderValues m_sharedValues;
	core::Ref<graphics::Shader> m_shader;
	core::Ref<graphics::ShaderUniforms> m_material;

};