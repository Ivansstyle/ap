#include "flowspace.h"
#include "GLFunctions.h"
#include <vector>
#include <cmath>
#include <iostream>


FlowSpace::FlowSpace() : m_spherePtr(NULL)
{

}

FlowSpace::~FlowSpace()
{

}

void FlowSpace::init(Vec4 _size, int _resolution, Vec4 _origin)
{
  CalculateFlowSize(_resolution);

  // main flow creation sequence
  createFlows(_size, _origin);

//  //Test middle flow
//  Flow f = Flow();
//  f.SetPos(Vec4(0.0f,0.0f,-1.0f));
//  f.SetFlowVecPos(m_flowsize);
//  f.GetSpherePtr(m_spherePtr);
//  m_flows.push_back(f);

}
void FlowSpace::update(int _behaviour)
{

  for (auto& i : m_flows)
  {
      i.behaviour = _behaviour;
      i.update();

  }
}

void FlowSpace::drawFlows()
{
  for (auto& i : m_flows)
  {
      i.draw();
  }
}


void FlowSpace::createFlows(const Vec4 _size,Vec4 _origin)
{
  Vec4 _size_in_space;
  if (_size.m_z > 0)
    {
    _size_in_space = Vec4
        (_size.m_x,
         _size.m_y,
         -_size.m_z);
    }
  else
    {
    _size_in_space = Vec4
        (_size.m_x,
         _size.m_y,
         _size.m_z);
    }
  GLfloat w = _size_in_space.m_x / 2.0f;
  GLfloat h = _size_in_space.m_y / 2.0f;
  GLfloat d = _size_in_space.m_z / 2.0f;

  int r_max = int(ceil(float(_size.m_x / (2.0f * m_flowsize)))) ; //Rows
  int c_max = int(ceil(float(_size.m_y / (2.0f * m_flowsize)))) ; //Colls
  int s_max = int(ceil(float(_size.m_z / (2.0f * m_flowsize)))) ; //Segmemnts


  m_flows.reserve(r_max*c_max*s_max);

    for (int s = 0; s < s_max; ++s)
    {
      for (int c = 0; c < c_max; ++c)
      {
        for (int r = 0; r < r_max; ++r)
        {
          Flow f = Flow();
          f.SetFlowID(r, c, s);
          f.SetPos(Vec4( -w + m_flowsize + ((2.0f * m_flowsize) * r),
                         h - m_flowsize - ((2.0f * m_flowsize) * c),
                        -d - m_flowsize - ((2.0f * m_flowsize) * s))
                        + _origin);

          f.SetFlowVecPos(m_flowsize);
          f.GetSpherePtr(m_spherePtr);
          f.SetNullFriends(r,c,s,r_max,c_max,s_max);
          m_flows.push_back(f);
        }
      }
    }

    for (auto& i : m_flows)
    {
      for (auto& j : m_flows)
      {
        i.isMyFriend(&j);
      }
  }
}

void FlowSpace::CalculateFlowSize(int _resolution)
{
    if      (_resolution == 1) m_flowsize = 0.1f;
    else if (_resolution == 2) m_flowsize = 0.07f;
    else if (_resolution == 3) m_flowsize = 0.05f;
    else if (_resolution == 4) m_flowsize = 0.045f;
}

void FlowSpace::SetSpherePtr(ControlSphere *_ptr)
{
  m_spherePtr = _ptr;
}

void FlowSpace::FlowSearch(Particle *_pPtr)
{
  for (auto& i : m_flows)
  {
     if( _pPtr->isInFlow(&i))  break;
  }

}



/* TRASHBIN
 *
 *pointer check
 *  std::cout<<"pointer at sphere is "<<m_spherePtr<<std::endl;
    std::cout<< " sphere pos_m_x  is "<<m_spherePtr->GetPos().m_x<< std::endl;
 *
 *
 *
 std::cout<< " #### test '#### m_flowsize = "<<m_flowsize<<std::endl;
  std::cout<< " #### test '#### _size.m_x = "<<_size.m_x<<std::endl;
  std::cout<< " #### test '#### r_Max = "<<r_max<<std::endl;
  std::cout<< " #### test '#### c_Max = "<<c_max<<std::endl;
  std::cout<< " #### test '#### s_Max = "<<s_max<<std::endl;
 *
 * std::cout<< " #### test '#### c = "<<c<<std::endl;
 */
