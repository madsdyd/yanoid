# Python file that defines a _very_ simple map

print "In map1.py"

# Add a number of bricks
xv = [ 60, 120, 180, 240, 300, 360, 420, 480, 540 ]
yv = [ 30, 60, 90, 120 ]
for x in xv:
    for y in yv:
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
AddStatic(0, 0, 10, 400, "static_hit()")
AddStatic(630, 0, 10, 400, "static_hit()")
AddStatic(10, 0, 620, 10, "static_hit()")
AddStatic(0, 400, 640, 100, "static_hit()")

# Add a paddle
SetPaddle(300, 300, "graphics/objects/basic_paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_ball.png")

# And, call it - probably will go
RoundStart();

print("Loaded map1")
