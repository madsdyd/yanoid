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
#ifndef __MORE_ENTITIES_HH__
#define __MORE_ENTITIES_HH__

#include <string>
#include <SDL/SDL_mixer.h>

class TEntity;
class TPixmapEntity;

/* This is a brick... */
class TBrick : public TPixmapEntity {
protected:
  int hitnum;
public:
  TBrick(int x_, int y_, string pixmap, string hitfunction, int _hitnum = 1);
  virtual ~TBrick();
  virtual void OnCollision(TEntity& other, Uint32 currenttime=0);
};


/* A THole is an entity that kills balls.
   It is not visible. */
class THole : public TEntity {
public:
  THole(double x_, double y_, int w_, int h_);
  virtual ~THole();
  virtual void OnCollision(TEntity& other, Uint32 currenttime=0);
  void Render(SDL_Surface * surface) {};
};

/* Used for cheating */
extern bool hole_bounces;

#endif
