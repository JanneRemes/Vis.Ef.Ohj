#include "scene.h"
#include <core/log.h>
#include <graphics/Shader.h>
#include <graphics/Mesh.h>
#include <MyMaterials.h>
#include <teapot.h>

class Simple3DScene : public Scene
{
public:
	Simple3DScene()
	{

		LOG("3DScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_sharedValues.totalTime = 0.0f;

		FRM_SHADER_ATTRIBUTE attributes[2] = {
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL}
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

		m_shader = new graphics::Shader("assets/Simple3D.vs.txt", "assets/Simple3D.fs.txt", attributes, numAttributes);
		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);
		m_mesh = createTeapotMesh();

		checkOpenGL();
	}


	virtual ~Simple3DScene()
	{
		glDeleteProgram(m_shader->getProgram());
		LOG("3DScene destruct");
	}


	graphics::Mesh* createTeapotMesh()
	{
		// Create Index buffer
		graphics::IndexBuffer* IBO = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);

		// Create Vertex arrays
		graphics::VertexArray* VAO[] =
		{
			new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),
			new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices)
		};

		// Create Vertex buffer from Vertex arrays
		graphics::VertexBuffer* VBO = new graphics::VertexBuffer(&VAO[0], sizeof(VAO) / sizeof(VAO[0]));

		// Create Mesh from IBO and VBO
		return new graphics::Mesh(IBO, VBO);
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime;
		m_sharedValues.totalTime += deltaTime / 3;

		if (m_count > 1.0f)
			m_count = 0.0f;

		// Camera perspective matrix = field of view, aspect ratio, near plane distance and far plane distance
		float fAspect = (float)esContext->width / (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(
			slmath::radians(45.0f),
			fAspect,
			5.0f,
			1000.0f);

		// Look at view matrix = eye point, look at point and world up vector
		m_matView = slmath::lookAtRH(
			slmath::vec3(0.0f, 70.0f, 70.0f),
			slmath::vec3(0.0f, 15.0f, 0.0f),
			slmath::vec3(0.0f, 1.0f, 0.0f));

		// Update Teapot model matrix
		m_matModel = slmath::rotationX(-3.1415f * 0.5f);								// -90 degrees around X first
		m_matModel = slmath::rotationY(m_sharedValues.totalTime) * m_matModel;			// Rotate according to total time
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel;	// Translate
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
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Initialize GL state
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		checkOpenGL();

		// These values are needed to update for each mesh which is rendered (different model matrix)
		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;

		// Set matrix to shared values
		m_sharedValues.matModelViewProj = matModelViewProj;

		// Set the shader program and the texture
		m_material->bind();
		checkOpenGL();

		// Render the mesh using active material
		m_mesh->render();
		checkOpenGL();
	}
private:
	float m_count;

	SharedShaderValues m_sharedValues;
	core::Ref<graphics::Shader> m_shader;
	core::Ref<graphics::ShaderUniforms> m_material;
	core::Ref<graphics::Mesh> m_mesh;

	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;
};