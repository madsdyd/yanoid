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
#include "client.hh"

#include "log.hh"
#include "debug.hh"
#include "screen.hh"
#include "ConsoleSource/CON_console.h"
#include "game.hh"
#include "display.hh"

#include "menu.hh"

#include "motion.hh"
#include "ConsoleSource/CON_console.h"

/* **********************************************************************
 * The global client
 * *********************************************************************/
TClient * Client;

/* Setup for max deltaticks */ 
static const Uint32 max_deltaticks = 10;

/* **********************************************************************
 * Constructor, destructor
 * *********************************************************************/
TClient::TClient() : Game(NULL), paused(0) {};

TClient::~TClient() {
  if (Game) {
    delete Game;
  }
}

/* **********************************************************************
 * The pause stuff
 * *********************************************************************/
void TClient::PauseGame() {
  if (0 == paused) {
    pausetime = SDL_GetTicks();
  }
  paused++;
}

void TClient::ContinueGame() {
  paused--;
  if (0 == paused) {
    pausetime = SDL_GetTicks() - pausetime;
    game_start += pausetime;
  }
}

/* **********************************************************************
 * The Run method
 * Keeps running, until the player exits the client
 * Will (for now) create a default game, if none is present
 * Will load a sample map
 * *********************************************************************/
void TClient::Run() {
  /* The client is a wrapper around the total game. 
     But, there are two types of "games". The whole "session", 
     and a single "game".
     The client represents the session. As long as it is running, 
     something will go onto the screen, input will be handled, etc.
     However, when the client is exicted, the game ends. */

  TPreGameMenu * MyPre = new TPreGameMenu();

  QuitClient = !MyPre->Run();
  
  while(!QuitClient) {

    /* Make sure we have a valid game (This will go, when
       the menu system and such is in place -- I think. Hmm. */
    if (!Game) {
      Game = new TGame();
      Assert(Game != NULL, "Could not create a default game");
      /* Load a default map for now */
      if (Game->LoadMap("maps/map1.py")) {
	CON_ConOut("Map maps/map1.py succesfully loaded");
      } else {
	CON_ConOut("Error loading map maps/map1.py");
      }
      /* Initialize the timer stuff */
      game_start      = SDL_GetTicks();
      game_lastupdate = 0;
      LogLine(LOG_VERBOSE, "Game->Update(0)");
      Game->Update(0);
    }

    /* Update the game state */
    UpdateGame();
    
    /* Get the system to display the game state */
    Render();

    /* Handle SDL events */
    HandleEvents();

    /* If we are not in a game, show the menu, or something ... */

  }

  delete MyPre;
}

/* **********************************************************************
 * GameUpdate updates the time in the game
 * *********************************************************************/
void TClient::UpdateGame() {
  if (paused > 0) { return; };
  /* Update the deltatick */
  Uint32 deltaticks = SDL_GetTicks() - game_start - game_lastupdate;
  
  /* Update the game state in max_deltaticks ms at a time */
  while (deltaticks > max_deltaticks) {
    game_lastupdate += max_deltaticks;
    Game->Update(game_lastupdate);
    deltaticks -= max_deltaticks;
  }
  if (deltaticks != 0) {
    game_lastupdate += deltaticks;
    Game->Update(game_lastupdate);
  }
}

/* **********************************************************************
 * Render calls the appropiate modules for rendering
 * *********************************************************************/
void TClient::Render() {
  SDL_FillRect(Screen, NULL, SDL_MapRGB(Screen->format, 0, 0, 0));
  Display->Render(Screen);
  SDL_Flip(Screen);
}

/* **********************************************************************
 * HandleEvents handles SDL events.
 * *********************************************************************/
bool TClient::HandleGlobalKeys(SDL_Event * event) {
  switch (event->type) {
  case SDL_KEYDOWN:
    switch (event->key.keysym.sym) {
      /* The in game menu is bound to escape */
    case SDLK_ESCAPE: {	
      TInGameMenu * InGameMenu = new TInGameMenu(this);
      PauseGame();
      QuitClient = InGameMenu->Run();
      ContinueGame();
      delete InGameMenu;
      return true;
    }
    /* The console is bound to a number of keys */
    case SDLK_BACKQUOTE:
    case SDLK_F1:
    case SDLK_F3:
      ToggleConsole();
      return true;
      /* FullScreenToggle is bound to F11 */
    case SDLK_F11:
      SDL_WM_ToggleFullScreen(Screen);
      return true;
    }
  }
  /* Falling true - unhandled */
  return false;
}

void TClient::HandleEvents() {
  TEntity * paddle = Game->GetState()->MapState->GetPaddle();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    /* Check for global events */
    if (HandleGlobalKeys(&event)) {
      break;
    }
    /* Unhandled */
    /* If the console is down send events to it */
    if (ConsoleDown) {
      CON_ConsoleEvents(&event);
    } else {
      /* Treat events */
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
    } /* Console up */
  }
}

/* **********************************************************************
 * ToggleConsole - pauses the game, when the console is displayed.
 * *********************************************************************/
void TClient::ToggleConsole() {
  ConsoleDown = !ConsoleDown;
  if (ConsoleDown){
    /* Was up, has been moved down */
    PauseGame();
    SDL_EnableUNICODE(1);
  } else {
    /* Was down, has been moved up */
    SDL_EnableUNICODE(0);
    ContinueGame();
  }
}
