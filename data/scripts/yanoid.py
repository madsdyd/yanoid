def put_console(a):
    "Wrapper to convert values to printable strings for the yanoid console"
    yanoid.puts_console(str(a))

def fak(n):
    "The classic..."
    if (n > 0):
        return n * fak(n-1)
    else:
        return 1

    
def rules(n):
    "Simple bragging function, dont run with more than 100 as argument"
    if (n > 0):
        put_console("yanoid rules")
        rules(n-1)

def help():
    "Print some help"
    put_console("")
    put_console("Interprenter help(TM)")
    put_console("---------------------")
    put_console("Try one of the following commands")
    put_console("(remember the i in front and trailing())")
    put_console("put_console(a) - where a is a python expression")
    put_console("fak(n) - where n is a positive (small) number")
    put_console("rules(n) - where n is a positive (small) number")
    put_console("yanoid.yreload() - reloads scripts/yanoid.py")
    put_console("                   note the _y_")
    put_console("examples:")
    put_console("  i a = [ 'smart', 'i', 'en', 'fart' ]")
    put_console("  i put_console(a)")
    put_console("  i yanoid.yreload()")
    put_console("  i put_console(fak(4))")
    put_console("  i rules(4)")
