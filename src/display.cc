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
#include "client.hh"
#include "game.hh"
#include "highscore.hh"
#include "ConsoleSource/CON_console.h"
#include "ConsoleSource/DT_drawtext.h"
#include "motion.hh"


#include "screen.hh"
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

  TEntity * paddle = GameState->MapState->GetPaddle();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_LEFT:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( -0.5 );
	dynamic_cast<TFreeMotion*>(paddle->getMotion())->setDir( 0 );
      break;
      case SDLK_RIGHT:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.5 );
	dynamic_cast<TFreeMotion*>(paddle->getMotion())->setDir( 0 );
      break;
      case SDLK_UP:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.5 );
	dynamic_cast<TFreeMotion*>(paddle->getMotion())->setDir( M_PI_2 );	
	break;
      case SDLK_DOWN:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.5 );
	dynamic_cast<TFreeMotion*>(paddle->getMotion())->setDir( -M_PI_2 );
	break;
      case SDLK_ESCAPE:	
	QuitClient = true;
	break;
      case SDLK_BACKQUOTE:
      case SDLK_F1:
      case SDLK_F3:
	if (!ConsoleDown){
	  ConsoleDown = true;
	  SDL_EnableUNICODE(1);
	} else {
	  ConsoleDown = false;
	  SDL_EnableUNICODE(0);
	}
	return;
      case SDLK_F11:
	SDL_Surface * tmp; 
	if (SDL_FULLSCREEN & Screen->flags) {
	  tmp = SDL_SetVideoMode(Screen->w, Screen->h, 0,
				 SDL_HWSURFACE | SDL_DOUBLEBUF);
	} else {
	  tmp = SDL_SetVideoMode(Screen->w, Screen->h, 0,
				 SDL_HWSURFACE | SDL_DOUBLEBUF 
				 | SDL_FULLSCREEN);
	}
	if (tmp) {
	  SDL_FreeSurface(Screen);
	  Screen = tmp;
	}
	break;
      default:
	break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_LEFT:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.0 );
	break;
      case SDLK_RIGHT:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.0 );
	break;
      case SDLK_UP:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.0 );
	break;
      case SDLK_DOWN:
	if (!paddle) break;
	paddle->getMotion()->setVelocity( 0.0 );
	break;
      }
    default:
      break;
    }
    /* If the console is down send events that were not
     * filtered out yet to the console */
    if (ConsoleDown) {
      CON_ConsoleEvents(&event);
    }
  }
  
  // Draw highscore if necessary
  Highscore->Render(surface);

  if (ConsoleDown) {
    CON_DrawConsole();
  }

  // Draw framerate
  int oldticks = ticks;
  char framerate[20];
  ticks = SDL_GetTicks();
  if (ticks != oldticks) {
    sprintf(framerate, "%.2f FPS", 1000.0 / (ticks - oldticks));
    DT_DrawText(framerate, surface, 1, 1, surface->h - 40);
  }

}
