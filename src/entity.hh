/*
    Yet Another Arkanoid
    Copyright (C) 2001 Mads Bondo Dydensborg <mads@dydensborg.dk>
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
/* Entities are the basic units that can be manipulated in the game */

#ifndef __ENTITY_HH__
#define __ENTITY_HH__

#include <SDL/SDL.h>
#include <list>

class TEntity {
protected:
  // TODO: Some sort of fancy vector class? 
  int x, y; /* The position of the entity */
  int vel_x, vel_y; /* The velocity of this entity */
  SDL_Surface * currentsurface; /* The current image to blit */
public:
  TEntity(int x_, int y_);
  ~TEntity();
  
  void Update(Uint32 deltatime);
  void Render(SDL_Surface * surface);
};

/* Define a list of entities */
typedef std::list<TEntity *> TEntities;
typedef TEntities::iterator TEntitiesIterator;

#endif
