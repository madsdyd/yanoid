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

#include <math.h> // defines PI
#include "display.hh"
#include "screen.hh"
#include "client.hh"
#include "game.hh"
#include "highscore.hh"
#include "ConsoleSource/CON_console.h"
#include "ConsoleSource/DT_drawtext.h"

/* **********************************************************************
 * The global display
 * *********************************************************************/

TDisplay * Display;

/* **********************************************************************
 * Render, gets the gamestate, render it.
 * *********************************************************************/
void TDisplay::Render(SDL_Surface * surface) {
  static int ticks = 0;

  TGameState * GameState = Client->GetGame()->GetState();
  TEntitiesIterator End = GameState->MapState->Entities.end();
  TEntitiesIterator i;
  for (i = GameState->MapState->Entities.begin(); i != End; i++) {
    (*i)->Render(surface);
  }

  
  /* **********************************************************************
   * Draw the HUD...
   * *********************************************************************/

  int drawy = 10;
  char framerate[20];

  // Draw score
  sprintf(framerate, "Score %i", GameState->score);
  DT_DrawText(framerate, surface, 1, 1, drawy);
  

  // Draw lives
  sprintf(framerate, "Lives %i", GameState->lives);
  DT_DrawText(framerate, surface, 1, 400-7*10, drawy);

  // Draw time
  int minutes = GameState->gametime/60000;
  int seconds = (GameState->gametime%60000)/1000;
  sprintf(framerate, "Time % 2i:%02i", minutes, seconds);
  DT_DrawText(framerate, surface, 1, 800-9*20, drawy);
  
  /* The rest is drawn at the bottom of the screen */

  // Draw num_bricks
  sprintf(framerate, "Bricks %i", GameState->MapState->num_bricks);
  DT_DrawText(framerate, surface, 1, 800-9*20, surface->h-40);

  // Draw framerate
  int oldticks = ticks;
  ticks = SDL_GetTicks();
  if (ticks != oldticks) {
    sprintf(framerate, "%.2f", 1000.0 / (ticks - oldticks));
    DT_DrawText(framerate, surface, 1, 1, surface->h-40);
  }

  /* **********************************************************************
   * Draw highscore
   * *********************************************************************/
  Highscore->Render(surface);

  /* **********************************************************************
   * Draw Console on top of everything else
   * *********************************************************************/
  if (ConsoleDown) {
    CON_DrawConsole();
  }
}
