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
#include "debug.hh"
#include "interprenter.hh"
#include "motion.hh"
#include <vector>

/* **********************************************************************
 * These callbacks are used from the interprenter
 * The CurrentGame variable is used by most of them.
 * *********************************************************************/
static TGame * CurrentGame = NULL;
/* **********************************************************************
 * Adjust the score - adds the value to the current score 
 * *********************************************************************/
static PyObject * AdjustScore(PyObject * self, PyObject * args) {
  int dscore;
  if (!CurrentGame || !PyArg_ParseTuple(args, "i", &dscore)) {
    return NULL;
  }
  CurrentGame->GetState()->score += dscore;
  return Py_BuildValue("");
}

/* **********************************************************************
 * Adds a new map to the maplist for the game
 * *********************************************************************/
static PyObject * AddMap(PyObject * self, PyObject * args) {
  char * mapname;
  if (!CurrentGame || !PyArg_ParseTuple(args, "s", &mapname)) {
    return NULL;
  }
  CurrentGame->AddMapName(mapname);
  return Py_BuildValue("");
}


/* **********************************************************************
 * The method table
 * *********************************************************************/
static PyMethodDef game_methods[] = {
  {"AdjustScore", AdjustScore, METH_VARARGS},
  {"AddMap", AddMap, METH_VARARGS},
  {NULL, NULL}
};


/* **********************************************************************
 * The constructor currently constructs a single entity
 * *********************************************************************/
TGame::TGame() : lastupdate(0) {
  /* Get a map */
  Map = new TMap();
  if (!Map) {
    LogFatal("TGame::TGame - unable to create Map");
  }
  
  /* Set up the gamestate "pointer class" */
  GameState.MapState = Map->GetState();
  /* We only really use a single map at a time */
  CurrentGame = this;
}

/* **********************************************************************
 * Destructor, clean up... 
 * *********************************************************************/
TGame::~TGame() {
  if (Map) {
    delete Map;
  }
  LogLine(LOG_TODO, "Clean up TGame destructor and GameState");
}

/* **********************************************************************
 * The stuff to handle the modules
 * *********************************************************************/
bool TGame::ModuleAdded = false;

bool TGame::AddModule() {
  if (!ModuleAdded) {
    if (Interprenter->AddModule("yanoid_game", game_methods)) {
      ModuleAdded = true;
    }
  }    
  return ModuleAdded;
}

/* **********************************************************************
 * Update calculates difference, calls entities.
 * *********************************************************************/
void TGame::Update(Uint32 currenttime) {
  
  Uint32 deltatime = currenttime - lastupdate;
  /* Update all objects in game */
  Map->Update(deltatime);
  handleCollisions(currenttime);
  lastupdate = currenttime;

  /* If we are not in PLAYING state, no need to check for state changes, 
     as changing to PLAYING state always is initiated from the outside */
  if (TGameState::PLAYING != GameState.status) {
    // speed up ball if necessary
    return;
  }

  /* At this point, the status is always PLAYING */

  /* Check game state for a loosing player */
  if (Map->GetState()->num_balls == 0) {
    /* There are no balls. Player must loose a life, and we need to add 
       a ball */
    if (GameState.lives <= 1) {
      LogLine(LOG_VERBOSE, "Going into DEAD state");
      GameState.status = TGameState::DEAD;
    } else {
      LogLine(LOG_VERBOSE, "Going into CUT state");
      GameState.lives--;
      GameState.status = TGameState::CUT;
    }
  }
  
  /* Check game state for a winning player */
  if (Map->GetState()->num_bricks == 0) {
    /* There are no bricks, player has won a map */
    LogLine(LOG_VERBOSE, "Going into MAPDONE state");
    GameState.status = TGameState::MAPDONE;
  }

}

/* **********************************************************************
 * Get access to the game state
 * This requires updating stuff
 * *********************************************************************/

TGameState * TGame::GetState() {
  GameState.MapState = Map->GetState();
  GameState.gametime = lastupdate;
  return &GameState;
}

/* **********************************************************************
 * HandleCollisions and speed up ball if necessary
 * *********************************************************************/

