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
/* Entities are the basic units that can be manipulated in the game */

#ifndef __PIXMAP_ENTITY_HH__
#define __PIXMAP_ENTITY_HH__

#include "entity.hh"

class TPixmapEntity : public TEntity {
protected:
  SDL_Surface * currentsurface; /* The current image to blit */
public:
  TPixmapEntity(int x_, int y_, const std::string& pixmap_path,
		CollisionType c = BOX, EntityType e = MOVING);
  virtual ~TPixmapEntity();
  
  virtual void setPixmap(const std::string& pixmap_path);
  virtual void Render(SDL_Surface * surface);
  virtual bool pixelCollision(const TEntity&);
};

/* Define a list of pixmap entities */
typedef std::list<TEntity *> TPixmapEntities;
typedef TEntities::iterator TPixmapEntitiesIterator;

#endif
