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
#include "map.hh"

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
 * Constructor
 * Currently builds a simple map.
 * *********************************************************************/
TMap::TMap() {
  TEntity* e = new TEntity(100, 100);
  e->setVelocity(1,0);
  MapState.Entities.push_back(e);
  e->setName("Entity 1");

  e = new TEntity(400, 100);
  e->setVelocity(-1,0);
  e->setName("Entity 2");
  MapState.Entities.push_back(e);

  e = new TEntity(0, 100);
  e->setVelocity(0,0);
  e->setName("Wall 1");
  MapState.Entities.push_back(e);

  e = new TEntity(500, 100);
  e->setVelocity(0,0);
  e->setName("Wall 2");
  MapState.Entities.push_back(e);
  /*
  for (int x = 0; x < 760; x += 60) {
    for (int y = 0; y < 200; y += 20) {
      MapState.Entities.push_back(new TEntity(x, y));
    }
  }
  */
}

/* **********************************************************************
 * Destructor
 * *********************************************************************/
TMap::~TMap() {
  LogLine(LOG_TODO, "Clean up TMap?");
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
    // to another pos. in the list.
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


// helper function for update
bool TMap::relocateHighEntities(TEntitiesIterator& i)
{
  TEntitiesIterator newi = i;
  TEntitiesIterator nexti = i;
  ++nexti;
  TEntitiesIterator End = MapState.Entities.end();
  while(++newi != End) {
    if ((*newi)->y() >= (*i)->y() && newi != nexti) {
      TEntity* tmp = *i;
      i = MapState.Entities.erase(i);
      --i;
      MapState.Entities.insert(newi,1,tmp);
      return true;
    }
  }
  return false;
}


// helper function for update
bool TMap::relocateLowEntities(TEntitiesIterator& i)
{
  TEntitiesIterator newi = i;
  TEntitiesIterator previ = i;
  --previ;
  TEntitiesIterator Begin = MapState.Entities.begin();
  while(--newi != Begin) {
    if ((*newi)->y() <= (*i)->y() && newi != previ) {
      TEntity* tmp = *i;
      i = MapState.Entities.erase(i);
      --i;
      MapState.Entities.insert(++newi,1,tmp);
      return true;
    }
  }
  return false;
}

