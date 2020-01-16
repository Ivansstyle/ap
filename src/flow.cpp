#include "flow.h"
#include "GLFunctions.h"
#include <cmath>
#include <iostream>

//#define EQUILIBRIUM

Flow::Flow() : m_sphere_attraction_factor(5.5f) , m_decoy(0.993f),
               m_particle_interaction_factor(0.002f), controlSphere(NULL),
               m_equilibrium_factor(0.1f), m__sphere_interaction_r(0.18f),
               m_friends_equilibrium_factor(),
               m_suck_blow_force(0.0002f)
{
  fvec.ltf = Vec4(0,0,0);
  fvec.rtf = Vec4(0,0,0);

  fvec.ldf = Vec4(0,0,0);
  fvec.rdf = Vec4(0,0,0);


  fvec.ltb = Vec4(0,0,0);
  fvec.rtb = Vec4(0,0,0);

  fvec.ldb = Vec4(0,0,0);
  fvec.rdb = Vec4(0,0,0);

  m_friend.l = this;
  m_friend.r = this;
  m_friend.t = this;
  m_friend.d = this;
  m_friend.f = this;
  m_friend.b = this;
  m_friend.f = this;
  m_friend.b = this;
}


Vec4 Flow::CalculateParticleVector(Vec4 _ppos)
{

  // Calculate Resoluting Power times Interaction Factor
  Vec4 respow = ( fvec.ltf * (fvec.ltf + fvec_p.ltf).dist(_ppos) * m_particle_interaction_factor +
                  fvec.rtf * (fvec.rtf + fvec_p.rtf).dist(_ppos) * m_particle_interaction_factor +
                  fvec.ldf * (fvec.ldf + fvec_p.ldf).dist(_ppos) * m_particle_interaction_factor +
                  fvec.rdf * (fvec.rdf + fvec_p.rdf).dist(_ppos) * m_particle_interaction_factor +
                  fvec.ltb * (fvec.ltb + fvec_p.ltb).dist(_ppos) * m_particle_interaction_factor +
                  fvec.rtb * (fvec.rtb + fvec_p.rtb).dist(_ppos) * m_particle_interaction_factor +
                  fvec.ldb * (fvec.ldb + fvec_p.ldb).dist(_ppos) * m_particle_interaction_factor +
                  fvec.rdb * (fvec.rdb + fvec_p.rdb).dist(_ppos) * m_particle_interaction_factor
                 );
  respow = Vec4(respow.m_x / 8.0f,
                respow.m_y / 8.0f,
                respow.m_z /8.0f);
  return respow;
}


void Flow::update()
{
  if (behaviour == BEHAVIOR_SUCK) suck();
  if (behaviour == BEHAVIOR_BLOW) blow();

  InteractWithSphere();

  #ifdef EQUILIBRIUM

  FriendsEquilibrium();

  SelfEquilibrium();

  #endif

  Decoy();


}

