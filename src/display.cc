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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "display.hh"
#include "screen.hh"
#include "client.hh"
#include "game.hh"
#include "highscore.hh"
#include "console.hh"
#include "text.hh"
#include "surfacemanager.hh"
#include "statistics.hh"
#include "console.hh"

/* **********************************************************************
 * The global display
 * *********************************************************************/

TDisplay * Display;

/* **********************************************************************
 * TDisplay constructor and destructor
 * *********************************************************************/
TDisplay::TDisplay() {
  powerup_pix = "";
  powerup_surf = NULL;
  powerup_src.x = 0; powerup_src.y = 0;
  TextRender = new TText("graphics/fonts/largefont2.png");
}

TDisplay::~TDisplay() {
  if (powerup_surf) {
    SurfaceManager->ReleaseRessource(powerup_surf);
  }
  delete TextRender;
}

/* **********************************************************************
 * Render, gets the gamestate, render it.
 * *********************************************************************/
void TDisplay::Render(SDL_Surface * surface) {
#ifdef DEBUG
  static int ticks = 0;
  static TStatistics Stat(surface, 400);
#endif

  TGameState * GameState = Client->GetGame()->GetState();
  TEntitiesIterator End = GameState->MapState->StationaryEntities.end();
  TEntitiesIterator i;

  for (i = GameState->MapState->StationaryEntities.begin(); i != End; i++) {
    (*i)->Render(surface);
  }
  
  End = GameState->MapState->MovingEntities.end();

  for (i = GameState->MapState->MovingEntities.begin(); i != End; i++) {
    (*i)->Render(surface);
  }

  Uint32 time = SDL_GetTicks();
  for (std::vector<TEffect*>::iterator iter = effects.begin() ;
       iter != effects.end() ; ++iter) {
    if ((*iter)->isStopped()) {
      //      delete (*iter);
      //      iter = effects.erase(iter);
    }else{
      (*iter)->update(surface, time);
    }
  }

  /* **********************************************************************
   * Draw the HUD...
   * *********************************************************************/

  int drawy = 10;
  char buf[20];

  // Draw score
  sprintf(buf, "Score %i", GameState->score);
  // DT_DrawText(buf, surface, 0, 1, drawy);
  TextRender->Print(surface, 1, drawy, buf);
  // Draw lives
  sprintf(buf, "Lives %i", GameState->lives);
  // DT_DrawText(buf, surface, 0, 400-7*10, drawy);
  TextRender->Print(surface, 400-7*10, drawy, buf);

  // Draw time
  int minutes = GameState->gametime/60000;
  int seconds = (GameState->gametime%60000)/1000;
  sprintf(buf, "Time % 2i:%02i", minutes, seconds);
  // DT_DrawText(buf, surface, 0, 800-9*20, drawy);
  TextRender->Print(surface, 800-9*20, drawy, buf);

  // At the center bottom, draw the current powerup, if applicable
  if (GameState->shot_time_left > 0) {
    /* Load the pixmap if we do not already have it */
    if (powerup_pix != GameState->shot_pixmap) {
      if (powerup_surf) {
	SurfaceManager->ReleaseRessource(powerup_surf);
      }
      powerup_surf 
	= SurfaceManager->RequireRessource(GameState->shot_pixmap);
      if (powerup_surf) { /* Fill in values */
	powerup_pix = GameState->shot_pixmap;
      }
    }
    if (powerup_surf) {
      powerup_src.w = static_cast<short unsigned int>(powerup_surf->w);
      powerup_src.h = static_cast<short unsigned int>(powerup_surf->h);
      powerup_dest.x = static_cast<short int>(surface->w / 2 - 50);
      powerup_dest.y = static_cast<short int>(surface->h - (40 + powerup_src.h) / 2);
      powerup_dest.w = powerup_src.w;
      powerup_dest.h = powerup_src.h;
      SDL_BlitSurface(powerup_surf, &powerup_src, surface, &powerup_dest);
    }
    /* Draw the time left for this powerup - add a second to avoid 0*/
    seconds = (GameState->shot_time_left%60000)/1000 + 1;
    sprintf(buf, "%i", seconds);
    /* DT_DrawText(buf, surface, 0, 
		powerup_dest.x + powerup_dest.w + 10, 
		surface->h - (40 + DT_FontHeight(1) ) /2); */
    TextRender->Print(surface, powerup_dest.x + powerup_dest.w + 10, 
		      surface->h - (40 + TextRender->GetGlyphHeight() ) /2, 
		      buf);
  }
  
  /* The rest is drawn at the bottom of the screen, and only for debug
     builds*/
#ifdef DEBUG
  // Draw num_bricks
  sprintf(buf, "Bricks %i", GameState->MapState->num_bricks);
  // DT_DrawText(buf, surface, 0, 800-9*20, surface->h-40);
  TextRender->Print(surface, 800-9*20, surface->h-40, buf);

  // Draw framerate
  int oldticks = ticks;
  ticks = SDL_GetTicks();
  if (ticks != oldticks) {
    sprintf(buf, "%.2f", 1000.0 / (ticks - oldticks));
    // DT_DrawText(buf, surface, 0, 1, surface->h-40);
    TextRender->Print(surface, 1, surface->h-40, buf);
  }
  // Register frametime, Draw stat
  Stat.RegisterFrameTime(ticks - oldticks);
  if (DisplayStat) {
    Stat.Render(200, surface->h);
  }
#endif
}
