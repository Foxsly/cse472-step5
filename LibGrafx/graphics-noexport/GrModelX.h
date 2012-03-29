#pragma once
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

class CGrModelXp;
class CGrSphere;
class CGrTransform;
class CGrTexture;

#if !defined(LibGrafx)
#define LibGrafx
#endif

class LibGrafx CGrModelX
{
public:
    CGrModelX();
    virtual ~CGrModelX();

    void Clear();
    bool LoadFile(const wchar_t *filename);

    void SetTransform(const CGrTransform &t);

    bool IntersectionTest(const CGrSphere &sphere);

    const wchar_t *GetError() const;

    // Interface to the underlying effect
    class LibGrafx IEffect
    {
    public:
        IEffect();
        virtual ~IEffect();

        virtual const wchar_t *GetName() const = 0;
        virtual float GetAlpha() const = 0;
        virtual const float *GetDiffuse() const = 0;
        virtual const float *GetEmissive() const = 0;
        virtual const float *GetSpecular() const = 0;
        virtual float GetShininess() const = 0;
        virtual CGrTexture *GetTexture() = 0;
    };

    // Interface to a renderer
    class LibGrafx IRenderer
    {
    public:
        IRenderer();
        virtual ~IRenderer();

        virtual void PushMatrix() = 0;
        virtual void PopMatrix() = 0;
        virtual void MultMatrix(const CGrTransform &t) = 0;
        virtual void SetEffect(IEffect *effect) = 0;
        virtual void EndEffect(IEffect *effect) = 0;
        virtual void BeginTriangles() = 0;
        virtual void EndTriangles() = 0;
        virtual void TexCoord2fv(float *t) = 0;
        virtual void Normal3fv(float *n) = 0;
        virtual void Vertex3fv(float *v) = 0;
        virtual void NewMesh(const wchar_t *name);
    };

    class LibGrafx IBone
    {
    public:
        IBone();
        virtual ~IBone();


        //! Get the bone name
        /*! This function returns the bone name as a null
            terminated string
            \return Bone name */
        virtual const wchar_t *GetName() const = 0;

        //! Get the bone transform
        /*! This function return the transform for a bone. The absolute
            transform is the product of the local transform, the bone transform,
            and the parent absolute transform. This transform is set by the 
            model and places the bone local coordinate system relative to
            the parent bone coordinate system.
            \return Bone transform */
        virtual const CGrTransform &GetTransform() const = 0;

        //! Get the bone absolute transform
        /*! This gets an absolute transform for the bone. This is the 
            transform from the bone local coordinate system including any 
            local transformation to the model coordinate system including 
            any transform associated with the model by CGrModelX::SetTransform().

            This value is only valid after a call to CGrModelX::ComputeBonesAbsolute()!
            \return Bone absolute transform */
        virtual const CGrTransform &GetAbsoluteTransform() const = 0;

        //! Get the bone local transform
        /*! The bone local transform is applied after the bone transform and the
            parent transform and is used to animate the bone.

            The default for the local transform is an identity matrix.
            \return Bone local transform */
        virtual const CGrTransform &GetLocalTransform() const = 0;

        //! Set the bone local transform
        /*! The bone local transform is applied after the bone transform and the
            parent transform and is used to animate the bone.
            \param t New bone local transform */
        virtual void SetLocalTransform(const CGrTransform &t) = 0;
    };

    void Draw();
    void Draw(IRenderer *renderer);

    void ComputeBonesAbsolute();
    IBone *GetBone(const wchar_t *name);

    //! Get the position of a camera if specified in the ModelX file.
    /*! This function returns the position of a camera if one has been 
        specified in the ModelX file. If no camera is specified, this 
        function returns a zero vector (w value of zero). Otherwise, the return value is 
        the camera position in a vector.
        \par A camera position is specified in the original model as a node with 
        the name Camera.Position. 
        \return Position of the camera or a vector with a w value of zero if no camera position
        has been specified. */
    CGrVector GetCameraPosition(void);

    //! Get the target for a camera if specified in the ModelX file.
    /*! This function returns the target fir a camera if one has been 
        specified in the ModelX file. If no camera is specified, this 
        function returns a zero vector (w value of zero). Otherwise, the return value is 
        the camera target in a vector.
        \par A camera target is specified in the original model as a node with 
        the name Camera.Target.
        \return Target the camera or a vector with a w value of zero if no camera target
        has been specified. */
    CGrVector GetCameraTarget(void);

private:
    CGrModelXp *mModel;
};
