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
#include "motion.hh"

/* **********************************************************************
 * The constructor pt. loads a surface to blit around.
 * *********************************************************************/
TEntity::TEntity(int x_, int y_, Angle a, CollisionType c, EntityType e):
  _w(24), _h(16), position(TOrientedPoint(x_,y_,a)), collidepoint(0,0),
  name("unknown"), collision_type(c), entity_type(e), motion(0), mask(0)
{

}

TEntity::TEntity(int x_, int y_, int w_, int h_, 
		 Angle a, CollisionType c, EntityType e):
  _w(w_), _h(h_), position(TOrientedPoint(x_,y_,a)), collidepoint(0,0),
  name("unknown"), collision_type(c), entity_type(e), motion(0), mask(0)
{

}

TEntity::TEntity(const TOrientedPoint& p, CollisionType c, EntityType e): 
  _w(24), _h(16), position(p), collidepoint(0,0), name("unknown"), 
  collision_type(c), entity_type(e), motion(0), mask(0) {

}

/* **********************************************************************
 * The destructor just clears the surface
 * *********************************************************************/
TEntity::~TEntity() {

}

/* **********************************************************************
 * load - loads a entity from path
 * *********************************************************************/
void TEntity::load(const std::string& path) 
{
  string abspath = PathManager->Resolve(path);
}

/* **********************************************************************
 * setmotion set the motion of the entity
 * *********************************************************************/
void TEntity::setMotion(TMotion* m)
{
  if (motion)
    delete motion;
  motion = m;
}

/* **********************************************************************
 * The Update method simple moves the TEntity along with the velocity
 * *********************************************************************/
void TEntity::Update(Uint32 deltatime) 
{
  if (motion)
    motion->Update(deltatime,*this);
  /*
  position.setX((x() + static_cast<int>(static_cast<float>(static_cast<int>(deltatime)*velocity.x())/10.0)) % 800);
  position.setY((y() + static_cast<int>(static_cast<float>(static_cast<int>(deltatime)*velocity.y())/10.0)) % 600);
  */
  /*  cout << "TEntity::Update - delta, x, y " << deltatime 
      << ", " << x << ", " << y << endl; */
}

/* **********************************************************************
 * The render method simply blit the entity to the surface provided
 * (This will probably change)
 * *********************************************************************/
void TEntity::Render(SDL_Surface * surface) {
  SDL_Rect dest;

  dest.x = x(); dest.y = y();
  dest.w = _w; dest.h = _h;
  Uint32 color = SDL_MapRGB(surface->format, 0xFF, 0x00, 0x00);

  SDL_FillRect(surface, &dest, color);
}

/* **********************************************************************
 * boundingBoxCollision determines if the entity o's boundingBox, 
 * collides with this boundingbox
 * Much of this code is taken from sgelib
 * *********************************************************************/
bool TEntity::boundingBoxCollision(const TEntity& o)
{
  int x1 = position.x();
  int y1 = position.y();
  int x2 = o.position.x();
  int y2 = o.position.y();
  int w1 = _w;
  int h1 = _h;
  int w2 = o._w;
  int h2 = o._h;
  
  if(x1 < x2){
    if(x1+w1 > x2){
      if(y1 < y2){
	if(y1+h1 > y2){
	  collidepoint.setX(x2);
	  collidepoint.setY(y2);
	  return true;
	}
      }
      else{
	if(y2+h2 > y1){
	  collidepoint.setX(x2);
	  collidepoint.setY(y1);
	  return true;
	}
      }
    }	
  }
  else{
    if(x2+w2 > x1){
      if(y2 < y1){
	if(y2+h2 > y1){
	  collidepoint.setX(x1);
	  collidepoint.setY(y1);
	  return true;
	}
      }
      else{
	if(y1+h1 > y2){
	  collidepoint.setX(x1);
	  collidepoint.setY(y2);
	  return true;
	}
      }
    }	
  }
  return false;
}

/* **********************************************************************
 * PixelCollision determines whether the TEntity obj, 
 * of obj collides with this TEntity using pixel perfection. 
 * *********************************************************************/
bool TEntity::pixelCollision(const TEntity& o) {
  return boundingBoxCollision(o);
}


