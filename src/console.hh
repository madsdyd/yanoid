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
/* Implement a "quakelike" console */
#ifndef __CONSOLE_HH__
#define __CONSOLE_HH__

/* The purpose of this is - in the long run - to have a "quake" like console.
   For now, it is simply a couple of lines in the upper left corner */
#include <string>
#include <vector>
#include <map>
#include "readline.hh"
#include "tabcomplete.hh"

using namespace std;

typedef void (*TConsoleCommand)(string params);

#include <SDL/SDL.h>
class TText;
class TReadline;
class TConsole {
private:
  TText * TextRender;
  TReadline * Readline;
  TTabComplete * CommandComplete;
  typedef enum {is_up, is_down, is_moving} tconsole_state;
  unsigned int max_num_lines;
  vector<string> lines;
  int fade_time;
  size_t position;  /* The y current position of the console - 0 if up */
  double dposition; /* Used to scale the position, when animating */
  double delta_pos; /* The way we move the console - 0 if not moving */
  tconsole_state state;
  size_t max_pos;   /* The max position  - updated from Render, to the middle 
		    of the y position */
  /* The map of commands */
  map<string, TConsoleCommand> Commands;
  TConsoleCommand DefaultCommand;
  /* Called when a line needs to be handled */
  bool HandleLine(string line);
public:
  TConsole(string fontname, int num_lines, TTabComplete * ArgumentComplete = NULL);
  virtual ~TConsole();
  bool HandleEvent(SDL_Event * event);
  void Update(Uint32 deltatime);
  void Render(SDL_Surface * surface);
  /* Add a line to the console */
  void AddLine(string line);
  /* Used to add a line that will have "extratime" on the console lines 
     Used like this: Clear() - do something that takes a loong time -
     AddLine("something", long_time) - Update() */
  void AddLine(string line, Uint32 extratime);
  /* Take the console up and down */
  void Down();
  void Up();
  /* Clear the console */
  void Clear();
  /* Add a command to be handled to the console */
  void AddCommand(string name, TConsoleCommand command);
  /* Add a default command handler - used when everything else fails */
  void AddDefaultCommand(TConsoleCommand command);
  /* Callback for Readline */
  void CompletionsCallback(TCompletions * completions);
};
#endif

/* **********************************************************************
 * The global console
 * *********************************************************************/
extern TConsole * Console;
