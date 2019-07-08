
#include "StdAfx.h"
#include "CGlView.h"
#include "MyFlowerFunc.h"

PNT3 Ground[4];
VEC3 midGroundVec[2];
void checkRotation(float &x, float &y, float &z);
void MakeShadowMatrix(PNT3 points[3], GLfloat lightPos[4], GLfloat destMat[4][4]);
void drawAxes();


CGlView::CGlView(CWnd *pclWnd)
{
	m_pclWnd = pclWnd;
	m_hDC  = ::GetDC(pclWnd->m_hWnd);
	BOOL bSuccess;

	RECT rect; 
	m_pclWnd->GetClientRect(&rect);
	int iWidth = rect.right - rect.left;
	int iHeight = rect.bottom - rect.top;

	bSuccess = bSetWindowPixelFormat(m_hDC);
	ASSERT(&bSuccess);

	bSuccess = bCreateViewGLContext(m_hDC);
	ASSERT(&bSuccess);

	vInitGl(iWidth, iHeight);
	
	//set ground points
	Ground[0].setCoord(-29.0, 0.0, -29.0);
	Ground[1].setCoord(-29.0, 0.0, 29.0);
	Ground[2].setCoord(29.0, 0.0, -29.0);
	Ground[3].setCoord(29.0, 0.0, 29.0);
	
	// set flags
    onGrow=0;
	onLight=0;
	onDrawCurve=0;
	onDrawFrame=0;
	onDrawTube=0;
	onShadow=0;
	onReflection=0;
	onDrawAxis=0;
	
	xAngle = 0.0f;
	yAngle = 0.0f;
	zAngle = 0.0f;
	xShift = 0.0f;
	yShift = 0.0f;
	zShift = 0.0f;

	maxAngle = 0.0f;
	
	controlPoints[0]=2.0;
	controlPoints[1]=4.5;
	controlPoints[2]=0.0;
	controlPoints[3]=0.0;
	//spinFlag = 0;
}

CGlView::~CGlView()
{
	// make the rendering context not current
	if(wglGetCurrentContext()!=NULL) 
		wglMakeCurrent(NULL, NULL);
	
	// delete the Context
  if (m_hGLContext!=NULL)
  {
		wglDeleteContext(m_hGLContext);
    m_hGLContext = NULL;
  }

//! TEXTURE 1a 
  if(tex!=NULL)
 	  free(tex);
//! TEXTURE 1b

}

void CGlView::vResizeScene(int iWidth, int iHeigth)
{
	// prevent a divide by zero
	if (iHeigth==0)								
		iHeigth=1;

	glViewport(0, 0, iWidth, iHeigth);			

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)iWidth/(GLfloat)iHeigth,0.45f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}


