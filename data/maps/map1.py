# Python file that defines a _very_ simple map

# Add a number of bricks
for x in range(60, 750, 50):
    for y in range(30, 120, 30):
        if (y == 30):
            AddBrick(x, y, "basic_brick_hit()",
                     "graphics/objects/weird_brick.png")
        elif (y == 60):
            AddBrick(x, y, "basic_brick_hit()",
                     "graphics/objects/red_oval_brick.png")
        else:
            AddBrick(x, y, "basic_brick_hit()",
                     "graphics/objects/basic_brick.png")

            
# Add the walls
AddEntity("static", 0, 0, 1, 550, "static_hit()")
AddEntity("static", 799, 0, 1, 550, "static_hit()")
AddEntity("static", 1, 0, 799, 1, "static_hit()")
AddEntity("hole", 0, 550, 799, 50, "static_hit()")

# Add a paddle
SetPaddle(40, 12, "graphics/objects/paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_brick.png")

# And, call it - probably will go
RoundStart();
