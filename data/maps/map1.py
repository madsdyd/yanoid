# Python file that defines a _very_ simple map

# Add a number of bricks
for x in range(60, 750, 50):
    for y in range(30, 120, 30):
        if (y == 30):
            AddBrick(x, y, "basic_brick_hit()",
                     "graphics/objects/stay_brick16.png")
        elif (y == 60):
            AddBrick(x, y, "basic_brick_hit()",
                     "graphics/objects/3hit_brick16.png")
        else:
            AddBrick(x, y, "basic_brick_hit()",
                     "graphics/objects/1hit_brick16.png")

            
# Add the walls
AddEntity("static", 0, 0, 1, 550, "static_hit()")
AddEntity("static", 799, 0, 1, 550, "static_hit()")
AddEntity("static", 1, 0, 799, 1, "static_hit()")
AddEntity("hole", 0, 550, 799, 50, "static_hit()")

# Add a paddle
SetPaddle(400, 400, "graphics/objects/gun16.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_ball.png")

# And, call it - probably will go
RoundStart();
