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

#undef NO_MUSIC_THREAD

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
#include "soundmanager.hh"

#include "console.hh"
#include "game.hh"
#include "display.hh"
#include "client.hh"
#include "highscore.hh"
#include "interprenter.hh"
#include "entity.hh"
#include "pixmap_entity.hh"
#include "more_entities.hh"

/* **********************************************************************
 * A handler for segmentation errors 
 * *********************************************************************/
void SignalHandler(int signal) {
  cerr << "Received signal " << signal << endl; 
  BackTrace();
  exit(-1);
}

/* **********************************************************************
 * Various commands for the console
 * *********************************************************************/

/* Calls the python interprenter */
void Python(string command) {
  Interprenter->RunSimpleString(command);
}


/* Load a map */
void LoadMap(string map) {
  if (!Client || !(Client->GetGame())) { 
    Console->AddLine("Hmm - no client, or no game...");
  };
  if (Client->GetGame()->LoadMap(map)) {
    // Console->AddLine("Map %s succesfully loaded", map);
    Console->AddLine("Map " + map + " succesfully loaded");
  } else {
    Console->AddLine("Error loading map " + map);
  }
}

/* Prints the string you pass it into the console */
void PrintMe(string arg)
{
  Console->AddLine(arg);
}


#ifdef DEBUG
/* Toogle cheating */
void ToggleCheat(string arg) {
  hole_bounces = !hole_bounces;
  if (hole_bounces) {
    Console->AddLine("Cheat mode enabled");
  } else {
    Console->AddLine("Cheat mode disabled");
  }
}

/* Dup to console or not */
void DupLogToConsole(string arg) {
  duptoconsole = !duptoconsole;
  if (duptoconsole) {
    Console->AddLine("Logging information dupped to console");
  } else {
    Console->AddLine("Logging information not dupped to console");
  }
}
#endif

/* **********************************************************************
 * The main program. Yuhu.
 * *********************************************************************/

int main(int argc, char ** argv) {
  /* **********************************************************************
   * Initialize the logging stuff, if present
   * *********************************************************************/
#ifdef DEBUG
  Log = new TLog();
  Assert(Log != NULL, "Unable to create Log object");
  LogLine(LOG_VERBOSE, "Log object created");
  LogLineExt(LOG_INFO, ("Created object %i", 34));
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
       << "<mads@dydensborg.dk>" << endl 
       << "Copyright (C) 2001 Jonas Christian Drewsen "
       << "<jcd@xspect.dk>" << endl 
       << "Copyright (C) 2001 Bjarke Soerensen "
       << "<bs@wasd.dk" << endl 
       << "Yet Another Arkanoid comes with ABSOLUTLY NO WARRANTY." << endl
       << "This is free software, and you are welcome to "
       << "redistribute it" << endl 
       << "under certain conditions. See the file COPYING for details." 
       << endl;

  /* **********************************************************************
   * Initialize the ressource management system
   * *********************************************************************/
  /* This tabcompleter will be shared with the console */
  TTabComplete * PathCompleter = new TTabComplete();
  Assert(PathCompleter != NULL, "Unable to create pathcompleter");
  LogLine(LOG_VERBOSE, "PathCompleter created");
 
  PathManager = new TPathManager(PathCompleter);
  Assert(PathManager != NULL, "Unable to create pathmanager");
  LogLine(LOG_VERBOSE, "PathManager created");

  /* Add default datapaths */
  PathManager->AddPath(YANOID_DATADIR);
  PathManager->AddPath("data");
  
  /* **********************************************************************
   * Initialize the interprenter (uses the pathmanager)
   * Note, the default scripts and other related stuff, is not
   * run, until we have initialized the console
   * *********************************************************************/
  Interprenter = new TInterprenter();
  LogLine(LOG_VERBOSE, "Interprenter created");

  /* **********************************************************************
   * Initialize SDL
   * *********************************************************************/
#ifdef NO_MUSIC_THREAD
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
#else 
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
#endif
    LogFatal("Unable to initialize SDL: " << SDL_GetError());
    return -1;
  }
  /* Make sure it is shutdown, when we exit */
  Assert(0 == atexit(SDL_Quit), 
	 "Unable to register SDL_Quit with atexit");
  LogLine(LOG_VERBOSE, "SDL initialized");
  /* Set a caption */
  SDL_WM_SetCaption("-- Yanoid --", 0);

  /* **********************************************************************
   * Setup the screen. Resolution is fixed at the moment.
   * Colourdepth is 0 == use current
   * *********************************************************************/
  Screen = SDL_SetVideoMode(800, 600, 0, 
			    SDL_HWSURFACE | SDL_DOUBLEBUF
			    // SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN
			    // SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN
			    );
  // | SDL_FULLSCREEN);
  Assert(Screen != NULL, "Unable to set video mode");

