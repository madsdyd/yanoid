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
#include "pixmap_entity.hh"
#include "globals.hh"
#include "motion.hh"
#include "interprenter.hh"

/* **********************************************************************
 * Testing: A temporary TMap variable to use with some static functions
 * *********************************************************************/
TMap * CurrentMap = NULL;

/* **********************************************************************
 * These static methods are not so great, but for now, they will have to
 * do.
 * *********************************************************************/

/* **********************************************************************
 * Add a brick to the current map
 * *********************************************************************/
static PyObject * AddBrick(PyObject * self, PyObject * args) {
  int x; int y; char * type; char * pixmap;
  if (!CurrentMap || !PyArg_ParseTuple(args, "iiss", &x, &y, &type, &pixmap)) {
    return NULL;
  }
  CurrentMap->AddBrick(x, y, type, pixmap);
  return Py_None;
}

static PyMethodDef map_methods[] = {
  {"AddBrick", AddBrick, 4},
  {NULL, NULL}
};

static bool ConnectedToInterprenter = false;

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
 * load - loads a TMapState and associated entities from path.
 * *********************************************************************/
void TMapState::load(const std::string& path)
{

}

/* **********************************************************************
 * Constructor
 * Currently builds a simple map.
 * *********************************************************************/
TMap::TMap() {

  TEntity* e = new TPixmapEntity(100, 100, 0, "graphics/objects/red_oval_brick.png");
  TOrientedPoint p1(350,100), p2(500,370);
  e->setMotion(new TPathMotion(new TOrientedLinePath(p1,p2), 1.0));
  dynamic_cast<TPathMotion*>(e->getMotion())->setVelocity(8.0);
  MapState.Entities.push_back(e);
  e->setName("Entity 1");

  e = new TPixmapEntity(100, 100, 0, "graphics/objects/red_oval_brick.png");
  TOrientedPoint p1b(250,100), p2b(20,370);
  e->setMotion(new TPathMotion(new TOrientedLinePath(p1b,p2b), 1.0));
  dynamic_cast<TPathMotion*>(e->getMotion())->setVelocity(8.0);
  MapState.Entities.push_back(e);
  e->setName("Entity 1");

  /*
  e = new TPixmapEntity(400, 100, 0, "graphics/objects/weird_brick.png");
  TOrientedPoint bp1(400,100), bp2(0,50);
  e->setMotion(new TPathMotion(new TOrientedLinePath(bp1,bp2), -1.0));
  e->setName("Entity 2");
  MapState.Entities.push_back(e);
  */

  e = new TEntity(0, 0);
  e->setName("Roof");
  e->setW(10);
  e->setH(400);
  MapState.Entities.push_back(e);

  e = new TEntity(630, 0);
  e->setName("Wall 2");
  e->setW(10);
  e->setH(400);
  MapState.Entities.push_back(e);

  e = new TEntity(10, 0);
  e->setName("Roof");
  e->setW(620);
  e->setH(10);
  MapState.Entities.push_back(e);

  e = new TEntity(0, 400);
  e->setName("Floor");
  e->setW(640);
  e->setH(100);
  MapState.Entities.push_back(e);

  paddle = new TPixmapEntity(300, 200, 0, "graphics/objects/weird_brick.png");
  paddle->setMotion(new TFreeMotion);
  paddle->getMotion()->setVelocity(0.0);
  dynamic_cast<TFreeMotion*>(paddle->getMotion())->setDir(0);
  paddle->setName("Paddle");
  MapState.Entities.push_back(paddle);

  e = new TPixmapEntity(240, 50, 0, "graphics/objects/red_oval_brick.png");
  e->setMotion(new TFreeMotion);
  e->getMotion()->setVelocity(0.1);
  dynamic_cast<TFreeMotion*>(e->getMotion())->setDir(20.0);
  MapState.Entities.push_back(e);
  e->setName("Floater 1");

  /*
  for (int x = 0; x < 760; x += 60) {
    for (int y = 0; y < 200; y += 20) {
      MapState.Entities.push_back(new TEntity(x, y));
    }
  }
  */
  
  /* This is probably a stupid way to do it - but at least we make
     sure that we have connected the map handling commands to the 
     interprenter */
  if (!ConnectedToInterprenter) {
    if (Interprenter->AddModule("yanoid_map", map_methods)) {
      ConnectedToInterprenter = true;
      CurrentMap = this;
    }
  }
}

/* **********************************************************************
 * Destructor
 * *********************************************************************/
TMap::~TMap() {
  LogLine(LOG_TODO, "Clean up TMap?");
}

/* **********************************************************************
 * AddBrick 
 * This will need to be refined to allow for different types, etc.
 * *********************************************************************/
void TMap::AddBrick(int x, int y, string type, string pixmap) {
  /* Note, type is ignored for now ... */
  TEntity * e = new TPixmapEntity(x, y, 0, pixmap);
  e->setName("whadaumean I need a name?");
  MapState.Entities.push_back(e);
}

/* **********************************************************************
 * Load a map - set the current pointer, load the python mapname into the
 * interprenter, unset the current pointer, return
 * *********************************************************************/

bool TMap::LoadMap(string mapname) {
  CurrentMap = this;
  bool result = Interprenter->RunSimpleFile(PathManager->Resolve(mapname));
  /* Eventually we may want to unset the current map - currently we just 
     keep it, such that we can do our stuff from the console */
  // CurrentMap = NULL;
  return result;
}

/* **********************************************************************
 * Update - update all entities
 * *********************************************************************/
void TMap::Update(Uint32 deltatime) {
  TEntitiesIterator i;
  for (i = MapState.Entities.begin(); i != MapState.Entities.end(); i++) {
    (*i)->Update(deltatime);

    // Now in order to ensure that the entity list is ordered by
    // y axis we have to check if the current entity should be moved
    // to another pos. in the list. We want to order by y axis 
    // because it speeds up the collision detection.
    TEntitiesIterator candi = i;
    if (i != MapState.Entities.begin())
      --candi;

    if (i != MapState.Entities.end()) {
      if (i != MapState.Entities.begin() ){
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
  TEntitiesIterator End = MapState.Entities.end();
  while(++newi != End) {
    if ((*newi)->y() >= (*i)->y()) {
      if (*newi != *nexti) {
	TEntity* tmp = *i;
	i = MapState.Entities.erase(i);
	--i;
	MapState.Entities.insert(newi,1,tmp);
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
  TEntitiesIterator Begin = MapState.Entities.begin();
  while(--newi != Begin) {
    if ((*newi)->y() <= (*i)->y()) {
      if (*newi != *previ) {
	TEntity* tmp = *i;
	i = MapState.Entities.erase(i);
	--i;
	MapState.Entities.insert(++newi,1,tmp);
	return true;
      }else{
	return false;
      }
    }
  }
  return false;
}

