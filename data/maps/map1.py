# Python file that defines a _very_ simple map

# Add a number of bricks
xv = [ 60, 120, 180, 240, 300, 360, 420, 480, 540 ]
yv = [ 30, 60, 90, 120 ]
for x in xv:
    for y in yv:
        if (y == 30):
            AddBrick(x, y, "graphics/objects/weird_brick.png")
        elif (y == 60):
            AddBrick(x, y, "graphics/objects/red_oval_brick.png")
        else:
            AddBrick(x, y, "graphics/objects/basic_brick.png")
            
# Add the walls
AddStatic(0, 0, 10, 400)
AddStatic(630, 0, 10, 400)
AddStatic(10, 0, 620, 10)
AddStatic(0, 400, 640, 100)

# Add a paddle
SetPaddle(300, 300, "graphics/objects/basic_paddle.png")
