
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include "RayTutorial.h"
#include "ChildView.h"
#include <grafx.h>
#include "GlRenderer.h"
#include "MyRaytraceRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// Define our lights
const float LightAmbientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float Light0Pos[] = {200.f, 150.0f, 80.0f, 1.0f};
const float Light1Color[] = {0.6f, 0.6f, 0.6f, 1.0f};
const float Light1Pos[] = {-100.f, 50.0f, 100.0f, 1.0f};
const float Light0Color[] = {0.6f, 0.6f, 0.6f, 1.0f};


// CChildView

CChildView::CChildView()
{
    m_camera.Set(25, 50, 50, 0, 12, 0, 0, 1, 0);
    m_camera.SetFieldOfView(35);
    SetDoubleBuffer(true);

	m_raytrace = false;
	m_rayimage = NULL;

    //
    // Load our object
    //

    LoadModel(L"models/TrayScene.xmodl");



//    m_model.SetCamera(m_camera);
}

CChildView::~CChildView()
{
        delete m_rayimage[0];
        delete m_rayimage;
        m_rayimage = NULL;
}



//
// Name :         CChildView::LoadModel()
// Description :  Load a model to display.
//
void CChildView::LoadModel(const wchar_t *file)
{
    if(!m_model.LoadFile(file))
    {
        wstringstream msg;
        msg << L"Unable to open " << file << " - " << m_model.GetError() << ends;
        AfxMessageBox(msg.str().c_str());
    }

    // Set the camera to the settings from the file
    CGrVector cameraEye = m_model.GetCameraPosition();
    CGrVector cameraCenter = m_model.GetCameraTarget();
    if(cameraEye.W() != 0 && cameraCenter.W() != 0)
    {
        m_camera.SetEye(cameraEye);
        m_camera.SetCenter(cameraCenter);
    }
}


BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_RENDER_RAYTRACE, &CChildView::OnRenderRaytrace)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RAYTRACE, &CChildView::OnUpdateRenderRaytrace)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}



void CChildView::OnGLDraw(CDC * pDC)
{
	if(m_raytrace)
    {
        // Clear the color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up for parallel projection
        int width, height;
        GetSize(width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // If we got it, draw it
        if(m_rayimage)
        {
            glRasterPos3i(0, 0, 0);
            glDrawPixels(m_rayimagewidth, m_rayimageheight, 
                GL_RGB, GL_UNSIGNED_BYTE, m_rayimage[0]);
        }

        return;
    }

    glClearColor(0.3f, 0.5f, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    // Set up the camera
    //

    int width, height;
    GetSize(width, height);
    m_camera.Apply(width, height);

    //
    // Some standard parameters
    //

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Cull backfacing polygons
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Enable lighting
    glEnable(GL_LIGHTING);

    GLfloat black[] = {0, 0, 0, 1.0f};

    // Set ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmbientColor);

    // Set light 0
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    // Set light 1
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, Light1Pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1Color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, Light1Color);
    glLightfv(GL_LIGHT1, GL_AMBIENT, black);

    CGlRenderer renderer;

	glEnable(GL_NORMALIZE);
    m_model.Draw(&renderer);

    glFlush();
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_camera.MouseDown(point.x, point.y);

    COpenGLWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_camera.MouseDown(point.x, point.y, 2);

    COpenGLWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_camera.MouseMove(point.x, point.y, nFlags))
    {
         Invalidate();
    }

    COpenGLWnd ::OnMouseMove(nFlags, point);
}


BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_camera.MouseWheel(zDelta);
    Invalidate();

    return COpenGLWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CChildView::OnRenderRaytrace()
{
    m_raytrace = !m_raytrace;
    Invalidate();
    if(!m_raytrace)
        return;

	//
    // If an existing image already exists, delete it
    //

    if(m_rayimage != NULL)
    {
        delete m_rayimage[0];
        delete m_rayimage;
        m_rayimage = NULL;
    }

	//Get size of the window
	GetSize(m_rayimagewidth, m_rayimageheight);
	//Create the array
	m_rayimage = new BYTE *[m_rayimageheight];
	//Get the right row width
    int rowwid = m_rayimagewidth * 3;
    while(rowwid % 4)
        rowwid++;
	//Allocate memory
	m_rayimage[0] = new BYTE[m_rayimageheight * rowwid];
	for(int i=1; i<m_rayimageheight; i++)
    {
        m_rayimage[i] = m_rayimage[0] + i * rowwid;
    }
	//Default to bright color
	for(int i=0; i<m_rayimageheight; i++)
    {
        // Fill the image with green
        for(int j=0; j<m_rayimagewidth; j++)
        {
            m_rayimage[i][j * 3] = 0;               // red
            m_rayimage[i][j * 3 + 1] = BYTE(128);   // green
            m_rayimage[i][j * 3 + 2] = 0;           // blue
        }
    }

	//
    // Raytrace the image
    //

    // Instantiate the raytrace renderer
    CMyRaytraceRenderer renderer;

    // Configure the renderer
    // TODO1

    // Render the scene
    m_model.Draw(&renderer);

    // Tell it to do the actual ray tracing now
    // TODO2

    Invalidate();
}


void CChildView::OnUpdateRenderRaytrace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_raytrace);
}
