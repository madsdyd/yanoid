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
#include "readline.hh"
#include "log.hh"
#include "text.hh"
#include "tabcomplete.hh"

string TReadline::killed = "";

/* **********************************************************************
 * Constructor
 * *********************************************************************/
TReadline::TReadline(int backlogsize, TTabComplete * _Command, TTabComplete * _Arg)
  : cursorpos(0), savedpos(0), 
  nextbacklogcommand(0), maxlinesinbacklog(backlogsize), 
  isreadingline(false),
  CommandComplete(_Command), ArgumentComplete(_Arg),
  LastEventWasTab(false) {
};
/* **********************************************************************
 * Adjust the cursor
 * *********************************************************************/
void TReadline::AdjustCursor() {
  /* Adjust the cursor - should probably be done in HandleEvent */
  if (cursorpos < 0) {
    cursorpos = 0;
  }
  if (cursorpos > static_cast<int>(currentline.size())) {
    cursorpos = currentline.size();
  }
}

/* **********************************************************************
 * Render
 * *********************************************************************/
void TReadline::Render(SDL_Surface * surface, int x, int y, 
		  string prompt, TText * textrender) {
  string line_to_render = prompt;
  line_to_render 
    += currentline.substr(0, cursorpos) 
    + "|" + currentline.substr(cursorpos, currentline.size() - cursorpos);
  textrender->Print(surface, x, y, line_to_render);
}

