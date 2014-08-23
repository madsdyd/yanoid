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
#include <math.h>
#include "log.hh"
#include "console.hh"
#include "text.hh"
#include "readline.hh"
#include "tabcomplete.hh"

#define FADE_DELTA 5000 /* Every line is on screen for this long */
#define NUM_LINES_SHOW 4
/* The speed when moving up/down */
#define DELTA_POS 0.100
/* Min and max */
#define mymin(a,b) (((a)<(b))?(a):(b))
#define mymax(a,b) (((a)>(b))?(a):(b))
#define mymax3(a,b,c) (mymax((a),mymax((b),(c))))

#if !defined(rint)
#define rint(x) floor((x) + .5)
#endif

/* **********************************************************************
 * The global console
 * *********************************************************************/
TConsole * Console = NULL;

/* **********************************************************************
 * Constructor & destructor
 * *********************************************************************/
TConsole::TConsole(string fontname, int num_lines, 
		   TTabComplete * ArgumentComplete) {
  TextRender      = new TText(fontname);
  CommandComplete = new TTabComplete();
  Readline        = new TReadline(num_lines/10, CommandComplete, ArgumentComplete);
  /* Start the readline */
  Readline->Readline();
  max_num_lines = num_lines;
  fade_time     = 0;
  position      = 0;
  dposition     = 0.0;
  delta_pos     = 0;
  state         = is_up;
  max_pos       = 0;
  DefaultCommand = NULL;
}

TConsole::~TConsole() {
  delete TextRender;
  delete Readline;
  /* Delete this after ReadLine */
  delete CommandComplete;
}

/* **********************************************************************
 * The update function mainly makes sure the display lines time out,
 * or move the
 * *********************************************************************/
void TConsole::Update(Uint32 deltatime) {
  /*
  cout << "enter: deltatime: " << deltatime << ", position : " << position 
       << ", delta_pos : " << delta_pos << ", dposition : " << dposition << endl; 
  */
  /* Control the fade time */
  fade_time = fade_time - deltatime;
  if (0 > fade_time) {
    fade_time = 0;
  }
  /* And the position of the console */
  if (delta_pos != 0) { /* We need to move the background */
    state = is_moving;
    if (delta_pos < 0) {
      /* Moving up */
      dposition = mymax (0, dposition+delta_pos*deltatime);
      position = (int) floor(dposition);
      if (0 == position) {
	dposition = 0.0;
	delta_pos = 0;
	state = is_up;
	// LogLine(LOG_VERBOSE, "Console is UP");
      }
    } else {
      /* Moving down */
      dposition = mymin (static_cast<double>(max_pos), dposition+delta_pos*deltatime);
      position = (int) floor(dposition);
      if (max_pos == position) {
	dposition = static_cast<double>(max_pos);
	delta_pos = 0;
	state = is_down;
	// LogLine(LOG_VERBOSE, "Console is DOWN");
      }
    }
  }
  /*  cout << "exit: position : " << position << ", delta_pos : " << delta_pos 
      << ", dposition : " << dposition << endl; */
}
/* **********************************************************************
 * HandleLine - called when a line is ready in readline
 * *********************************************************************/
bool TConsole::HandleLine(string line) {
  /* Readline accepted a line - check if we can handle it */
  size_t firstspace = line.find(" ");
  string name, arg;
  if (firstspace < 0) {
    name = line;
    arg = "";
  } else {
    name = line.substr(0, firstspace);
    arg 
      =  line.substr(firstspace + 1, 
		     line.size() 
		     - (firstspace + 1));
  }
  // LogLineExt(LOG_VERBOSE, ("Got '%s' '%s'", name.c_str(), arg.c_str()));
  /* Add the line to the history */
  AddLine(line);
  map<string,TConsoleCommand>::iterator i = Commands.find(name);
  if (i != Commands.end()) {
    // LogLineExt(LOG_VERBOSE, ("Looked up %s", name.c_str()));
    (*i).second(arg);
  } else {
    if (DefaultCommand) {
      DefaultCommand(line);
    }
  }
  /* Start reading a new one */
  /* Hmm */
  return true;
}

/* **********************************************************************
 * Handleevents - just forward to Readline for now ... 
 * *********************************************************************/
