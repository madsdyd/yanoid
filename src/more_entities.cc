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
#include "log.hh"
#include "motion.hh"
#include "entity.hh"
#include "pixmap_entity.hh"
#include "more_entities.hh"
#include "soundmanager.hh"
#include "surfacemanager.hh"
#include "imageeffects.hh"
#include "screen.hh"

/* **********************************************************************
 * The hole bounces variable
 * *********************************************************************/
bool hole_bounces = false;


/* **********************************************************************
 * TBrick
 * *********************************************************************/
TBrick::TBrick(int x_, int y_, string pixmap, string hitfunction, 
	       int _hitnum)
  : TPixmapEntity(x_, y_, 0, pixmap, "BRICK"), hitnum(_hitnum) {
  SetScriptHitCall(hitfunction);
};
/* **********************************************************************
 * ~TBrick
 * *********************************************************************/
TBrick::~TBrick() {
};

/* **********************************************************************
 * MarkDying - can't die if hitnum < 0
 * *********************************************************************/
void TBrick::MarkDying() {
  if (hitnum >= 0) {
    removable = true;
  }
}


/* **********************************************************************
 * OnCollision - most bricks simply die, when they are part of a collision
 * *********************************************************************/
void TBrick::OnCollision(TEntity& other) {
  /* Calling our ancestors onCollision gives us scripts, etc. */
  if ("BALL" == other.getEntityType() ||
      "SHOT" == other.getEntityType()) {
    TPixmapEntity::OnCollision(other);
    if (hitnum > 0 && --hitnum == 0) {
      removable = true;

      // If we have an image initiate an exploding effect
      if (currentsurface) {
	SDL_Surface * img = SurfaceManager->DuplicateRessource(currentsurface);
	
	// pseudo random choosing the effect
	TImageParticleEffect* ef;
	//	int hack = ((int)this) / 10;
	//	if ( hack % 2) {
	ef = new TImageExplodeAllDirEffect(Screen, img, w(), h()); 
	//	} else {
	//	  ef = new TImageExplodeHorizontalBlocksEffect(Screen, img, w(), h()); 
	//	}
	ef->setDuration(300);
	ef->setPostHoldTime(0);
	ef->setLocation(position);
	ef->start();
	effects.push_back(ef);
      }

    }
  }
}

/* **********************************************************************
 * DelayBrick
 * *********************************************************************/
TDelayBrick::TDelayBrick(int x_, int y_, string removefunction, int millisecs_tolive) :
  TEntity(x_, y_, 0, 0, 0, "BRICK", TEntity::STATIONARY, TEntity::BOX), 
  millisecs_left(millisecs_tolive) {
  SetScriptHitCall(removefunction);
}

void TDelayBrick::Update(Uint32 deltatime) {
  millisecs_left -= deltatime;
  // LogLineExt(LOG_VER_2, ("millisecs_left : %i", millisecs_left));
  if (millisecs_left < 0) {
    ExecuteScriptHitCall();
    removable = true;
  }
}

/* **********************************************************************
 * PowerUp
 * *********************************************************************/
/* **********************************************************************
 * Constructor, very similar to TBrick
 * *********************************************************************/
TPowerUp::TPowerUp(int x, int y, string pixmap, string hitfunction)
  : TPixmapEntity(x, y, 0, pixmap, "POWERUP", MOVING, PIXEL) {
  SetScriptHitCall(hitfunction);
  setMotion(new TFreeMotion);
  dynamic_cast<TFreeMotion*>(getMotion())->setDir(1.5 * M_PI);
  dynamic_cast<TFreeMotion*>(getMotion())->setVelocity(0.05);
  dynamic_cast<TFreeMotion*>(getMotion())->setAccel(10.0);
}
  
/* **********************************************************************
 * Collision - also very similar to bricks, except that we only remove
 * if hit by a paddle or shot.
 * *********************************************************************/
