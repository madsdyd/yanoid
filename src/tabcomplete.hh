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
/* Data structure to do "tab completion" in.  The tabcompletion
   structure is more or less a tree with a key and tree branches.  The
   key is the prefix of all values in the center leave. The left
   branch is < the key, and can not have a common prefix with the
   key. The right branch is > the key, and can not have a common
   prefix with the key. 

   (The key is marked as complete or not. A complete key can be
   completed on, a non-complete is just a prefix - may not work )

   Insertion may require that the current key is changed to make sure
   that the prefix rule is maintained. This does not touch the right
   and left branches, but can require additional nodes to be
   inserted. It probably degenerates into a single char key tree, if a
   sufficient number of values are inserted, but for my use (a couple
   of 100 values) it should be OK.

   Lookup looks at the key. If there are no common prefix, completion
   should take place in the left or right branch. If there is a common
   prefix, and no prefix for the search value, search the middle
   branch. If there 

   I have no idea about the complexity, but I think it is OK */
#ifndef __TABCOMPLETE_HH__
#define __TABCOMPLETE_HH__
#include <string>
#include <list>

typedef list<string> TCompletions;
typedef TCompletions::iterator TCompletionsIterator;

class TTabNode;
class TTabComplete {
protected:
  TTabNode * root;
public:
  TTabComplete();
  ~TTabComplete();
  void Insert(string value);
  /* Returns the maximal tab value - need to implement tab, tab
     support */
  string Complete(string prefix);
  /* Returns the maximal value, _and_ a list of string suffixes (in
     lexical sort order) */
  string Complete(string prefix, TCompletions * suffixes);
  void Print();
};
#endif
