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
#include "menu.hh"
#include "screen.hh"
#include "client.hh"
#include "fontmanager.hh"
#include <SDL/SDL.h>
#include "ConsoleSource/DT_drawtext.h"


/* **********************************************************************
 * The Base TMenu class
 * *********************************************************************/

/* **********************************************************************
 * Constructor and destructor
 * *********************************************************************/

TMenu::TMenu(bool capture_background, bool display_splash) {
  font = FontManager->RequireRessource("graphics/fonts/LargeFont.bmp");
  if (!font) {
    LogFatal("Unable to load highscore font graphics/fonts/LargeFont.bmp");
    exit(-1);
  }
  cap_back   = capture_background;
  background = NULL;
  dis_splash = display_splash;
  focused    = 0;
  cancel     = false;
  close      = false;
}

TMenu::~TMenu() {
  if (font) FontManager->ReleaseRessource(font);
}

/* **********************************************************************
 * Run - checks for events, renders the menu, decide it is time to leave.
 * *********************************************************************/
bool TMenu::Run() {
  cancel = false; close = false;
  /* Copy the background, if neccesary */
  if (cap_back) {
    LogLine(LOG_VERBOSE, "Capturing the screen");
    /* Copy the screen - make it a background */
    background = SDL_CreateRGBSurface(SDL_SRCALPHA, Screen->w, Screen->h, 
				      Screen->format->BitsPerPixel,
				      Screen->format->Rmask,
				      Screen->format->Gmask,
				      Screen->format->Bmask,
				      Screen->format->Amask);
    if (background == NULL) {
      LogLine(LOG_ERROR, "TInGameMenu::Run - unable to create background");
    } else {
      SDL_Rect d;
      d.x = 0; d.y = 0; 
      d.w = background->w;
      d.h = background->h;
      SDL_BlitSurface(Screen, &d, background, &d);
      SDL_SetAlpha(background, SDL_SRCALPHA, 64);
    }
  }
  /* While until the menu is done */
  while(!cancel && !close) {
    Render();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      HandleEvent(&event);
    }
  }
  /* If cancelled, return false, if not, return true */
  return !cancel;
}

/* **********************************************************************
 * Render - renders the menu
 * *********************************************************************/
void TMenu::Render() {
  RenderBackground();
  RenderSplash();
  RenderItems(0, 0, Screen->w, Screen->h);
  SDL_Flip(Screen);
}

/* **********************************************************************
 * RenderSplash - renders the splash screen
 * *********************************************************************/
void TMenu::RenderSplash() {
  if (dis_splash) {
    /* Put on the splash screen */
    SDL_Rect src, dest;
    src.x = 0; src.y = 0; src.w = Splash->w; src.h = Splash->h;
    dest.x = (Screen->w-src.w)/2; dest.y = 100; dest.w = src.w; dest.h = src.h;
    SDL_BlitSurface(Splash, &src, Screen, &dest);
  }
}
/* **********************************************************************
 * RenderBackground - renders the background
 * *********************************************************************/
void TMenu::RenderBackground() {
  if (background) {
    SDL_FillRect(Screen, NULL, 
		 SDL_MapRGBA(Screen->format, 16, 16, 16, SDL_ALPHA_OPAQUE));
    SDL_Rect d;
    d.x = 0; d.y = 0; 
    d.w = background->w;
    d.h = background->h;
    SDL_BlitSurface(background, &d, Screen, &d);
  } else {
    SDL_FillRect(Screen, NULL, SDL_MapRGB(Screen->format, 0, 0, 0));
  }
}

/* **********************************************************************
 * RenderItems - draw the lines. Put a + in front of the selected one... 
 * *********************************************************************/
void TMenu::RenderItems(int xlow, int ylow, int xhigh, int yhigh) {
  /* Render from the center of the area. We use a fixed font, so we can
     calculate where it is 
     20 is the height spacing of the font, 16 is the width */
  int h = yhigh - ylow;
  int w = xhigh - xlow;
  int drawy = (h - items.size()*20) / 2 + ylow;
  unsigned int count = 0;
  int drawx;
  string tmp;
  TItemsIterator End = items.end();
  for (TItemsIterator i = items.begin(); i != End; i++) {
    tmp = *i;
    /* Mark the focused one */
    if (count == focused) {
      tmp = ">>> " + tmp + " <<<";
    }
    drawx = (w - tmp.size()*16) / 2 + xlow;
    DT_DrawText(tmp.c_str(), Screen, *font, drawx, drawy);
    count++;
    drawy += 20;
  }
}
/* **********************************************************************
 * Handle keypresses (no mouse)
 * *********************************************************************/

