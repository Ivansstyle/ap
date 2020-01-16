#ifndef MAT4_H__
#define MAT4_H__

#ifdef __linux__
  #include <GL/gl.h>
#endif
#ifdef __APPLE__
  #include <OpenGL/gl.h>
#endif

//#include"Vec4.h"
class Vec4;

class Mat4
{
public:
  Mat4(GLfloat _s = 1.0f);
  Mat4(const Mat4 &r) = default;
  void operator *= (const Mat4 &_rhs);
  Mat4 operator * (const Mat4 &_rhs) const;
  void identity();
  void loadModelView() const;
  void loadProjection() const;
  const Mat4 & transpose();

  union
  {
    GLfloat m_m[4][4];
    GLfloat m_openGL[16];
    struct
    {
      GLfloat m_00;
      GLfloat m_01;
      GLfloat m_02;
      GLfloat m_03;

      GLfloat m_10;
      GLfloat m_11;
      GLfloat m_12;
      GLfloat m_13;

      GLfloat m_20;
      GLfloat m_21;
      GLfloat m_22;
      GLfloat m_23;

      GLfloat m_30;
      GLfloat m_31;
      GLfloat m_32;
      GLfloat m_33;
    };
  };
};

#endif
