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

/* **********************************************************************
 * The Run method - basically a wrapper around calls to Display
 * and Game. I Suppose.
 * *********************************************************************/
void TClient::Run() {
  Uint32 ticks = SDL_GetTicks();
  Uint32 start = ticks;
  int count = 0;
  while((ticks-start) < 5000) {
    Game->Update(ticks-start);
    SDL_FillRect(Screen, NULL, SDL_MapRGB(Screen->format, 0, 0, 0));
    Display->Render(Screen);
    SDL_UpdateRect(Screen, 0, 0, 0, 0);
    SDL_Flip(Screen);
    ticks = SDL_GetTicks();
    count++;
  }
  cout << "Info ; " << count << " frames in " << (ticks - start) << " ms "
       << (count/((ticks-start)/1000.0)) << " fps" << endl;
}
