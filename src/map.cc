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
  for (int x = 0; x < 760; x += 60) {
    for (int y = 0; y < 200; y += 20) {
      MapState.Entities.push_back(new TEntity(x, y));
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
 * Get the mapstate
 * *********************************************************************/
TMapState * TMap::GetState() {
  return &MapState;
}

/* **********************************************************************
 * Update - update all entities
 * *********************************************************************/
void TMap::Update(Uint32 deltatime) {
  TEntitiesIterator End = MapState.Entities.end();
  TEntitiesIterator i;
  for (i = MapState.Entities.begin(); i != End; i++) {
    (*i)->Update(deltatime);
  }
}
