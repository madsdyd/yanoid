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
#include "interprenter.hh"

/* **********************************************************************
 * The constructor pt. loads a surface to blit around.
 * *********************************************************************/
TEntity::TEntity(double x_, double y_, Angle a, CollisionType c, EntityType e):
  _w(24), _h(16), position(TOrientedPoint(x_,y_,a)), collidepoint(0,0),
  name("unknown"), collision_type(c), entity_type(e), motion(0), mask(0),
  collidecorner(0)
{

}

TEntity::TEntity(double x_, double y_, int w_, int h_, 
		 Angle a, CollisionType c, EntityType e):
  _w(w_), _h(h_), position(TOrientedPoint(x_,y_,a)), collidepoint(0,0),
  name("unknown"), collision_type(c), entity_type(e), motion(0), mask(0),
  changed(true),  collidecorner(0)
{

}

TEntity::TEntity(const TOrientedPoint& p, CollisionType c, EntityType e): 
  _w(24), _h(16), position(p), collidepoint(0,0), name("unknown"), 
  collision_type(c), entity_type(e), motion(0), mask(0), changed(true),
  collidecorner(0)
{

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
  if (entity_type == STATIONARY) {
    changed = false;
    return;
  }

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

  if (!changed)
    return;

  dest.x = static_cast<int>(x()); 
  dest.y = static_cast<int>(y());
  dest.w = _w; dest.h = _h;
  Uint32 color = SDL_MapRGB(surface->format, 0xFF, 0x00, 0x00);

  SDL_FillRect(surface, &dest, color);

  changed = false;
}

/* **********************************************************************
 * boundingBoxCollision determines if the entity o's boundingBox, 
 * collides with this boundingbox
 * Much of this code is taken from sgelib
 * *********************************************************************/