void TPowerUp::OnCollision(TEntity& other) {
  if ("PADDLE" == other.getEntityType()) {
    /* Make sure our hitfunction is called - 
       it should spawn a relevant powerup... */
    ExecuteScriptHitCall();
    removable = true;

      // If we have an image initiate an exploding effect
      if (currentsurface) {
	SDL_Surface * img = SurfaceManager->DuplicateRessource(currentsurface);
	TImageImplodeAllDirEffect* ef = 
	  new TImageImplodeAllDirEffect(Screen, img, w(), h()); 
	ef->setDuration(300);
	ef->setPostHoldTime(0);
	ef->setLocation(position);
	ef->start();
	effects.push_back(ef);
      }
  }
}
/* **********************************************************************
 * Shot
 * *********************************************************************/
/* **********************************************************************
 * Constructor, very similar to PowerUp
 * *********************************************************************/
TShot::TShot(int x, int y, string pixmap, string hitfunction,
	     string type)
  : TPixmapEntity(x, y, 0, pixmap, "SHOT", MOVING, PIXEL), 
  shot_type(type) {
  SetScriptHitCall(hitfunction);
  setMotion(new TFreeMotion);
  dynamic_cast<TFreeMotion*>(getMotion())->setDir(0.5 * M_PI);
  dynamic_cast<TFreeMotion*>(getMotion())->setVelocity(0.2);
  dynamic_cast<TFreeMotion*>(getMotion())->setAccel(0.2);
}
  
/* **********************************************************************
 * Collision - also very similar to bricks, except that we only remove
 * if hit by a paddle or shot.
 * *********************************************************************/
void TShot::OnCollision(TEntity& other) {
  /* Make sure our hitfunction is called */
  if ("BRICK" == other.getEntityType()) {
    TPixmapEntity::OnCollision(other);
    if (shot_type == "REMOVEALL") {
      other.MarkDying();
    } else {
      removable = true;
    }
  }
  if ("STATIC" == other.getEntityType()) {
    TPixmapEntity::OnCollision(other);
    removable = true;
  }
}
/* **********************************************************************
 * THole constructor
 * *********************************************************************/
THole::THole(double x_, double y_, int w_, int h_) : 
  TEntity(x_, y_, w_, h_, 0, "HOLE") {
};

/* **********************************************************************
 * Destructor
 * *********************************************************************/
THole::~THole() {
  
}
/* **********************************************************************
 * Collision
 * *********************************************************************/
void THole::OnCollision(TEntity& other) {
  // LogLineExt(LOG_VERBOSE, ("THole::OnCollision(%s)", other.getEntityType().c_str()));
  if ("BALL" == other.getEntityType()) {
    /* Ah, hit by a ball */
    if (!hole_bounces) {
      other.MarkDying();
    } else {
      /* To cheat the ball into collision reflection, we
	 have to swap the order... */
      // TEntity::OnCollision(other);
      // I dare not touch this......
      // other.OnCollision(*this);
    }
  } 
  else if ("POWERUP" == other.getEntityType()) {
    other.MarkDying();
  } else {
    LogLineExt(LOG_WARNING, 
	       ("THole hit by %s", other.getEntityType().c_str()));
  }
}

/* **********************************************************************
 * The paddle constructor
 * *********************************************************************/
TPaddle::TPaddle(int x_, int y_, 
		  const std::string normal_pixmap_path,
		  const std::string wide_pixmap_path,
		  const std::string narrow_pixmap_path) : 
  TPixmapEntity(x_, y_, 0, normal_pixmap_path, "PADDLE", MOVING, PIXEL),
  minx(900.0), maxx(-1.0), milli_seconds_to_normal(0), 
  widesurface(NULL), narrowsurface(NULL),
  AngleModifier(0.05), MovementAngleModifier(1.55)
{
  /* Pixmap has loaded normal into current. Store it */
  normalsurface = SurfaceManager->DuplicateRessource(currentsurface);
  /* Require the other surfaces */
  widesurface = SurfaceManager->RequireRessource(wide_pixmap_path);
  narrowsurface = SurfaceManager->RequireRessource(narrow_pixmap_path);
  Assert(widesurface != NULL && narrowsurface != NULL, "Error loading"
	 " graphics for TPaddle");
}

TPaddle::~TPaddle() {
  /* Release all the surfaces */
  SurfaceManager->ReleaseRessource(normalsurface);
  SurfaceManager->ReleaseRessource(widesurface);
  SurfaceManager->ReleaseRessource(narrowsurface);
}


