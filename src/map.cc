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
#include <Python.h>

#include "map.hh"
#include "ressourcemanager.hh"
#include "entity.hh"
#include "pixmap_entity.hh"
#include "more_entities.hh"
#include "globals.hh"
#include "motion.hh"
#include "interprenter.hh"

/* **********************************************************************
 * Testing: A temporary TMap variable to use with some static functions
 * Note: This is becomming semi permanent...
 * *********************************************************************/
static TMap * CurrentMap = NULL;

/* **********************************************************************
 * These static methods are not so great, but for now, they will have to
 * do.
 * *********************************************************************/

/* **********************************************************************
 * Add a brick or similar object to the current map
 * *********************************************************************/
static PyObject * AddObject(PyObject * self, PyObject * args) {
  int x; int y; int w; int h; 
  char * type; char * hitfunction; char * pixmap;
  if (!CurrentMap || !PyArg_ParseTuple(args, "ssiiiis", 
				       &type, &hitfunction,
				       &x, &y, &w, &h, &pixmap)) {
    return NULL;
  }
  if (CurrentMap->AddEntity(type, hitfunction, x, y, w, h, pixmap)) {
    return Py_BuildValue("");
  } else {
    return NULL;
  }
}

/* **********************************************************************
 * 
 * *********************************************************************/
static PyObject * SetPaddle(PyObject * self, PyObject * args) {
  int x; int y;  
  char * pathtype; 
  double velocity;
  double angle; /* Have to use a type that works with parsetuple */
  char * pixmap;
  if (!CurrentMap || !PyArg_ParseTuple(args, "iisdds", 
				       &x, &y, &pathtype,
				       &velocity, &angle, &pixmap)) {
    return NULL;
  }
  if (CurrentMap->SetPaddle(x, y, pathtype, velocity, angle, pixmap)) {
    return Py_BuildValue("");
  } else {
    return NULL;
  }
  
}

static PyMethodDef map_methods[] = {
  {"AddObject", AddObject, METH_VARARGS},
  {"SetPaddle", SetPaddle, METH_VARARGS},
  {NULL, NULL}
};


/* **********************************************************************
 * TMapState stuff
 * *********************************************************************/

/* **********************************************************************
 * TMapState constructor
 * *********************************************************************/
TMapState::TMapState() : paddle(NULL), num_balls(0), num_bricks(0) {

}
/* **********************************************************************
 * TMapState destructor
 * *********************************************************************/
TMapState::~TMapState() {
  TEntitiesIterator End = Entities.end();
  TEntitiesIterator i;
  for (i = Entities.begin(); i != End; i++) {
    delete(*i);
  }  
}


/* **********************************************************************
 * Get the paddle
 * *********************************************************************/
TEntity * TMapState::GetPaddle() {
  return paddle;
}


/* **********************************************************************
 * load - loads a TMapState and associated entities from path.
 * *********************************************************************/
void TMapState::load(const std::string& path)
{

}

/* **********************************************************************
 * TMap stuff
 * *********************************************************************/

/* **********************************************************************
 * Constructor
 * Currently builds a simple map.
 * *********************************************************************/
TMap::TMap() {
  /* Create a TMapState */
  MapState = new TMapState();
  if (!MapState) {
    LogFatal("TMap::TMap - unable to create mapstate");
  }
}
/* **********************************************************************
 * Clear the map
 * *********************************************************************/


/* **********************************************************************
 * Destructor
 * *********************************************************************/
TMap::~TMap() {
  if (MapState) {
    delete MapState;
  }
}
/* **********************************************************************
 * Add the modules
 * *********************************************************************/
bool TMap::ModuleAdded = false;

bool TMap::AddModule() {
  if (!ModuleAdded) {
    if (Interprenter->AddModule("yanoid_map", map_methods)) {
      ModuleAdded = true;
    }
  }    
  return ModuleAdded;
}

/* **********************************************************************
 * Load a map - set the current pointer, load the python mapname into the
 * interprenter, unset the current pointer, return
 * *********************************************************************/

bool TMap::Load(string mapname) {
  /* Make sure we have a fresh mapstate */
  delete MapState;
  MapState = new TMapState();
  if (!MapState) {
    LogFatal("TMap::Load - unable to create mapstate");
  }
  CurrentMap = this;
  bool result = Interprenter->RunSimpleFile(mapname);
  /* Eventually we may want to unset the current map - currently we just 
     keep it, such that we can do our stuff from the console */
  // CurrentMap = NULL;
  return result;
}

/* **********************************************************************
 * AddEntity
 * This will need to be refined to allow for different types, etc.
 * *********************************************************************/
