#ifndef FLWR_H
#define FLWR_H

class PNT3{
protected:
	double x,y,z;
public:
	PNT3(){ x=0;y=0;z=0; }
	PNT3(double x, double y, double z){ this->x = x; this->y = y; this->z = z; }
	PNT3( PNT3 &pnt ){this->x = pnt.x; this->y = pnt.y; this->z = pnt.z; }
	double getX(){return x;}
	double getY(){return y;}
	double getZ(){return z;}
	void setX(double x){this->x = x;}
	void setY(double y){this->y = y;}
	void setZ(double z){this->z = z;}
	void setCoord( double x, double y, double z ){this->x = x; this->y = y; this->z = z;}

	~PNT3(){}
};
class VEC3:public PNT3{
public:
	VEC3():PNT3(){}
	VEC3(PNT3 pnt){ this->x = pnt.getX(); this->y = pnt.getY(); this->z = pnt.getZ(); }
	void setVec( PNT3 pnt  ){ this->x = pnt.getX(); this->y = pnt.getY(); this->z = pnt.getZ(); }
	void normalize() 
	{ 
		//size of vector:
		double n = vectorSize();
		//unit vector:
		setCoord( this->x / n, this->y / n, this->z / n); 
	}
	double vectorSize(){return sqrt( x*x + y*y + z*z ); }
	VEC3 crossProduct( VEC3 vec1 )
	{
		VEC3 cross;
		PNT3 pnt;
		pnt.setX( (this->y * vec1.getZ()) - (vec1.getY() * this->z) );
		pnt.setY( -( (this->x * vec1.getZ() ) - (vec1.getX() * this->z ) ) );
		pnt.setZ( (this->x * vec1.getY() ) - (vec1.getX() * this->y) );
		cross.setVec( pnt );
		return cross;
	}
	void DrawVec(PNT3 pntA)
	{ 
		glVertex3f((GLfloat)pntA.getX(),(GLfloat)pntA.getY(),(GLfloat)pntA.getZ());
		glVertex3f((GLfloat)pntA.getX() + (GLfloat)this->x,(GLfloat)pntA.getY() + (GLfloat)this->y,(GLfloat)pntA.getZ() + (GLfloat)this->z);
	}

	~VEC3(){}
};
class EXTRUDE{
private:
	PNT3 curve;	//point on curve
public:	
	double time; //position on curve - 0 < time < n*PI 
	//TNB Frame point 'curve'
	VEC3 unitTangent,
		 unitNormal,
		 unitBinormal;

	EXTRUDE(){this->time = 0; this->curve.setCoord(0,0,0);}
	EXTRUDE(double length){ this->time = length;}
	EXTRUDE( EXTRUDE &extrude )
	{
		this->curve = extrude.curve;
		this->time = extrude.time;
	}
	void setLength( double length ){ this->time = length; curve.setCoord( x(time),y(time),z(time) );}

	void DrawCurve(double maxAngle)
	{
		double t;
		glBegin(GL_LINE_STRIP);
		glColor3f(1,0,0);
		for( t = 0.0f; t <= maxAngle; t += 0.02f )
			glVertex3d(x(t),y(t),z(t));
		glEnd();
	}
	void DrawFrenetFrame()
	{	
		glPushMatrix();
		glBegin(GL_LINES);
		glColor3f(0,0,1);
		unitTangent.DrawVec(curve);
		glColor3f(1,0,0);
		unitNormal.DrawVec(curve);
		glColor3f(0,1,0);
		unitBinormal.DrawVec(curve);
		glEnd();
		glPopMatrix();
	}
	void DrawFrenetFrame(double maxAngle)
	{
		double t;
		
		glPushMatrix();
		glBegin(GL_LINES);
		for( t = 0.0f; t <= maxAngle; t += 0.5f )
		{
			PNT3 crv( x(t),y(t),z(t) );
			VEC3 tangent,
				 normal,
				 binormal;
			EXTRUDE::buildTNBFrame(tangent,binormal,normal,t);

			glColor3f(0,0,1);
			tangent.DrawVec(crv);
			glColor3f(1,0,0);
			normal.DrawVec(crv);
			glColor3f(0,1,0);
			binormal.DrawVec(crv);
		}
		glEnd();
		glPopMatrix();
	}
	
