#include "Mat4.h"
#include "Vec4.h"
#include <cstring>


Mat4::Mat4(GLfloat _s)
{
  memset(&m_m, 0, sizeof(m_m));
  m_00 = _s;
  m_11 = _s;
  m_22 = _s;
  m_33 = _s;
}

void Mat4::identity()
{
  m_00 = 1.0f;
  m_11 = 1.0f;
  m_22 = 1.0f;
  m_33 = 1.0f;
}

void Mat4::loadModelView() const
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // set the above matrix as the default
  glMultMatrixf(m_openGL); // load our current matrix into the model matrix
}

void Mat4::loadProjection() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf(m_openGL);
  glMatrixMode(GL_MODELVIEW);
}
