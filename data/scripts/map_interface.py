# This file defines a number of helper functions to be used by map defining code

# AddBrick(int x, int y, string pixmap)
# This function adds a brick at x, y, with pixmap pixmap.
def AddBrick(x, y,  pixmap):
    "Wrapper to call the current map, and insert a brick into it"
    yanoid_map.AddObject("brick", x, y, 0, 0, pixmap)

def AddStatic(x, y, w, h):
    "Wrapper to call the current map, and insert an object into it"
    yanoid_map.AddObject("static", x, y, w, h, "")
    

    
