    #ifndef FLOW_H
#define FLOW_H
#include "controlsphere.h"
#include "Vec4.h"


#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#endif

#ifdef __linux__
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#define BEHAVIOR_DEF 0
#define BEHAVIOR_SUCK 1
#define BEHAVIOR_BLOW 2


typedef struct FlowID{
 int row;
 int col;
 int seg;
  bool operator ==(const FlowID &_rhs) const;
}FlowID;

typedef struct FlowVectors{ //L - left,  R - Right, T - top,
                            //D - Down, F - front, B - back.
  Vec4 ltf;
  Vec4 rtf;
  Vec4 ldf;             //Execution sequence: 1)from L to R                         //
  Vec4 rdf;             //                    2)from T to D
  Vec4 ltb;             //                    3)from F to B
  Vec4 rtb;
  Vec4 ldb;
  Vec4 rdb;

  bool operator ==(const FlowVectors &_rhs) const;
  void operator +=(const FlowVectors &_rhs) const;
  void operator -=(const FlowVectors &_rhs) const;


}FlowVectors;

class Flow
{
public:
  Flow();
  //~Flow();

  int behaviour;

  void   SetFlowID(int _r, int _c, int _s);
  FlowID GetFlowID();

  void update();
  void draw();
  void drawVectors();
  void suck();
  void blow();

  void SetPos(Vec4 _pos);
  Vec4 GetPos();

  float GetSize();
  void SetSize(float _size);

  Vec4 CalculateParticleVector(Vec4 _ppos);

  void SetFlowVecPos(float _size);
  void GetSpherePtr(ControlSphere* _ptr);
  void InteractWithSphere();

  //Interaction with "friend" flows
  void SetNullFriends(int _r, int _c, int _s,
                      int _rmax, int _cmax, int _smax);
  void isMyFriend(Flow* _flowPtr);
  void SetFriendVecFromPtr(FlowVectors* _fvecs);
  void SetFriendVec(FlowVectors _fvecs, Flow* _friend);
  void AddFriendVecFromPtr(FlowVectors *_fvecs);
  FlowVectors* fVec(); // returns a pointer to Flow FlowVector struct

  void SelfEquilibrium();
  void FriendsEquilibrium();

  ///
  /// \brief CalulateEquilibrium
  /// \param[in] _m
  /// \param[] _f
  /// \return
  ///
  Vec4 CalulateEquilibrium(Vec4 _m, Vec4 _f);

  void Decoy();


private:


  struct FlowFriends{
    Flow* l;
    Flow* r;
    Flow* t;
    Flow* b;
    Flow* f;
    Flow* d;
  };

  float           m_fsize;
  float           m_sphere_attraction_factor;
  GLfloat         m_decoy;
  float           m_particle_interaction_factor;
  FlowID          m_id;
  Vec4            m_position;
  FlowFriends     m_friend;
  FlowVectors     fvec;
  FlowVectors     fvec_p;
  ControlSphere*  controlSphere;
  float           m_equilibrium_factor;
  float           m_friends_equilibrium_factor;
  float           m__sphere_interaction_r;
  float           m_suck_blow_force;


};

#endif // FLOW_H

/* TRASHBIN
 *
 *
 *
 *
 */
