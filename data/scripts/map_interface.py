# This file defines a number of helper functions to be used by map defining code

# Map_AddBrick(int x, int y, string type, string pixmap)
# This function adds a brick at x, y, of type type and with pixmap pixmap.
def AddBrick(x, y, type, pixmap):
    "Wrapper to call the current map, and insert an entity into it"
    yanoid_map.AddBrick(x, y, type, pixmap)

    
