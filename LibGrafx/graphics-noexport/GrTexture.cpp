//
//  Name :         GrTexture.cpp
//  Description :  Implementation of the CGrTexture class.
//                 This class allows for the simple manipulation of BITMAP images.
//  Version :      See GrTexture.h
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

#include <stdafx.h>
#include "GrTexture.h"
#include <cassert>

using namespace std;

#include <GL/glu.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrTexture::CGrTexture()
{
   m_height = 0;
   m_width = 0;
   m_image = NULL;
   m_texname = 0;

   m_initialized = false;
   m_mipinitialized = false;
}

CGrTexture::CGrTexture(const CGrTexture &p_img)
{
   m_height = 0;
   m_width = 0;
   m_image = NULL;
   m_initialized = false;
   m_mipinitialized = false;

   Copy(p_img);
}

CGrTexture::~CGrTexture()
{
    Clear();
}


void CGrTexture::Clear()
{
    if(m_image != NULL)
    {
        delete [] m_image[0];
        delete [] m_image;
        m_image = NULL;
    }

    if(m_initialized)
    {
        glDeleteTextures(1, &m_texname);
        m_initialized = false;
    }

    if(m_mipinitialized)
    {
        glDeleteTextures(1, &m_miptexname);
        m_mipinitialized = false;
    }
}


bool CGrTexture::IsEmpty() const {return m_width <= 0 || m_height <= 0;}

BYTE *CGrTexture::operator[](int i) {return m_image[i];}
const BYTE *CGrTexture::operator[](int i) const {return m_image[i];}
BYTE *CGrTexture::Row(int i) {return m_image[i];}
const BYTE *CGrTexture::Row(int i) const {return m_image[i];}

int CGrTexture::Width() const {return m_width;}
int CGrTexture::Height() const {return m_height;}
BYTE *CGrTexture::ImageBits() const {return m_image[0];}


//////////////////////////////////////////////////////////////////////
// Basic Manipulations
//////////////////////////////////////////////////////////////////////

//
// Name :         CGrTexture::Copy()
// Description :  Copy another image into this one.
//

void CGrTexture::Copy(const CGrTexture &p_img)
{
    SameSize(p_img);

    for(int i=0;  i<m_height;  i++)
    {
        for(int j=0;  j<m_width * 3;  j++)
        {
            m_image[i][j] = p_img.m_image[i][j];
        }
    }
}


CGrTexture & CGrTexture::operator =(const CGrTexture &p_img)
{
   Copy(p_img);
   return *this;
}

//
// Name :         CGrTexture::TexName()
// Description :  Obtain the texture name.  If the texture name has
//                not yet been assigned, do so, now.
//


GLuint CGrTexture::TexName()
{
    if(m_initialized)
        return m_texname;

    if(m_image == NULL)
        return 0;

    glGenTextures(1, &m_texname);
    glBindTexture(GL_TEXTURE_2D, m_texname);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, m_image[0]);

    m_initialized = true;

    return m_texname;
}


GLuint CGrTexture::MipTexName()
{
    if(m_mipinitialized)
        return m_miptexname;

    if(m_image == NULL)
        return 0;

    glGenTextures(1, &m_miptexname);
    glBindTexture(GL_TEXTURE_2D, m_miptexname);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_width, m_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_image[0]);

    m_mipinitialized = true;

    return m_miptexname;

}

void CGrTexture::SameSize(const CGrTexture &p_img)
{
	SetSize(p_img.m_width, p_img.m_height);
}

//
// Name :         CGrTexture::SetSize()
// Description :  Sets the size of the image and allocates memory.
//                An image size of 0 is an empty empty.
//

void CGrTexture::SetSize(int p_x, int p_y)
{
   if(p_x == m_width && m_height == p_y)
      return;

   Clear();

	// Member variables
   m_height = p_y;
   m_width = p_x;

   if(p_x <= 0 || p_y <= 0)
      return;

   // Allocate memory for the image.  Note that storage for rows must
   // be on DWORD boundaries.  (32 bit boundaries)
   const int PADSIZE = 4;

   int usewidth = (m_width * 3 + (PADSIZE - 1)) / PADSIZE;
   usewidth *= PADSIZE;

   BYTE *image = new BYTE[usewidth * m_height];
   m_image = new BYTE *[m_height];
   for(int i=0;  i<m_height;  i++, image += usewidth)
   {
      m_image[i] = image;
   }
}

