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
#include "entity.hh"

#include "debug.hh"
#include "surfacemanager.hh"
/* **********************************************************************
 * The constructor pt. loads a surface to blit around.
 * *********************************************************************/
TEntity::TEntity(int x_, int y_) {
  x = x_; y = y_; 
  vel_x = x/60; vel_y = y/20;
  currentsurface
    = SurfaceManager->RequireRessource("graphics/objects/basic_brick.png");
  Assert(NULL != currentsurface, "Error loading graphics for entity");
}

/* **********************************************************************
 * The destructor just clears the surface
 * *********************************************************************/
TEntity::~TEntity() {
  SurfaceManager->ReleaseRessource(currentsurface);
}


/* **********************************************************************
 * The Update method simple moves the TEntity along with the velocity
 * *********************************************************************/
void TEntity::Update(Uint32 deltatime) {
  x += (int) (deltatime*vel_x/10.0);
  y += (int) (deltatime*vel_y/10.0);
  /* cout << "TEntity::Update - delta, x, y " << deltatime 
     << ", " << x << ", " << y << endl; */
}

/* **********************************************************************
 * The render method simply blit the entity to the surface provided
 * (This will probably change)
 * *********************************************************************/
void TEntity::Render(SDL_Surface * surface) {
  SDL_Rect src, dest;

  src.x = 0; src.y = 0; 
  src.w = currentsurface->w; src.h = currentsurface->h;

  dest.x = x; dest.y = y;
  dest.w = src.w; dest.h = src.h;
  
  SDL_BlitSurface(currentsurface, &src, surface, &dest);
}
