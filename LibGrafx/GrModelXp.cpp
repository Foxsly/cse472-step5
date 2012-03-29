#include "StdAfx.h"
#include "GrModelXp.h"
#include <wchar.h>
#include "xml-noexport/xmlhelp.h"

using namespace std;

inline bool GetXmlAttribute(IXMLDOMNamedNodeMap *attributes, const wchar_t *name, CGrTransform &transform)
{
    IXMLDOMNode *node;
    CComBSTR nameB = name;
    attributes->getNamedItem(nameB, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    std::wstringstream str(valueV.bstrVal);
    for(int r=0;  r<4; r++)
    {
        for (int c=0;  c<4;  c++)
        {
            str >> transform.M(r, c);
        }
    }

    return true;
}

inline bool GetXmlAttribute(IXMLDOMNamedNodeMap *attributes, const wchar_t *name, CGrVector &v)
{
    IXMLDOMNode *node;
    CComBSTR nameB = name;
    attributes->getNamedItem(nameB, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    std::wstringstream str(valueV.bstrVal);
    double x, y, z, w;
    str >> x >> y >> z >> w;
    v.Set(x, y, z, w);

    return true;
}

static void ToXYZ(wstring &v, float &x, float &y, float &z)
{
    // Find the two spaces
    const wchar_t *vs = v.c_str();
    wchar_t *vs2;
    x = (float)wcstod(vs, &vs2);
    y = (float)wcstod(vs2, &vs2);
    z = (float)wcstod(vs2, NULL);
}

CGrModelXp::CGrModelXp(void)
{
    mRootBone = -1;     // Not known
    mTransform.SetIdentity();
}

CGrModelXp::~CGrModelXp(void)
{
    Clear();
}


void CGrModelXp::Draw()
{
    // Compute the bones
    ComputeBonesAbsolute();

    // Settings for these models
    glFrontFace(GL_CW);
    glEnable(GL_NORMALIZE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Loop over the meshes
    for(vector<Mesh *>::iterator m=mMeshes.begin();  m!=mMeshes.end();  m++)
    {
        Mesh *mesh = *m;

        glPushMatrix();
        glTransform(mBones[mesh->mBone].mAbsoluteTransform);

        // Loop over the mesh parts
        for(std::vector<MeshPart>::iterator p=mesh->mParts.begin();  p!=mesh->mParts.end();  p++)
        {
            MeshPart *part = &(*p);

            // Get pointers to the mesh components
            Effect *effect = &mEffects[part->mEffect];
            VertexBuffer *vbuffer = &mVertices[part->mVertices];
            IndexBuffer *ibuffer = &mIndices[part->mIndices];

            // Set the material for the effect

            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, effect->mDiffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, effect->mSpecular);
            glMaterialfv(GL_FRONT, GL_EMISSION, effect->mEmissive);
            glMaterialfv(GL_FRONT, GL_SHININESS, &effect->mShininess);

            // Set of the vertex buffer
            glVertexPointer(3,      // Number of coordinates per vertex
                            GL_FLOAT,   // Type
                            0,          // Stride (assume packed)
                            &vbuffer->mVertices[part->mBaseVertex * 3]);

            glNormalPointer(GL_FLOAT,   // Type
                            0,          // Stride (assume packed)
                            &vbuffer->mNormals[part->mBaseVertex * 3]);

            bool hasTexture = !vbuffer->mTcoords.empty() && effect->mTexture != NULL;
            if(hasTexture)
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glBindTexture(GL_TEXTURE_2D, effect->mTexture->MipTexName());

                glTexCoordPointer(2, GL_FLOAT,   // Type
                                0,          // Stride (assume packed)
                                &vbuffer->mTcoords[part->mBaseVertex * 2]);
            }

            glDrawElements(GL_TRIANGLES, part->mNumTriangles * 3,
                GL_UNSIGNED_INT, &ibuffer->mIndices[part->mStartIndex]);

            if(hasTexture)
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisable(GL_TEXTURE_2D);
            }

        }

        glPopMatrix();
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glFrontFace(GL_CCW);
}



void CGrModelXp::Draw(CGrModelX::IRenderer *renderer)
{
    // Compute the bones
    ComputeBonesAbsolute();

    renderer->PushMatrix();

    // Loop over the meshes
    for(vector<Mesh *>::iterator m=mMeshes.begin();  m!=mMeshes.end();  m++)
    {
        Mesh *mesh = *m;

        renderer->NewMesh(mesh->mName.c_str());

        renderer->PushMatrix();
        renderer->MultMatrix(mBones[mesh->mBone].mAbsoluteTransform);

        // Loop over the mesh parts
        for(std::vector<MeshPart>::iterator p=mesh->mParts.begin();  p!=mesh->mParts.end();  p++)
        {
            MeshPart *part = &(*p);

            Effect *effect = &mEffects[part->mEffect];
            VertexBuffer *vbuffer = &mVertices[part->mVertices];
            IndexBuffer *ibuffer = &mIndices[part->mIndices];

            // Set the material for the effect
            renderer->SetEffect(effect);

            bool hasTexture = !vbuffer->mTcoords.empty() && effect->mTexture != NULL;
            float *tcoords = NULL;
            if(hasTexture)
            {
                tcoords = &vbuffer->mTcoords[part->mBaseVertex * 2];
            }

            float *vertices = &vbuffer->mVertices[part->mBaseVertex * 3];
            float *normals = &vbuffer->mNormals[part->mBaseVertex * 3];
            int *indices = &ibuffer->mIndices[part->mStartIndex];

            renderer->BeginTriangles();
            for(int t=0;  t<part->mNumTriangles * 3;  t+=3)
            {
                int ndx;

                ndx = indices[t + 2];
                if(tcoords != NULL)
                    renderer->TexCoord2fv(&tcoords[ndx * 2]);
                renderer->Normal3fv(&normals[ndx * 3]);
                renderer->Vertex3fv(&vertices[ndx * 3]);

                ndx = indices[t + 1];
                if(tcoords != NULL)
                    renderer->TexCoord2fv(&tcoords[ndx * 2]);
                renderer->Normal3fv(&normals[ndx * 3]);
                renderer->Vertex3fv(&vertices[ndx * 3]);

                ndx = indices[t];
                if(tcoords != NULL)
                    renderer->TexCoord2fv(&tcoords[ndx * 2]);
                renderer->Normal3fv(&normals[ndx * 3]);
                renderer->Vertex3fv(&vertices[ndx * 3]);
            }
            renderer->EndTriangles();

            renderer->EndEffect(effect);
        }

        renderer->PopMatrix();
    }

    renderer->PopMatrix();
}




bool CGrModelXp::IntersectionTest(const CGrSphere &sphere) 
{
    // Compute the bones
    ComputeBonesAbsolute();

    double rsquared = sphere.GetRadius() * sphere.GetRadius();

    // Test every mesh for an intersection

    // Loop over the meshes
    for(vector<Mesh *>::iterator m=mMeshes.begin();  m!=mMeshes.end();  m++)
    {
        Mesh *mesh = *m;


        // Local to world transformation
        CGrTransform tran2 = mBones[mesh->mBone].mAbsoluteTransform;

        // Do we intersect the bounding sphere?
        if(sphere.IntersectionTest(tran2 * mesh->mBoundingSphere))
        {
            // If so, test the actual vertices of the mesh parts
            // Loop over the mesh parts
            for(std::vector<MeshPart>::iterator p=mesh->mParts.begin();  p!=mesh->mParts.end();  p++)
            {
                MeshPart *part = &(*p);
                VertexBuffer *vbuffer = &mVertices[part->mVertices];
                IndexBuffer *ibuffer = &mIndices[part->mIndices];

                for(int i=0;  i<part->mNumTriangles * 3;  i++)
                {
                    // Vertex index
                    int ndx = ibuffer->mIndices[part->mStartIndex + i];

                    // Vertex?
                    CGrVector v(vbuffer->mVertices[(part->mBaseVertex + ndx) * 3], 
                        vbuffer->mVertices[(part->mBaseVertex + ndx) * 3 + 1], 
                        vbuffer->mVertices[(part->mBaseVertex + ndx) * 3 + 2]);

                    v = tran2 * v;
                    if(sphere.IntersectionTest(v))
                        return true;
                }
            }
        }

    }

    return false;
}




void CGrModelXp::Clear()
{
    // Destroy any bones
    mRootBone = -1;
    mBones.clear();
    mEffects.clear();
    mVertices.clear();
    mIndices.clear();
    mTextures.clear();
    mBonesByName.clear();

    // Destroy any allocated meshes
    for(vector<Mesh *>::iterator m=mMeshes.begin();  m!=mMeshes.end();  m++)
    {
        delete *m;
    }

    mMeshes.clear();
}


//
// Name :         CGrModelXp::LoadFile()
// Description :  Load the model from an XML file.
// Parameters :   filename - Name of the file to load from
// Returns :      true if successful
//                If false, an error message is stored in mErrorMessage
//
bool CGrModelXp::LoadFile(const wchar_t *filename)
{
    // Make a path to where this file is located
    wstring path(filename);
    int i;
    for(i=path.length() - 1;  i>=0;  i--)
    {
        if(path[i] == L'\\' || path[i] == '/')
        {
            path = path.substr(0, i + 1);
            break;
        }
    }

    if(i < 0)
        path = L"";

    CXmlDocument xml;

    if(!xml.OpenXmlDocument(filename, L"model"))
        return Error(xml.GetErrorMessage());

    if(!XmlTestVersion(&xml, xml.GetRootNode()))
        return Error(L"XMODL file is not the correct version. Version 2.0 required");

    Clear();

    XmlLoad(&xml, xml.GetRootNode());

    // Once we have loaded all of the meshes, we find all of the 
    // necessary textures and load them as well.
    // Loop over the meshes
    for(vector<Mesh *>::iterator m=mMeshes.begin();  m!=mMeshes.end();  m++)
    {
        Mesh *mesh = *m;

        // Loop over the effects
        for(vector<Effect>::iterator e=mEffects.begin(); e!=mEffects.end(); e++)
        {
            Effect *effect = &(*e);

            // Does this effect use a texture?
            if(!effect->mTextureFile.empty())
            {
                // Do we know it already?
                std::map<wstring, CGrTexture>::iterator t=mTextures.find(effect->mTextureFile);
                if(t == mTextures.end())
                {
                    // Load the texture
                    mTextures[effect->mTextureFile] = CGrTexture();
                    effect->mTexture = &mTextures[effect->mTextureFile];
                    wstring filepath = path + effect->mTextureFile;
                    effect->mTexture->LoadFile(filepath.c_str());
                }
                else
                {
                    // We have already loaded it
                    effect->mTexture = &t->second;
                }

            }
        }
    }

    return true;
}

//
// Name :         CGrModelXp::XmlTestVersion()
// Description :  Test to ensure this file version can be read by this reader version
//
bool CGrModelXp::XmlTestVersion(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    topNode->get_attributes(&attributes);
    wstring name;
    if(!GetXmlAttribute(attributes, L"version", name))
        return false;

    // Versions we can read:
    if(name == L"2.0")
        return true;

    return false;
}



//
// Name :         CGrModelXp::ComputeBonesAbsolute()
// Description :  The bones as loaded by the model are relative to the
//                parent bone. Create a list of bones that is relative to
//                the world, an absolute set of bone transforms.
//
void CGrModelXp::ComputeBonesAbsolute()
{
    // This is simplified because Microsoft sorted the bones into
    // topological order.
    for(unsigned int i=0;  i<mBones.size();  i++)
    {
        if(mBones[i].mParent < 0)
        {
            // This bone has no parent
            mBones[i].ComputeAbsoluteTransform(mTransform);
        }
        else
        {
            // This bone is the product of the bone
            // transform and it's parent transform
            mBones[i].ComputeAbsoluteTransform(mBones[mBones[i].mParent].mAbsoluteTransform);
        }
    }
}



CGrModelX::IBone *CGrModelXp::GetBone(const wchar_t *name)
{
    map<wstring, int>::iterator bone = mBonesByName.find(name);
    if(bone == mBonesByName.end())
        return NULL;

    return &mBones[bone->second];
}


void CGrModelXp::XmlLoad(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "bones")
                XmlLoadBones(xml, node);
            else if(nodeName == "effects")
                XmlLoadEffects(xml, node);
            else if(nodeName == "vertexbuffers")
                XmlLoadVertexBuffers(xml, node);
            else if(nodeName == "indexbuffers")
                XmlLoadIndexBuffers(xml, node);
            else if(nodeName == "meshes")
                XmlLoadMeshes(xml, node);
        }
    }
}