void CGlView::vDrawGLScene()									
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();								

	gluLookAt(ScrollValue[0],ScrollValue[1],ScrollValue[2],
			  ScrollValue[3],ScrollValue[4],ScrollValue[5],
			  ScrollValue[6],ScrollValue[7],ScrollValue[8]);
	
	glTranslatef(0, -2, -15);
	glRotated(45,4.0,2.0,1.0);
	
	glTranslatef(xShift,yShift,zShift);

	checkRotation(xAngle,yAngle,zAngle);
	glRotatef(xAngle,1.0f,0.0f,0.0f);
	glRotatef(yAngle,0.0f,1.0f,0.0f);
	glRotatef(zAngle,0.0f,0.0f,1.0f);

	MyLights();
	
	
	if( maxAngle < 3*PI && onGrow )
		maxAngle += 0.1f;
	if( onGrow && maxAngle >= 3*PI )
	{
		if( controlPoints[0] > 0.0 )
			controlPoints[0] -= 0.01f;
		if( controlPoints[1] > 0.0 )
			controlPoints[1] -= 0.01f;
		if( controlPoints[2] > -2.0 )
			controlPoints[2] -= 0.01f;
		if( controlPoints[3] > -4.5 )
			controlPoints[3] -= 0.01f;
	}

	if( onShadow && onLight )
	{
		drawShadow();
	}


	//Surface
	glPushMatrix();
	if( onReflection )
	{
		reFlection();
	}
	else
	{
		drawSurface();
	}
	glPopMatrix();
	
	glPushMatrix();
	if( onDrawAxis )
	{
		drawAxes();
	}
	glPopMatrix();

	drawFlower(false);


	// swap the buffer
	SwapBuffers(m_hDC);
}
BOOL CGlView::bSetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

    pixelDesc.nSize     = sizeof(PIXELFORMATDESCRIPTOR);
    pixelDesc.nVersion	= 1;

    pixelDesc.dwFlags   =    
	
		PFD_DRAW_TO_WINDOW |// Format Must Support Window
		PFD_SUPPORT_OPENGL |// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	// Must Support Double Buffering
		PFD_TYPE_RGBA,		// Request An RGBA Format
		32,					// Select A 16Bit Color Depth
		0, 0, 0, 0, 0, 0,	// Color Bits Ignored (?)
		0,					// No Alpha Buffer
		0,					// Shift Bit Ignored (?)
		0,					// No Accumulation Buffer
		0, 0, 0, 0,			// Accumulation Bits Ignored (?)
		16,					// 16Bit Z-Buffer (Depth Buffer)  
		0,					// No Stencil Buffer
		0,					// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,		// Main Drawing Layer
		0,					// Reserved (?)
		0, 0, 0;			// Layer Masks Ignored (?)

    int iGLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
		ASSERT(&iGLPixelIndex);

    if (SetPixelFormat( hDC, iGLPixelIndex, &pixelDesc))
			return false;

    return TRUE;
}
BOOL CGlView::bCreateViewGLContext(HDC hDC)
{
		m_hGLContext = wglCreateContext(hDC);
    if (m_hGLContext == NULL)
      return FALSE;

    if (wglMakeCurrent(hDC, m_hGLContext)==FALSE)
			return FALSE;

    return TRUE;
}
void CGlView::vInitGl(int iWidth, int iHeigth)
{
	glShadeModel(GL_SMOOTH);								
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);		
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glViewport(0,0,iWidth,iHeigth);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();								

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)iWidth/(GLfloat)iHeigth,0.45f,100.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();	

	GenerateTextures();
}
void CGlView::GenerateTextures()
{
    glGenTextures( 4, Textures);
    int i;
    char *imagesName[4]={"stem1.bmp","rose.bmp","flwrcenter.bmp", "grass.bmp",};
	for(i=0; i<4; i++)
	{
      tex=auxDIBImageLoad(imagesName[i]);
	  if(tex!=NULL)
	  {
        glBindTexture( GL_TEXTURE_2D, Textures[i]);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //TEXTURE in MEMORY
        //properties
                    //2D for XYZ
                                  //0-details level
                                      //3-number of color components
                                          //2^n
                                                       //2^k
                                                                 //border width
                                                                    //format GL_RED,GL_BLUE,GL_ALPHA,GL_RGBA
                                                                             //pixel type
                                                                                              //image's memory address
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tex->sizeX, tex->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
		// not MUST
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, tex->sizeX, tex->sizeY, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
   	    free(tex);
	  }
    }
}