void TGame::handleCollisions(Uint32 currenttime) 
{
  TMapState* themap = GameState.MapState;
  TEntitiesIterator end = themap->Entities.end();

  int screenw = 800; /* screen width, a hack */

  // take special care of our paddle
  TEntity * paddle = GameState.MapState->GetPaddle();
  if (paddle->x() <= 1 ) {
    paddle->getMotion()->setVelocity( 0.0 );
    paddle->getMotion()->setCurrentVelocity( 0.0 );
    paddle->getMotion()->setAccel( 0.0 );
    paddle->setX(2);
  } else if ((paddle->x()+paddle->w()) >= (screenw-1) ) {
    paddle->getMotion()->setVelocity( 0.0 );
    paddle->getMotion()->setCurrentVelocity( 0.0 );
    paddle->getMotion()->setAccel( 0.0 );
    paddle->setX(screenw-2-paddle->w());
  }

  std::vector<TEntity*> resetEntityList;
  // uhh O(n^2). But not quite soo, because we can skip
  // alot of cycles..
  for (TEntitiesIterator i1 = themap->Entities.begin() ; 
       i1 != end ; ++i1) {
    /* We only check moving stuff against stationary objects */
    TEntity::EntityType i1type = (*i1)->getEntityType();
    if (i1type == TEntity::STATIONARY) {
      //      continue;
    }
    // speed up ball
    if (i1type == TEntity::BALL) {
      if (GameState.MapState->ballbirth == 0)
	GameState.MapState->ballbirth = currenttime;
      (*i1)->getMotion()->setVelocity(GameState.MapState->ballspeed
				      + (currenttime - GameState.MapState->ballbirth) * 
				      GameState.MapState->ballacceleration);
    }

    double maxy = (*i1)->y() + static_cast<double>((*i1)->h());

    TEntity::CollisionType i1coll = (*i1)->getCollisionType();
    TEntitiesIterator i2 = themap->Entities.begin();

    for ( ; *i2 != *i1 && i2 != end ; ++i2) {
      if (i1 == i2) {
	continue;
      }
      // Check to see if the y for the i2 iterator is
      // bigger than maxy. If so we know that the rest
      // of the objects in the list must be located at a
      // too high y to bee colliding and we can skip the tests
      if (maxy < (*i2)->y()) {
	break;
      }
      
      //
      // To make sure we doesn't check two objects against eachother 2 times
      // we only check if i1->y > i2-> or if they are equal i1->x > i2->x
      //
      if ((*i1)->y() <= (*i2)->y()) {
	if ((*i1)->y() == (*i2)->y()) {
	  if ((*i1)->x() < (*i2)->x()) {
	    continue;
	  }else{
	    //proceed
	  }
	}else{
	  continue;
	}  
      }

      /*
	If its a ball and it allready has collided
	just ignore it.
       */
      if ((i1type == TEntity::BALL && (*i1)->getLastUpdate() == currenttime) || 
	  ((*i2)->getEntityType() == TEntity::BALL && (*i2)->getLastUpdate() == currenttime))
	continue;

      /* If there are no collision between the current two entities, 
	 continue */
      if (! (*i1)->boundingBoxCollision(*(*i2)))
	continue;
      
      /* Call the OnCollision events for both entities */
      (*i1)->OnCollision(*(*i2),currenttime);
      (*i2)->OnCollision(*(*i1),currenttime);

      // If necessary make pixel perfect detection..
      if (i1coll == TEntity::PIXEL || (*i2)->getCollisionType() == TEntity::PIXEL) {
	// cout << "Making pixel perfect detection" << endl;
      }
    }
  }
}

/* **********************************************************************
 * Load a map - TMap handles it
 * *********************************************************************/
bool TGame::LoadMap(string mapname) {
  bool result = Map->Load(mapname);
  GameState.MapState = Map->GetState();
  return result;
}

/* **********************************************************************
 * Safely go to next map
 * *********************************************************************/
bool TGame::LoadNextMap() {
  int cmap = GetState()->currentmap;
  GetState()->currentmap++;
  while(GetState()->currentmap != cmap) {
    // Are there any maps left in out maplist?
    if (!HasMap(GetState()->currentmap)) {
      LogLine(LOG_VERBOSE, "No More Maps -> resetting");
      GetState()->currentmap = 0;
    }
    // load the next map
    if (LoadMap(GetMapName(GetState()->currentmap))) {
      return true;
    }
    GetState()->currentmap++;
  }
  /* Been through the complete list */
  return false;
}
