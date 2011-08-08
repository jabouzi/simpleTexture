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
    float pos[4] = {-10, 0, 8000, 0};
    light1->getPosition(pos[0], pos[1], pos[2]);
    glLightfv(GL_LIGHT1, GL_POSITION, pos);
    
    glBindTexture(GL_TEXTURE_2D, texture);   
    /*GLfloat matAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 10.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);*/
    glPushMatrix();
    glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);  
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);  
    gluSphere(quadric, EARTH_RADIUS, 360, 180);
    glPopMatrix();
    drawNames();
    
    // position the light
    //float lightPos[4] = {-10, 0, 5, 0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //glEnable(GL_LIGHT0);  
    if (light1->grabsMouse())
        drawLight(GL_LIGHT1, 1.2f);
    else
        drawLight(GL_LIGHT1); 
}

void Viewer::init()
{
    setSceneRadius(10000);
	showEntireScene();

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
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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

    light1->setPosition(-10, 0, 8000);
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

QString Viewer::helpString() const
{
  QString text("<h2>S i m p l e V i e w e r</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}
