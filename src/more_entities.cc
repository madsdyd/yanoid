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
  : TPixmapEntity(x_, y_, 0, pixmap), hitnum(_hitnum) {
  SetScriptHitCall(hitfunction);
};
/* **********************************************************************
 * ~TBrick
 * *********************************************************************/
TBrick::~TBrick() {
};

/* **********************************************************************
 * OnCollision - most bricks simply die, when they are part of a collision
 * *********************************************************************/
void TBrick::OnCollision(TEntity& other, Uint32 currenttime=0) {
  TPixmapEntity::OnCollision(other, currenttime);
  if (hitnum > 0 && --hitnum == 0) {
    removable = true;
  }
}


/* **********************************************************************
 * THole constructor
 * *********************************************************************/
THole::THole(double x_, double y_, int w_, int h_) : TEntity(x_, y_, w_, h_) {

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
  LogLine(LOG_VERBOSE, "THole::OnCollision called");
  if (TEntity::BALL == other.getEntityType()) {
    /* Ah, hit by a ball */
    if (!hole_bounces) {
      other.MarkDying();
    } else {
      TEntity::OnCollision(other, t);
    }
  } else {
    LogLine(LOG_WARNING, "THole should not be hit by anything but balls");
  }
}



