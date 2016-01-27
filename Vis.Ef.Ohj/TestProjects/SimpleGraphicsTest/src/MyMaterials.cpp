#include <MyMaterials.h>

GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* sharedShaderValues) : graphics::ShaderUniforms(shader), m_sharedShaderValues(sharedShaderValues)
{

}

GlobalShaderUniforms::~GlobalShaderUniforms()
{
}
