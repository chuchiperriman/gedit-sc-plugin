plugindir = $(libdir)/gedit-2/plugins

INCLUDES = \
	-I$(top_srcdir) 						\
	$(GEDIT_CFLAGS) 						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	`pkg-config --cflags gtksourcecompletion-1.0`			\
	`pkg-config --cflags gconf-2.0`					\
	-DGEDIT_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libsourcecompletion.la

libsourcecompletion_la_SOURCES = \
	gsc-provider-csymbols.h		\
	gsc-provider-csymbols.c		\
	gsc-proposal-csymbol.h		\
	gsc-proposal-csymbol.c		\
	gsc-trigger-symbols.h		\
	gsc-trigger-symbols.c		\
	sourcecompletion-plugin.h	\
	sourcecompletion-plugin.c

libsourcecompletion_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) `pkg-config --libs gtksourcecompletion-1.0` `pkg-config --libs gconf-2.0`

# Glade files (if you use glade for your plugin, list those files here)
gladedir = $(datadir)/gedit-2/glade
glade_DATA =

plugin_in_files = sourcecompletion.gedit-plugin.desktop.in

%.gedit-plugin: %.gedit-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.gedit-plugin.desktop.in=.gedit-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA) $(glade_DATA)

DISTCLEANFILES = $(plugin_DATA) $(glade_DATA)