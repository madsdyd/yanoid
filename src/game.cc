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

#include "game.hh"
#include "log.hh"
#include "motion.hh"

/* **********************************************************************
 * The global game 
 * *********************************************************************/

TGame * Game;

/* **********************************************************************
 * The constructor currently constructs a single entity
 * *********************************************************************/
TGame::TGame() {
  lastupdate = 0; // TODO: Fix
  
  /* Set up the gamestate "pointer class" */
  GameState.MapState = Map.GetState();
}

/* **********************************************************************
 * Destructor, clean up... 
 * *********************************************************************/
TGame::~TGame() {
  LogLine(LOG_TODO, "Clean up TGame destructor and GameState");
}


/* **********************************************************************
 * Update calculates difference, calls entities.
 * *********************************************************************/
void TGame::Update(Uint32 currenttime) {
  Uint32 deltatime = currenttime - lastupdate;
  /* Update all objects in game */
  Map.Update(deltatime);
  handleCollisions();
  lastupdate = currenttime;
}

/* **********************************************************************
 * Get access to the game state
 * This requires updating stuff
 * *********************************************************************/

TGameState * TGame::GetState() {
  return &GameState;
}

/* **********************************************************************
 * HandleCollisions
 * *********************************************************************/

void TGame::handleCollisions() 
{
  TMapState* themap = GameState.MapState;
  TEntitiesIterator end = themap->Entities.end();

  // uhh O(n^2). But not quite soo, because we can skip
  // alot of cycles..
  for (TEntitiesIterator i1 = themap->Entities.begin() ; 
       i1 != end ; ++i1) {
    int maxy = (*i1)->y() + (*i1)->h();
    TEntity::EntityType i1type = (*i1)->getEntityType();
    TEntity::CollisionType i1coll = (*i1)->getCollisionType();
    TEntitiesIterator i2 = i1;
    i2++;
    for ( ; *i2 != *i1 && i2 != end ; ++i2) {

      // Check to see if the y for the i2 iterator is
      // bigger than maxy. If so we know that the rest
      // of the objects in the list must be located at a
      // too high y to bee colliding and we can skip the tests

      if (maxy < (*i2)->y())
	break;
      
      // Next we check to see if both i1 and i2 are of
      // type STATIONARY since it doesn't make sense to 
      // collision detect stationary entities. At the same time use 
      // shortcut or to determine if bounding box detection fails.

      if (i1type == TEntity::STATIONARY && (*i2)->getEntityType() == i1type 
	  || (! (*i1)->boundingBoxCollision(*(*i2))))
	continue;
      
      /*
      LogLine(LOG_INFO, "Bounding Box Collision between " + (*i1)->getName() +
	      " and " + (*i2)->getName());
      */
      // this section is for debugging only
      {

	//	cout << "dir1 " << (*i1)->getMotion()->getDirection() << " dir2 " << (*i2)->getMotion()->getDirection() << endl;
	if ((*i1)->getMotion()) {
	  (*i1)->getMotion()->reverseDirection();
	  (*i1)->getMotion()->rewind();
	}
	if ((*i2)->getMotion()) {
	  (*i2)->getMotion()->reverseDirection();
	  (*i2)->getMotion()->rewind();
	}
	//	cout << "dir1 " << (*i1)->getMotion()->getDirection() << " dir2 " << (*i2)->getMotion()->getDirection() << endl;

      }

      // If necessary make pixel perfect detection..
      if (i1coll == TEntity::PIXEL || (*i2)->getCollisionType() == TEntity::PIXEL) 
	cout << "Making pixel perfect detection" << endl;

    }
  }
}




