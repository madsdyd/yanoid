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
#include <signal.h>

#include <SDL/SDL.h>

#include "debug.hh"
#include "log.hh"
#include "screen.hh"
#include "ressourcemanager.hh"
#include "surfacemanager.hh"
#include "musicmanager.hh"
#include "ConsoleSource/CON_console.h"
#include "game.hh"
#include "display.hh"
#include "client.hh"
#include "highscore.hh"

/* **********************************************************************
 * A handler for segmentation errors 
 * *********************************************************************/
void SignalHandler(int signal) {
  cerr << "Received signal " << signal << endl; 
  BackTrace();
  exit(-1);
}

/* Prints the string you pass it into the console */
void PrintMe(char *String)
{
  CON_ConOut("%s", String);
}

/* lets the user change the alpha level */
void AlphaChange(char *alpha)
{
  CON_ConsoleAlpha(atoi(alpha));
  CON_ConOut("Alpha set to %s.", alpha);
}


/* show/hide highscore */
void DisplayHighscore(char *arg)
{
  string _arg(arg);
  if (_arg == "show")
    Highscore->displayRankings();
  else
    Highscore->displayNone();

  CON_ConOut("Highscore %s",arg);
}

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
   * Hangle signals *
   * *********************************************************************/
  signal(SIGSEGV, SignalHandler);

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
   * Initialize SDL
   * *********************************************************************/
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    LogFatal("Unable to initialize SDL: " << SDL_GetError());
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

#ifdef DEBUG
  cout << "Flags : ";
  if (SDL_HWSURFACE & (Screen->flags)) {
    cout << "SDL_HWSURFACE ";
  }
  if (SDL_DOUBLEBUF & Screen->flags) {
    cout << "SDL_DOUBLEBUF ";
  }
  if (SDL_FULLSCREEN & Screen->flags) {
    cout << "SDL_FULLSCREEN ";
  }
  cout << endl;
#endif

  /* **********************************************************************
   * Setup the audio. wave configuration is fixed at the moment.
   * *********************************************************************/

  // open 11025 Hz, 8-bit, 1 channel, 512 chunksize
  Assert( Mix_OpenAudio(11025, AUDIO_U8, 1, 512) >= 0 , 
	  "Unable to set audio mode");
  LogLine(LOG_VERBOSE, 
	  "Audiomode set (11025 Hz, 8-bit, 1 ch, 512 byte chunks)");
  
  /* **********************************************************************
   * Initialize the surface manager (requires SDL to be initialized and 
   * the video mode to be set!)
   * *********************************************************************/

  SurfaceManager = new TSurfaceManager();
  Assert(PathManager != NULL, "Unable to create SurfaceManager");
  LogLine(LOG_VERBOSE, "SurfaceManager Initialized");

  /* A small test */
  SDL_Surface * mysurf 
    = SurfaceManager->RequireRessource("graphics/yanoid.png");
  Assert(NULL != mysurf, "Error getting SDL_Surface *");
  
  /* **********************************************************************
   * Initialize the music manager (requires SDL to be initialized and 
   * the audio mode to be set!)
   * *********************************************************************/

  MusicManager = new TMusicManager();
  Assert(PathManager != NULL, "Unable to create MusicManager");
  LogLine(LOG_VERBOSE, "MusicManager Initialized");

  /* A small MOD test */
  Mix_Music * modmusic
    = MusicManager->RequireRessource("music/yanoid.mod");
  Assert(NULL != modmusic, "Error getting Mix_Music *");

  /* A small ogg vorbis test */
  Mix_Music * oggmusic
    = MusicManager->RequireRessource("sounds/yanoid.ogg");
  Assert(NULL != oggmusic, "Error getting Mix_Music *");

  /* **********************************************************************
   * Initialize the console (requires SDL to be initialized )
   * *********************************************************************/

  SDL_Rect Con_Init = { 0, 0, 800, 300 };
  
  /* Init the console */
  // hacky
  Assert(CON_ConsoleInit(PathManager->Resolve("graphics/fonts/ConsoleFont.bmp").c_str(), 
			 Screen, 100, Con_Init) == 0,
	 "Unable to initialize console");
  
  /* Add some commands to the console */
  CON_AddCommand(&PrintMe, "printme");
  CON_AddCommand(&AlphaChange, "alpha");
  CON_AddCommand(&DisplayHighscore, "highscore");
  
  CON_ListCommands();
  
  CON_ConsoleAlpha(190);
  
  /* **********************************************************************
   * TEST - initialize game, display and client.
   * *********************************************************************/
  Game    = new TGame();
  Display = new TDisplay();
  Highscore = new THighscore(Screen->w / 2  - 160, Screen->h / 2 - 150);

  TClient * Client = new TClient();
  

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
  SDL_Delay(2000);

  // Test the MOD music
  if ( ! Mix_PlayingMusic() ) {
    Mix_PlayMusic(modmusic, 0);
  }

  // Test the ogg vorbis music
  if ( ! Mix_PlayingMusic() ) {
    //    Mix_PlayMusic(oggmusic, 0);
  }

  // Test the client
  Client->Run();

  // Delete the stuff
  delete (Client);
  delete (Display);
  delete (Game);
  
  // TODO: Freeing surface, etc.
  SurfaceManager->ReleaseRessource(mysurf);
  LogLine(LOG_TODO, "Seems music is not released");
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