/* **********************************************************************
 * Called, when this entity collides with another
 * *********************************************************************/
void TPaddle::OnCollision(TEntity& other) {

  /* This must only be called, when at least boundingCollision have been 
     called 
     The purpose of this method is to resolve collisions between this object
     and the other. */

  /* We do not want the paddle to script on 
     hitting powerups */
  if ( "POWERUP" != other.getEntityType() ) {
    if ( other.getMoveType() == TEntity::STATIONARY ) {
	if (getCollideCorner() < 3) {
	  getMotion()->setCurrentVelocity( 0.0 );
	  getMotion()->setAccel( 0.0 );
	  setX(other.x() + other.w());
	}else{
	  getMotion()->setCurrentVelocity( 0.0 );
	  getMotion()->setAccel( 0.0 );
	  setX(other.x() - w());
	}
    }
    ExecuteScriptHitCall();
  }
}


/* **********************************************************************
 * Update to track our max x and y position. Yes, this will only
 * work for horizontal paddles, but that is what we have now.
 * *********************************************************************/
void TPaddle::Update(Uint32 deltatime) {
  TPixmapEntity::Update(deltatime);
  /* Track x changes */
  if ((x() + w()) > maxx) { maxx = x() + w(); };
if (x() < minx) { minx = x(); };
  /* Track if we need to go to normal surface */
  if (milli_seconds_to_normal > 0) {
    milli_seconds_to_normal -= deltatime;
    if (milli_seconds_to_normal <= 0) {
      GoNormal();
    }
  }
}


/* **********************************************************************
 * Narrow and wide, simple
 * *********************************************************************/
void TPaddle::GoNormal() {
  /* we may not fit */
  int curw = w();
  setPixmap(normalsurface);
  setX(x() + (curw - w())/2.0);
  milli_seconds_to_normal = 0;
  if ((x()+w()) > maxx) { setX(maxx - w()); }; 
  if (x() < minx) { setX(minx); };
  /* Note, this may not work, but its the best I can do currently */
}

void TPaddle::GoNarrow(int seconds) {
  /* Simple, since we _always_ fit */
  int curw = w();
  setPixmap(narrowsurface);
  setX(x() + (curw - w())/2.0);
  milli_seconds_to_normal = seconds * 1000;
}

void TPaddle::GoWide(int seconds) {
  /* we may not fit */
  int curw = w();
  setPixmap(widesurface);
  setX(x() + (curw - w())/2.0);
  milli_seconds_to_normal = seconds * 1000;
  if ((x()+w()) > maxx) { setX(maxx - w()); }; 
  if (x() < minx) { setX(minx); };
  /* Note, this may not work, but its the best I can do currently */
}

/* **********************************************************************
 * Ball
 * *********************************************************************/
/* **********************************************************************
 * Constructor, very similar to TBrick
 * *********************************************************************/
TBall::TBall(int x, int y, double vel,
	     const string& pixmap, const string& hitfunction)
  : TPixmapEntity(x, y, 0, pixmap, "BALL", MOVING, PIXEL),
    is_dying(false)
{
  SetScriptHitCall(hitfunction);
  setMotion(new TFreeMotion);
  dynamic_cast<TFreeMotion*>(getMotion())->setDir(1 * M_PI / 3);
  dynamic_cast<TFreeMotion*>(getMotion())->setVelocity(vel);
}
  
/* **********************************************************************
 * Collision - Currently just call parent function
 * *********************************************************************/