void Flow::blow()
{
    fvec.ltf += (((fvec.ltf + fvec_p.ltf + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.ltf + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.rtf += (((fvec.rtf + fvec_p.rtf + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.rtf + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.ldf += (((fvec.ldf + fvec_p.ldf + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.ldf + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.rdf += (((fvec.rdf + fvec_p.rdf + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.rdf + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.ltb += (((fvec.ltb + fvec_p.ltb + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.ltb + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.rtb += (((fvec.rtb + fvec_p.rtb + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.rtb + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.ldb += (((fvec.ldb + fvec_p.ldb + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.ldb + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
    fvec.rdb += (((fvec.rdb + fvec_p.rdb + m_position) - controlSphere->GetPos()))* (1.0f/((fvec_p.rdb + m_position).dist(controlSphere->GetPos()))) * m_suck_blow_force;
}

void Flow::suck()
{
    fvec.ltf += ((controlSphere->GetPos() - (fvec.ltf + fvec_p.ltf + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.ltf + m_position)))) * m_suck_blow_force;
    fvec.rtf += ((controlSphere->GetPos() - (fvec.rtf + fvec_p.rtf + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.rtf + m_position)))) * m_suck_blow_force;
    fvec.ldf += ((controlSphere->GetPos() - (fvec.ldf + fvec_p.ldf + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.ldf + m_position)))) * m_suck_blow_force;
    fvec.rdf += ((controlSphere->GetPos() - (fvec.rdf + fvec_p.rdf + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.rdf + m_position)))) * m_suck_blow_force;
    fvec.ltb += ((controlSphere->GetPos() - (fvec.ltb + fvec_p.ltb + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.ltb + m_position)))) * m_suck_blow_force;
    fvec.rtb += ((controlSphere->GetPos() - (fvec.rtb + fvec_p.rtb + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.rtb + m_position)))) * m_suck_blow_force;
    fvec.ldb += ((controlSphere->GetPos() - (fvec.ldb + fvec_p.ldb + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.ldb + m_position)))) * m_suck_blow_force;
    fvec.rdb += ((controlSphere->GetPos() - (fvec.rdb + fvec_p.rdb + m_position))* (1.0f/(controlSphere->GetPos().dist(fvec_p.rdb + m_position)))) * m_suck_blow_force;
}





void Flow::SetFriendVec(FlowVectors _fvecs, Flow *_friend)
{
  _friend->SetFriendVecFromPtr(&_fvecs);

}

void Flow::SetFriendVecFromPtr(FlowVectors *_fvecs)
{
  fvec.ltf = _fvecs->ltf;
  fvec.rtf = _fvecs->rtf;
  fvec.ldf = _fvecs->ldf;
  fvec.rdf = _fvecs->rdf;
  fvec.ltb = _fvecs->ltb;
  fvec.rtb = _fvecs->rtb;
  fvec.ldb = _fvecs->ldb;
  fvec.rdb = _fvecs->rdb;
}

void Flow::AddFriendVecFromPtr(FlowVectors *_fvecs)
{
  fvec.ltf = _fvecs->ltf;
  fvec.rtf = _fvecs->rtf;
  fvec.ldf = _fvecs->ldf;
  fvec.rdf = _fvecs->rdf;
  fvec.ltb = _fvecs->ltb;
  fvec.rtb = _fvecs->rtb;
  fvec.ldb = _fvecs->ldb;
  fvec.rdb = _fvecs->rdb;
}

FlowVectors* Flow::fVec()
{
  return &fvec;
}

void Flow::draw()
{
  glPushMatrix();

      glTranslatef(m_position.m_x, m_position.m_y, m_position.m_z);

      drawVectors();

      glColor4f(0.2f,0.2f,1.0f,0.5f);
      glLineWidth(0.1f);

      GLFunctions::WiredCube(m_fsize*2.0f, m_fsize*2.0f, m_fsize*2.0f);
      glLineWidth(3.0f);

      glColor4f(1.0f,0.1f,0.0f,0.8f);
      glPointSize(2.0f);

      glBegin(GL_POINTS);
        glVertex3f(m_position.m_x,m_position.m_y,m_position.m_z);
        glColor4f(1.0f,1.0f,1.0f,1.0f);
      glEnd(); //glPoint

  glPopMatrix();
}

void Flow::InteractWithSphere()
{
  float interaction_distance = m_fsize + controlSphere->GetR() + m__sphere_interaction_r; // it works
                                                  // but returns number that is -0.2f

  if (m_position.dist(controlSphere->GetPos()) < interaction_distance)
  {
    fvec.ltf += controlSphere->GetVel() *
(2.0f/((fvec.ltf + fvec_p.ltf).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.rtf += controlSphere->GetVel() *
(2.0f/((fvec.rtf + fvec_p.rtf).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.ldf += controlSphere->GetVel() *
(2.0f/((fvec.ldf + fvec_p.ldf).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.rdf += controlSphere->GetVel() *
(2.0f/((fvec.rdf + fvec_p.rdf).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.ltb += controlSphere->GetVel() *
(2.0f/((fvec.ltb + fvec_p.ltb).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.rtb += controlSphere->GetVel() *
(2.0f/((fvec.rtb + fvec_p.rtb).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.ldb += controlSphere->GetVel() *
(2.0f/((fvec.ldb + fvec_p.ldb).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));


    fvec.rdb += controlSphere->GetVel() *
(2.0f/((fvec.rdb + fvec_p.rdb).dist(controlSphere->GetPos()) *
                                  m_sphere_attraction_factor));
  }
}

void Flow::Decoy()
{
  fvec.ltf = Vec4(fvec.ltf.m_x * m_decoy, fvec.ltf.m_y * m_decoy, fvec.ltf.m_z * m_decoy);
  fvec.rtf = Vec4(fvec.rtf.m_x * m_decoy, fvec.rtf.m_y * m_decoy, fvec.rtf.m_z * m_decoy);
  fvec.ldf = Vec4(fvec.ldf.m_x * m_decoy, fvec.ldf.m_y * m_decoy, fvec.ldf.m_z * m_decoy);
  fvec.rdf = Vec4(fvec.rdf.m_x * m_decoy, fvec.rdf.m_y * m_decoy, fvec.rdf.m_z * m_decoy);
  fvec.ltb = Vec4(fvec.ltb.m_x * m_decoy, fvec.ltb.m_y * m_decoy, fvec.ltb.m_z * m_decoy);
  fvec.rtb = Vec4(fvec.rtb.m_x * m_decoy, fvec.rtb.m_y * m_decoy, fvec.rtb.m_z * m_decoy);
  fvec.ldb = Vec4(fvec.ldb.m_x * m_decoy, fvec.ldb.m_y * m_decoy, fvec.ldb.m_z * m_decoy);
  fvec.rdb = Vec4(fvec.rdb.m_x * m_decoy, fvec.rdb.m_y * m_decoy, fvec.rdb.m_z * m_decoy);
}

void Flow::drawVectors()
{
  glLineWidth(1.0f);
  glColor4f(1.0f,1.0f,0.1f,1.0f);
  glBegin(GL_LINES);

  Vec4 o, x;

  o =  fvec_p.ltf;
  x =  fvec_p.ltf + fvec.ltf;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);

  o =  fvec_p.rtf;
  x =  fvec_p.rtf + fvec.rtf;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);


  o =  fvec_p.ldf;
  x =  fvec_p.ldf + fvec.ldf;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);


  o =  fvec_p.rdf;
  x =  fvec_p.rdf + fvec.rdf;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);


  o =  fvec_p.ltb;
  x =  fvec_p.ltb + fvec.ltb;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);


  o =  fvec_p.rtb;
  x =  fvec_p.rtb + fvec.rtb;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);

  o =  fvec_p.ldb;
  x =  fvec_p.ldb + fvec.ldb;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);


  o =  fvec_p.rdb;
  x =  fvec_p.rdb + fvec.rdb;
  glVertex3f(o.m_x, o.m_y, o.m_z);
  glVertex3f(x.m_x, x.m_y, x.m_z);
  glEnd(); //GL_LINES
}

void Flow::SetFlowID(int _r, int _c, int _s)
{
  m_id.col = _c;
  m_id.row = _r;
  m_id.seg = _s;
}

void Flow::SetFlowVecPos(float _size)
{
  fvec_p.ltf = Vec4 (-1, 1, 1) * _size;
  fvec_p.rtf = Vec4 ( 1, 1, 1) * _size;
  fvec_p.ldf = Vec4 (-1,-1, 1) * _size;
  fvec_p.rdf = Vec4 ( 1,-1, 1) * _size;
  fvec_p.ltb = Vec4 (-1, 1,-1) * _size;
  fvec_p.rtb = Vec4 ( 1, 1,-1) * _size;
  fvec_p.ldb = Vec4 (-1,-1,-1) * _size;
  fvec_p.rdb = Vec4 ( 1,-1,-1) * _size;
  m_fsize = _size;
}

void Flow::SetNullFriends(int _r, int _c, int _s, int _rmax, int _cmax, int _smax)
{

  if (_r == 0)        {m_friend.l = NULL;}
  if (_r == _rmax -1) {m_friend.r = NULL;}
  if (_c == 0)        {m_friend.t = NULL;}
  if (_c == _cmax -1) {m_friend.d = NULL;}
  if (_s ==  0)       {m_friend.f = NULL;}
  if (_s == _smax -1) {m_friend.b = NULL;}
}

void Flow::isMyFriend(Flow *_flowPtr)
{
  if (_flowPtr->GetFlowID() == m_id)
  {
    return;
  }
  if  (_flowPtr->GetFlowID().row + 1 == m_id.row &&
            _flowPtr->GetFlowID().col == m_id.col &&
            _flowPtr->GetFlowID().seg == m_id.seg &&
            m_friend.l != NULL)
  {
    m_friend.l = _flowPtr; //Left friend
  }

  else if (_flowPtr->GetFlowID().row - 1 == m_id.row &&
           _flowPtr->GetFlowID().col == m_id.col &&
           _flowPtr->GetFlowID().seg == m_id.seg &&
           m_friend.r != NULL)
  {
    m_friend.r = _flowPtr; //Right friend
  }

  if (_flowPtr->GetFlowID().col + 1 == m_id.col &&
           _flowPtr->GetFlowID().row == m_id.row &&
           _flowPtr->GetFlowID().seg == m_id.seg &&
           m_friend.t != NULL)
  {
    m_friend.t = _flowPtr; //Top friend
  }

  else if (_flowPtr->GetFlowID().col - 1 == m_id.col &&
           _flowPtr->GetFlowID().row == m_id.row &&
           _flowPtr->GetFlowID().seg == m_id.seg &&
           m_friend.d != NULL)
  {
    m_friend.d = _flowPtr; //Down friend

  }

  if (_flowPtr->GetFlowID().seg + 1 == m_id.seg &&
           _flowPtr->GetFlowID().col == m_id.col &&
           _flowPtr->GetFlowID().row == m_id.row &&
           m_friend.f != NULL)
  {
    m_friend.f = _flowPtr; //Front friend
  }

  else if (_flowPtr->GetFlowID().seg -1 == m_id.seg &&
           _flowPtr->GetFlowID().col == m_id.col &&
           _flowPtr->GetFlowID().row == m_id.row &&
           m_friend.b != NULL)
  {

    m_friend.b= _flowPtr; //Back friend

  }
  return;
}

void Flow::SelfEquilibrium()
{
  Vec4 sum =  fvec.ltf +
              fvec.rtf +
              fvec.ldf +
              fvec.rdf +
              fvec.ltb +
              fvec.rtb +
              fvec.ldb +
              fvec.rdb;

  sum = Vec4(sum.m_x/8.0f, sum.m_y/8.0f, sum.m_z/8.0f);

  fvec.ltf = Vec4(Vec4(fvec.ltf.m_x + sum.m_x,
                  fvec.ltf.m_y + sum.m_y,
                  fvec.ltf.m_z + sum.m_z)
                * m_equilibrium_factor) + (fvec.rtf * fvec.rtf.dot(sum) * m_equilibrium_factor);
                // m_equilibrium_factor;

  fvec.rtf = Vec4(Vec4(fvec.rtf.m_x + sum.m_x,
                  fvec.rtf.m_y + sum.m_y,
                  fvec.rtf.m_z + sum.m_z)
                * m_equilibrium_factor) + (fvec.rtf * fvec.rtf.dot(sum)* m_equilibrium_factor);
                //* m_equilibrium_factor);

  fvec.ldf = Vec4(Vec4(fvec.ldf.m_x + sum.m_x,
                  fvec.ldf.m_y + sum.m_y,
                  fvec.ldf.m_z + sum.m_z)
                * m_equilibrium_factor) + (fvec.ldf * fvec.ldf.dot(sum)* m_equilibrium_factor);
                 //m_equilibrium_factor);

  fvec.rdf = Vec4(Vec4(fvec.rdf.m_x + sum.m_x,
                  fvec.rdf.m_y + sum.m_y,
                  fvec.rdf.m_z + sum.m_z)
                * m_equilibrium_factor) + (fvec.rdf * fvec.rdf.dot(sum)* m_equilibrium_factor);
               // * m_equilibrium_factor);

  fvec.ltb = Vec4(Vec4(fvec.ltb.m_x + sum.m_x,
                  fvec.ltb.m_y + sum.m_y ,
                  fvec.ltb.m_z + sum.m_z )
                * m_equilibrium_factor) + (fvec.ltf * fvec.ltb.dot(sum)* m_equilibrium_factor);
               // * m_equilibrium_factor);

  fvec.rtb = Vec4(Vec4(fvec.rtb.m_x + sum.m_x,
                  fvec.rtb.m_y + sum.m_y,
                  fvec.rtb.m_z + sum.m_z )
                * m_equilibrium_factor) + (fvec.rtf * fvec.rtb.dot(sum));
//                * m_equilibrium_factor);

  fvec.ldb = Vec4(Vec4(fvec.ldb.m_x + sum.m_x,
                  fvec.ldb.m_y + sum.m_y,
                  fvec.ldb.m_z + sum.m_z)
                * m_equilibrium_factor) + (fvec.ldf / fvec.rdb.dist(sum));
                //* m_equilibrium_factor);

  fvec.rdb = Vec4(Vec4(fvec.rdb.m_x + sum.m_x,
                  fvec.rdb.m_y + sum.m_y,
                  fvec.rdb.m_z + sum.m_z)
                * m_equilibrium_factor) + (fvec.rdf / fvec.ldb.dist(sum));
                //* m_equilibrium_factor);

}

void Flow::FriendsEquilibrium()
{
  if (m_friend.l != NULL)
  {
    std::cout<<"works"<<std::endl;

    FlowVectors* fv = m_friend.l->fVec();
    fvec.ltf += fv->ltf + (Vec4(fv->ltf.m_x, fv->ltf.m_y, fv->ltf.m_z) * m_friends_equilibrium_factor);
    fvec.ldf += fv->ldf + (Vec4(fv->ldf.m_x, fv->ldf.m_y, fv->ldf.m_z) * m_friends_equilibrium_factor);
    fvec.ltb += fv->ltb + (Vec4(fv->ltb.m_x, fv->ltb.m_y, fv->ltb.m_z) * m_friends_equilibrium_factor);
    fvec.ldb += fv->ldb + (Vec4(fv->ldb.m_x, fv->ldb.m_y, fv->ldb.m_z) * m_friends_equilibrium_factor);
  }

  if (m_friend.r != NULL)
  {
    FlowVectors* fv = m_friend.r->fVec();
    fvec.rtf += fv->rtf * m_friends_equilibrium_factor;
    fvec.rdf += fv->rdf * m_friends_equilibrium_factor;
    fvec.rtb += fv->rtb * m_friends_equilibrium_factor;
    fvec.rdb += fv->rdb * m_friends_equilibrium_factor;
  }
  if (m_friend.t != NULL)
  {
    FlowVectors* fv = m_friend.t->fVec();
    fvec.rtf += fv->rtf * m_friends_equilibrium_factor;
    fvec.ltf += fv->ltf * m_friends_equilibrium_factor;
    fvec.rtb += fv->rtb * m_friends_equilibrium_factor;
    fvec.ltb += fv->ltb * m_friends_equilibrium_factor;
  }
  if (m_friend.d != NULL)
  {
    FlowVectors* fv = m_friend.d->fVec();
    fvec.rdf += fv->rdf * m_friends_equilibrium_factor;
    fvec.ldf += fv->ldf * m_friends_equilibrium_factor;
    fvec.rdb += fv->rdb * m_friends_equilibrium_factor;
    fvec.ldb += fv->ldb * m_friends_equilibrium_factor;
  }
  if (m_friend.f != NULL)
  {
    FlowVectors* fv = m_friend.f->fVec();
    fvec.rtf += fv->rtf * m_friends_equilibrium_factor;
    fvec.ltf += fv->ltf * m_friends_equilibrium_factor;
    fvec.rdf += fv->rdf * m_friends_equilibrium_factor;
    fvec.ldf += fv->ldf * m_friends_equilibrium_factor;
  }
  if (m_friend.b != NULL)
  {
    FlowVectors* fv = m_friend.b->fVec();
    fvec.rtb += fv->rtb * m_friends_equilibrium_factor;
    fvec.ltb += fv->ltb * m_friends_equilibrium_factor;
    fvec.rdb += fv->rdb * m_friends_equilibrium_factor;
    fvec.ldb += fv->ldb * m_friends_equilibrium_factor;
  }




}

Vec4 Flow::CalulateEquilibrium(Vec4 _m, Vec4 _f)
{
  _m;
}

void Flow::SetPos(Vec4 _pos)
{
  m_position = _pos;
}

void Flow::GetSpherePtr(ControlSphere *_ptr)
{
  controlSphere = _ptr;
}

Vec4 Flow::GetPos()
{
  return m_position;
}

float Flow::GetSize()
{
  return m_fsize;
}

void Flow::SetSize(float _size)
{
    m_fsize = _size;
}

FlowID Flow::GetFlowID()
{
  return m_id;
}

bool FlowID::operator ==(const FlowID &_rhs) const
{
  if(row == _rhs.row &&
     col == _rhs.col &&
     seg == _rhs.seg)
    return true;
  else return false;
}


/* TRASHBIN
 *
 * std::cout<<"~~~~~~~~~~~## VECTOR L T F _ pos == "<<fvec_p.ltf.m_x<< " "
          <<fvec_p.ltf.m_y<<" "<<fvec_p.ltf.m_z <<std::endl;
 *
 *std::cout<<"interact d ="<<interaction_distance
          <<" controlsphere pos x from here is "<<controlSphere->GetPos().m_x
         <<"2 random vectors dist is = "<<Vec4(1,1,1).dist(Vec4(-1,-1,-1))
        <<"m_position is "<<m_position.m_x<<std::endl;

        std::cout<<"interaction is happening"<< std::endl;

        // std::cout<<"distance calculated is = "<< m_position.dist(controlSphere->GetPos()) << std::endl;
 *
 *
 * //  std::cout<< "addr of m_friend.l after assignement ="<<m_friend.l<<std::endl;

//  std::cout<< "addr of m_friend.r after assignement ="<<m_friend.r<<std::endl;

//  std::cout<< "addr of m_friend.t after assignement ="<<m_friend.t<<std::endl;

//  std::cout<< "addr of m_friend.d after assignement ="<<m_friend.d<<std::endl;

//  std::cout<< "addr of m_friend.f after assignement ="<<m_friend.f<<std::endl;

//  std::cout<< "addr of m_friend.b after assignement ="<<m_friend.b<<std::endl;

//    std::cout<< "HOST adress is = "<<this<<std::endl<<std::endl;
//    std::cout<< "friend D adress is = "<<m_friend.d<<std::endl<<std::endl<<std::endl;
//     std:: cout<<"!!!!!!!! m_friend.b addr!!! = "<<m_friend.b<<std::endl<<std::endl;
//     std::cout<< "flowPtr col = "<<_flowPtr->GetFlowID().col<<
//                 "flowPtr row = "<<_flowPtr->GetFlowID().row<<
//                 "flowPtr seg = "<<_flowPtr->GetFlowID().col<<
//                 "flowPtr addr = "<<_flowPtr<<
//                 "id col = "<<m_id.col<<
//                 "id row = "<<m_id.row<<
//                 "id seg = "<<m_id.seg<<
//                 "self addr= "<<this<<


                 //Down
 */
