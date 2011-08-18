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

void Viewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float pos[4] = {0, 0, 9000, 0};
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
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
    
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);  
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);  
    gluSphere(quadric, EARTH_RADIUS, 360, 180);
    glPopMatrix();
    //drawNames();
    drawLines();
    drawCurve();

    /*if (light1->grabsMouse())
        drawLight(GL_LIGHT1, 1.2f);
    else
        drawLight(GL_LIGHT1); */
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
    glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
              
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
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
    glEnable(GL_BLEND);
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
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}

void Viewer::drawLines()
{
    int lineWidth = 1;
    float px, py, pz = 0.0;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(lineWidth);
    glColor4f(0,0,0,1.0f);
    for (int y=0; y<=EARTH_LAT_RES; y++) {
        glBegin(GL_LINE_STRIP);
        for (int x=0; x<=EARTH_LON_RES; x++) {            
            float    angX, angY;
            angX = (x * 360.f / EARTH_LON_RES) * PI / 180.f;
            angY = (-90.f + (y * 180.f / EARTH_LAT_RES)) * PI / 180.f;
            px = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
            py = EARTH_RADIUS * sinf(angY);
            pz = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
            glVertex3f(px,py,pz);
        }
        glEnd();
    }

    glColor4f(0,0,0,1.0f);
    for (int x=0; x<=EARTH_LON_RES; x++) {
       glBegin(GL_LINE_STRIP);
       for (int y=0; y<=EARTH_LAT_RES; y++) {           
           float    angX, angY;
            angX = (x * 360.f / EARTH_LON_RES) * PI / 180.f;
            angY = (-90.f + (y * 180.f / EARTH_LAT_RES)) * PI / 180.f;
            px = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
            py = EARTH_RADIUS * sinf(angY);
            pz = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
            glVertex3f(px,py,pz);
       }
       glEnd();
    }
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}

void Viewer::drawCurve()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
    glEnable(GL_BLEND);
    
    Vector v1[2];
    Vector v2[2];
    Vector v3[2];
    Vector v4[2];
    lonLat2Point(21.4273779,39.8148383, &v1[0],0);
    lonLat2Point(21.427377,9.8148383, &v2[0],100);
    lonLat2Point(45.0, -73.0, &v1[1],0);
    lonLat2Point(45.0, -73.0, &v2[1],100);
    lonLat2Point(21.4273779,39.8148383, &v3[0],0);
    lonLat2Point(21.427377,9.8148383, &v4[0],100);
    lonLat2Point(34.0, 9.0, &v3[1],0);
    lonLat2Point(34.0, 9.0, &v4[1],100);

    /*glPushMatrix();
    glColor3f(1.0,1.0,0.0);
    glPointSize (5); 
    glBegin(GL_POINTS);       
        glVertex3f(v1[0].x, v1[0].y, v1[0].z);
        glVertex3f(v1[1].x, v1[1].y, v1[1].z);
        glVertex3f(v2[0].x, v2[0].y, v2[0].z);
        glVertex3f(v2[1].x, v2[1].y, v2[1].z);
    glEnd();
    glPopMatrix();*/

    GLfloat ctrlPts [4][3] = { {v1[0].x, v1[0].y, v1[0].z}, {v2[0].x, v2[0].y, v2[0].z},
                                   {v2[1].x, v2[1].y, v2[1].z},  {v1[1].x, v1[1].y, v1[1].z} };
    GLfloat ctrlPts2 [4][3] = { {v3[0].x, v3[0].y, v3[0].z}, {v4[0].x, v4[0].y, v4[0].z},
                                   {v4[1].x, v4[1].y, v4[1].z},  {v3[1].x, v3[1].y, v3[1].z} };

    glMap1f (GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts);
    //glMap1f (GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts2);
    glEnable (GL_MAP1_VERTEX_3);

    GLint k;
    
    glColor3f (1.0, 1.0, 0.0);
    glLineWidth(2);
    glBegin (GL_LINE_STRIP);
        for (k = 0; k <= 50; k++)
            //glPushMatrix();
            glEvalCoord1f (GLfloat (k) / 50.0);
            //glTranslatef(100,0,0);
            //glPopMatrix();
    glEnd ( );

    glColor3f (1.0, 0.0, 1.0);
    glPointSize (3);          
    glBegin (GL_POINTS);             
        for (k = 0; k < 4; k++)
        {
            glVertex3fv (&ctrlPts [k][0]);
        }
    glEnd ( );

    glDisable (GL_MAP1_VERTEX_3);    
    

    
    glMap1f (GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ctrlPts2);
    glEnable (GL_MAP1_VERTEX_3);
    
    glColor3f (1.0, 0.0, 0.0);
    glLineWidth(2);
    glBegin (GL_LINE_STRIP);
        for (k = 0; k <= 50; k++)
            //glPushMatrix();
            glEvalCoord1f (GLfloat (k) / 50.0);
            //glTranslatef(100,0,0);
            //glPopMatrix();
    glEnd ( );

    glColor3f (1.0, 0.0, 1.0);
    glPointSize (3);          
    glBegin (GL_POINTS);             
        for (k = 0; k < 4; k++)
        {
            glVertex3fv (&ctrlPts2 [k][0]);
        }
    glEnd ( );

    glDisable (GL_MAP1_VERTEX_3);
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}
