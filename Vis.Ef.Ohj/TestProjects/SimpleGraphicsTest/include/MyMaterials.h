#pragma once
#include <graphics\Shader.h>

struct SharedShaderValues : public core::Object
{
	float totalTime;

	// Model view projection matrix. Used to transform position vertices to clip space.
	slmath::mat4 matModelViewProj;
};


class GlobalShaderUniforms : public graphics::ShaderUniforms
{
public:

	GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* sharedShaderValues);
	~GlobalShaderUniforms();

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
		m_3Did = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
	}

	virtual void bind(graphics::Shader* shader)
	{
		shader->bind();

		if (m_sharedShaderValues)
		{
			glUniform1f(m_id, m_sharedShaderValues->totalTime);
			glUniformMatrix4fv(m_3Did, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		}
	}

private:

	const SharedShaderValues* m_sharedShaderValues;
	GLint m_id;
	GLint m_3Did;
};