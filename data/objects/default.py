# Load the bricks code

bricks = [ "objects/basic_brick.py" ]

for script in bricks:
    put_console("Executing " + yanoid.resolve(script))
    runthis = "execfile('" + yanoid.resolve(script) + "')"
    eval(runthis)

def static_hit():
    print "static_hit"
