#include "PlayerMaterial.h"


PlayerMaterial::PlayerMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs, ps, rv, ss)
{
}


PlayerMaterial::~PlayerMaterial()
{
}
