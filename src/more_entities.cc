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
void TBrick::OnCollision(TEntity& other, Uint32 currenttime=0) {
  /* Calling our ancestors onCollision gives us scripts, etc. */
  if ("BALL" == other.getEntityType() ||
      "SHOT" == other.getEntityType()) {
    TPixmapEntity::OnCollision(other, currenttime);
    if (hitnum > 0 && --hitnum == 0) {
      removable = true;
    }
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
void TPowerUp::OnCollision(TEntity& other, Uint32 currenttime=0) {
  if ("PADDLE" == other.getEntityType()) {
    /* Make sure our hitfunction is called - 
       it should spawn a relevant powerup... */
    TPixmapEntity::OnCollision(other, currenttime);
    removable = true;
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
void TShot::OnCollision(TEntity& other, Uint32 currenttime=0) {
  /* Make sure our hitfunction is called */
  if ("BRICK" == other.getEntityType()) {
    TPixmapEntity::OnCollision(other, currenttime);
    if (shot_type == "REMOVEALL") {
      other.MarkDying();
    } else {
      removable = true;
    }
  }
  if ("STATIC" == other.getEntityType()) {
    TPixmapEntity::OnCollision(other, currenttime);
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
void THole::OnCollision(TEntity& other,Uint32 t) {
  // LogLine(LOG_VERBOSE, "THole::OnCollision called");
  if ("BALL" == other.getEntityType()) {
    /* Ah, hit by a ball */
    if (!hole_bounces) {
      other.MarkDying();
    } else {
      /* To cheat the ball into collision reflection, we
	 have to swap the order... */
      // TEntity::OnCollision(other, t);
      // I dare not touch this......
      other.OnCollision(*this, t);
    }
  } 
  else if ("POWERUP" == other.getEntityType()) {
    other.MarkDying();
  } else {
    LogLineExt(LOG_WARNING, 
	       ("THole hit by %s", other.getEntityType().c_str()));
  }
}