#ifdef DEBUG
  string tmp = "";
  if (SDL_FULLSCREEN & Screen->flags) {
    tmp += "SDL_FULLSCREEN ";
  }
  if (SDL_HWSURFACE & (Screen->flags)) {
    tmp += "SDL_HWSURFACE ";
  }
  if (SDL_SWSURFACE & (Screen->flags)) {
    tmp += "SDL_SWSURFACE ";
  }
  if (SDL_DOUBLEBUF & Screen->flags) {
    tmp += "SDL_DOUBLEBUF ";
  }
  LogLineExt(LOG_VERBOSE, ("Videomode set (%ix%i - %s)", 
			   Screen->w, Screen->h, tmp.c_str()));
#endif

  /* Hide the mouse */
  SDL_ShowCursor(SDL_DISABLE);
  
  /* **********************************************************************
   * Setup the audio. wave configuration is fixed at the moment.
   * *********************************************************************/

  // open 11025 Hz, 8-bit, 1 channel, 512 chunksize
  bool AudioInitialized = false;
#ifndef NO_MUSIC_THREAD
  AudioInitialized = (Mix_OpenAudio(11025, AUDIO_U8, 1, 256) >= 0);
  if (!AudioInitialized) {
    cout << "Unable to open audio" << endl;
  } else { 
    LogLine(LOG_VERBOSE, 
	    "Audiomode set (11025 Hz, 8-bit, 1 ch, 256 byte chunks)");
  }
#endif
  /* **********************************************************************
   * Initialize the surface manager (requires SDL to be initialized and 
   * the video mode to be set!)
   * *********************************************************************/

  SurfaceManager = new TSurfaceManager();
  Assert(PathManager != NULL, "Unable to create SurfaceManager");
  LogLine(LOG_VERBOSE, "SurfaceManager Initialized");

  /* **********************************************************************
   * Try and load the yanoid.png file - if this fails, assume that the 
   * user did not install yanoid and changed dirs before running yanoid.
   * *********************************************************************/
  Splash = SurfaceManager->RequireRessource("graphics/yanoid.png");
  if (!Splash) {
    cerr << "There was a problem loading the yanoid logo. This could indicate "
	 << "that yanoid was not installed properly and you changed directories "
	 << "before running yanoid. If you have choosen not to install yanoid, "
	 << "then please run yanoid from the top level directory of the source "
	 << "distribution, like this: \"src/yanoid\". Thanks. " << endl;
    exit(-1);
  }
  /* **********************************************************************
   * Initialize the music manager (requires SDL to be initialized and 
   * the audio mode to be set!)
   * *********************************************************************/

#ifndef NO_MUSIC_THREAD
  MusicManager = new TMusicManager(AudioInitialized);
  Assert(MusicManager != NULL, "Unable to create MusicManager");
  LogLine(LOG_VERBOSE, "MusicManager Initialized");

#endif
  /* **********************************************************************
   * Initialize the sound manager
   * *********************************************************************/
  SoundManager = new TSoundManager(AudioInitialized);
  Assert(SoundManager != NULL, "Unable to create SoundManager");
  LogLine(LOG_VERBOSE, "SoundManager Initialized");
  
  /* **********************************************************************
   * Initialize the console (requires SDL to be initialized )
   * *********************************************************************/
  
  Console = new TConsole("graphics/fonts/consolefont2.png", 1000, PathCompleter);
  Assert(Console != NULL, "Unable to create console");
  /* Add a number of commands to be treated */
  Console->AddCommand("loadmap", &LoadMap);
  Console->AddCommand("printme", &PrintMe);  