	void buildTNBFrame( VEC3 &Tangent , VEC3 &Binormal , VEC3 &Normal, double t )
	{
		VEC3 dblDerV,	//derived curve vector
			 derV;		//double derived curve vector

		derV.setCoord( derX(t),derY(t),derZ(t) );
		dblDerV.setCoord( dblDerX(t),dblDerY(t),dblDerZ() );
		
		Tangent.setVec(derV);
		Binormal = derV.crossProduct(dblDerV);
		Normal = Binormal.crossProduct(Tangent);

		Tangent.normalize();
		Binormal.normalize();
		Normal.normalize();
	}
	double* buildTransformationMatrix()
	{
		double *tranMat = new double[16];

		double T[] = {unitTangent.getX(),unitTangent.getY(),unitTangent.getZ(),0};
		
		double N[] = {unitNormal.getX(),unitNormal.getY(),unitNormal.getZ(),0};
		
		double B[] = {unitBinormal.getX(),unitBinormal.getY(),unitBinormal.getZ(),0};
		
		double C[] = {curve.getX(),curve.getY(),curve.getZ(),1};

		copyArrayToMatrix( N, tranMat, 0 , 3 );
		copyArrayToMatrix( B, tranMat, 4 , 7 );
		copyArrayToMatrix( T, tranMat, 8 , 11 );
		copyArrayToMatrix( C, tranMat, 12 , 15 );
		return tranMat;
	}
	void copyArrayToMatrix( double *Arr, double *Matrix, int startInd, int endInd)
{
	int j = 0;
	
	for( int i = startInd; i <= endInd; i++ )
	{
		Matrix[i] = Arr[j];
		j++;
	}
}
	
	virtual void DrawObject(BOOL isShadow)=0;
	
    virtual double x( double t )=0;
	virtual double y( double t )=0;
	virtual double z( double t )=0;
	virtual double derX( double t )=0;
	virtual double derY( double t )=0;
	virtual double derZ( double t )=0;
	virtual double dblDerX( double t )=0;
	virtual double dblDerY( double t )=0;
	virtual	double dblDerZ()=0;
	
	~EXTRUDE(){}
};
class STEM: public EXTRUDE
{
private:
	double b,r;
public:	
	STEM():EXTRUDE(){this->b = 0; this->r = 0;}
	STEM(double length, double radius, double zConst):EXTRUDE(length){ this->r = radius; this->b = zConst; }
	void setRadius( double radius ){ this->r = radius; }
	void setHeightAcceleration( double zConst ){ this->b = zConst; }
	void DrawObject(BOOL isShadow)
	{
			EXTRUDE::buildTNBFrame(unitTangent,unitBinormal,unitNormal,time);
			
			double *transMat;
			transMat = EXTRUDE::buildTransformationMatrix();
			
			glPushMatrix();
			glMultMatrixd(  transMat );
			GLUquadricObj *obj;
			obj= gluNewQuadric();
			gluQuadricTexture ( obj, GL_TRUE );
			if(!isShadow)
			{
				glColor3f(1,1,1);
			}
			else 
			{
				glColor3f(0,0,0);
			}
			gluCylinder(obj,0.15,0.15,0.15,16,16);
			gluDeleteQuadric(obj);
			glPopMatrix();
	}

	double x( double t ){ return r*cos(t); }
	double y( double t ){ return r*sin(t); }
	double z( double t ){ return b * t; }
	double derX( double t ){ return r*-sin(t); }
	double derY( double t ){ return r*cos(t); }
	double derZ( double t ){ return b; }
	double dblDerX( double t ){ return r*-cos(t); }
	double dblDerY( double t ){ return r*-sin(t); }
	double dblDerZ(){ return 0; }
};
class HEAD: public EXTRUDE
{
private:
	double r;
	double cntrlA,cntrlB,cntrlC,cntrlD;

