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
  _w(24), _h(16), position(TOrientedPoint(x_,y_,a)), name("unknown"), 
  collision_type(c), entity_type(e), motion(0) 
{

}

TEntity::TEntity(const TOrientedPoint& p, CollisionType c, EntityType e): 
  _w(24), _h(16), position(p), name("unknown"), 
  collision_type(c), entity_type(e), motion(0) {

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
 * PixelCollision determines whether the TEntity obj, 
 * of obj collides with this TEntity using pixel perfection. 
 * *********************************************************************/
bool TEntity::pixelCollision(const TEntity& o) {
  return boundingBoxCollision(o);
}

