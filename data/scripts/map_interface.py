# This file defines a number of helper functions to be used by map defining code

# AddBrick(string type, int x, int y, string hitfunction, string pixmap)
# Adds a brick of type type at x, y, with pixmap pixmap.
def AddBrick(type, x, y, hitfunction, pixmap):
    "Add a brick to the current map"
    yanoid_map.AddObject(type, hitfunction, x, y, 0, 0, pixmap)

# AddEntity(string type, int x, int y, int w, int h, string hitfunction)
# Adds a static (simple object, no pixmap) at x,y having size w,h
def AddEntity(type, x, y, w, h, hitfunction):
    "Add a static (simple object, no pixmap) to the current map"
    yanoid_map.AddObject(type, hitfunction, x, y, w, h, "")

# AddBallDefault(string pixmap)
# Adds a ball at a default position
def AddBallDefault(pixmap):
    "Adds a ball at a default position"
    yanoid_map.AddObject("default-ball", "", 0, 0, 0, 0, pixmap)

# SetPaddle(int x, int y, string pixmap)
# Creates and sets the paddle for the current map
# Will expose more attributes later
def SetPaddle(x, y, pixmap):
    "Creates and sets a paddle for the current map"
    yanoid_map.SetPaddle(x, y, "FreeMotion", 0.0, 0.0, pixmap)

    
    
    
