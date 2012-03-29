//
// Name :          OpenGLWnd.h
// Description :   Header file for COpenGLWnd OpenGL superclass.
//

#pragma once

#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//! Superclass for MFC windows that use OpenGL

//! This is a class derived from CWnd that will automatically set up
//! for OpenGL.  You can use this whereever you would use
//! CWnd and you'll automatically have OpenGL in this window. 
//! 
//! \author Charles B. Owen
//! \version 01-09-01 1.00 CBO Initial version number.
//! \version 03-03-03 1.01 CBO Improved error handing.
//! \version 03-16-03 1.02 CBO Fixed problem with saving image
//! \version 01-10-11 1.03 CBO Some cleanup prior to the semester
//! \version 01-22-12 1.04 CBO Added support for stereo and stencil buffers

class COpenGLWnd : public CWnd
{
public:
    //! Constructor
    COpenGLWnd();

    //! Destructor
    virtual ~COpenGLWnd();

    //! Function to draw in OpenGL that a derived class will override.
    /*! This function is called whenever the window should be called. The 
        viewport is already set up when this function is called. There is a
        default implementation that just clears the screen to Cyan. Applications
        using this class should override this function as a location for OpenGL
        drawing code.
        \param pDC Pointer to a device context active while drawing */
    virtual void OnGLDraw(CDC *pDC);

    //! Support for a menu option to save the current image as a file.
    /*! Call from a menu option. This function brings up the File Save
        dialog box on its own. */
    void OnSaveImage();

    //! Get the size of the window in pixels.
    /*! \param width Where to put the width.
        \param height Where to put the height */
    void GetSize(int &width, int &height);

    //! Get the height of the window in pixels.
    int GetHeight();

    //! Get the width fo the window in pixels.
    int GetWidth();

    //! Get the pixels for the current image.
    /*! Data is in a 2D array in BGR interleaved format. 
        When done with the array, destroy it using: 
        \code 
            delete buffer[0]; 
            delete buffer;
        \endcode
        \param pixels Pointer to location where a pointer will be 
        set that points to the pixel data.  */
    bool ObtainPixels(GLbyte **&pixels);

    //! Obtain a handle to the OpenGL context.
    HGLRC HGLRc() {return m_hrc;}

    //! Set the handle to the OpenGL context. 
    /*! This function is for very specialized uses only 
        \param hrc The new OpenGL context to use.
    */
    void SetHGLRc(HGLRC hrc) {m_hrc = hrc;}

    //! Set the double buffer status for the window. Default is false.
    /*! This function must be called before the window is created.
        The best place to call this function is in the constructor
        for the window. 
        \param doublebuffer Set to true/false to enable/disable double buffering
    */
    void SetDoubleBuffer(bool doublebuffer);

    //! Get the double buffering status.
    bool GetDoubleBuffer() const {return m_doublebuffer;}

    //! Set the number of stencil bits
    /*! This function sets the number of stencil bits for the 
        created window. Stencil bits are not available on all platforms.
        This function must be called before the window is created.
        The best place to call this function is in the constructor
        for the window. The default for this value is zero.
        \param b Number of bits. */
    void SetStencilBits(int b) {m_stencilBits = b;}

    //! Set the stereo flag
    /*! This function sets the stereo option for the
        created window. Stereo support is not available on all platforms.
        This function must be called before the window is created.
        The best place to call this function is in the constructor
        for the window. The default for this value is false.
        \param s Stereo bit */
    void SetStereo(bool s) {m_stereo = s;}

protected:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COpenGLWnd)

    //! Called prior to the window being created. 
    /*! Override this function to make changes to the create structure.
        \param cs Reference to the create structure for the window. */
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

    //! \cond SPECIAL
    //{{AFX_MSG(COpenGLWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    //! \endcond

private:
    int FormattedErrorAfxMsgBox(const TCHAR *p_msg);

    bool m_created;
    bool m_doublebuffer;
    BOOL CreateRGBPalette(HDC hDC);
    int  m_stencilBits;
    bool m_stereo;

    HGLRC m_hrc;
    CPalette* m_pPal ;		//Palette

   	//
    // Support for generating RGB color palette
    //
    unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift) ;

    static unsigned char   m_oneto8[2];
    static unsigned char   m_twoto8[4];
    static unsigned char   m_threeto8[8];
    static int             m_defaultOverride[13];
    static PALETTEENTRY    m_defaultPalEntry[20];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

