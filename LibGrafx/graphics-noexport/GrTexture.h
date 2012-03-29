//
// Name :         GrTexture.h
// Description :  Header for CGrTexture, texture image class for OpenGL.
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//
#pragma once

#if !defined(_GRTEXTURE_H)
#define _GRTEXTURE_H

#if !defined(LibGrafx)
#define LibGrafx
#endif

#include <atlimage.h>
#include <fstream>
#include <GL/gl.h>

//! Class to load and utilize texture files in OpenGL programs.

/*! The CGrTexture class loads an image file into memory and allows it be
be easily used as a texture. Both regular texture mapping and Mipmapping
are supported by the model.
 
Each dimension of the texture image must be a power of two. This function
will load any image that can be loaded using the MFC function: CImage::Load().

\author Charles B. Owen
\version 1.01 10-23-1999 Declared version number
\version 1.02 02-23-2003 Fixed bug where one constructor did not inititalize m_mipinitialized.
\version 1.03 05-02-2011 Eliminated memory leak due to failing to release textures
\version 1.04 01-14-2012 Now uses CImage from MFC to load images. Will load anything it will load.
\version 1.05 01-28-2012 Documentation updates
\version 1.06 01-30-2012 Name change from CTexture to CGrTexture for consistency
*/

class LibGrafx CGrTexture  
{
public:
    //! Default Constructor
    CGrTexture();
    //! Copy Constructor. 
    /*! The copy constructor will copy another texture image. 
        \param img The other texture image */
    CGrTexture(const CGrTexture &img);
    virtual ~CGrTexture();

    //! Load a texture from a file
    /* \param filename The filename as a character string in the current 
       setting of Unicode or multibyte strings. */
    bool LoadFile(LPCTSTR filename);

    //! Load a texture from an MFC image
    /* \param image The image we are loading from */
    bool LoadFrom(const ATL::CImage *image);

    //! Returns an OpenGL texture name for this texture.
    /*! An OpenGL texture name is an integer. The first time this
        function is called the texture is created. Subsequent calls
        return the previously created texture name. */
    GLuint TexName();

    //! Returns an OpenGL texture name for this texture assuming mipmapping.
    /*! An OpenGL texture name is an integer. The first time this
        function is called the texture is created. Subsequent calls
        return the previously created texture name. The texture will
        be created with support for mipmapping. */
    GLuint MipTexName();

    //! Clear the texture image 
    /*! Clears the texture image and releases any memory. This function must be 
        called when the OpenGL context is active or it will fail to release the
        texture objects OpenGL is using */
    void Clear();

    //! Fill the texture image with a color
    /*! Fills the texture image with a single R, G, B color. The 
        values for R, G, and B are in the range 0 to 255.
        \param r Red component.
        \param g Green component.
        \param b Blue component. */
    void Fill(int r, int g, int b);

    //! Set the texture image size
    /*! Sets the texture image size. THis will clear the texture image if it
        has already been loaded. 
        \param wid New texture image width 
        \param hit New texture image height */
    void SetSize(int wid, int hit);


    //! Set a texture pixel value
    /*! Sets the value of a texture pixel. 
        \param x X location (column)
        \param y Y location (row)
        \param r Red component.
        \param g Green component.
        \param b Blue component. */
    void Set(int x, int y, int r, int g, int b);

    //! Set this texture image to be the same size as another
    /*! Sets the texture image size identical to that of another
        texture image. 
        \param img The other texture image */
    void SameSize(const CGrTexture &img);

    //! Copies a texture image into this one.
    void Copy(const CGrTexture &p_img);

    //! Returns true if the texture image is empty.
    bool IsEmpty() const;

    //! Assignment operator that copies a texture image into this one.
    CGrTexture &operator=(const CGrTexture &p_img);

    //! Bracket operator gets access to a given row of the image
    /*! This operator can be used like this:  \code
    BYTE value = texture[row][byte];
\endcode
        Use with care. The return value is that same as that for
        ImageBits. It can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R. Rows start on the texture
        word (32 bit) boundary. */
    BYTE *operator[](int i);

    //! Bracket operator gets access to a given row of the image
        /*! This operator can be used like this:  \code
    BYTE value = texture[row][byte];
\endcode
        Use with care. The return value is that same as that for
        Row. It can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R for the given row. */
    const BYTE *operator[](int i) const;

    //! Access to pixels in a given row of the image
        /*! This operator can be used like this:  \code
    BYTE *row = texture.Row(row);
    BYTE red = row[column * 3 + 2];
\endcode
        Use with care. It can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R for the given row. */
    BYTE *Row(int i);

    //! Access to pixels in a given row of the image
    /*! This operator can be used like this:  \code
    const BYTE *row = texture.Row(row);
    BYTE red = row[column * 3 + 2];
\endcode
        Use with care. It can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R for the given row. */
    const BYTE *Row(int i) const;

    //! Gets the texture image width
    int Width() const;

    //! Gets the texture image height
    int Height() const;

    //! Direct access to the texture image bits. 
    /*! This function can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R. Rows start on the texture
        word (32 bit) boundary. */
    BYTE *ImageBits() const;

private:
    bool LoadFrom(const ATL::CImage *image, LPCTSTR filename);

    // Set true if the texture map has been initialized as a MIPMAP texture
    bool m_mipinitialized;

    // Set true if the texture map has been initialized as a normal texture
    bool m_initialized;

    // OpenGL name for the normal texture
    GLuint m_texname;

    // OpenGL name for the MIPMAP texture
    GLuint m_miptexname;

    // Texture image height
    int m_height;

    // Texture image width
    int m_width;

    // The actual texture image data
    BYTE ** m_image;
};

#endif 
