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
#include <iostream>
#include <string>
#include <list>
#include "tabcomplete.hh"
/* **********************************************************************
 * This is the tab node class. Most of the stuff actually happens in here
 * *********************************************************************/
class TTabNode {
protected:
  TTabNode * left;
  TTabNode * center;
  TTabNode * right;
  string key;
  /* Used for pushing key suffixes "down" */
  void Prefix(string value);
  string FillInSuffixes_(string prefix, TCompletions * suffixes);
  string FillInSuffixes(string prefix, TCompletions * suffixes);
public:
  TTabNode(string _key);
  ~TTabNode();
  /* Insert a string into the structure */
  void Insert(string value);
  /* Complete a string - return the longest string with this prefix,
     that is not ambiqitious */
  string Complete(string prefix);
  /* As Complete(prefix), but with suffixes */
  string Complete(string prefix, TCompletions * suffixes);
  /* Print contests (used for debugging) */
  void Print(string preprefix, string prefix);
};


/* **********************************************************************
 * Constructor
 * *********************************************************************/
TTabNode::TTabNode(string _key) : left(NULL), center(NULL), right(NULL),
				  key(_key) {};

/* **********************************************************************
 * Destructor
 * *********************************************************************/
TTabNode::~TTabNode() {
  delete left;
  delete center;
  delete right;
}

/* **********************************************************************
 * Prefix
 * *********************************************************************/
void TTabNode::Prefix(string prefix) {
  key = prefix + key;
  if (left) {
    left->key = prefix + left->key;
  }
  if (right) {
    right->key = prefix + right->key;
  }
}

/* **********************************************************************
 * Insert, uh oh.
 * *********************************************************************/
void TTabNode::Insert(string value) {
  // cout << "Inserting -" << value << "- into -" << key << "-" << endl;
  
  /* The easy case is if there are no prefix sharing, in which case, this
     just a binary tree... */
  /* Make sure value is not and not equal to key */
  if (value.size() == 0 || key == value) {
    return;
  }
  /* If we have been constructed with an empty key */
  if (key.size() == 0) {
    key = value;
    return;
  }
  /* Left */
  if (value[0] < key[0]) {
    /* easy, left side */
    if (left) {
      left->Insert(value);
    } else {
      left = new TTabNode(value);
    }
    return;
  }
  /* Right */
  if (value[0] > key[0]) {
    if (right) {
      right->Insert(value);
    } else {
      right = new TTabNode(value);
    }
    return;
  }
  /* Okay, the value and key shares a prefix. Lets get the place where they 
     differ - this is the length of the new key */
  int key_size = key.size();
  int value_size = value.size();
  int min_size = value_size < key_size ? value_size : key_size;
  int i; 
  int pos = 0; /* Is i defined to have the right value? cant remember 
		  The compiler will optimize it away, I hope. */
  /* We use i = 0 to make sure that i < min_size to start with... */
  for (i = 0; i < min_size; i++) {
    if (key[i] != value[i]) {
      break;
    }
    pos = i;
  }
  pos++;
  /* OK, pos is number of common chars.. Lets change this key, and insert the 
     suffixes */
  // cout << "pos : " << pos << endl;
  string key_suffix = key.substr(pos, key_size - pos);
  string value_suffix = value.substr(pos, value_size - pos);
  // cout << "keysuffix -" << key_suffix << "-  value_suffix -" << value_suffix 
  //     << "-" << endl;

  /* Check if we need to change the key */
  if (pos != key_size) {
    string key_prefix = key.substr(0, pos);
    // cout << "Changing key from -" << key << "- to -" << key_prefix << "-" << endl;
    key = key_prefix;
    /* We insert a new key between this and the center key  ... */
    if (center) { 
      TTabNode * tmp = new TTabNode(key_suffix);
      tmp->center = center;
      center = tmp;
    } else {
      center =  new TTabNode(key_suffix);
    }
    
  }
  /* We definitly need a center key */
  if (!center) {
    center = new TTabNode("");
  }

  /* Insert the value_suffix key in center */
  center->Insert(value_suffix);
}

/* **********************************************************************
 * Complete
 * *********************************************************************/
string TTabNode::Complete(string prefix) {
  // cout << "Completing -" << prefix << "- in -" << key << "-" << endl;
  if (prefix.size() == 0) {
    return "";
  }
  if (prefix[0] < key[0] && left) {
    return left->Complete(prefix);
  }
  if (prefix[0] > key[0] && right) {
    return right->Complete(prefix);
  }
  if (prefix[0] == key[0]) {
    /* If prefix.size() <= key.size() && they match on the chars - return whole key 
       if prefix.size() > key.size() && they match on the chars - cut key from prefix, 
       recurse into center (if present) 
       If no match, return longest match 
    */
    int key_size = key.size();
    int prefix_size = prefix.size();
    int min_size = prefix_size < key_size ? prefix_size : key_size;
    int i; 
    int pos = 0; /* Is i defined to have the right prefix? cant remember 
		    The compiler will optimize it away, I hope. */
    /* We use i = 0 to make sure that i < min_size to start with... */
    for (i = 0; i < min_size; i++) {
      if (key[i] != prefix[i]) {
	break;
      }
      pos = i;
    }
    pos++;
    /* OK, pos is number of common chars.. */
    // cout << "pos : " << pos << endl;
    if (pos <= key_size && pos == prefix_size) {
      return key;
    }
    if (pos == key_size && key_size < prefix_size) {
      /* Cut it up. */
      if (center) {
	return key + center->Complete(prefix.substr(pos, prefix_size - pos));
      } else {
	return key;
      }
    }
    /* If we reach here, there was a mismatch before the end of either 
       was reached. In this case, the match is only to the pos */
    return key.substr(0, pos);
  }
  /* No completion - return the empty string */
  return "";
}

