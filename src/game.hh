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

#include <SDL/SDL.h>
#include "log.hh"
#include "map.hh"
#include <vector>

/* This is the TGameState class, used for passing information from the 
   TGame to the TDisplay class 
   This _WILL_ change. Several times. */
class TGameState {
public:
  TMapState * MapState;
  /* This stuff will move/change */
  typedef enum { CUT, PLAYING, MAPDONE, DEAD } StatusType;
  StatusType status;
  int lives; /* This will go/move somewhere else */
  int score;
  int currentmap;
  Uint32 gametime;
  TGameState() {
    /* We start in cut, 5 lives, no score */
    status = CUT;
    lives = 2;
    score = 0;
    currentmap = 0;
  }
  ~TGameState() {
    LogLine(LOG_TODO, "TGameState::~ Clean up stuff?");
  }
};

/* The TGame contains all the game state, etc. */
class TGame {
protected:
  Uint32 lastupdate;
  /* used to store time that is skipped - add to lastupdate */
  Uint32 pausetime;
  TMap * Map;
  std::vector<std::string> map_names;
  TGameState GameState;
  static bool ModuleAdded;
  void handleCollisions(Uint32 currenttime);
public:
  TGame();
  ~TGame();
  static bool AddModule();
  std::string GetMapName(int i) const{ return map_names[i]; }
  void AddMapName(const std::string& n) { map_names.push_back(n); }
  bool HasMap(unsigned int i) { return map_names.size() > i; }
  void Update(Uint32 currenttime);
  TGameState * GetState();
  /* Safely load another map */
  bool LoadNextMap();
  bool LoadMap(string mapname);
};

