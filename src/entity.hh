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
#include <string>
#include "velocity.hh"

class TEntity {
public:
  typedef enum CollisionType { BOX, PIXEL, NONE } CollisionType;
  typedef enum EntityType { MOVING, STATIONARY } EntityType;
protected:
  // TODO: Some sort of fancy vector class? 
  int _x, _y; /* The position of the entity */
  int _w, _h; /* The bounding box size */
  TVelocity velocity;
  std::string name;
  CollisionType collision_type;
  EntityType entity_type;
  SDL_Surface * currentsurface; /* The current image to blit */
public:
  TEntity(int x_, int y_, CollisionType c = BOX, EntityType e = MOVING);
  ~TEntity();
  
  void Update(Uint32 deltatime);
  void Render(SDL_Surface * surface);
  void setVelocity(int dx, int dy) { velocity.setX(dx); velocity.setY(dy); }
  TVelocity getVelocity() const { return velocity; }
  void setName(const std::string& n) { name = n; };
  std::string getName() const { return name; };
  inline CollisionType getCollisionType() const { return collision_type; }
  inline EntityType getEntityType() const { return entity_type; }
  inline int x() const { return _x; };
  inline int y() const { return _y; };
  inline int w() const { return _w; };
  inline int h() const { return _h; };
  inline void setX(int x_) { _x = x_; }
  inline void setY(int y_) { _y = y_; }
  inline bool boundingBoxCollision(const TEntity& obj) {
    return ! ((obj._y+obj._h) < _y || (_y+_h) < obj._y || 
	      (obj._x+obj._w) < _x || (_x+_w) < obj._x);
  }
  bool pixelCollision();
};

/* Define a list of entities */
typedef std::list<TEntity *> TEntities;
typedef TEntities::iterator TEntitiesIterator;

#endif