bool TConsole::HandleEvent(SDL_Event * event) {
  /* If escape, abort the current readline. I think */
  bool handled = Readline->HandleEvent(event);
  /* Check if it completed reading a line */
  if (!Readline->IsReadingLine()) {
    HandleLine(Readline->GetCurrentLine());
    /* Start reading a new line */
    Readline->Readline();
  }
  /* Check if there are completions here ... */
  if (Readline->HasCompletions()) {
    LogLine(LOG_VERBOSE, "Completions now!");
    TCompletions * tmp = Readline->GetCurrentCompletions();
    TCompletionsIterator i;
    TCompletionsIterator End = tmp->end();
    for (i = tmp->begin(); i != End; i++) {
      AddLine((*i));
    }
  }
  return handled;
}

/* **********************************************************************
 * The render function renders the lines that are currently
 * visible. It can be in one of two modes, displaying only a couple of
 * lines with no background or a background with several lines of text
 * *********************************************************************/
void TConsole::Render(SDL_Surface * surface) {
  /* Update maxpos */
  max_pos = surface->h/2;
  size_t i = 0;
  size_t y;

  if (0 != position) {
    /* render with background */
    i = mymin( position / TextRender->GetGlyphHeight(), 
	      lines.size());
    /* Start printing from the top of the screen, or lower, if fewer lines
     */
    y = position - i * TextRender->GetGlyphHeight();
    /*display->textrender->Pos(0, display->GetHeight() 
			     - position
			     + i*display->textrender->size); */
    /* OK, this is hackish, because my own code is confusing me, and I dont 
       really care to tidy it up, because I am not only lazy but sleepy as well. */
    SDL_Rect dest;
    dest.y = 0;
    dest.x = 0;
    dest.w = static_cast<short unsigned int>(surface->w);
    dest.h = static_cast<short unsigned int>(position + 2 * TextRender->GetGlyphHeight());
    SDL_FillRect(surface, &dest, SDL_MapRGB(surface->format, 0, 0, 0));
  } else {
    // cout << "fadetime " << fade_time << endl;
    /* render without background */
    i   = mymin((int) lines.size(), 
		(int) rint(fade_time/FADE_DELTA));
    /* Start printing from the top of the screen */
    y = TextRender->GetGlyphHeight();
    /* display->textrender->Pos(0, display->GetHeight() 
       - display->textrender->size); */
  }
  while(i > 0) {
    // display->textrender->PrintLn(lines[lines.size()-i--]);
    TextRender->Print(surface, 0, static_cast<int>(y), lines[lines.size()-i--]);
    y += TextRender->GetGlyphHeight();
  }
  if (0 != position) {
    Readline->Render(surface, 0, static_cast<int>(y), "> ", TextRender);
    y += TextRender->GetGlyphHeight();
    TextRender->Print(surface, 0, static_cast<int>(y), "--------------------------------------------------");
    // display->textrender->PrintLn("--------------------------------------------------------");
  }
}

/* **********************************************************************
 * Add a line to all the lines
 * *********************************************************************/
void TConsole::AddLine(string line) {
  /* Check if we need to remove a line */
  while (lines.size() >= max_num_lines) {
    lines.erase(lines.begin());
  }
  /* Add it */
  lines.push_back(line);
  /* Update the timer */
  fade_time += FADE_DELTA;
  fade_time  = mymin(fade_time, NUM_LINES_SHOW * FADE_DELTA);  
#ifdef DEBUG
  // LogLineExt(LOG_VERBOSE, ("Coonsole: %s", line.c_str()));
#endif
}

void TConsole::AddLine(string line, Uint32 extratime) {
  AddLine(line);
  /* This could cause problems with too many lines beeing displayed */
  fade_time += extratime;
}

/* **********************************************************************
 * Up and down
 * *********************************************************************/
void TConsole::Down() {
  // AddLine("Going down");
  delta_pos = DELTA_POS;
}

void TConsole::Up() {
  // AddLine("Going up");
  delta_pos = -DELTA_POS;
  
}

/* **********************************************************************
 * Clear
 * *********************************************************************/
void TConsole::Clear() {
  fade_time = 0;
}

/* **********************************************************************
 * AddCommand
 * *********************************************************************/
void TConsole::AddCommand(string name, TConsoleCommand command) {
  Commands[name] = command;
  CommandComplete->Insert(name);
}
void TConsole::AddDefaultCommand(TConsoleCommand command) {
  DefaultCommand = command;
}

/* **********************************************************************
 * Handling commands
 * *********************************************************************/
/*
bool TConsole::CommandConsume(TCommand * Command) {
    if ("console-toggle-display" == Command->name) {
    if (is_up == state) {
      Down();
    } else if (is_down == state) {
      Up();
    } else {
      // AddLine("Toogling delta");
      delta_pos = -delta_pos;
    }
    return true;
  }
  return false;
}
*/
