#include "controls.h"
#include <iostream>


Controls::Controls()
{
  KillControls();



}

void Controls::TakeControls(SDL_Event* _e)
{
  m_e = _e;

  if (m_e->key.state == SDL_RELEASED)
  {
   KillControls();
  }
  if( m_e->type == SDL_TEXTINPUT)
  {

  }

  if (m_e->type == SDL_KEYDOWN)
  {
    if (m_e->key.keysym.sym == SDLK_r) { event.r = true; *delete_particles = true;}
    if (m_e->key.keysym.sym == SDLK_o) { event.blow = true; *flow_behavior = 2;}
    if (m_e->key.keysym.sym == SDLK_i) { event.suck = true; *flow_behavior = 1;}
    if (m_e->key.keysym.sym == SDLK_f) { event.f = true; *draw_flows = !*draw_flows; }
    if (m_e->key.keysym.sym == SDLK_s) { event.s = true; *draw_space = !*draw_space; }
    if (m_e->key.keysym.sym == SDLK_LEFT) { event.left = true;}
    if (m_e->key.keysym.sym == SDLK_RIGHT) { event.right = true;}
    if (m_e->key.keysym.sym == SDLK_UP) { event.up = true;}
    if (m_e->key.keysym.sym == SDLK_DOWN) { event.down = true;}
    if (m_e->key.keysym.sym == SDLK_SPACE) { event.space = true;}
    if (m_e->key.keysym.sym == SDLK_LCTRL) { event.ctrl = true;}
    if (m_e->key.keysym.sym == SDLK_LSHIFT ) {event.shift = true;}
  }
  if (m_e->type == SDL_KEYUP)
  {
      if (m_e->key.keysym.sym == SDLK_r) { event.r = false; *delete_particles = false;}
      if (m_e->key.keysym.sym == SDLK_o) { event.blow =false; *flow_behavior = 0;}
      if (m_e->key.keysym.sym == SDLK_i) { event.suck = false; *flow_behavior = 0;}
      if (m_e->key.keysym.sym == SDLK_f) { if (event.f)
          {

          }event.f = false; }
      if (m_e->key.keysym.sym == SDLK_s) { if (event.s)
          {
              *draw_space = !*draw_space;
          }event.s = false;}
    if (m_e->key.keysym.sym == SDLK_LEFT) { event.left = false;}
    if (m_e->key.keysym.sym == SDLK_RIGHT) { event.right = false;}
    if (m_e->key.keysym.sym == SDLK_UP) { event.up = false;}
    if (m_e->key.keysym.sym == SDLK_DOWN) { event.down = false;}
    if (m_e->key.keysym.sym == SDLK_SPACE) { event.space = false;}
    if (m_e->key.keysym.sym == SDLK_LCTRL) { event.ctrl = false;}
    if (m_e->key.keysym.sym == SDLK_LSHIFT ) {event.shift = false;}
  }
    if( m_e->type == SDL_MOUSEBUTTONDOWN)
    {
          SDL_GetMouseState(&event.mouse_x, &event.mouse_y);
    }

}


void Controls::KillControls()
{

    event.shift = false;
    event.ctrl = false;
    event.down = false;
    event.left = false;
    event.right = false;
    event.up = false;
    event.space = false;
    event.lmouse = false;
    event.rmouse = false;
    event.r = false;
    event.blow = false;
    event.suck = false;
    event.f = false;
    event.s = false;

      event.letter = 0;

}

Event Controls::GiveControls()
{
  return event;
}
char Controls::GetInput()
{
    if(m_e->type == SDL_TEXTINPUT)
    return m_e->text.text[0];
    else return NULL;

}
/*    TRASHBIN
 *
 * from Controls::KillControls()
 *
  if (m_e->key.keysym.sym != SDLK_LEFT) { event.left = false;}
  if (m_e->key.keysym.sym != SDLK_RIGHT) { event.right = false;}
  if (m_e->key.keysym.sym != SDLK_UP) { event.up = false;}
  if (m_e->key.keysym.sym != SDLK_DOWN) { event.down = false;}
  if (m_e->key.keysym.sym != SDLK_SPACE) { event.space = false;}
  if (m_e->key.keysym.sym != SDLK_LCTRL) { event.ctrl = false;}
  if (m_e->key.keysym.sym != SDLK_LSHIFT ) {event.shift = false;}

 *
 */