void CGrModelXp::XmlLoadBones(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    topNode->get_attributes(&attributes);
    GetXmlAttribute(attributes, L"root-bone", mRootBone);

    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "bone")
                XmlLoadBone(xml, node);
        }
    }
}

void CGrModelXp::XmlLoadBone(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    Bone bone;

    // Attributes to load:  index, name, transform, parent
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    topNode->get_attributes(&attributes);
    GetXmlAttribute(attributes, L"index", bone.mIndex);
    wstring name;
    GetXmlAttribute(attributes, L"name", name);
    bone.SetName(name.c_str());

    bone.mParent = -1;
    GetXmlAttribute(attributes, L"parent", bone.mParent);
    GetXmlAttribute(attributes, L"transform", bone.mTransform);

    mBonesByName[bone.GetName()] = (int)mBones.size();
    mBones.push_back(bone);
}


void CGrModelXp::XmlLoadMeshes(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "mesh")
                XmlLoadMesh(xml, node);
        }
    }
}

void CGrModelXp::XmlLoadMesh(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    // Create the mesh object
    Mesh *mesh = new Mesh();
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    topNode->get_attributes(&attributes);
    GetXmlAttribute(attributes, L"name", mesh->mName);
    GetXmlAttribute(attributes, L"bone", mesh->mBone);

    wstring bs;
    if(GetXmlAttribute(attributes, L"bounding-sphere", bs))
    {
        wstringstream str(bs);
        double x, y, z, r;
        str >> x >> y >> z >> r;
        mesh->mBoundingSphere.SetOrigin(CGrVector(x, y, z));
        mesh->mBoundingSphere.SetRadius(r);
    }


    // Now the parts of the mesh
    // vertices, indices, parts
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "parts")
                XmlLoadMeshParts(xml, node, mesh);
        }
    }

    // Add to list of meshes
    mMeshes.push_back(mesh);
}


