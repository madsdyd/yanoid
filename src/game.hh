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

/* This is the TGameState class, used for passing information from the 
   TGame to the TDisplay class 
   This _WILL_ change. Several times. */
#include <SDL/SDL.h>
#include "log.hh"
#include "map.hh"


class TGameState {
public:
  TMapState * MapState;
  ~TGameState() {
    LogLine(LOG_TODO, "TGameState::~ Clean up stuff?");
  }
};

/* The TGame contains all the game state, etc. */
class TGame {
protected:
  Uint32 lastupdate;
  TMap Map;
  TGameState GameState;
public:
  TGame();
  ~TGame();
  void Update(Uint32 currenttime);
  TGameState * GetState();
};

/* The global game variable */
extern TGame * Game;
