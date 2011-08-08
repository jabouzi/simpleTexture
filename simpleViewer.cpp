/****************************************************************************

 Copyright (C) 2002-2011 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.3.10.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "simpleViewer.h"
#include "countries.h"
#include <math.h>

using namespace std;
using namespace qglviewer;

Viewer::~Viewer()
{
	gluDeleteQuadric(quadric);
	glDeleteTextures(1, &texture);
}

// Draws a spiral
void Viewer::draw()
{
    float pos[4] = {0, 0, 9000, 0};
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    //light1->getPosition(pos[0], pos[1], pos[2]);
    glLightfv(GL_LIGHT1, GL_POSITION, pos);
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, texture);   
    /*GLfloat matAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 10.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);*/
    //glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);  
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);  
    gluSphere(quadric, EARTH_RADIUS, 360, 180);
    glPopMatrix();
    //glPushMatrix();
    //drawNames();
    //glPopMatrix();
    //glPushMatrix();
    //glLoadIdentity();
    drawCurve();
    //glPopMatrix();
    //glPopMatrix();
    
    // position the light
    //float lightPos[4] = {-10, 0, 5, 0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //glEnable(GL_LIGHT0);  
    //if (light1->grabsMouse())
        //drawLight(GL_LIGHT1, 1.2f);
    //else
        //drawLight(GL_LIGHT1); 
}

void Viewer::init()
{
    setSceneRadius(10000);
	showEntireScene();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GLU_TRUE);

	QImage image = QGLWidget::convertToGLFormat(QImage("earth.jpg"));
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    //glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D,     // texture to specify
			  GL_RGBA,           // internal texture storage format
			  image.width(),     // texture width
			  image.height(),    // texture height
			  GL_RGBA,           // pixel format
			  GL_UNSIGNED_BYTE,  // color component format
			  image.bits());     // pointer to texture image
              
    glEnable(GL_LIGHT1);
    GLfloat lightKa[] = {.0f, .0f, .0f, 1.0f};      // ambient light
    GLfloat lightKd[] = {.9f, .9f, .9f, 1.0f};      // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};               // specular light
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightKs);
    
    light1 = new ManipulatedFrame();
    setMouseTracking(true);

    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    light1->setPosition(0, 0, 9000);
    glPopMatrix();
}

void Viewer::lonLat2Point(float lon, float lat, Vector *pos, GLdouble extra)
{
    /*pos->x = (EARTH_RADIUS + extra) * cos(lon) * cos(lat);
    pos->y = (EARTH_RADIUS + extra) * sin(lon) * cos(lat);
    pos->z = (EARTH_RADIUS + extra) * sin(lat);*/

    float    angX, angY;
    angX = (180.f+lat) * PI / 180.f;
    angY = lon * PI / 180.f;
    pos->x = fabsf(cosf(angY)) * (EARTH_RADIUS + extra) * sinf(angX);
    pos->y = (EARTH_RADIUS + extra) * sinf(angY);
    pos->z = fabsf(cosf(angY)) * (EARTH_RADIUS + extra) * cosf(angX);
    //qDebug() << pos->x << pos->y << pos->z;
}

void Viewer::drawNames()
{
    glLineWidth(1);
    Vector countries_positions[NUM_COUNTRIES];
    Vector countries_positions2[NUM_COUNTRIES];
    QFont myFont( "TypeWriter", 6, QFont::Bold);
    for (int i=0; i<NUM_COUNTRIES-1; i++) {
       lonLat2Point(countries[i].lon, countries[i].lat, &countries_positions[i],0);
       lonLat2Point(countries[i].lon, countries[i].lat, &countries_positions2[i],1000);
       glBegin(GL_LINES);
           glColor4f(1,0,0,1.0f);
           glVertex3f (countries_positions[i].x  ,  countries_positions[i].y  ,  countries_positions[i].z);
           glVertex3f (countries_positions2[i].x  ,  countries_positions2[i].y  ,  countries_positions2[i].z);
       glEnd();
       glColor4f(1.0, 1.0, 1.0, 1.0);
       renderText(countries_positions2[i].x  ,  countries_positions2[i].y  ,  countries_positions2[i].z, QString(countries[i].name), myFont );
    }
}

void Viewer::drawCurve()
{

    Vector v1[2];
    Vector v2[2];
    lonLat2Point(21.4273779,39.8148383, &v1[0],0);
    lonLat2Point(21.427377,9.8148383, &v2[0],4000);
    lonLat2Point(45.0, -73.0, &v1[1],0);
    lonLat2Point(45.0, -73.0, &v2[1],4000);

    //glMatrixMode(GL_MODELVIEW);
    
    glBegin(GL_POINTS);
    glPushMatrix();
    glLoadIdentity();
    //glColor3f(1.0,1.0,0.0);
    glPopMatrix();
    glVertex3f(v1[0].x, v1[0].y, v1[0].z);
    glVertex3f(v1[1].x, v1[1].y, v1[1].z);
    glVertex3f(v2[0].x, v2[0].y, v2[0].z);
    glVertex3f(v2[1].x, v2[1].y, v2[1].z);
    glEnd();

    GLfloat ctrlPts [4][3] = { {v1[0].x, v1[0].y, v1[0].z}, {v2[0].x, v2[0].y, v2[0].z},
                                   {v2[1].x, v2[1].y, v2[1].z},  {v1[1].x, v1[1].y, v1[1].z} };

    glMap1f (GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
    glEnable (GL_MAP1_VERTEX_3);

    GLint k;
    glPushMatrix();
    //glLoadIdentity();
    //glColor3f (1.0, 1.0, 1.0);
    
    glBegin (GL_LINE_STRIP);             //  Generate Bezier "curve".
        glLineWidth(10);
        for (k = 0; k <= 50; k++)
            glEvalCoord1f (GLfloat (k) / 50.0);
    glEnd ( );
    glPopMatrix();
    glPushMatrix();
    //glLoadIdentity();
    //glColor3f (1.0, 0.0, 0.0);
    //glPopMatrix();
    glBegin (GL_POINTS);                 //  Plot control points.
    glPointSize (10);                   //  Set point size to 5.0.
        for (k = 0; k < 4; k++);
                glVertex3fv (&ctrlPts [k][0]);
    glEnd ( );
    glPopMatrix();
    
}
