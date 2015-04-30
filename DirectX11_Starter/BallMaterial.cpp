#include "BallMaterial.h"


BallMaterial::BallMaterial(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* rv, ID3D11SamplerState* ss) : Material(vs, ps, rv, ss)
{
}


BallMaterial::~BallMaterial()
{
}
