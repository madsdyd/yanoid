# Python file that defines a _very_ simple map

# Add a number of bricks
for x in range(350, 450, 50):
    for y in range(220-2*18, 220+2*18, 18):
        AddBrick("brick", x, y, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")

from math import *
factor = pi / 180.0

# Add a kind of circle of bricks.
for t in range(0, 360, 30):
    x = cos(t * factor)
    y = sin(t * factor)
    AddBrick("brick", 400 - 24 + x*100, 220 - 8 + y*100,
             "basic_brick_hit()",
             "graphics/objects/weird_brick.png") 
    AddBrick("brick", 400 - 24 + x*150, 220 - 8 + y*150,
             "basic_brick_hit()",
             "graphics/objects/basic_brick.png") 
    AddBrick("brick", 400 - 24 + x*200, 220 - 8 + y*200,
             "basic_brick_hit()",
             "graphics/objects/red_oval_brick.png") 
            
# Add the walls
AddEntity("static", -10, -10, 11, 620, "static_hit()")
AddEntity("static", 799, -10, 10, 620, "static_hit()")
AddEntity("static", 1, -10, 798, 11, "static_hit()")
AddEntity("hole", 1, 550, 798, 50, "")

# Add a paddle
SetPaddle(400, 500, "graphics/objects/basic_paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/balls/red.png")

# And, call it - probably will go
RoundStart();
