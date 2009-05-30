/*
 * sc-plugin.c - Adds (auto)completion support to gedit
 *
 * Copyright (C) 2007 - chuchiperriman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gdk/gdk.h>
#include <glib/gi18n-lib.h>
#include <gedit/gedit-debug.h>

#include "sc-plugin.h"
#include "sc-provider-csymbols.h"
#include "sc-symbols-panel.h"
#include "sc-ctags.h"


#define SC_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), SC_TYPE_PLUGIN, ScPluginPrivate))

#define SC_PANEL_KEY "sc-panel-key"

struct _ScPluginPrivate
{
	GeditWindow *gedit_window;
	GtkWidget *window;
	GtkWidget *panel;
};

typedef struct _ViewAndCompletion ViewAndCompletion;

GEDIT_PLUGIN_REGISTER_TYPE (ScPlugin, sc_plugin)

static void
sc_plugin_init (ScPlugin *plugin)
{
	plugin->priv = SC_PLUGIN_GET_PRIVATE (plugin);
	gedit_debug_message (DEBUG_PLUGINS,
			     "ScPlugin initializing");
}

static void
sc_plugin_finalize (GObject *object)
{
	gedit_debug_message (DEBUG_PLUGINS,
			     "ScPlugin finalizing");
	G_OBJECT_CLASS (sc_plugin_parent_class)->finalize (object);
}

static void
populate_panel (ScPlugin *self, GeditDocument *doc)
{
	GList *symbols;
	gchar *uri;
	
	/*TODO free this uri?*/
	uri = gedit_document_get_uri_for_display (doc);
	symbols = sc_ctags_exec_get_symbols (CTAGS_EXEC_FILE, uri);
	sc_symbols_panel_populate (self->priv->panel, symbols);
}

static void
tab_added_cb (GeditWindow *geditwindow,
	      GeditTab    *tab,
	      ScPlugin    *self)
{
	GeditDocument *doc = gedit_tab_get_document (tab);
	populate_panel (self, doc);
}

static void
create_panel (GeditPlugin *plugin,
	      GeditWindow *window)
{
        GeditPanel              *side_panel;
        GtkWidget               *symbol_browser_panel;
        GtkWidget               *image;
        ScPlugin		*self;
        
        self = SC_PLUGIN (plugin);
        /*TODO get icon for left pane tab */
        image = gtk_image_new_from_stock (GTK_STOCK_CONVERT,
                                          GTK_ICON_SIZE_MENU);
        
        g_return_if_fail (g_object_get_data (G_OBJECT (window), SC_PANEL_KEY) == NULL);
        
        /* add a new panel to the side pane */
        
        side_panel = gedit_window_get_side_panel (window);
        symbol_browser_panel = sc_symbols_panel_new (window);

	self->priv->panel = symbol_browser_panel;
        
        gedit_panel_add_item (side_panel, 
                        symbol_browser_panel, 
                        "Symbol Browser", 
                        image);
        
        gtk_widget_show_all (GTK_WIDGET (side_panel));
        
        g_object_set_data (G_OBJECT (window), 
                        SC_PANEL_KEY,
                        symbol_browser_panel);
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	ScPlugin * self = SC_PLUGIN(plugin);
	self->priv->gedit_window = window;
	gedit_debug (DEBUG_PLUGINS);
	
	create_panel (plugin, window);
	
	g_signal_connect (window, "tab-added",
			  G_CALLBACK (tab_added_cb),
			  self);

	g_signal_connect (window, "active-tab-changed",
			  G_CALLBACK (tab_added_cb),
			  self);
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{
	gedit_debug (DEBUG_PLUGINS);
	/*TODO Disconnect signals*/
}

static void
impl_update_ui (GeditPlugin *plugin,
		GeditWindow *window)
{
}

static void
sc_plugin_class_init (ScPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = sc_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
	plugin_class->update_ui = impl_update_ui;

	g_type_class_add_private (object_class, 
				  sizeof (ScPluginPrivate));
}

