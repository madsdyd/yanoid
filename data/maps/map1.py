# Python file that defines a _very_ simple map
xv = [ 0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660]
yv = [ 0, 30, 60, 90 ]
for x in xv:
    for y in yv:
        if (y == 0):
            AddBrick(x, y, "ged", "graphics/objects/weird_brick.png")
        elif (y == 30):
            AddBrick(x, y, "ged", "graphics/objects/red_oval_brick.png")
        else:
            AddBrick(x, y, "ged", "graphics/objects/basic_brick.png")
            