bool TReadline::HandleEvent(SDL_Event * event) {
  if (SDL_KEYDOWN != event->type) {
    return false;
  }
  if (!IsReadingLine()) {
    return false;
  }
  /* If CTRL is pressed, we handle that first... 
     Note, that some of this is duplicated below, to handle HOME, END, 
     DELETE & BACKSPACE
     Lazy me.
*/
  if (event->key.keysym.mod & KMOD_CTRL) {
    LastEventWasTab = false;
    //LogLine(LOG_INFO, "Setting LastEventWasTab to false - 1");
    switch(event->key.keysym.sym) {
    case SDLK_a: { /* Start of line */
      cursorpos = 0;
      return true;
    }
    case SDLK_e: { /* End of line */
      cursorpos = currentline.size();
      return true;
    }
    case SDLK_h: { /* Delete char under cursor */
      /* cant delete if we are at end */
      if (static_cast<int>(currentline.size()) == cursorpos) {
	return true;
      }
      currentline.erase(cursorpos, 1);
      return true;
    }
    case SDLK_d: { /* Delete char left of cursor */
      /* Can't delete if at beginning of line */
      if (0 == cursorpos) {
	return true;
      }
      currentline.erase(--cursorpos, 1);
      return true;
    }
    case SDLK_k: { /* Kill to end of line */
      killed = currentline.substr(cursorpos, currentline.size()-cursorpos);
      currentline.erase(cursorpos, currentline.size()-cursorpos);
      return true;
    }
    case SDLK_y: { /* Yank killed text */
      currentline.insert(cursorpos, killed);
      cursorpos += killed.size();
      return true;
    }
    case SDLK_t: { /* Transpose chars */
      if (cursorpos > 0 && cursorpos < static_cast<int>(currentline.size())) {
	char tmp = currentline[cursorpos];
	currentline[cursorpos] = currentline[cursorpos-1];
	currentline[cursorpos-1] = tmp;
	cursorpos++;
	AdjustCursor();
      }
      return true;
    }
    default:
      return false;
    }
  }

  /* Check for last event */
  if (SDLK_TAB != event->key.keysym.sym) {
    LastEventWasTab = false;
    // LogLine(LOG_INFO, "Setting LastEventWasTab to false - 2");
  }

  /* Switch on the keyevents we handle, with no modifier */
  switch (event->key.keysym.sym) {
    /* Accept the current line */
  case SDLK_RETURN: {
    /* Add line to history */
    while (backlog.size() >= maxlinesinbacklog) {
      backlog.erase(backlog.begin());
    }
    /* Add it */
    backlog.push_back(currentline);
    StopReadingLine();
    return true;
  }
  /* Tab completion */
  case SDLK_TAB: {
    /* Figure out if it is a single or double complete -
       we have a second for this... */
    bool DoubleTab = false;
    if (LastEventWasTab) {
      DoubleTab  = true;
      // LogLine(LOG_INFO, "Setting LastEventWasTab to false - 3");
      LastEventWasTab = false; /* Do not want tripple tabs */
    } else {
      // LogLine(LOG_INFO, "Setting LastEventWasTab to true - 1");
      LastEventWasTab = true;
    }
    /* Check if we need to complete on command or arg */
    int firstspace = currentline.find(" ");
    if (firstspace < 0 || firstspace >= cursorpos) {
      /* Matching on command */
      if (CommandComplete) {
	string tmp;
	if (DoubleTab) {
	  Completions.clear();
	  tmp = CommandComplete->Complete(currentline.substr(0, firstspace), 
					  &Completions) + " ";
	  hascompletions = true;
	  // LogLine(LOG_INFO, "Full completion");
	} else {	  
	  tmp = CommandComplete->Complete(currentline.substr(0, firstspace)) + " ";
	  hascompletions = false;
	  // LogLine(LOG_INFO, "Simple completion");
	} 

	if (firstspace < 0) {
	  /* No spaces, cursor still in first word */
	  currentline = tmp;
	  cursorpos = currentline.size();
	} else {
	  /* Spaces, cursor in first word */
	  currentline.replace(0, firstspace, tmp);
	  cursorpos += tmp.size() - firstspace;
	  AdjustCursor();
	}
	return true;
      } /* No command complete tab structure */
      return false;
    } else {
      /* Matching on argument 
	 The code is very similar to above, could probably be put in a function. 
         We can only match on the string _after_ the first space... 
         This means there is exactly one match. This could be changed, if we cared...
      */
      if (ArgumentComplete) {
	string tmp;
	if (DoubleTab) {
	  Completions.clear();
	  tmp = 
	    ArgumentComplete->Complete(currentline.substr(firstspace + 1, 
							 currentline.size() 
							 - (firstspace+1)), 
				      &Completions);
	  hascompletions = true;
	  // LogLine(LOG_INFO, "Full completion");
	} else {	  
	  tmp = 
	    ArgumentComplete->Complete(currentline.substr(firstspace + 1, 
							 currentline.size() 
							 - (firstspace+1)));
	  hascompletions = false;
	  // LogLine(LOG_INFO, "Simple completion");
	} 

	currentline.replace(firstspace+1, currentline.size() - (firstspace+1), tmp);
	cursorpos = currentline.size();
	return true;
      } /* No argument completer */
      return false;
    }
    return false;
  } /* Case TAB */

  
  /* Move the cursor around */
  case SDLK_HOME: { /* Start of line */
    cursorpos = 0;
    return true;
  }
  case SDLK_END: { /* End of line */
    cursorpos = currentline.size();
    return true;
  }
  case SDLK_LEFT: {
    cursorpos--;
    AdjustCursor();
    return true;
  }
  case SDLK_RIGHT: {
    cursorpos++;
    AdjustCursor();
    return true;
  }
  /* Delete and backspace */
  case SDLK_DELETE: { /* Delete char under cursor */
    /* cant delete if we are at end */
    if (static_cast<int>(currentline.size()) == cursorpos) {
      return true;
    }
    currentline.erase(cursorpos, 1);
    return true;
  }
  case SDLK_BACKSPACE: { /* Delete char left of cursor */
    /* Can't delete if at beginning of line */
    if (0 == cursorpos) {
      return true;
    }
    currentline.erase(--cursorpos, 1);
    return true;
  }
  
  /* Use previous lines entered */
  case SDLK_UP: {
    if (backlog.size() == 0) {
      return true;
    }
    /* If we are moving away from the new line we are typing, save the line */
    if (0 == nextbacklogcommand) {
      savedline = currentline;
      savedpos = cursorpos;
    }
    if (nextbacklogcommand < static_cast<int>(backlog.size())) {
      nextbacklogcommand++;
    }
    currentline = backlog[backlog.size() - nextbacklogcommand];
    cursorpos = currentline.size();
    return true;
  }
  case SDLK_DOWN: {
    if (nextbacklogcommand > 0) {
      nextbacklogcommand--;
      if (0 == nextbacklogcommand) {
	currentline = savedline;
	cursorpos   = savedpos;
      } else {
	currentline = backlog[backlog.size() - nextbacklogcommand];
	cursorpos = currentline.size();
      }
    }
    return true;
  }

  
  default: { /* Check if this is a char we support */
    char ch[2];
    ch[1] = 0;
    if ( (event->key.keysym.unicode & 0xFF80) == 0 ) {
      ch[0] = event->key.keysym.unicode & 0x7F;
      if (ch[0] >= ' ' && ch[0] <= '~') {
	currentline.insert(cursorpos, ch);
	cursorpos++;
	return true;
      }
    }
    else {
      // ("An International Character.\n");
      ;
    }
  }
    return false;
  } /* Switch */
}
