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
#include "screen.hh"
#include "game.hh"
#include "display.hh"

/* Setup for max deltaticks */ 
static const Uint32 max_deltaticks = 10;

/* **********************************************************************
 * The Run method - basically a wrapper around calls to Display
 * and Game. I Suppose.
 * *********************************************************************/
void TClient::Run() {
  Uint32 start = SDL_GetTicks();
  Uint32 deltaticks = 0;
  Uint32 lastupdate = 0;
  int count = 0;
  lastupdate = 0;
  /* Make sure the game is updated at time 0 */
  Game->Update(lastupdate);
  while(!QuitGame) {

    /* Update the game state in max_deltaticks ms at a time */
    while (deltaticks > max_deltaticks) {
      lastupdate += max_deltaticks;
      Game->Update(lastupdate);
      deltaticks -= max_deltaticks;
    }
    if (deltaticks != 0) {
      lastupdate += deltaticks;
      Game->Update(lastupdate);
    }
    
    /* Get the system to display the game state */
    SDL_FillRect(Screen, NULL, SDL_MapRGB(Screen->format, 0, 0, 0));
    Display->Render(Screen);
    SDL_Flip(Screen);
    
    /* Count the number of frames */
    count++;
    
    /* Update the deltatick */
    deltaticks = SDL_GetTicks() - start - lastupdate;
  }
  cout << "Info ; " << count << " frames in " << lastupdate << " ms "
       << (count/(lastupdate/1000.0)) << " fps" << endl;
}
