#include "StdAfx.h"
#include "GlRenderer.h"

CGlRenderer::CGlRenderer(void)
{
}


CGlRenderer::~CGlRenderer(void)
{
}

void CGlRenderer::SetEffect(CGrModelX::IEffect *effect)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, effect->GetDiffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, effect->GetSpecular());
    glMaterialfv(GL_FRONT, GL_EMISSION, effect->GetEmissive());
    glMaterialf(GL_FRONT, GL_SHININESS, effect->GetShininess());

    bool hasTexture = effect->GetTexture() != NULL;
    if(hasTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, effect->GetTexture()->MipTexName());
    }
}

void CGlRenderer::EndEffect(CGrModelX::IEffect *effect)
{
    bool hasTexture = effect->GetTexture() != NULL;
    if(hasTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }
}