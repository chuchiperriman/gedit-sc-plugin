INCLUDES = 				\
	-I$(top_srcdir) -I$(srcdir) 	\
	$(DISABLE_DEPRECATED_CFLAGS)	\
	$(WARN_CFLAGS) 			\
	-DICON_DIR=\""$(GEDIT_PLUGINS_DATA_DIR)/gedit-sc-plugin"\"	\
	$(GEDIT_CFLAGS)

AM_CFLAGS =\
         -Wall\
         -g

# testing executables
EXTRA_PROGRAMS = test-ctags
noinst_PROGRAMS = test-ctags

test_ctags_SOURCES =			\
	$(top_srcdir)/src/sc-symbol.h	\
	$(top_srcdir)/src/sc-symbol.c	\
	$(top_srcdir)/src/sc-ctags.h	\
	$(top_srcdir)/src/sc-ctags.c	\
	$(top_srcdir)/src/sc-utils.h	\
	$(top_srcdir)/src/sc-utils.c	\
        test-ctags.c

test_ctags_LDADD = 	\
	$(GEDIT_LIBS)

