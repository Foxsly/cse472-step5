#pragma once

#include <grafx.h>

class CMyRaytraceRenderer : public CGrModelX::IRenderer
{
public:
    CMyRaytraceRenderer(void);
    virtual ~CMyRaytraceRenderer(void);

    virtual void PushMatrix() {}
    virtual void PopMatrix() {}
    virtual void MultMatrix(const CGrTransform &t) {}
    virtual void SetEffect(CGrModelX::IEffect *effect) {}
    virtual void EndEffect(CGrModelX::IEffect *effect) {}
    virtual void BeginTriangles() {}
    virtual void EndTriangles() {}
    virtual void TexCoord2fv(float *t) {}
    virtual void Normal3fv(float *n) {}
    virtual void Vertex3fv(float *v) {}
};