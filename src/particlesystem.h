#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#define MAX_PARTICLES 2^13

#include "particle.h"
#include "controls.h"
#include "controlsphere.h"

#ifdef __APPLE__ //OpenGL for MacOsX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#endif


#include "particle.h"
#include "space.h"
#include "controlsphere.h"
#include "controls.h"

#include <stdlib.h>
#include <vector>


class ParticleSystem
{
public:

  Controls  controls;
  bool      TriggerTick;
  double    TriggerSpeed;

  ParticleSystem();
  ~ParticleSystem();

 void init();
 void update();
 void draw();
 void resize(int w, int h);

 void CreateParticles();
 void ParticleUpdate();
 void SearchForFlows(Vec4 _pos);

 void TimingUpdate();
 bool ParticleTriggerTick();

 bool isInit() const;

 void takeControl(SDL_Event* _e);

private:

  std::vector<Particle> m_particles;
  Space                 space;
  ControlSphere         controlsphere;


  float                 emitter_out_velocity;
  float                 emitter_spinning_speed;
  float                 point_size_factor;

  bool m_draw_space;
  bool m_draw_flows;
  bool m_delete_particles;
  int m_flow_behavior;

  void ParticleDraw();
  GLfloat mod(GLfloat _x);

protected:

 bool m_isInit;


 /// The time since the object was initialised, which is used for animation purposes
 double m_startTime;

 /// A member that is updated when update() is called indicating the elapsed time
 double m_elapsedTime;


};

#endif // PARTICLESYSTEM_H

/* TRASHBIN


*/
