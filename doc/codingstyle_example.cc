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

/* ***************************************************************************
 * Info about what is in the file
 *
 * **************************************************************************/

/*
  This is a class comment block
 */
class TMyClass : public TMyBaseClass {
public:
  enum State { STATE_Invalid, STATE_Runnning, STATE_Stopped };
  typedef int property_t;
  void setProperty(property_t prop);
  property_t getProperty() const;
private:
  property_t MyProperty;
};


/*
  This is a function comment block
 */
void MyClass::setProperty(property_t prop)
{

  /*
    This is a big comment block,
    where i can write a lot.
  */
  for (int i = 0; i < 999 ; ++i) {
    std::cerr << "DUMMY" << std::endl;
  } 

  // this is a short comment
  MyProperty = prop;
}

