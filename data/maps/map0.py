# Python file that defines a _very_ simple map

# Add a number of bricks
AddBrick(400, 50, "basic_brick_hit()",
         "graphics/objects/basic_brick.png")
            
# Add the walls
AddEntity("static", 0, 0, 1, 550, "static_hit()")
AddEntity("static", 799, 0, 1, 550, "static_hit()")
AddEntity("static", 1, 0, 799, 1, "static_hit()")
AddEntity("hole", 0, 550, 799, 50, "static_hit()")

# Add a paddle
SetPaddle(400, 400, "graphics/objects/paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_ball.png")

# And, call it - probably will go
RoundStart();