bool TMap::AddEntity(string type, string hitfunction, 
		     int x, int y, int w, int h, 
		     string pixmap) {
  /* Do different things, according to type */
  if ("brick" == type ) {
    /* This is a pixmapentity */
    MapState->Entities.push_back(new TBrick(x, y, pixmap, hitfunction));
    MapState->num_bricks++;
    return true;
  } else if ("static" == type) {
    /* In lack of a better name */
    TEntity * e = new TEntity(x, y, w, h);
    e->SetScriptHitCall(hitfunction);
    MapState->Entities.push_back(e);
    return true;
  } else if ("hole" == type) {
    TEntity * e = new THole(x, y, w, h);
    e->SetScriptHitCall(hitfunction);
    MapState->Entities.push_back(e);
    LogLine(LOG_VERBOSE, "hole created");
    return true;
  } else if ("default-ball" == type) {
    if (!MapState->paddle) {
      LogLine(LOG_ERROR, "Can't add default ball, with no paddle");
      return false;
    }
    /* Note, the placement is ... well, off, since we do not 
       know the size of the entity. Never mind for now */
    /*TEntity * e = new TPixmapEntity(MapState->paddle->x() 
				    + (MapState->paddle->w()/2),
				    MapState->paddle->y() - 16,
				    0, pixmap, TEntity::PIXEL, 
				    TEntity::MOVING);*/
    TEntity * e = new TPixmapEntity(300, 250, 0, pixmap, 
				    TEntity::PIXEL, TEntity::BALL);
    e->SetScriptHitCall(hitfunction);
    // TODO, error handling.. 
    e->setMotion(new TFreeMotion);
    //    dynamic_cast<TFreeMotion*>(e->getMotion())->setDir(1 * M_PI / 5);
    dynamic_cast<TFreeMotion*>(e->getMotion())->setDir(2 * M_PI / 5);
    dynamic_cast<TFreeMotion*>(e->getMotion())->setVelocity(0.15);
    e->setName("Default Ball");
    MapState->Entities.push_back(e);
    MapState->num_balls++;
    return true;
  } else {
    LogLine(LOG_WARNING, "TMap::AddEntity - unknown type " + type);
    return false;
  }
}


/* **********************************************************************
 * Set the paddle - should probably mean that we remove the current
 * from the list of entities.
 * *********************************************************************/

bool TMap::SetPaddle(int x, int y, string pathtype, double velocity,
		     double angle, string pixmap) {
  /* Check if the mapstate already have a paddle
     we can not handle that */
  if (MapState->paddle) {
    LogLine(LOG_WARNING, "TMap::SetPaddle - paddle already set");
    return false;
  } 

  /* First, try and make things work */
  TEntity * paddle = 
    new TPixmapEntity(x, y, 0, pixmap, TEntity::BOX, TEntity::PADDLE);
  paddle->setName("Paddle");
  if ("FreeMotion" == pathtype) {
    paddle->setMotion(new TFreeMotion);    
    dynamic_cast<TFreeMotion*>(paddle->getMotion())->setDir(angle);
  } else {
    LogLine(LOG_ERROR, "TMap::SetPaddle - unknown pathtype "  
	    + pathtype);
    delete paddle;
    return false;
  }
  
  /* Set standard parameters */
  paddle->getMotion()->setVelocity(velocity);
  paddle->setName("Paddle");
  MapState->Entities.push_back(paddle);
  MapState->paddle = paddle;
  return true;
}

/* **********************************************************************
 * Update - update all entities
 * *********************************************************************/
void TMap::Update(Uint32 deltatime) {
  TEntitiesIterator i;
  TEntitiesIterator candi;
  for (i = MapState->Entities.begin(); i != MapState->Entities.end(); i++) {
    (*i)->Update(deltatime);

    // Now in order to ensure that the entity list is ordered by
    // y axis we have to check if the current entity should be moved
    // to another pos. in the list. We want to order by y axis 
    // because it speeds up the collision detection.
    candi = i;
    if (i != MapState->Entities.begin())
      --candi;

    if (i != MapState->Entities.end()) {
      if (i != MapState->Entities.begin() ){
	if (relocateHighEntities(i) || relocateLowEntities(i))
	  i = candi;
      }else {
	if (relocateHighEntities(i))
	  i = candi;
      }
    }else {
      if (relocateLowEntities(i)) 
	i = candi;
    }
  }
  /* Clean out entities that are no longer alive */
  TEntity * tmp;
  for (i = MapState->Entities.begin(); i != MapState->Entities.end();) {
    candi = i;
    i++;
    if ((*candi)->IsRemovable()) {
      tmp = (*candi);
      if (TEntity::BALL == tmp->getEntityType()) {
	MapState->num_balls--;
	LogLine(LOG_VERBOSE, "Ball removed");
	Assert(MapState->num_balls >= 0, "Cant have a negative number of balls");
      }
      /* The only stationary entities that are removed are bricks */
      MapState->Entities.erase(candi);
      delete tmp;
    }
  }

}


/* helper function for update
   Moves the entity associated with the iterator i, 
   to the correct position in the upper half of the 
   list from itself.
*/
bool TMap::relocateHighEntities(TEntitiesIterator& i)
{
  TEntitiesIterator newi = i;
  TEntitiesIterator nexti = i;
  ++nexti;
  TEntitiesIterator End = MapState->Entities.end();
  while(++newi != End) {
    if ((*newi)->y() >= (*i)->y()) {
      if (*newi != *nexti) {
	TEntity* tmp = *i;
	i = MapState->Entities.erase(i);
	--i;
	MapState->Entities.insert(newi,1,tmp);
	return true;
      }else{
	return false;
      }
    }
  }
  return false;
}


/* helper function for update
   Moves the entity associated with the iterator i, 
   to the correct position in the lower half of the 
   list from itself.
*/
bool TMap::relocateLowEntities(TEntitiesIterator& i)
{
  TEntitiesIterator newi = i;
  TEntitiesIterator previ = i;
  --previ;
  TEntitiesIterator Begin = MapState->Entities.begin();
  while(--newi != Begin) {
    if ((*newi)->y() <= (*i)->y()) {
      if (*newi != *previ) {
	TEntity* tmp = *i;
	i = MapState->Entities.erase(i);
	--i;
	MapState->Entities.insert(++newi,1,tmp);
	return true;
      }else{
	return false;
      }
    }
  }
  return false;
}

