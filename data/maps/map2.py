# Python file that defines a _very_ simple map

# Add a number of bricks
for x in range(350, 450, 50):
    for y in range(220-2*18, 220+2*18, 18):
        AddBrick(x, y, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")

from math import *
factor = pi / 180.0

# Add a kind of circle of bricks.
for t in range(0, 360, 30):
    x = cos(t * factor)
    y = sin(t * factor)
    AddBrick(400 - 24 + x*100, 220 - 8 + y*100, "basic_brick_hit()",
                     "graphics/objects/weird_brick.png") 
    AddBrick(400 - 24 + x*150, 220 - 8 + y*150, "basic_brick_hit()",
                     "graphics/objects/basic_brick.png") 
    AddBrick(400 - 24 + x*200, 220 - 8 + y*200, "basic_brick_hit()",
                     "graphics/objects/red_oval_brick.png") 
            
# Add the walls
AddStatic(0, 0, 1, 550, "static_hit()")
AddStatic(799, 0, 1, 550, "static_hit()")
AddStatic(1, 0, 799, 1, "static_hit()")
AddStatic(0, 550, 799, 50, "static_hit()")

# Add a paddle
SetPaddle(400, 500, "graphics/objects/basic_paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_ball.png")

# And, call it - probably will go
RoundStart();
