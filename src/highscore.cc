/*
    Yet Another Arkanoid
    Copyright (C) 2001 Mads Bondo Dydensborg <mads@dydensborg.dk>
    Copyright (C) 2001 Jonas Christian Drewsen <jcd@xspect.dk>
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

#include "highscore.hh"
#include "log.hh"
#include "debug.hh"
#include "ConsoleSource/DT_drawtext.h"
#include <sstream>
#include <iomanip.h>

/* **********************************************************************
 * The global highscore
 * *********************************************************************/

THighscore * Highscore;

/* **********************************************************************
 * The constructor
 * *********************************************************************/

THighscore::THighscore(int x_, int y_): 
  TEntity(x_,y_), DisplayMode(NONE), NumRankings(10)
{
  LogLine(LOG_TODO, "Clean up THighscore contructor");
  Score s = 42;
  for (int i = 0 ; i < 10 ; ++i) {
    Rankings.push_back(make_pair(string("Julemanden"),s--)); 
  }
  // load font transparent 1, or solid 0
  fontHandle = DT_LoadFont("../data/graphics/fonts/LargeFont.bmp", 0);
  Assert(fontHandle != -1, "Unable to load highscore fontmap.");
}

/* **********************************************************************
 * Destructor, clean up... 
 * *********************************************************************/
THighscore::~THighscore()
{
  LogLine(LOG_TODO, "Clean up THighscore destructor");
}

/* **********************************************************************
 * Update calculates difference.
 * *********************************************************************/
void THighscore::Update(Uint32 currenttime) {
  static Uint32 lastupdate = 0;
  //  Uint32 deltatime = currenttime - lastupdate;
 
  lastupdate = currenttime;
}

/* **********************************************************************
 * Render will render the highscore according to the current
 * state of the highscore
 * *********************************************************************/
void THighscore::Render(SDL_Surface * surface)
{
  switch (DisplayMode) {
  case HIGHSCORE: {
    //    LogLine(LOG_VERBOSE, "Displaying highscore rankings");
    int drawx=x;
    int drawy=y;
    unsigned int count = 0;
    for(std::list<pair<std::string, Score> >::const_iterator i = 
	  Rankings.begin(); 
	i != Rankings.end() && count < NumRankings; ++i, ++count ) {
      std::ostringstream RankLine;
      RankLine << setw(20) << i->first 
	       << setw(10) << setiosflags(ios::right) << i->second;
      cerr << RankLine.str() << endl;
      DT_DrawText(RankLine.str().c_str(), surface, fontHandle, drawx, drawy);
      drawy += 20;
    }
  }
  break;
  case INPUT: {
    LogLine(LOG_VERBOSE, "Displaying highscore input");
  }
  break;
  case NONE: {
    ;
  }
  break;
  default:
    LogLine(LOG_VERBOSE, "Got unknown display mode during display "
	    "of highscore");
  }
}

/* **********************************************************************
 * Tell whether a score of s, is enough to get on the highscore 
 * *********************************************************************/
bool THighscore::isCandidate(const Score& s)
{
  for(std::list<pair<std::string, Score> >::iterator i = Rankings.begin() ;
      i != Rankings.end() ; ++i) {
    if (i->second < s)
      return true;
  }  
  return false;
}

/* **********************************************************************
 * Update will insert the name and score into the correct position
 * in the rankings. Will return true on success;
 * *********************************************************************/
bool THighscore::update(const std::string& name, const Score& s)
{
  string realname = name;
  if (name.empty())
    realname = "John D.";

  bool success = false;
  unsigned int count = 0;
  for(std::list<pair<std::string, Score> >::iterator i = Rankings.begin() ;
      i != Rankings.end() ; ++i, ++count) {

    // Ensure that we have only MAX_RANKINGS rankings in list
    if (count == MAX_RANKINGS) {
      Rankings.erase(i, Rankings.end());
      break;
    }

    if (i->second < s) {
      Rankings.insert(i,1,make_pair(realname,s));
      success = true;
    }
  }
  return success;
}

/* **********************************************************************
 * DisplayNameInput enables displaying of user interface for inputting
 * username. This will disable all other display modes set by THighscore
 * *********************************************************************/
void THighscore::displayNameInput()
{
  DisplayMode = INPUT;
}


/* **********************************************************************
 * DisplayRankings enables displaying of the highscore rankings.
 * This will disable all other display modes set by THighscore
 * *********************************************************************/
void THighscore::displayRankings()
{
  DisplayMode = HIGHSCORE; 
}

/* **********************************************************************
 * DisplayNone will disable all display modes set by THighscore
 * *********************************************************************/
void THighscore::displayNone()
{
  DisplayMode = NONE; 
}
