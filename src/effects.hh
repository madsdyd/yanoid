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

#ifndef __EFFECTS__
#define __EFFECTS__

#include <SDL/SDL.h>
#include "point.hh"
#include <iostream>

class TEffect {
protected:
  TPoint Location;
  Uint32 last_update;
  Uint32 begin_time;
  Uint32 post_hold_time;
  bool cleanup;
  typedef enum State { RUNNING, PAUSED, STOPPED } State;
  State state;
  Uint32 Duration;
  bool no_erase;
  SDL_Surface * background;
  virtual void saveBackground() = 0;
  virtual void blitBackground() = 0;
public:
  TEffect() : Location(0,0), last_update(0),begin_time(0),post_hold_time(0),
	      cleanup(false), Duration(1000),no_erase(true) {}
  virtual ~TEffect() {}
  void start() { state = RUNNING; } 
  void pause() { state = PAUSED; }
  void stop() { state = STOPPED; }
  bool isStopped() { return state == STOPPED; } 
  void setLocation(const TPoint& location) { Location = location; }
  TPoint getLocation() { return Location; }
  void setDuration(Uint32 dur);
  void setPostHoldTime(Uint32 hold);
  Uint32 getDuration() { return Duration; }
  void update(Uint32 currenttime);
  virtual void doEffect(Uint32 currenttime) = 0;
  void run(SDL_Surface* screen);
};

#endif

