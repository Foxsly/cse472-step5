//
//  Name :         GrImageI.cpp
//  Description :  Implementation of the CGrImageI class.
//                 This class allows for the simple manipulation of integer images.
//  Version :      See GrImageI.h
//

#include <stdafx.h>
#include "GrImageI.h"
#include <cassert>

using namespace std;

#include <GL/glu.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrImageI::CGrImageI()
{
    m_planes = 3;       // Default is RGB
    m_rowpitch = 0;
    m_height = 0;
    m_width = 0;
    m_image = NULL;
}

CGrImageI::CGrImageI(const CGrImageI &p_img)
{
    m_planes = 3;       // Default is RGB
    m_rowpitch = 0;
    m_height = 0;
    m_width = 0;
    m_image = NULL;

    Copy(p_img);
}

CGrImageI::~CGrImageI()
{
    Clear();
}


void CGrImageI::Clear()
{
    if(m_image != NULL)
    {
        delete [] m_image[0];
        delete [] m_image;
        m_image = NULL;
    }

}


bool CGrImageI::IsEmpty() const {return m_width <= 0 || m_height <= 0;}

int *CGrImageI::operator[](int i) {return m_image[i];}
const int *CGrImageI::operator[](int i) const {return m_image[i];}
int *CGrImageI::GetRow(int i) {return m_image[i];}
const int *CGrImageI::GetRow(int i) const {return m_image[i];}

int CGrImageI::GetWidth() const {return m_width;}
int CGrImageI::GetHeight() const {return m_height;}
int *CGrImageI::GetImageBits() const {return m_image[0];}



//////////////////////////////////////////////////////////////////////
// Basic Manipulations
//////////////////////////////////////////////////////////////////////

//
// Name :         CGrImageI::Copy()
// Description :  Copy another image into this one.
//

void CGrImageI::Copy(const CGrImageI &p_img)
{
    SetSameSize(p_img);

    for(int i=0;  i<m_height;  i++)
    {
        memcpy(m_image[i], p_img.m_image[i], m_width * m_planes * sizeof(int));
    }
}


CGrImageI & CGrImageI::operator =(const CGrImageI &p_img)
{
    Copy(p_img);
    return *this;
}



void CGrImageI::SetSameSize(const CGrImageI &p_img, int planes)
{
    if(planes < 0)
        SetSize(p_img.m_width, p_img.m_height, p_img.m_planes);
    else
        SetSize(p_img.m_width, p_img.m_height, planes);
}

//
// Name :         CGrImageI::SetSize()
// Description :  Sets the size of the image and allocates memory.
//                An image size of 0 is an empty empty.
//

void CGrImageI::SetSize(int p_x, int p_y, int planes)
{
    // If already the right size, don't do anything, we are happy
    if(p_x == m_width && m_height == p_y && m_planes == planes)
        return;

    Clear();

    // Member variables
    m_height = p_y;
    m_width = p_x;
    m_planes = planes;

    if(p_x <= 0 || p_y <= 0)
        return;

    // Allocate memory for the image.
    int usewidth = m_width * m_planes;
    m_rowpitch = usewidth;

    int *image = new int[usewidth * m_height];
    m_image = new int *[m_height];
    for(int i=0;  i<m_height;  i++, image += usewidth)
    {
        m_image[i] = image;
    }
}

void CGrImageI::Set(int x, int y, int r, int g, int b, int a)
{
    if(x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        int *img = m_image[y] + x * m_planes;
        if(m_planes == 1)
        {
            *img++ = b;
        }
        else if(m_planes == 3)
        {
            *img++ = b;
            *img++ = g;
            *img++ = r;
        }
        else if(m_planes == 4)
        {
            *img++ = b;
            *img++ = g;
            *img++ = r;
            *img++ = a;
        }

    }
}


void CGrImageI::Fill(int r, int g, int b, int a)
{
    switch(m_planes)
    {
    case 1:
        for(int i=0; i<m_height;  i++)
        {
            int *img = m_image[i];
            for(int j=0;  j<m_width;  j++)
            {
                *img++ = b;
            }
        }
        break;

    case 3:
        for(int i=0; i<m_height;  i++)
        {
            int *img = m_image[i];
            for(int j=0;  j<m_width * 3;  j+=3)
            {
                *img++ = b;
                *img++ = g;
                *img++ = r;
            }
        }
        break;

    case 4:
        for(int i=0; i<m_height;  i++)
        {
            int *img = m_image[i];
            for(int j=0;  j<m_width * 4;  j+=4)
            {
                *img++ = b;
                *img++ = g;
                *img++ = r;
                *img++ = a;
            }
        }
        break;
    }

}