bool TEntity::boundingBoxCollision(TEntity& o)
{
  double x1 = position.x();
  double y1 = position.y();
  double x2 = o.position.x();
  double y2 = o.position.y();
  double w1 = static_cast<double>(_w);
  double h1 = static_cast<double>(_h);
  double w2 = static_cast<double>(o._w);
  double h2 = static_cast<double>(o._h);
  
  if(x1 < x2){
    if(x1+w1 > x2){
      if(y1 < y2){
	if(y1+h1 > y2){
	  collidepoint.setX(x2);
	  collidepoint.setY(y2);
	  o.collidepoint.setX(x2);
	  o.collidepoint.setY(y2);
	  o.collidecorner = 1;
	  collidecorner = 3;
	  return true;
	}
      }
      else{
	if(y2+h2 > y1){
	  collidepoint.setX(x2);
	  collidepoint.setY(y1);
	  o.collidepoint.setX(x2);
	  o.collidepoint.setY(y1);
	  o.collidecorner = 2;
	  collidecorner = 4;
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
	  o.collidepoint.setX(x1);
	  o.collidepoint.setY(y1);
	  o.collidecorner = 3;
	  collidecorner = 1;
	  return true;
	}
      }
      else{
	if(y1+h1 > y2){
	  collidepoint.setX(x1);
	  collidepoint.setY(y2);
	  o.collidepoint.setX(x1);
	  o.collidepoint.setY(y2);
	  o.collidecorner = 4;
	  collidecorner = 2;
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
bool TEntity::pixelCollision(TEntity& o) {
  return boundingBoxCollision(o);
}

/* **********************************************************************
 * Called, when this entity collides with another
 * *********************************************************************/
void TEntity::OnCollision(TEntity& other) {

  /* This must only be called, when at least boundingCollision have been 
     called 
     The purpose of this method is to resolve collisions between this object
     and the other. 
     This is done by rewinding the time for both objects, finding the point
     in time where they collide, change their motion according to some
     yet-to-be-defined rules, and then forward time again. */
  TEntity* ball=0;
  TEntity* tother=0;
  if (getEntityType() == TEntity::BALL) {
    ball = this;
    tother = &other;
  } else if (other.getEntityType() == TEntity::BALL) {
    return;
    ball = &other;
    tother = this;
  } 

  if (getMotion() && ball ) {
    double colx = ball->x();
    double coly = ball->y();
    ball->getMotion()->rewind(*ball);
    TFreeMotion* motion = dynamic_cast<TFreeMotion*>(ball->getMotion());
    //    cerr << ball->getName() << motion->getDir() << " y: " << ball->y() << endl;
    //    cerr << ball->y() << ", " << ball->collidepoint.y() << ", " << coly << ": " << ball->h() << " CORNER: " << tother->collidecorner;
    double newangle = 0.0;


    double ballwidth = 0;
    double ballheight = 0;
    switch(tother->collidecorner) {
    case 1:
      ballwidth = ball->w();
      ballheight = ball->h();
      break;
    case 2:
      ballwidth = ball->w();
      break;
    case 3:
      break;
    case 4:
      ballheight = ball->h();
      break;
    }
    double dx = ball->collidepoint.x() - (ball->x() + ballwidth);
    double dy = ball->collidepoint.y() - (ball->y() + ballheight);

    // Equation of two lines intersecting, knowing that one of 
    // the lines is vertical. The taking only the y component of the 
    // intersection point.
    double lin_intersect_y = dy * ((ball->collidepoint.x() - (ball->x() + ballwidth) ) / dx) + ball->y() + ballheight; 
    
    bool verticalhit = false;
    switch(tother->collidecorner) {
    case 1:
      if ( lin_intersect_y > ball->collidepoint.y() ) {
	verticalhit = true;
      }
      break;
    case 2:
      if ( lin_intersect_y < ball->collidepoint.y() ) {
	verticalhit = true;
      }
      break;
    case 3:
      if ( lin_intersect_y > ball->collidepoint.y() ) {
	verticalhit = true;
      }
      break;
    case 4:
      if ( lin_intersect_y < ball->collidepoint.y() ) {
	verticalhit = true;
      }
      break;
    }

    
    if ( verticalhit ) {
      // OK collision on the side
      
      // a hit from the left
      newangle = (dy < 0) ? 
	M_PI - motion->getDir() :
	3.0 * M_PI - motion->getDir();
      
      cerr << "<<" << newangle << endl;
      if (dy >= 0) {
	ball->setX(colx - 2 * ( (colx + ballwidth) -  ball->collidepoint.x()));
      }else{
	ball->setX(colx + 2 * ( ball->collidepoint.x() - colx ));
      }
      ball->setY(coly);
      
      
    }else{
      // OK collision on the top
      
      // a hit from the top
      newangle = (dx < 0) ? 
	2.0 * M_PI - motion->getDir() :
	2.0 * M_PI - motion->getDir();
      
      //      cerr << "vv" << newangle << endl;
      
      if (dy >= 0) {
	cerr << "vv1 " << " lin.intersect: " << lin_intersect_y << newangle << "ballheight: " << ballheight << " coly: " << coly << " collidepoint " << ball->collidepoint.y() << "corner: " << tother->collidecorner << " name: " << ball->getName() << endl;
	//	ball->setY(coly + 2 * ( ball->collidepoint.y() -  (coly + ballheight)) );
	ball->setY(coly - 2 * ( (coly + ballheight) -  ball->collidepoint.y()) );
      }else{
	cerr << "vv2 "  << " lin.intersect: " << lin_intersect_y << newangle << "ballheight: " << ballheight << " coly: " << coly << " collidepoint " << ball->collidepoint.y() << "corner: " << tother->collidecorner << " name: " << ball->getName() << endl;
	ball->setY(coly - 2 * ( ball->collidepoint.y() -  coly) );
      }

      ball->setX(colx);
      
    }
    
  motion->setDir(newangle);
  }
}

/* **********************************************************************
 * Script hit call operations
 * *********************************************************************/
void TEntity::SetScriptHitCall(string function) {
  ScriptHitCall = function;
}
 
void TEntity::ExecuteScriptHitCall() {
  if ("" != ScriptHitCall) {
    if(!Interprenter->RunSimpleString(ScriptHitCall)) {
      LogLine(LOG_WARNING, "TEntity::ExecuteScriptHitCall error");
    }
  }
}
