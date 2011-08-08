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

#include <QGLViewer/qglviewer.h>
#include <QDebug>

struct Vector {
    float x, y, z;
};

#define PI 3.14159265
#define EARTH_RADIUS    6378

class Viewer : public QGLViewer
{
public:
	virtual ~Viewer();
protected :
  virtual void draw();
  virtual void init();
private:
	GLuint		texture;
	GLUquadric	*quadric;
    qglviewer::ManipulatedFrame* light1;

    void drawNames();
    void lonLat2Point(float , float , Vector *, GLdouble);
};
