
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include "RayTutorial.h"
#include "ChildView.h"
#include <grafx.h>

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

    //
    // Load our object
    //

    LoadModel(L"models/TrayScene.xmodl");



//    m_model.SetCamera(m_camera);
}

CChildView::~CChildView()
{
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

    m_model.Draw();

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
