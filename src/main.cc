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

-3) Initialize the log system
-2) Print version information
-1) Initialize ressource management systems
0) Initialize SDL (and, potentially other libs)
1) Initialize video
2) Initialize audio
3) 
4) Perform the main menu/game loop
5) Shutdown (Delete ressources) */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "SDL/SDL.h"
#include "debug.hh"
#include "log.hh"
#include "screen.hh"
#include "ressourcemanager.hh"
#include "surfacemanager.hh"

int main(int argc, char ** argv) {
  /* **********************************************************************
   * Initialize the logging stuff, if present
   * *********************************************************************/
#ifdef DEBUG
  Log = new TLog();
  Assert(Log != NULL, "Unable to create Log object");
  LogLine(LOG_VERBOSE, "Log object created");
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
   * Initialize the ressource management system
   * *********************************************************************/

  PathManager = new TPathManager();
  Assert(PathManager != NULL, "Unable to create pathmanager");
  LogLine(LOG_VERBOSE, "PathManager created");

  /* Add default datapaths */
  PathManager->AddPath(YANOID_DATADIR);
  PathManager->AddPath("data");
  
  /* A small test */
  /*
    LogLine(LOG_INFO, "Makefile == " + PathManager->Resolve("Makefile"));
    LogLine(LOG_INFO, "foobar == " + PathManager->Resolve("foobar"));
    LogLine(LOG_INFO, "/graphics/foobar == " 
    + PathManager->Resolve("/graphics/foobar"));
    LogLine(LOG_INFO, "graphics/foobar == " 
    + PathManager->Resolve("graphics/foobar"));
  */

  /* **********************************************************************
   * Initialize the surface manager
   * *********************************************************************/

  SurfaceManager = new TSurfaceManager();
  Assert(PathManager != NULL, "Unable to create SurfaceManager");
  LogLine(LOG_VERBOSE, "SurfaceManager Initialized");

  /* A small test */
  THandle tmp = SurfaceManager->RequireHandle("graphics/yanoid.png");
  Assert(0 != tmp, "Error getting handle to surface");
  SDL_Surface * mysurf = SurfaceManager->RequireRessource(tmp);
  Assert(NULL != mysurf, "Error getting SDL_Surface *");
  

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
   * Do nothing for at very short time
   * *********************************************************************/
  /* More testing */
  SDL_Rect src, dest;
  src.x = 0; src.y = 0; src.w = mysurf->w; src.h = mysurf->h;
  dest.x = (Screen->w-src.w)/2; dest.y = (Screen->h-src.h)/2;
  dest.w = src.w; dest.h = src.h;
  SDL_BlitSurface(mysurf, &src, Screen, &dest);
  SDL_UpdateRect(Screen, 0, 0, 0, 0);
  SDL_Delay(3000);

  // TODO: Freeing surface, etc.


  /* **********************************************************************
   * Exit gracefully
   * *********************************************************************/

  LogLine(LOG_VERBOSE, "Deleting SurfaceManager");
  delete(SurfaceManager);
  LogLine(LOG_VERBOSE, "Deleting PathManager");
  delete(PathManager);
#ifdef DEBUG
  LogLine(LOG_VERBOSE, "Deleting Log object - Exiting gracefully");
  delete(Log);
#endif

  return 0;
}