void TBall::OnCollision(TEntity& other) {

  /* This must only be called, when at least boundingCollision have been 
     called 
     The purpose of this method is to resolve collisions between this object
     and the other. */
  
  if (other.getEntityType() == "POWERUP")
    return;
  
  if (getMotion()) {
    double colx = x();
    double coly = y();
    getMotion()->rewind(*this);
    TFreeMotion* motion = dynamic_cast<TFreeMotion*>(getMotion());
    double newangle = 0.0;
    double ballwidth = 0;
    double ballheight = 0;
    
    // if the ball is on the way down the drain, skip collision response
    if ( other.getEntityType() == "PADDLE" && 
	 (y() + h()) > other.y() ) {

      getMotion()->setVelocity(0.5);
      setX(colx);
      setY(coly);
      // make sure the right reflektion is done
      if (!is_dying && 
	  ((motion->getDir() > (3*M_PI/2) && x() < (other.x() + other.w()/2)) ||
	   (motion->getDir() < (3*M_PI/2) && x() > (other.x() + other.w()/2)) ) )
	motion->setDir(3.0 * M_PI - motion->getDir());       
      is_dying = true;
      return;
    }

    switch(other.getCollideCorner()) {
    case 1:
      ballwidth = w();
      ballheight = h();
      break;
    case 2:
      ballwidth = w();
      break;
    case 3:
      break;
    case 4:
      ballheight = h();
      break;
    }
    double dx = collidepoint.x() - (x() + ballwidth);
    double dy = collidepoint.y() - (y() + ballheight);
      
    // Equation of two lines intersecting, knowing that one of 
    // the lines is vertical. The taking only the y component of the 
    // intersection point.
    double lin_intersect_y = dy * ((other.getCollidePoint().x() - (x() + ballwidth) ) / dx) + y() + ballheight; 
      
    bool verticalhit = false;
    switch(other.getCollideCorner()) {
    case 1:
      if ( lin_intersect_y > other.getCollidePoint().y() && dx > 0) {
	verticalhit = true;
      }
      break;
    case 2:
      if ( lin_intersect_y < other.getCollidePoint().y() && dx > 0) {
	verticalhit = true;
      }
      break;
    case 3:
      if ( lin_intersect_y < other.getCollidePoint().y() && dx < 0) {
	verticalhit = true;
      }
      break;
    case 4:
      if ( lin_intersect_y > other.getCollidePoint().y() && dx < 0) {	
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
	
      if (dx >= 0) {
	setX(colx - 2 * ( (colx + ballwidth) -  other.getCollidePoint().x()));	
      }else{
	setX(colx + 2 * ( other.getCollidePoint().x() - colx ));
      }
      setY(coly);
	
	
    }else{
      // OK collision on the top/bottom
	
      newangle = (dx < 0) ? 
	2.0 * M_PI - motion->getDir() :
	2.0 * M_PI - motion->getDir();
	
      if (dy >= 0) {

	setY(coly - 2 * ( (coly + ballheight) -  other.getCollidePoint().y()) -1  );

	// If the ball has hit the paddle from above we make some modifications to the angle
	// depending where on the paddle the ball has hit.
	if (other.getEntityType() == "PADDLE") {
	  TPaddle* paddle =  dynamic_cast<TPaddle*>(&other);
	  double lx = (x() + w()/2) - paddle->x();
	  lx = (lx < 0) ? 0 :  ( (lx > paddle->w()) ? paddle->w() : lx);
	  double modangle = ( (paddle->w()/2) < lx) ? -log(lx - paddle->w()/2) : log(paddle->w()/2 - lx);
	  modangle = ( (paddle->w()/2) == lx) ? 0 : modangle;

	  // if the ball hits the middle of the bat no angle modifier is applied
	  modangle = (fabs(paddle->w()/2 - lx) < (paddle->w() * 0.3)) ? 0 : modangle;

	  // OK now we see if the paddle is moving and make the appropriate changes to 
	  // the angle of the ball.
	  double modangle2 = 0;
	  if (motion->getCurrentVelocity() != 0)
	    modangle2 = -(paddle->getMoveAngleMod() * paddle->getMotion()->getCurrentVelocity());

	  newangle += modangle * paddle->getAngleMod() + modangle2;

	  // make some adjustment so the ball doesn't move horizontal ever
	  if (newangle < (M_PI / 7)) {
	    newangle = M_PI / 7;
	  } else if (newangle > (6 *M_PI / 7)) {
	    newangle = 6 * M_PI / 7;
	  }
	}
      }else{
	setY(coly + 2 * ( other.getCollidePoint().y() -  coly) + 1 );
      }
	
      setX(colx);
	
    }
    motion->setDir(newangle);
  } 

  TPixmapEntity::OnCollision(other);
}
