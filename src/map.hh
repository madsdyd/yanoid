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
/* This is Map state - currently a list of entities */

#include "log.hh"
#include "entity.hh"

class TMapState {
public:
  TEntities Entities;
  ~TMapState();
  void load(const std::string& path);
};

/* This is a map clas. It can load a map, and contains the state of a map
 */

class TMap {
protected:
  TMapState MapState;
  bool relocateHighEntities(TEntitiesIterator& i);
  bool relocateLowEntities(TEntitiesIterator& i);
public:
  TMap();
  ~TMap();
  inline TMapState * GetState() { return &MapState; }
  void Update(Uint32 deltatime);
};
