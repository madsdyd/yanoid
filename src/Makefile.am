bin_PROGRAMS = yanoid

SUBDIRS = ConsoleSource

yanoid_SOURCES = interprenter.cc \
                game.cc map.cc entity.cc velocity.cc \
		display.cc client.cc \
		surfacemanager.cc musicmanager.cc \
		ressourcemanager.cc highscore.cc \
	        debug.cc log.cc screen.cc main.cc 

yanoid_LDADD = ConsoleSource/libSDL_console.la

# none of these are installed - here for dist

noinst_HEADERS = interprenter.hh \
		 map.hh entity.hh game.hh velocity.hh \
		 display.hh client.hh \
		 surfacemanager.hh musicmanager.hh \
		 ressourcemanager.hh highscore.hh \
		 debug.hh log.hh screen.hh
