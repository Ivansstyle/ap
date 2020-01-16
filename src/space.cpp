#include "space.h"
#include <iostream>
//#include <cmath>

Space::Space() : m_size(0,0,0), m_origin(0,0,-1.2f), m_resolution(2)
{
}

Space::~Space()
{
}

void Space::init()
{
  normal.ceiling = Vec4(0,-1,0);
  normal.floor =   Vec4(0,1,0);
  normal.front =   Vec4(0,0,-1);
  normal.back =    Vec4(0,0,1);
  normal.left =    Vec4(1,0,0);
  normal.right =   Vec4(-1,0,0);
  normal.zero =    Vec4(0,0,0);

  //Default gravity
  m_gravity = Vec4(0,-0.0000005f,0);

  //FlowSpace initialisation
  flowspace.init(m_size, m_resolution, m_origin);
}


Vec4 Space::GetSize()
{
    return m_size;
}

void Space::SetSize(Vec4 _size)
{
    m_size = _size;
}

Vec4 Space::GetOrigin() const
{
  return m_origin;
}

void Space::SetOrigin(Vec4 _origin)
{
  m_origin = _origin;
}

Vec4 Space::GetGravity()
{
  return m_gravity;
}

void Space::SetGravity(Vec4 _gravity)
{
  m_gravity += _gravity;
}
void Space::resolution(int _res)
{
    m_resolution  = _res;
}

Vec4 Space::isInSpace(Vec4 _ppos)
{
  Vec4 _size_in_space;
  if (m_size.m_z > 0)
    {
    _size_in_space = Vec4
        (m_size.m_x,
         m_size.m_y,
         -m_size.m_z);
    }
  else
    {
    _size_in_space = Vec4
        (m_size.m_x,
         m_size.m_y,
         m_size.m_z);
    }
  GLfloat w = _size_in_space.m_x / 2.0f;
  GLfloat h = _size_in_space.m_y / 2.0f;
  GLfloat d = _size_in_space.m_z / 2.0f;

  // The problem was that I declared size.m_z as a positive number,
  // spent 2 hours fixing this *&^$*%£*^%"!

    if      (_ppos.m_x >  (w + m_origin.m_x)) {return normal.right;}
    else if (_ppos.m_y >  (h + m_origin.m_y)) {return normal.ceiling;}
    else if (_ppos.m_z <  (d + m_origin.m_z)){return normal.front;}
    else if (_ppos.m_x < (-w + m_origin.m_x)){return normal.left;}
    else if (_ppos.m_y < (-h + m_origin.m_y)){return normal.floor;}
    else if (_ppos.m_z > (-d + m_origin.m_z)){return normal.back;}
    else return normal.zero;
}

Vec4 Space::SetBackToSpace(Vec4 _ppos, Vec4 _normal)
{
  Vec4 _size_in_space;
  if (m_size.m_z > 0)
    {
    _size_in_space = Vec4
        (m_size.m_x,
         m_size.m_y,
         -m_size.m_z);
    }
  else
    {
    _size_in_space = Vec4
        (m_size.m_x,
         m_size.m_y,
         m_size.m_z);
    }
  GLfloat w = _size_in_space.m_x / 2.0f;
  GLfloat h = _size_in_space.m_y / 2.0f;
  GLfloat d = _size_in_space.m_z / 2.0f;

  if      (_normal == normal.right)  {return Vec4( w - 0.0002f + m_origin.m_x, _ppos.m_y, _ppos.m_z);}
  else if (_normal == normal.left)   {return Vec4(-w + 0.0002f + m_origin.m_x, _ppos.m_y, _ppos.m_z);}
  else if (_normal == normal.ceiling){return Vec4(_ppos.m_x, h - 0.0002f + m_origin.m_y,  _ppos.m_z);}
  else if (_normal == normal.floor)  {return Vec4(_ppos.m_x,-h + 0.0002f + m_origin.m_y,  _ppos.m_z);}
  else if (_normal == normal.back)   {return Vec4(_ppos.m_x, _ppos.m_y, -d - 0.0002f + m_origin.m_z);}
  else if (_normal == normal.front)  {return Vec4(_ppos.m_x, _ppos.m_y,  d + 0.0002f + m_origin.m_z);}
  else return _ppos;
}

void Space::testDrawSpace()
{
  glPushMatrix();
  glColor3f(0.9f,0.9f,0.9f);
  glLineWidth(1.5f);

  glTranslatef(m_origin.m_x, m_origin.m_y, m_origin.m_z);
  GLFunctions::WiredCube(m_size.m_x, m_size.m_y, m_size.m_z);

  glLineWidth(3.0f);

  glPopMatrix();
}


// TRASHBIN
/*
 *
 *
  Vec4 _size = m_size + m_origin;
  GLfloat w = _size.m_x / 2.0f;
  GLfloat h = _size.m_y / 2.0f;
  GLfloat d = _size.m_z / 2.0f;

  std::cout<<"m_origin.m_z in Space::isInSpace = "<<m_origin.m_z<<std::endl;
  std::cout<<"m_size.m_z in Space::isInSpace = "<<m_size.m_z<<std::endl;
  std::cout<<"_size.m_z in Space::isInSpace = "<<_size_in_space.m_z<<std::endl;


  glColor3f(1.0f,1.0f,1.0f);
  glBegin(GL_LINE_LOOP);

  glVertex3f(m_size.m_x, m_size.m_y, -m_size.m_z);
  glVertex3f(-m_size.m_x, m_size.m_y, -m_size.m_z);
  glVertex3f(-m_size.m_x, -m_size.m_y, -m_size.m_z);
  glVertex3f(m_size.m_x, -m_size.m_y, -m_size.m_z);
  glVertex3f(m_size.m_x, -m_size.m_y, m_size.m_z);
  glVertex3f(m_size.m_x, m_size.m_y, m_size.m_z);
  glVertex3f(-m_size.m_x, m_size.m_y, m_size.m_z);

  glVertex3f(-m_size.m_x, -m_size.m_y, m_size.m_z);
  glVertex3f(-m_size.m_x, -m_size.m_y, -m_size.m_z);
  glVertex3f(-m_size.m_x, m_size.m_y, -m_size.m_z);
  glVertex3f(-m_size.m_x, m_size.m_y, m_size.m_z);
  glVertex3f(-m_size.m_x, -m_size.m_y, m_size.m_z);
  glVertex3f(m_size.m_x, -m_size.m_y, m_size.m_z);
  glVertex3f(m_size.m_x, -m_size.m_y, -m_size.m_z);
  glVertex3f(m_size.m_x, m_size.m_y, -m_size.m_z);

  glVertex3f(m_size.m_x, m_size.m_y, m_size.m_z);
  glEnd(); //GL_LINE_LOOP

 std::cout<<"d in Space::isInSpace = "<<d<<std::endl;

*/
