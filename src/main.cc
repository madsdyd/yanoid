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
/* This is the main file for yanoid. It performs the following:

-1) Initialize the log system
0) Initialize SDL (and, potentially other libs)
1) Initialize video
2) Initialize audio
3) Initialize ressource management systems
4) Perform the main menu/game loop
5) Shutdown */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "SDL/SDL.h"
#include "debug.hh"
#include "log.hh"
#include "screen.hh"

int main(int argc, char ** argv) {
  /* **********************************************************************
   * Initialize the logging stuff, if present
   * *********************************************************************/
#ifdef DEBUG
  Log = new TLog();
  Assert(Log != NULL, "Unable to create Log object");
#endif

  /* **********************************************************************
   * Print our version, and so on.
   * *********************************************************************/
  cout << "Yet Another Arkanoid (yanoid) version " << VERSION << endl
       << "Copyright (C) 2001 Mads Bondo Dydensborg "
       << "<mads@dydensborg.dk> et al." << endl 
       << "Yet Another Arkanoid comes with ABSOLUTLY NO WARRANTY." << endl
       << "This is free software, and you are welcome to "
       << "redistribute it" << endl 
       << "under certain conditions. See the file COPYING for details." 
       << endl;

  /* **********************************************************************
   * Initialize SDL
   * *********************************************************************/
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LogLine(LOG_FATAL, "Unable to initialize SDL");
    return -1;
  }
  /* Make sure it is shutdown, when we exit */
  Assert(0 == atexit(SDL_Quit), 
	 "Unable to register SDL_Quit with atexit");
  LogLine(LOG_VERBOSE, "SDL initialized");

  /* **********************************************************************
   * Setup the screen. Resolution is fixed at the moment.
   * Colourdepth is 0 == use current
   * *********************************************************************/
  Screen = SDL_SetVideoMode(800, 600, 0, 
			    SDL_HWSURFACE | SDL_DOUBLEBUF 
			    | SDL_FULLSCREEN);
  Assert(Screen != NULL, "Unable to set video mode");
  LogLine(LOG_VERBOSE, "Videomode set (800x600, fullscreen)");



  /* **********************************************************************
   * Exit gracefully
   * *********************************************************************/
  LogLine(LOG_VERBOSE, "Exiting gracefully");
  return 0;
}