#ifdef DEBUG
  Console->AddCommand("togglecheat", &ToggleCheat);
  Console->AddCommand("duplogtoconsole", &DupLogToConsole);
#endif
  /* Let python handle everything else... */
  Console->AddDefaultCommand(&Python);

  /* **********************************************************************
   * Initialize Display and client;
   * *********************************************************************/
  Client    = new TClient();
  Display   = new TDisplay();
  Highscore = new THighscore(Screen->w / 2  - 160, Screen->h / 2 - 150);

  /* **********************************************************************
   * Add default modules, run default script
   * *********************************************************************/

  TMap::AddModule();
  TGame::AddModule();
  TClient::AddModule();
  Interprenter->LoadDefaultScripts();
  /* Dump the Interprenter help to the console */
  Interprenter->RunSimpleString("help()");
  
  /* **********************************************************************
   * Display a splash screen.
   * *********************************************************************/
  Assert(NULL != Splash, "Error getting SDL_Surface for splash screen");
  SDL_Rect src, dest, erasedest;
  src.x = 0; src.y = 0; src.w = Splash->w; src.h = Splash->h;

  // scroll down splash
  for(int i=0 ; i < (Screen->h-src.h)/32; i++) {
    dest.y = (i*32)/2; 
    dest.x = (Screen->w-src.w)/2;
    dest.w = src.w; dest.h = src.h;
    erasedest = dest;
    erasedest.y = ((i-1)*32)/2;
    SDL_FillRect(Screen, &erasedest, 0x00000000);
    SDL_BlitSurface(Splash, &src, Screen, &dest);
    //    SDL_UpdateRect(Screen, 0, 0, 0, 0);
    SDL_Flip(Screen);
  }

#ifndef NO_MUSIC_THREAD
  /* Start the music */
  MusicManager->PlayMusic("music/yanoid.xm", -1);
#endif 

  /* Let the impression sink in... ;-) */
  SDL_Delay(2000);

  // scroll up splash to make room for menu
  int endi = dest.y - 50;
  for(int i=0 ; i < endi/8; i++) {
    erasedest = dest;
    dest.y -= 8; 
    dest.x = (Screen->w-src.w)/2;
    dest.w = src.w; dest.h = src.h;
    SDL_FillRect(Screen, &erasedest, 0x00000000);
    SDL_BlitSurface(Splash, &src, Screen, &dest);
    //    SDL_UpdateRect(Screen, 0, 0, 0, 0);
    SDL_Flip(Screen);
  }

  /* Run the client */
  Client->Run();

  /* Delete various stuff */
  delete Display;
  delete Highscore;
  delete Client;
  
  // TODO: Freeing surface, etc.
  SurfaceManager->ReleaseRessource(Splash);
  // MusicManager->ReleaseRessource(modmusic);
  /* **********************************************************************
   * Exit gracefully
   * *********************************************************************/
  LogLine(LOG_VERBOSE, "Deleting SoundManager");
  delete SoundManager;
  LogLine(LOG_VERBOSE, "Deleting MusicManager");
  delete MusicManager;
  LogLine(LOG_VERBOSE, "Deleting Console");
  delete Console;
  LogLine(LOG_VERBOSE, "Deleting SurfaceManager");
  delete SurfaceManager;
  LogLine(LOG_VERBOSE, "Deleting PathManager");
  delete PathManager;
  LogLine(LOG_VERBOSE, "Deleting PathCompleter");
  delete PathCompleter;
  LogLine(LOG_VERBOSE, "Deleting Interprenter");
  delete Interprenter;
#ifdef DEBUG
  LogLine(LOG_VERBOSE, "Deleting Log object - Exiting gracefully");
  delete Log;
#endif

  /* Print bye */
  cout << "Thank you for playing Yanoid - see you soon!" << endl;
 
  return 0;
}