void CGlView::reFlection()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);								// All this code would be written only to Stencil Buffer
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);			//Cleaning of Stencil Buffer

	glColorMask(FALSE, FALSE, FALSE, FALSE);			//Starting the rewriting of Stencil Buffer
	glDisable(GL_DEPTH_TEST);
	drawSurface();
	glEnable(GL_DEPTH_TEST);
	glColorMask(TRUE, TRUE, TRUE, TRUE);				//Ending of the rewriting of Stencil Buffer
			
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);				//Restoring of the Stencil Buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//Starting the Drawing of reflected scene
	if (true)
	{
		glPushMatrix();	
		glRotated(-180,1,0,0);
		glScalef(1, 1, -1); //swap on Z axis
		glEnable(GL_CULL_FACE);
		glCullFace (GL_BACK);
		drawFlower(false);
		glCullFace (GL_FRONT);
		drawFlower(false);
		glDisable(GL_CULL_FACE);
		glPopMatrix();									//Ending of the Drawing of reflected scene
	}
		

	glDepthMask(FALSE);
	drawSurface();
	glDepthMask(TRUE);
	// Disable GL_STENCIL_TEST to show All, else it will be cut on GL_STENCIL
	glDisable(GL_STENCIL_TEST);

}
void CGlView::drawSurface()
{
	VEC3 normal;
	

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Textures[3]);

	glPolygonMode(GL_BACK, GL_NONE);
    glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_QUADS);
	glColor4f(1.0f,1.0f,1.0f,0.6);
		
	midGroundVec[0].setCoord( Ground[1].getX() - Ground[0].getX() , Ground[1].getY() - Ground[0].getY() , Ground[1].getZ() - Ground[0].getZ() ) ;
	midGroundVec[1].setCoord( Ground[2].getX() - Ground[0].getX(),Ground[2].getY() - Ground[0].getY(),Ground[2].getZ() - Ground[0].getZ() ) ;

	normal = midGroundVec[0].crossProduct( midGroundVec[1] );
	normal.normalize();

	glNormal3d(normal.getX(),normal.getY(),normal.getZ() );
	glTexCoord2f(0,0);
	glVertex3d( Ground[1].getX(), Ground[1].getY(), Ground[1].getZ());	
	glTexCoord2f(0,3);
	glVertex3d( Ground[0].getX(), Ground[0].getY(), Ground[0].getZ());	
	glTexCoord2f(3,3);
	glVertex3d( Ground[2].getX(), Ground[2].getY(), Ground[2].getZ());		
	glTexCoord2f(3,0);  
	glVertex3f( Ground[3].getX(),Ground[3].getY(), Ground[3].getZ());
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void CGlView::drawShadow()
{
        //SHADING begin
        float cubeXform[4][4];
        //we'll define cubeXform matrix in MakeShadowMatrix Sub
        // Disable lighting, we'll just draw the shadow
        //else instead of shadow we'll see stange projection of the same objects
        glDisable (GL_LIGHTING);   
		// floor shadow
        //!!!!!!!!!!!!!
        glPushMatrix();
        //!!!!!!!!!!!!   
        MakeShadowMatrix(Ground, light0 , cubeXform);
        glMultMatrixf (&cubeXform[0][0]);
		drawFlower(onShadow);        
		//!!!!!!!!!!!!!
        glPopMatrix();
        //!!!!!!!!!!!!!

		glEnable( GL_LIGHTING );
}
void CGlView::drawFlower( BOOL isShadow )
{
	float angle = 0;

	static double radius=0.6,bConst=0.7;
	
	STEM stem(angle,radius,bConst);
	HEAD head(2*PI,1.5);
	
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glTranslatef(0,(GLfloat)0.1,0);
	if( onDrawTube )
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,Textures[0]);
		for( angle = 0; angle <= maxAngle; angle += 0.1f )
		{
			stem.setLength( angle );
			stem.DrawObject(isShadow);	
		}
		
		//position matrix on end of stem
		double *mat = stem.buildTransformationMatrix();
		
		glBindTexture(GL_TEXTURE_2D,Textures[2]);
		glPushMatrix();
		glMultMatrixd( mat );
		GLUquadricObj *obj;
		obj= gluNewQuadric();
		gluQuadricTexture ( obj, GL_TRUE );
		glTranslatef(0.0,0.0,0.4);
		glRotatef(90,0,1,0);
		gluSphere(obj,0.45,10,10);
		gluDeleteQuadric(obj);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D,Textures[1]);
		glPushMatrix();
		glMultMatrixd( mat );
		glRotatef(180,1,0,0);
		glScalef(0.4,0.4,0.4);
		glTranslatef(0.0,0.0,-3.0);
		for( angle = 0; angle <= 2*PI; angle += PI/3 )
		{
			head.setControlPoints(controlPoints);
			head.setLength( angle );
			head.DrawObject(isShadow);
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	if( onDrawCurve )
		stem.DrawCurve(3*PI);

	if( onDrawFrame )
		stem.DrawFrenetFrame(3*PI);
	glPopMatrix();
}
void CGlView::MyLights()
{
	light0[0]=ScrollValue[9];
	light0[1]=ScrollValue[10];
	light0[2]=ScrollValue[11];
	light0[3]=1;
	
	glLightfv (GL_LIGHT0, GL_POSITION, light0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	if( onLight )
	{	
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
	
	//_____________________________________________light_pos_______________________________
	glPushMatrix();
	glTranslatef(light0[0],light0[1],light0[2]);
	GLUquadricObj *obj;
	glColor3f(1,1,1);
	obj= gluNewQuadric();
	gluSphere(obj,0.2,10,10);
	gluDeleteQuadric(obj);
	glPopMatrix();
	//_____________________________________________light_pos_end___________________________
}

void MakeShadowMatrix(PNT3 points[3], GLfloat lightPos[4], GLfloat destMat[4][4])
{
	VEC3 midVec2[2];
	GLfloat planeCoeff2;
	midVec2[0].setCoord( points[1].getX() - points[0].getX() , points[1].getY() - points[0].getY() , points[1].getZ() - points[0].getZ() ) ;
	midVec2[1].setCoord( points[2].getX() - points[0].getX(),points[2].getY() - points[0].getY(),points[2].getZ() - points[0].getZ() ) ;
	
	VEC3 planeCoeff = midVec2[0].crossProduct( midVec2[1] );
	planeCoeff.normalize();
	
	GLfloat dot;

	// Find the plane equation coefficients
	// Find the first three coefficients the same way we
	// find a normal.

	// Find the last coefficient by back substitutions
	planeCoeff2 = - (
		(planeCoeff.getX()* (points[2].getX()+0.1) + (planeCoeff.getY()*(points[2].getY()+0.1)) +
		(planeCoeff.getZ()*(points[2].getZ()+0.1))));


	// Dot product of plane and light position
	dot = planeCoeff.getX() * lightPos[0] +
			planeCoeff.getY() * lightPos[1] +
			planeCoeff.getZ() * lightPos[2] +
			planeCoeff2;

	// Now do the projection
	// First column
    destMat[0][0] = dot - lightPos[0] * planeCoeff.getX();
    destMat[1][0] = 0.0f - lightPos[0] * planeCoeff.getY();
    destMat[2][0] = 0.0f - lightPos[0] * planeCoeff.getZ();
    destMat[3][0] = 0.0f - lightPos[0] * planeCoeff2;

	// Second column
	destMat[0][1] = 0.0f - lightPos[1] * planeCoeff.getX();
	destMat[1][1] = dot - lightPos[1] * planeCoeff.getY();
	destMat[2][1] = 0.0f - lightPos[1] * planeCoeff.getZ();
	destMat[3][1] = 0.0f - lightPos[1] * planeCoeff2;

	// Third Column
	destMat[0][2] = 0.0f - lightPos[2] * planeCoeff.getX();
	destMat[1][2] = 0.0f - lightPos[2] * planeCoeff.getY();
	destMat[2][2] = dot - lightPos[2] * planeCoeff.getZ();
	destMat[3][2] = 0.0f - lightPos[2] * planeCoeff2;

	// Fourth Column
	destMat[0][3] = 0.0f - lightPos[3] * planeCoeff.getX();
	destMat[1][3] = 0.0f - lightPos[3] * planeCoeff.getY();
	destMat[2][3] = 0.0f - lightPos[3] * planeCoeff.getZ();
	destMat[3][3] = dot - lightPos[3] * planeCoeff2;
}
void drawAxes()
{
	//axes
	glBegin(GL_LINES);	
	    //x  RED
	    glColor3f(1.0f,0.0f,0.0f);						
		glVertex3f( 0.0f, 0.0f, 0.0f);	
		glVertex3f( 3.0f, 0.0f, 0.0f);	
	    //y  GREEN 
	    glColor3f(0.0f,1.0f,0.0f);						
		glVertex3f( 0.0f, 0.0f, 0.0f);	
		glVertex3f( 0.0f, 3.0f, 0.0f);	
	    //z  BLUE
	    glColor3f(0.0f,0.0f,1.0f);						
		glVertex3f( 0.0f, 0.0f, 0.0f);	
		glVertex3f( 0.0f, 0.0f, 3.0f);	
    glEnd();
}
void checkRotation(float &x, float &y, float &z)
{
	if( x == 360 || x == -360  )
		 x = 0;

	if( y == 360 || y == -360 )
		 y = 0;

	if( z == 360 || z == -360 )
		 z = 0;
}