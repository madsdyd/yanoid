bin_PROGRAMS = yanoid

SUBDIRS = ConsoleSource

yanoid_SOURCES = map.cc entity.cc game.cc display.cc client.cc \
		surfacemanager.cc musicmanager.cc \
		ressourcemanager.cc \
	        debug.cc log.cc screen.cc main.cc 

yanoid_LDADD = ConsoleSource/libSDL_console.la

# none of these are installed - here for dist

noinst_HEADERS = map.hh entity.hh game.hh display.hh client.hh \
		 surfacemanager.hh musicmanager.hh \
		 ressourcemanager.hh \
		 debug.hh log.hh screen.hh
