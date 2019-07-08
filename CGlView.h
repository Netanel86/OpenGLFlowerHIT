#ifndef CGLVIEW_H
#define CGLVIEW_H

#define PI 3.14//15926535898
#include<math.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

class CGlView
{
	// methods //////////////////////////////////////////////////////////
	public:
		CGlView(CWnd *pclWnd);
		~CGlView();

		void vResizeScene(int iWidth, int iHeigth);
		void vDrawGLScene();
		void MyLights();
		void drawSurface();
		void drawFlower(BOOL isShadow);
		void reFlection();
		void drawShadow();
		
	protected:
	private:
		void vInitGl(int iWidth, int iHeigth);
		BOOL bSetWindowPixelFormat(HDC hDC);
		BOOL bCreateViewGLContext(HDC hDC);

	// members //////////////////////////////////////////////////////////
	public:
		float light0[4],light1[4]; 
		int onGrow,
			onLight,
			onDrawTube,
			onDrawFrame,
			onDrawCurve,
			onDrawAxis,
			onShadow,
			onReflection;

		float xAngle,
			  yAngle,
			  zAngle,
			  xShift,
			  yShift,
			  zShift;

		double ScrollValue[12];
		
		//Flower Growth Members
		float maxAngle;
		float controlPoints[4];
		
	
	protected:
	private:			
		HGLRC	m_hGLContext;	// required GLContext
		CWnd *m_pclWnd;			// window inwhich opengl will be drawn
		HDC		m_hDC;				// handle to m_pclWnds device context
//! TEXTURE 1a 
    	AUX_RGBImageRec *tex;
		GLuint Textures[6];

		void GenerateTextures();
};

#endif