	void curvedsurface(double cntrlA, double cntrlB, double cntrlC, double cntrlD)
{

	PNT3 A(-1.5,cntrlA,cntrlC),//-1.5,0.5,-2.5
		 B(-1.5,cntrlB,cntrlD),//-1.5,-0.5,-4.0
		 C(1.5,cntrlB,cntrlD),//1.5,-0.5,-4.0
		 D(1.5,cntrlA,cntrlC),//1.5,0.5,-2.5
		 
		 E(-2.0,2.0,0.0),
		 F(-1.5,2.0,-2.0),
		 G(1.5,2.0,-2.0),
		 H(2.0,2.0,0.0),
		 
		 I(-2.0,-1.5,0.0),
		 J(-1.5,-1.5,-1.5),
		 K(1.5,-1.5,-1.5),
		 L(2.0,-1.5,0.0),
		
		 M(-1.0,-2.0,1.0),
		 N(0.0,-2.5,0.0),
		 O(0.0,-2.5,0.0),
		 P(1.0,-2.0,1.0);

	double a = 1.0,
		   b = 1.0 - a,
		   c = 1.0,
		   d = 1.0 - c;

	double detail = 10,
		   change = 1 / detail;


	//Surface Vectors
	PNT3 Vec[21][21],	//suface points matrix
		 derPa,derPb;	//derived surface point

	VEC3 derTangentVa,derTangentVb,	//surface tangent vectors
		 normalVec[21][21];	//normal vectors

	for( int i = 0; i <= detail; i++ )
	{
		for( int j = 0; j <= detail; j++ )
		{

			//point on surface
			Vec[i][j].setX(   A.getX()*a*a*a*c*c*c   + B.getX()*3*a*a*a*c*c*d
							+ C.getX()*3*a*a*a*c*d*d + D.getX()*a*a*a*d*d*d
							+ E.getX()*3*a*a*b*c*c*c + F.getX()*9*a*a*b*c*c*d
							+ G.getX()*9*a*a*b*c*d*d + H.getX()*3*a*a*b*d*d*d
							+ I.getX()*3*a*b*b*c*c*c + J.getX()*9*a*b*b*c*c*d
							+ K.getX()*9*a*b*b*c*d*d + L.getX()*3*a*b*b*d*d*d
							+ M.getX()*b*b*b*c*c*c   + N.getX()*3*b*b*b*c*c*d
							+ O.getX()*3*b*b*b*c*d*d + P.getX()*b*b*b*d*d*d );

			Vec[i][j].setY(   A.getY()*a*a*a*c*c*c   + B.getY()*3*a*a*a*c*c*d
							+ C.getY()*3*a*a*a*c*d*d + D.getY()*a*a*a*d*d*d
							+ E.getY()*3*a*a*b*c*c*c + F.getY()*9*a*a*b*c*c*d
							+ G.getY()*9*a*a*b*c*d*d + H.getY()*3*a*a*b*d*d*d
							+ I.getY()*3*a*b*b*c*c*c + J.getY()*9*a*b*b*c*c*d
							+ K.getY()*9*a*b*b*c*d*d + L.getY()*3*a*b*b*d*d*d
							+ M.getY()*b*b*b*c*c*c   + N.getY()*3*b*b*b*c*c*d
							+ O.getY()*3*b*b*b*c*d*d + P.getY()*b*b*b*d*d*d );

			Vec[i][j].setZ(   A.getZ()*a*a*a*c*c*c   + B.getZ()*3*a*a*a*c*c*d
							+ C.getZ()*3*a*a*a*c*d*d + D.getZ()*a*a*a*d*d*d
							+ E.getZ()*3*a*a*b*c*c*c + F.getZ()*9*a*a*b*c*c*d
							+ G.getZ()*9*a*a*b*c*d*d + H.getZ()*3*a*a*b*d*d*d
							+ I.getZ()*3*a*b*b*c*c*c + J.getZ()*9*a*b*b*c*c*d
							+ K.getZ()*9*a*b*b*c*d*d + L.getZ()*3*a*b*b*d*d*d
							+ M.getZ()*b*b*b*c*c*c   + N.getZ()*3*b*b*b*c*c*d
							+ O.getZ()*3*b*b*b*c*d*d + P.getZ()*b*b*b*d*d*d );

			//derivation on 'a' of point = tangent on 'a'
			derPa.setX(   A.getX()*3*a*a*c*c*c        + B.getX()*9*a*a*c*c*d
						+ C.getX()*9*a*a*c*d*d        + D.getX()*3*a*a*d*d*d
						+ E.getX()*3*(2*a-3*a*a)*c*c*c   + F.getX()*9*(2*a-3*a*a)*c*c*d
						+ G.getX()*9*(2*a-3*a*a)*c*d*d   + H.getX()*3*(2*a-3*a*a)*d*d*d
						+ I.getX()*3*(1-4*a+3*a*a)*c*c*c + J.getX()*9*(1-4*a+3*a*a)*c*c*d
						+ K.getX()*9*(1-4*a+3*a*a)*c*d*d + L.getX()*3*(1-4*a+3*a*a)*d*d*d
						+ M.getX()*3*(2*a-1-a*a)*c*c*c   + N.getX()*9*(2*a-1-a*a)*c*c*d
						+ O.getX()*9*(2*a-1-a*a)*c*d*d   + P.getX()*3*(2*a-1-a*a)*d*d*d );

			derPa.setY(   A.getY()*3*a*a*c*c*c        + B.getY()*9*a*a*c*c*d
						+ C.getY()*9*a*a*c*d*d        + D.getY()*3*a*a*d*d*d
						+ E.getY()*3*(2*a-3*a*a)*c*c*c   + F.getY()*9*(2*a-3*a*a)*c*c*d
						+ G.getY()*9*(2*a-3*a*a)*c*d*d   + H.getY()*3*(2*a-3*a*a)*d*d*d
						+ I.getY()*3*(1-4*a+3*a*a)*c*c*c + J.getY()*9*(1-4*a+3*a*a)*c*c*d
						+ K.getY()*9*(1-4*a+3*a*a)*c*d*d + L.getY()*3*(1-4*a+3*a*a)*d*d*d
						+ M.getY()*3*(2*a-1-a*a)*c*c*c   + N.getY()*9*(2*a-1-a*a)*c*c*d
						+ O.getY()*9*(2*a-1-a*a)*c*d*d   + P.getY()*3*(2*a-1-a*a)*d*d*d );

			derPa.setZ(   A.getZ()*3*a*a*c*c*c        + B.getZ()*9*a*a*c*c*d
						+ C.getZ()*9*a*a*c*d*d        + D.getZ()*3*a*a*d*d*d
						+ E.getZ()*3*(2*a-3*a*a)*c*c*c   + F.getZ()*9*(2*a-3*a*a)*c*c*d
						+ G.getZ()*9*(2*a-3*a*a)*c*d*d   + H.getZ()*3*(2*a-3*a*a)*d*d*d
						+ I.getZ()*3*(1-4*a+3*a*a)*c*c*c + J.getZ()*9*(1-4*a+3*a*a)*c*c*d
						+ K.getZ()*9*(1-4*a+3*a*a)*c*d*d + L.getZ()*3*(1-4*a+3*a*a)*d*d*d
						+ M.getZ()*3*(2*a-1-a*a)*c*c*c   + N.getZ()*9*(2*a-1-a*a)*c*c*d
						+ O.getZ()*9*(2*a-1-a*a)*c*d*d   + P.getZ()*3*(2*a-1-a*a)*d*d*d );

			//derivation on 'b' of point =  tangent on 'b'
			derPb.setX(   A.getX()*3*a*a*a*c*c        + B.getX()*3*a*a*a*(2*c-3*c*c)
						+ C.getX()*3*a*a*a*(1-4*c+3*c*c) + D.getX()*3*a*a*a*(-1+2*c-c*c)
						+ E.getX()*9*a*a*b*c*c        + F.getX()*9*a*a*b*(2*c-3*c*c)
						+ G.getX()*9*a*a*b*(1-4*c+3*c*c) + H.getX()*9*a*a*b*(-1+2*c-c*c)
						+ I.getX()*9*a*b*b*c*c        + J.getX()*9*a*b*b*(2*c-3*c*c)
						+ K.getX()*9*a*b*b*(1-4*c+3*c*c) + L.getX()*9*a*b*b*(-1+2*c-c*c)
						+ M.getX()*3*b*b*b*c*c        + N.getX()*3*b*b*b*(2*c-3*c*c)
						+ O.getX()*3*b*b*b*(1-4*c+3*c*c) + P.getX()*3*b*b*b*(-1+2*c-c*c) );
			
			derPb.setY(	  A.getY()*3*a*a*a*c*c        + B.getY()*3*a*a*a*(2*c-3*c*c)
						+ C.getY()*3*a*a*a*(1-4*c+3*c*c) + D.getY()*3*a*a*a*(-1+2*c-c*c)
						+ E.getY()*9*a*a*b*c*c        + F.getY()*9*a*a*b*(2*c-3*c*c)
						+ G.getY()*9*a*a*b*(1-4*c+3*c*c) + H.getY()*9*a*a*b*(-1+2*c-c*c)
						+ I.getY()*9*a*b*b*c*c        + J.getY()*9*a*b*b*(2*c-3*c*c)
						+ K.getY()*9*a*b*b*(1-4*c+3*c*c) + L.getY()*9*a*b*b*(-1+2*c-c*c)
						+ M.getY()*3*b*b*b*c*c        + N.getY()*3*b*b*b*(2*c-3*c*c)
						+ O.getY()*3*b*b*b*(1-4*c+3*c*c) + P.getY()*3*b*b*b*(-1+2*c-c*c));
			
			derPb.setZ(   A.getZ()*3*a*a*a*c*c        + B.getZ()*3*a*a*a*(2*c-3*c*c)
						+ C.getZ()*3*a*a*a*(1-4*c+3*c*c) + D.getZ()*3*a*a*a*(-1+2*c-c*c)
						+ E.getZ()*9*a*a*b*c*c        + F.getZ()*9*a*a*b*(2*c-3*c*c)
						+ G.getZ()*9*a*a*b*(1-4*c+3*c*c) + H.getZ()*9*a*a*b*(-1+2*c-c*c)
						+ I.getZ()*9*a*b*b*c*c        + J.getZ()*9*a*b*b*(2*c-3*c*c)
						+ K.getZ()*9*a*b*b*(1-4*c+3*c*c) + L.getZ()*9*a*b*b*(-1+2*c-c*c)
						+ M.getZ()*3*b*b*b*c*c        + N.getZ()*3*b*b*b*(2*c-3*c*c)
						+ O.getZ()*3*b*b*b*(1-4*c+3*c*c) + P.getZ()*3*b*b*b*(-1+2*c-c*c) );
			
			//set as vectors
			derTangentVa.setVec(derPa);
			derTangentVb.setVec(derPb);

			//normal vector = derivedVectorONa X derivedVectorONb
			normalVec[i][j] = derTangentVa.crossProduct( derTangentVb );
			//normalize for unit normal vector
			normalVec[i][j].normalize();

			c -= change;
			d = 1.0 - c;

		}

		a -= change;
		b = 1.0 - a;

		c = 1.0;
		d = 1.0 - c;
	}

	for(int m = 0; m < detail; m++)
	{
		glBegin(GL_TRIANGLE_STRIP);
			for(int n = 0; n <= detail; n++)
			{
				if( n==0 || !(n%2) )
					glTexCoord2f(0,0);
				else
					glTexCoord2f(1,0);
				glNormal3d(normalVec[m][n].getX(),normalVec[m][n].getY(),normalVec[m][n].getZ());
				glVertex3d(Vec[m][n].getX(),Vec[m][n].getY(),Vec[m][n].getZ());
				
				if( n==0 || !(n%2) )
					glTexCoord2f(0,1);
				else
					glTexCoord2f(1,1);
				glNormal3d(normalVec[m+1][n].getX(),normalVec[m+1][n].getY(),normalVec[m+1][n].getZ());
				glVertex3d(Vec[m+1][n].getX(),Vec[m+1][n].getY(),Vec[m+1][n].getZ());
			}
		glEnd();
	}
}

public:
	HEAD():EXTRUDE(){ this->r = 0; }
	HEAD( double length, double radius ):EXTRUDE(length){ this->r = radius; }
	