bool TMenu::HandleEvent(SDL_Event * event) {
  if (SDL_KEYDOWN == event->type) {
    switch (event->key.keysym.sym) {
    case SDLK_DOWN: {
      focused += 1;
      if (focused >= items.size()) {
	focused = 0;
      }
      return true;
    }
    case SDLK_UP: {
      if (focused == 0) {
	focused = items.size() - 1;
      } else {
	focused -= 1;
      }
      return true;
    }
    case SDLK_SPACE:
    case SDLK_RETURN: {
      SelectFocused();
      return true;
    }
    case SDLK_ESCAPE: {
      cancel = true;
    }
    }
  }
  return false;
};

/* **********************************************************************
 * The DialogMenu
 * *********************************************************************/
void TDialogMenu::Render() {
  SDL_FillRect(Screen, NULL, SDL_MapRGB(Screen->format, 0, 0, 0));
  RenderSplash();
  RenderLines();
  /*
  string text = "Here is room for a help text";
  int drawx = (Screen->w - text.size()*16) / 2;
  DT_DrawText(text.c_str(), Screen, *font, drawx, 250);
  */
  RenderItems(0, 300, Screen->w, Screen->h);
  SDL_Flip(Screen);
};

void TDialogMenu::SelectFocused() {
  cancel = true;
};

TDialogMenu::TDialogMenu(string caption,
			 bool capture_background = false, 
			 bool display_splash = true) 
  : TMenu(capture_background, display_splash) {
  items.push_back(caption);
};

/* **********************************************************************
 * These menus are used internally.
 * *********************************************************************/

/* **********************************************************************
 * The Help Menu
 * *********************************************************************/
class THelpMenu : public TDialogMenu {
protected:
  void RenderLines();
public:
  THelpMenu() : TDialogMenu("Return") {};
};

void THelpMenu::RenderLines() {
  string text = "Here is room for a help text";
  int drawx = (Screen->w - text.size()*16) / 2;
  DT_DrawText(text.c_str(), Screen, *font, drawx, 250);
}

/* **********************************************************************
 * The About Menu
 * *********************************************************************/
class TAboutMenu : public TDialogMenu {
protected:
  void RenderLines();
public:
  TAboutMenu() : TDialogMenu("Return") {};
};

void TAboutMenu::RenderLines() {
  string text = "Here is room for an about text";
  int drawx = (Screen->w - text.size()*16) / 2;
  DT_DrawText(text.c_str(), Screen, *font, drawx, 250);
}

/* **********************************************************************
 * The PreGameMenu
 * *********************************************************************/

TPreGameMenu::TPreGameMenu() : TMenu(false) {
  HelpMenu = new THelpMenu();
  AboutMenu = new TAboutMenu();
  items.push_back("Start Game");
  items.push_back("Help");
  items.push_back("About");
  items.push_back("Exit yanoid");
}

TPreGameMenu::~TPreGameMenu() {
  delete HelpMenu;
  delete AboutMenu;
}

void TPreGameMenu::SelectFocused() {
  // LogLineExt(LOG_VERBOSE, ("TPreGameMenu, selected %s", items[focused].c_str()));
  /* Yes, this stinks */
  switch (focused) {
  case 0: /* Start game */
    close = true;
    return;
  case 1: 
    HelpMenu->Run();
    return;
  case 2:
    AboutMenu->Run();
    return;
  case 3: /* Exit */
    cancel = true;
    return;
  default:
    LogLineExt(LOG_ERROR, ("TPreGameMenu - focused out of range %i", focused));
  }  
}

/* **********************************************************************
 * The in game menu
 * *********************************************************************/
TInGameMenu::TInGameMenu(TClient * client) 
  : TMenu(true, false), Client(client) {
  items.push_back("Resume game");
  items.push_back("Toggle console");
  items.push_back("Toggle fullscreen");
  items.push_back("End game");
}

void TInGameMenu::SelectFocused() {
  switch(focused) {
  case 0: /* Resume game is the same as cancelling the menu */
    cancel = true;
    return;
  case 1: /* Toggle the console */
    Client->ToggleConsole();
    cancel = true;
    return;
  case 2:
    SDL_WM_ToggleFullScreen(Screen);
    return;
  case 3: /* Exit is close */
    close = true;
    return;
  default:
    LogLineExt(LOG_ERROR, ("TInGameMenu - focused out of range %i", focused));
  }
}

/* **********************************************************************
 * TGameOver menu
 * *********************************************************************/
void TGameOverMenu::RenderLines() {
  string text = "Game over";
  int drawx = (Screen->w - text.size()*16) / 2;
  DT_DrawText(text.c_str(), Screen, *font, drawx, 250);
}

/* **********************************************************************
 * TRoundOverMenu
 * *********************************************************************/
void TRoundOverMenu::RenderLines() {
  string text = "You loose a life";
  int drawx = (Screen->w - text.size()*16) / 2;
  DT_DrawText(text.c_str(), Screen, *font, drawx, 250);
}
