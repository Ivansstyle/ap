#ifndef SPACE_H
#define SPACE_H

#include "flowspace.h"
#include "Vec4.h"
#include "GLFunctions.h"

#ifdef __APPLE__ //OpenGL for MacOsX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#endif



class Space
{
public:
    FlowSpace flowspace;

    Space();
    ~Space();

    void init();
    void initFlowSpace();
    void testDrawSpace();

    Vec4 GetSize();
    void SetSize(Vec4 _size);

    Vec4 GetOrigin() const;
    void SetOrigin(Vec4 _origin);

    Vec4 GetGravity();
    void SetGravity(Vec4 _gravity);

    void resolution(int _res);

    Vec4 isInSpace(Vec4 _ppos);
    Vec4 SetBackToSpace(Vec4 _ppos, Vec4 _normal);



private:

    struct Normal{
      Vec4      floor;
      Vec4      ceiling;
      Vec4      front;
      Vec4      back;
      Vec4      left;
      Vec4      right;
      Vec4      zero;
    };

    Normal      normal;

    Vec4        m_size;
    Vec4        m_origin;
    Vec4        m_gravity;

    float       m_airresistance;
    int         m_resolution;

};

#endif // SPACE_H