void CGrModelXp::XmlLoadVertexBuffers(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    // Loop over the children of the vertexbuffer tag. Each will
    // be a vertex buffer we are adding to this mesh
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "vertices")
                XmlLoadVertices(xml, node);
        }
    }
}


void CGrModelXp::XmlLoadIndexBuffers(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    // Loop over the children of the indexbuffer tag. Each will
    // be an index buffer we are adding to this mesh
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {         
            if(nodeName == "indices")
                XmlLoadIndices(xml, node);
        }
    }
}


void CGrModelXp::XmlLoadVertices(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    // Create a vertex buffer for the vertices
    VertexBuffer vbuffer;

    // Now the individual vertices
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "v")
            {
                float x, y, z;

                CComPtr<IXMLDOMNamedNodeMap> attributes;
                node->get_attributes(&attributes);
                wstring v;

                GetXmlAttribute(attributes, L"v", v);
                ToXYZ(v, x, y, z);
                vbuffer.mVertices.push_back(x);
                vbuffer.mVertices.push_back(y);
                vbuffer.mVertices.push_back(z);

                GetXmlAttribute(attributes, L"n", v);
                ToXYZ(v, x, y, z);
                vbuffer.mNormals.push_back(x);
                vbuffer.mNormals.push_back(y);
                vbuffer.mNormals.push_back(z);

                v.clear();
                GetXmlAttribute(attributes, L"t", v);
                if(!v.empty())
                {
                    const wchar_t *vs = v.c_str();
                    wchar_t *vs2;
                    x = (float)wcstod(vs, &vs2);
                    y = (float)wcstod(vs2, &vs2);
                    vbuffer.mTcoords.push_back(x);
                    vbuffer.mTcoords.push_back(1 - y);    // y is reversed from Microsoft standard
                }
            }
        }
    }

    mVertices.push_back(vbuffer);
}


