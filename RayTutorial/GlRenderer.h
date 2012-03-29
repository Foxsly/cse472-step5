#pragma once
#include <grafx.h>
#include "gl/gl.h"

class CGlRenderer : public CGrModelX::IRenderer
{
public:
	CGlRenderer(void);
	~CGlRenderer(void);

    virtual void PushMatrix() {glPushMatrix();}
    virtual void PopMatrix() {glPopMatrix();}
	virtual void MultMatrix(const CGrTransform &t) {t.glMultMatrix();}
    virtual void SetEffect(CGrModelX::IEffect *effect);
    virtual void EndEffect(CGrModelX::IEffect *effect);
    virtual void BeginTriangles() {glBegin(GL_TRIANGLES);}
    virtual void EndTriangles() {glEnd();}
    virtual void TexCoord2fv(float *t) {glTexCoord2fv(t);}
    virtual void Normal3fv(float *n) {glNormal3fv(n);}
    virtual void Vertex3fv(float *v) {glVertex3fv(v);}
};