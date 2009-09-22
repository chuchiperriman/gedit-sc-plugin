plugindir = $(libdir)/gedit-2/plugins

INCLUDES = \
	-I$(top_srcdir) 						\
	$(GEDIT_CFLAGS) 						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	-DICON_DIR=\""$(GEDIT_PLUGINS_DATA_DIR)/gedit-sc-plugin"\"	\
	-DGEDIT_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

AM_CFLAGS =\
         -Wall\
         -g

plugin_LTLIBRARIES = libsourcecompletion.la

libsourcecompletion_la_SOURCES = 	\
	chcompletionutils.h		\
	chcompletionutils.c		\
	sc-utils.h			\
	sc-utils.c			\
	sc-symbol.h			\
	sc-symbol.c			\
	sc-ctags.h			\
	sc-ctags.c			\
	sc-language-manager.h		\
	sc-language-manager.c		\
	sc-lm-c.h			\
	sc-lm-c.c			\
	sc-provider-csymbols.h		\
	sc-provider-csymbols.c		\
	sc-provider-csymbols-goto.h	\
	sc-provider-csymbols-goto.c	\
	sc-provider-project-csymbols.h	\
	sc-provider-project-csymbols.c	\
	sc-symbols-panel.h		\
	sc-symbols-panel.c		\
	sc-menu.h			\
	sc-menu.c			\
	sc-plugin.h			\
	sc-plugin.c

libsourcecompletion_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)

# Glade files (if you use glade for your plugin, list those files here)
gladedir = $(datadir)/gedit-2/glade
glade_DATA =

plugin_in_files = sc.gedit-plugin.desktop.in

%.gedit-plugin: %.gedit-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.gedit-plugin.desktop.in=.gedit-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA) $(glade_DATA)

DISTCLEANFILES = $(plugin_DATA) $(glade_DATA)