	void setRadius( double radius ){ this->r = radius; }
	void setControlPoints(float *cntrlpnts)
	{
		this->cntrlA = cntrlpnts[0];
		this->cntrlB = cntrlpnts[1];
		this->cntrlC = cntrlpnts[2];
		this->cntrlD = cntrlpnts[3];
	}

	void DrawObject(BOOL isShadow)
	{
		EXTRUDE::buildTNBFrame(unitTangent,unitBinormal,unitNormal,time);
			
		double *transMat;
		transMat = EXTRUDE::buildTransformationMatrix();

		glPushMatrix();
		glMultMatrixd(  transMat );
		if (!isShadow)
		{
			glColor3f(1,1,1);
		}
		else
		{
			glColor3f(0,0,0);
		}
		glRotatef(10,0,0,1);
		glRotatef(90,0,1,0);
		curvedsurface(cntrlA,cntrlB,cntrlC,cntrlD);
		glPopMatrix();	
	}

	double x( double t ){ return r*sin(t); }
	double y( double t ){ return r*cos(t); }
	double z( double t ){ return 0; }
	double derX( double t ){ return r*cos(t); }
	double derY( double t ){ return r*-sin(t); }
	double derZ( double t ){ return 0; }
	double dblDerX( double t ){ return r*-sin(t); }
	double dblDerY( double t ){ return r*-cos(t); }
	double dblDerZ(){ return 0; }

};




#endif