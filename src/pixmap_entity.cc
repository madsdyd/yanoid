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
#include "pixmap_entity.hh"

#include "debug.hh"
#include "surfacemanager.hh"

/* **********************************************************************
 * The constructor pt. loads a surface to blit around.
 * *********************************************************************/
TPixmapEntity::TPixmapEntity(int x_, int y_, const std::string& path,
			     CollisionType c, EntityType e): 
  TEntity(x_, y_, c, e) {

  currentsurface
    = SurfaceManager->RequireRessource(path);
  Assert(NULL != currentsurface, "Error loading graphics for entity");
  _h = currentsurface->h;
  _w = currentsurface->w;

}

/* **********************************************************************
 * The destructor just clears the surface
 * *********************************************************************/
TPixmapEntity::~TPixmapEntity() {
  SurfaceManager->ReleaseRessource(currentsurface);
}


/* **********************************************************************
 * SetPixmap changes the pixmap of the entity
 * *********************************************************************/
void TPixmapEntity::setPixmap(const std::string& path) {
  SurfaceManager->ReleaseRessource(currentsurface);

  currentsurface
    = SurfaceManager->RequireRessource(path);
  Assert(NULL != currentsurface, "Error loading graphics for entity");
  _h = currentsurface->h;
  _w = currentsurface->w;
  
}

/* **********************************************************************
 * The render method simply blit the entity to the surface provided
 * (This will probably change)
 * *********************************************************************/
void TPixmapEntity::Render(SDL_Surface * surface) {
  SDL_Rect src, dest;

  src.x = 0; src.y = 0; 
  src.w = currentsurface->w; src.h = currentsurface->h;

  dest.x = x(); dest.y = y();
  dest.w = src.w; dest.h = src.h;
  
  SDL_BlitSurface(currentsurface, &src, surface, &dest);
}

/* **********************************************************************
 * PixelCollision determines whether the TEntity obj, 
 * of obj collides with this TEntity using pixel perfection. 
 * *********************************************************************/
bool TPixmapEntity::pixelCollision(const TEntity& o) {
  return false;
}