/* **********************************************************************
 * Complete
 * *********************************************************************/
string TTabNode::Complete(string prefix, TCompletions * suffixes) {
  /* This operation assumes that key is != "" */
  // cout << "Completing -" << prefix << "- in -" << key << "-" << endl;
  if (prefix.size() == 0) {
    return FillInSuffixes("", suffixes);
    // return "";
  }
  if (prefix[0] < key[0] && left) {
    return left->Complete(prefix, suffixes);
  }
  if (prefix[0] > key[0] && right) {
    return right->Complete(prefix, suffixes);
  }
  if (prefix[0] == key[0]) {
    /* If prefix.size() <= key.size() && they match on the chars - return whole key 
       if prefix.size() > key.size() && they match on the chars - cut key from prefix, 
       recurse into center (if present) 
       If no match, return longest match 
    */
    int key_size = key.size();
    int prefix_size = prefix.size();
    int min_size = prefix_size < key_size ? prefix_size : key_size;
    int i; 
    int pos = 0; /* Is i defined to have the right prefix? cant remember 
		    The compiler will optimize it away, I hope. */
    /* We use i = 0 to make sure that i < min_size to start with... */
    for (i = 0; i < min_size; i++) {
      if (key[i] != prefix[i]) {
	break;
      }
      pos = i;
    }
    pos++;
    /* OK, pos is number of common chars.. */
    // cout << "pos : " << pos << endl;
    if (pos <= key_size && pos == prefix_size) {
      // FillInSuffixes_(key.substr(pos, key_size-pos), suffixes); 
      if (center) {
	center->FillInSuffixes("", suffixes); 
      }
      return key;
    }
    if (pos == key_size && key_size < prefix_size) {
      /* Cut it up. */
      if (center) {
	return key 
	  + center->Complete(prefix.substr(pos, prefix_size - pos),
			     suffixes);
      } else {
	return key;
      }
    }
    /* If we reach here, there was a mismatch before the end of either 
       was reached. In this case, the match is only to the pos */
    return key.substr(0, pos); /* Note, something in here???? */
  }
  /* No completion - return the empty string */
  return ""; // FillInSuffixes_("", suffixes);
}

/* **********************************************************************
 * A wrapper to make sure that we do not include stuff we "know".
 * *********************************************************************/
string TTabNode::FillInSuffixes_(string prefix, TCompletions *
				 suffixes) {
  // cout << "FillInSuffixes_(" << prefix << ") key is " << key << endl;
  string tmp = key;
  key = "";
  FillInSuffixes(prefix, suffixes);
  key = tmp;
  return "";
}


/* **********************************************************************
 * TTabNode - FillInSuffixes - as print
 * *********************************************************************/
string TTabNode::FillInSuffixes(string prefix, TCompletions * suffixes) {
  // cout << "FillIn : " << prefix << endl;
  /* Similar to print, really */
  if (left) {
    left->FillInSuffixes(prefix, suffixes);
  }
  if (center) {
    center->FillInSuffixes(prefix + key, suffixes);
  } else {
    cout << "Adding " << prefix << " + " << key << endl;
    suffixes->push_back(prefix + key);
  }
  if (right) {
    right->FillInSuffixes(prefix, suffixes);
  }
  return "";
}

/* **********************************************************************
 * TTabNode::Print
 * *********************************************************************/
void TTabNode::Print(string preprefix, string prefix) {
  string spacekey;
  for (unsigned int i = 0; i < preprefix.size(); i++) {
    spacekey = spacekey + " ";
  }
  if (left) {
    // cout << "l:"; 
    left->Print(spacekey, prefix);
  }
  if (center) {
    // cout << "c:"; 
    center->Print(preprefix + " " + key, prefix + key );
  } else {
    cout << preprefix << " " << key << " (" << prefix << " + " << key << ")" << endl;
  }
  if (right) {
    //cout << "r:"; 
    right->Print(preprefix, prefix);
  }
}


/* **********************************************************************
 * **********************************************************************
 * **********************************************************************
 * **********************************************************************
 * **********************************************************************
 * **********************************************************************
 * **********************************************************************
 * **********************************************************************
 * *********************************************************************/


/* **********************************************************************
 * Constructor
 * *********************************************************************/
TTabComplete::TTabComplete() {
  root = new TTabNode("");
};

/* **********************************************************************
 * Destructor
 * *********************************************************************/
TTabComplete::~TTabComplete() {
  delete root;
}

/* **********************************************************************
 * Insert
 * *********************************************************************/
void TTabComplete::Insert(string value) {
  root->Insert(value);
}

/* **********************************************************************
 * Complete
 * *********************************************************************/
string TTabComplete::Complete(string prefix) {
  return root->Complete(prefix);
}

/* **********************************************************************
 * Complete with string list
 * *********************************************************************/
string TTabComplete::Complete(string prefix, TCompletions * suffixes)
{
  if (!suffixes) {
    return root->Complete(prefix);
  }
  return root->Complete(prefix, suffixes);
}

/* **********************************************************************
 * Print
 * *********************************************************************/
void TTabComplete::Print() {
  root->Print("", "");
}
