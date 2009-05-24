INCLUDES = 				\
	-I$(top_srcdir) -I$(srcdir) 	\
	$(DISABLE_DEPRECATED_CFLAGS)	\
	$(WARN_CFLAGS) 			\
	$(GEDIT_CFLAGS)

AM_CFLAGS =\
         -Wall\
         -g

# testing executables
EXTRA_PROGRAMS = test-ctags
noinst_PROGRAMS = test-ctags

test_ctags_SOURCES =			\
	$(top_srcdir)/src/sc-ctags.h	\
	$(top_srcdir)/src/sc-ctags.c	\
        test-ctags.c

test_ctags_LDADD = 	\
	$(GEDIT_LIBS)

