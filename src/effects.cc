/*
    Yet Another Arkanoid
    Copyright (C) 2001 Mads Bondo Dydensborg <mads@dydensborg.dk>
    Copyright (C) 2001 Jonas Christian Drewsen <jcd@xspect.dk>
    Copyright (C) 2001 contributers of the yanoid project
    Please see the file "AUTHORS" for a list of contributers

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "effects.hh"

/*
  How long time should the effect last.
 */
void TEffect::setDuration(Uint32 dur) { 
  if (dur > post_hold_time)
    Duration = dur; 
}

/*
  How long after the effect has been played
  should it remain in it's last position.
 */
void TEffect::setPostHoldTime(Uint32 hold) { 
  if (hold < Duration)
    post_hold_time = hold; 
}

void TEffect::update(SDL_Surface * surf, Uint32 currenttime) {

  if (state == STOPPED)
    return;

  if (!begin_time)
    begin_time = currenttime;

  surface = surf;

  if ( (currenttime - begin_time) > Duration) {

    if (!no_erase) {
      blitBackground();
    }else{
      begin_time = currenttime;
      no_erase=false;
    }
    stop();
    begin_time = 0;
    return;

  } else if ( (currenttime - begin_time) > (Duration - post_hold_time)) {
    return;
  }

  if (!no_erase) {
    blitBackground();
  }else{
    begin_time = currenttime;
    no_erase=false;
  } 

  doEffect(currenttime);
}

/*
  A nice litte help function that just runs
  the effect using the update function. But
  looks for a ESC press for aborting.
 */
void TEffect::run(SDL_Surface *Screen) {
  start();
  while(!isStopped()) {

    // Update/render the effect
    update(Screen, SDL_GetTicks());

    SDL_Event event;
    // handle keyevents
    while(SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
	switch (event.key.keysym.sym) {
	  /* The in game menu is bound to escape */
	case SDLK_ESCAPE: {	
	  stop();
	  begin_time = 0;
	  return;
	}
	}
      }
    }
    
    SDL_Flip(Screen);
  }
}