void CGrModelXp::XmlLoadIndices(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    IndexBuffer ibuffer;

    CComPtr<IXMLDOMNamedNodeMap> attributes;
    topNode->get_attributes(&attributes);
    wstring istr;
    GetXmlAttribute(attributes, L"i", istr);
    const wchar_t *s1 = istr.c_str();
    wchar_t *s2;
    while(true)
    {
        int i = wcstol(s1, &s2, 10);
        if(s2 == s1)
            break;

        s1 = s2;
        ibuffer.mIndices.push_back(i);
    }

    mIndices.push_back(ibuffer);
}


void CGrModelXp::XmlLoadMeshParts(CXmlDocument *xml, IXMLDOMNode *topNode, Mesh *mesh)
{
    // Now the parts
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "part")
            {
                MeshPart part;

                CComPtr<IXMLDOMNamedNodeMap> attributes;
                node->get_attributes(&attributes);
                wstring v;

                GetXmlAttribute(attributes, L"base-vertex", part.mBaseVertex);
                GetXmlAttribute(attributes, L"num-vertices", part.mNumVertices);
                GetXmlAttribute(attributes, L"num-triangles", part.mNumTriangles);
                GetXmlAttribute(attributes, L"start-index", part.mStartIndex);
                GetXmlAttribute(attributes, L"effect", part.mEffect);
                GetXmlAttribute(attributes, L"vertices", part.mVertices);
                GetXmlAttribute(attributes, L"indices", part.mIndices);

                mesh->mParts.push_back(part);
            }
        }
    }
}


