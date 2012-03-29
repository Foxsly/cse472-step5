//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

#pragma once
//! \cond ignore
#include <string>
#include <vector>
#include <map>

#include "grafx.h"

#include "xml-noexport/XmlDocument.h"

class CGrModelXp
{
public:
    CGrModelXp(void);
    virtual ~CGrModelXp(void);

    void Clear();
    bool LoadFile(const wchar_t *filename);

    void SetTransform(const CGrTransform &t) {mTransform = t;}
    void Draw();
    void Draw(CGrModelX::IRenderer *renderer);

    bool IntersectionTest(const CGrSphere &sphere);

    const wchar_t *GetError() const {return mErrorMessage.c_str();}

    void ComputeBonesAbsolute();
    CGrModelX::IBone *GetBone(const wchar_t *name);

protected:

private:
    bool Error(const wchar_t *msg);
    bool Error(const wchar_t *msg1, const wchar_t *msg2);

    // Transform that places the object
    CGrTransform mTransform;

    // Bones representation
    class Bone : public CGrModelX::IBone
    {
    public:
        Bone() {mLocalTransform.SetIdentity();}

        virtual const wchar_t *GetName() const {return mName.c_str();}
        virtual const CGrTransform &GetTransform() const {return mTransform;}
        virtual const CGrTransform &GetAbsoluteTransform() const {return mAbsoluteTransform;}

        virtual const CGrTransform &GetLocalTransform() const {return mLocalTransform;}
        virtual void SetLocalTransform(const CGrTransform &t) {mLocalTransform = t;}

        void SetName(const wchar_t *name) {mName = name;}

        int mIndex;
        int mParent;
        CGrTransform mTransform;
        CGrTransform mAbsoluteTransform;

        void ComputeAbsoluteTransform(const CGrTransform &parent);

    private:
        CGrTransform mLocalTransform;           // Local transformation
        std::wstring mName;
    };

    std::vector<Bone> mBones;
    int mRootBone;

    std::map<std::wstring, int> mBonesByName;

    //
    // Effects
    //
    class Effect : public CGrModelX::IEffect
    {
    public:
        virtual const wchar_t *GetName() const {return mName.c_str();}
        virtual float GetAlpha() const {return mAlpha;}
        virtual const float *GetDiffuse() const {return mDiffuse;}
        virtual const float *GetEmissive() const {return mEmissive;}
        virtual const float *GetSpecular() const {return mSpecular;}
        virtual float GetShininess() const {return mShininess;}
        virtual CGrTexture *GetTexture() {return mTexture;}
        virtual const float *GetSpecularOther() const {return mSpecularOther;}
        virtual const float *GetTransmission() const {return mTransmission;}
        virtual float GetEta() {return mEta;}

        std::wstring mName;

        float mAlpha;
        float mDiffuse[4];
        float mEmissive[4];
        float mSpecular[4];
        float mShininess;
        float mSpecularOther[4];
        float mTransmission[4];
        float mEta;

        std::wstring mTextureFile;
        CGrTexture *mTexture;

        void Zero(float *a) {for(int i=0;  i<4; i++) a[i] = 0;}
    };

    // The actual effects loaded from the file
    std::vector<Effect> mEffects;

    // 
    // Vertices
    //

    // Vertex buffer representation
    struct VertexBuffer
    {
        std::vector<float> mVertices;
        std::vector<float> mNormals;
        std::vector<float> mTcoords;
    };

    // Vertex buffers associated with the mesh
    std::vector<VertexBuffer> mVertices;

    //
    // Index buffers
    //

    // Index buffer representation
    struct IndexBuffer
    {
        std::vector<int> mIndices;
    };

    // Index buffers associated with the mesh
    std::vector<IndexBuffer> mIndices;

    //
    // Meshes
    //

    // Mesh part representation
    struct MeshPart
    {
        int mBaseVertex;
        int mNumVertices;
        int mNumTriangles;
        int mStartIndex;
        int mEffect;
        int mVertices;
        int mIndices;
    };

    // Mesh representation
    struct Mesh
    {
        std::wstring mName;
        int mBone;

        std::vector<MeshPart> mParts;

        CGrSphere mBoundingSphere;
    };

    std::vector<Mesh *> mMeshes;

    // Texture management
    std::map<std::wstring, CGrTexture> mTextures;

    // Any current error message
    std::wstring mErrorMessage;

    void XmlGetColor(IXMLDOMNamedNodeMap *attributes, const wchar_t *name, float *color);

    void XmlLoad(CXmlDocument *xml, IXMLDOMNode *topNode);
    bool XmlTestVersion(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadBones(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadBone(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadMeshes(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadMesh(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadVertexBuffers(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadIndexBuffers(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadVertices(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadIndices(CXmlDocument *xml, IXMLDOMNode *topNode);
    void XmlLoadMeshParts(CXmlDocument *xml, IXMLDOMNode *topNode, Mesh *mesh);
    void XmlLoadEffects(CXmlDocument *xml, IXMLDOMNode *topNode);
};

//! \endcond
