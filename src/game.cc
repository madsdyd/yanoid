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

/* **********************************************************************
 * The global game 
 * *********************************************************************/

TGame * Game;

/* **********************************************************************
 * The constructor currently constructs a single entity
 * *********************************************************************/
TGame::TGame() {
  lastupdate = 0; // TODO: Fix
  GameState.Entities.push_back(new TEntity);
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
  TEntitiesIterator End = GameState.Entities.end();
  TEntitiesIterator i;
  for (i = GameState.Entities.begin(); i != End; i++) {
    (*i)->Update(deltatime);
  }
  lastupdate = currenttime;
}

/* **********************************************************************
 * Get access to the game state
 * *********************************************************************/

TGameState * TGame::GetState() {
  return &GameState;
}