void CGrTexture::Set(int x, int y, int r, int g, int b)
{
   if(x >= 0 && x < m_width && y >= 0 && y < m_height)
   {
      BYTE *img = m_image[y] + x * 3;
      *img++ = b;
      *img++ = g;
      *img++ = r;
   }
}


void CGrTexture::Fill(int r, int g, int b)
{
   for(int i=0;  i<m_height;  i++)
   {
      BYTE *img = m_image[i];
      for(int j=0;  j<m_width * 3;  j+=3)
      {
         *img++ = b;
         *img++ = g;
         *img++ = r;
      }

   }

}

//////////////////////////////////////////////////////////////////////
// Generic file and memory reading operations
//////////////////////////////////////////////////////////////////////



//
//  Name :         CGrTexture::LoadFile()
//  Description :  Load this image from a file of type BMP or PPM
//  Returns :      true if successful

bool CGrTexture::LoadFile(LPCTSTR filename)
{
    Clear();

    ATL::CImage image;
    HRESULT hr = image.Load(filename);
    if(FAILED(hr))
    {
        LPTSTR errorText = NULL;

        FormatMessage(
           // use system message tables to retrieve error text
           FORMAT_MESSAGE_FROM_SYSTEM
           // allocate buffer on local heap for error text
           |FORMAT_MESSAGE_ALLOCATE_BUFFER
           // Important! will fail otherwise, since we're not 
           // (and CANNOT) pass insertion parameters
           |FORMAT_MESSAGE_IGNORE_INSERTS,  
           NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
           hr,
           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
           (LPTSTR)&errorText,  // output 
           0, // minimum size for output buffer
           NULL);   // arguments - see note 

        tstring msg = TEXT("Unable to read image file: ");
        msg += filename;
        msg += TEXT(" - ");
        msg += errorText;
        LocalFree(errorText);

        AfxMessageBox(msg.c_str());

        return false;
    }

    return LoadFrom(&image);
}

bool CGrTexture::LoadFrom(const ATL::CImage *image)
{
    return LoadFrom(image, NULL);
}


bool CGrTexture::LoadFrom(const ATL::CImage *image, LPCTSTR filename)
{
    SetSize(image->GetWidth(), image->GetHeight());

    int bpp = image->GetBPP();
    int pitch = image->GetPitch();
    BYTE *bits = (BYTE *)image->GetBits();
    bool idib = image->IsDIBSection();
    if(!image->IsDIBSection())
    {
        tstring msg = TEXT("Unable to read image");
        if(filename != NULL)
        {
            msg += TEXT(" file:");
            msg += filename;
        }
        else
        {
            msg += TEXT(": ");
        }

        msg += TEXT(" - File format could not be loaded");

        AfxMessageBox(msg.c_str());
    }

    switch(bpp)
    {
    case 8:
        {
            for(int r=0;  r<m_height;  r++)
            {
                BYTE *pixel = bits;

                for(int c=0;  c<m_width;  c++)
                {
                    int g = pixel[0];

                    Set(c, m_height - r - 1, g, g, g);

                    pixel++;
                }

                bits += pitch;
            }
        }
        break;

    case 24:
        {
            for(int r=0;  r<m_height;  r++)
            {
                BYTE *pixel = bits;

                for(int c=0;  c<m_width;  c++)
                {
                    int blu = pixel[0];
                    int grn = pixel[1];
                    int red = pixel[2];

                    Set(c, m_height - r - 1, red, grn, blu);

                    pixel += 3;
                }

                bits += pitch;
            }
        }
        break;

    case 32:
        {
            for(int r=0;  r<m_height;  r++)
            {
                BYTE *pixel = bits;

                for(int c=0;  c<m_width;  c++)
                {
                    int blu = pixel[0];
                    int grn = pixel[1];
                    int red = pixel[2];

                    Set(c, m_height - r - 1, red, grn, blu);

                    pixel += 4;
                }

                bits += pitch;
            }
        }
        break;

    default:
        {
            tstring msg = TEXT("Unable to read image");
            if(filename != NULL)
            {
                msg += TEXT(" file:");
                msg += filename;
            }
            else
            {
                msg += TEXT(": ");
            }

            msg += TEXT(" - File format could not be loaded");

            AfxMessageBox(msg.c_str());
        }
        break;
    }

    return true;
}


