SUBDIRS = src data

EXTRA_DIST = bootstrap config doc CREDITS yanoid.spec

# Hook for making sure that we do not distribute CVS stuff
dist-hook:
	find $(distdir) -type d -name "*CVS*" | xargs rm -Rf