void CGrModelXp::XmlLoadEffects(CXmlDocument *xml, IXMLDOMNode *topNode)
{
    // Now the parts
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "effect")
            {
                Effect effect;
                effect.mTexture = NULL;

                CComPtr<IXMLDOMNamedNodeMap> attributes;
                node->get_attributes(&attributes);
                wstring v;

                double a = 1;
                GetXmlAttribute(attributes, L"name", effect.mName);
                GetXmlAttribute(attributes, L"alpha", a);
                effect.mAlpha = float(a);
                XmlGetColor(attributes, L"diffuse", effect.mDiffuse);
                XmlGetColor(attributes, L"emissive", effect.mEmissive);
                XmlGetColor(attributes, L"specular", effect.mSpecular);
                XmlGetColor(attributes, L"specularOther", effect.mSpecularOther);
                XmlGetColor(attributes, L"transmission", effect.mTransmission);

                GetXmlAttribute(attributes, L"shininess", a);
                effect.mShininess = float(a);

                a = 1;
                GetXmlAttribute(attributes, L"eta", a);
                effect.mEta = float(a);
                GetXmlAttribute(attributes, L"texture", effect.mTextureFile);
                

                mEffects.push_back(effect);
            }
        }
    }
}



void CGrModelXp::XmlGetColor(IXMLDOMNamedNodeMap *attributes, const wchar_t *name, float *color)
{
    wstring c;
    if(!GetXmlAttribute(attributes, BSTR(name), c))
    {
        color[0] = color[1] = color[2] = color[3] = 0;
        return;
    }

    const wchar_t *vs = c.c_str();
    wchar_t *vs2;
    color[0] = (float)wcstod(vs, &vs2);
    color[1] = (float)wcstod(vs2, &vs2);
    color[2] = (float)wcstod(vs2, NULL);
    color[3] = 1;
}



//
// Name :         CGrModelXp::Error()
// Description :  Error indication function. Returns false and sets an error message.
//
bool CGrModelXp::Error(const wchar_t *msg)
{
    mErrorMessage = msg;
    return false;
}

bool CGrModelXp::Error(const wchar_t *msg1, const wchar_t *msg2)
{
    mErrorMessage = msg1;
    mErrorMessage += msg2;
    return false;
}


void CGrModelXp::Bone::ComputeAbsoluteTransform(const CGrTransform &parent)
{
    mAbsoluteTransform = parent * mTransform * mLocalTransform;
}
