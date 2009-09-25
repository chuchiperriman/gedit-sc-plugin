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
#include <gdk/gdkkeysyms.h>
#include <glib/gi18n-lib.h>
#include <gedit/gedit-debug.h>

#include "sc-plugin.h"
#include "sc-menu.h"
#include "sc-utils.h"
#include "sc-provider-csymbols.h"
#include "sc-provider-csymbols-goto.h"
#include "sc-provider-project-csymbols.h"
#include "sc-symbols-panel.h"
#include "sc-language-manager.h"
#include "sc-lm-c.h"
#include "sc-ctags.h"

#define SC_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), SC_TYPE_PLUGIN, ScPluginPrivate))
#define document_get_provider_symbols(doc) (GTK_SOURCE_COMPLETION_PROVIDER (g_object_get_data (G_OBJECT (doc), SC_PROVIDER_SYMBOLS_KEY)))
#define document_get_provider_symbols_goto(doc) (GTK_SOURCE_COMPLETION_PROVIDER (g_object_get_data (G_OBJECT (doc), SC_PROVIDER_SYMBOLS_GOTO_KEY)))
#define document_get_provider_project_symbols(doc) (GTK_SOURCE_COMPLETION_PROVIDER (g_object_get_data (G_OBJECT (doc), SC_PROVIDER_PROJECT_SYMBOLS_KEY)))

#define SC_STOCK_ICONS "sc-stock-icons"
#define SC_PROVIDER_SYMBOLS_KEY "sc-provider-symbols"
#define SC_PROVIDER_SYMBOLS_GOTO_KEY "sc-provider-symbols-goto"
#define SC_PROVIDER_PROJECT_SYMBOLS_KEY "sc-provider-project-symbols"

const gchar* ICONS[] = {
	"symbol-class.png",
	"symbol-constant.png",
	"symbol-define.png",
	"symbol-enumerator.png",
	"symbol-function.png",
	"symbol-macro.png",
	"symbol-member.png",
	"symbol-method.png",
	"symbol-struct.png",
	"symbol-typedef.png",
	"symbol-variable.png",
	NULL
};

struct _ScPluginPrivate
{
	GeditWindow	*gedit_window;
	GtkWidget 	*window;
	GtkWidget 	*panel;
	ScMenu		*menu;
	GHashTable	*lmanagers;
	ScLanguageManager *current_lm;
};

typedef struct _ViewAndCompletion ViewAndCompletion;

GEDIT_PLUGIN_REGISTER_TYPE (ScPlugin, sc_plugin)

static void document_enable (ScPlugin *self, GeditDocument *doc);
static void document_disable (ScPlugin *self, GeditDocument *doc);

static void
sc_plugin_init (ScPlugin *plugin)
{
	plugin->priv = SC_PLUGIN_GET_PRIVATE (plugin);
	
	plugin->priv->lmanagers = g_hash_table_new_full (g_str_hash,
							 g_str_equal,
							 g_free,
							 g_object_unref);
	plugin->priv->current_lm = NULL;
	
	gedit_debug_message (DEBUG_PLUGINS,
			     "ScPlugin initializing");
}

static void
sc_plugin_finalize (GObject *object)
{
	ScPlugin *self = SC_PLUGIN (object);

	if (self->priv->current_lm)
	{
		sc_language_manager_set_active (self->priv->current_lm, FALSE);
		self->priv->current_lm = NULL;
	}
	
	g_hash_table_destroy (self->priv->lmanagers);

	
	gedit_debug_message (DEBUG_PLUGINS,
			     "ScPlugin finalizing");
	G_OBJECT_CLASS (sc_plugin_parent_class)->finalize (object);
}

static void
populate_panel (ScPlugin *self, GeditDocument *doc)
{
	GList *symbols = NULL;
	if (self->priv->current_lm)
	{
		symbols = sc_language_manager_get_document_symbols (self->priv->current_lm);
	}

	sc_symbols_panel_populate (self->priv->panel, symbols);
}

static gboolean
view_key_press_event_cb (GtkWidget *view,
                         GdkEventKey *event,
                         gpointer user_data)
{
	
	GeditDocument *doc;
	doc = GEDIT_DOCUMENT (gtk_text_view_get_buffer (GTK_TEXT_VIEW (view)));
	GtkSourceCompletion *comp = gtk_source_view_get_completion (GTK_SOURCE_VIEW (view));
	guint s = event->state & gtk_accelerator_get_default_mod_mask();
	
        if (s == GDK_CONTROL_MASK && event->keyval == GDK_m)
        {
		/*	
        	GList *providers = g_list_append (NULL, document_get_provider_symbols_goto(doc));
		gtk_source_completion_show (comp,
					    providers,
					    NULL,
					    NULL);
		g_list_free (providers);
                return TRUE;
                */
        }

        return FALSE;
}

static void
tab_changed_cb (GeditWindow *geditwindow,
		GeditTab    *tab,
		ScPlugin    *self)
{
	GeditDocument *doc = gedit_tab_get_document (tab);

	document_enable (self, doc);
	populate_panel (self, doc);
}

static void
tab_state_changed_cb (GeditWindow *window,
		      ScPlugin *self)
{
	GeditDocument *doc = gedit_window_get_active_document (window);
	document_enable (self, doc);
	populate_panel (self, doc);
}

static void
create_panel (GeditPlugin *plugin,
	      GeditWindow *window)
{
        GeditPanel              *side_panel;
        GtkWidget               *symbol_browser_panel;
        GtkWidget               *image;
	GdkPixbuf               *pixbuf;
        ScPlugin		*self;
        
        self = SC_PLUGIN (plugin);
        
	pixbuf = sc_utils_get_theme_pixbuf (SC_APP_ICON_NAME);
        image = gtk_image_new_from_pixbuf (pixbuf);
	g_object_unref (pixbuf);
	
        side_panel = gedit_window_get_side_panel (window);
        symbol_browser_panel = sc_symbols_panel_new (window);

	self->priv->panel = symbol_browser_panel;
        
        gedit_panel_add_item (side_panel, 
                        symbol_browser_panel, 
                        _("Symbol Browser"), 
                        image);
        
        gtk_widget_show_all (GTK_WIDGET (side_panel));
        
}

static ScLanguageManager*
get_language_manager (ScPlugin		*self, 
		      const gchar	*lang)
{
	ScLanguageManager *lm = g_hash_table_lookup (self->priv->lmanagers, lang);
	if (!lm)
	{
		/*TODO testing, we need to load the lm correctly*/
		/*
		gchar **mimes = gtk_source_language_get_mime_types (language);

		for (i = 0; mimes[i] != NULL; i++)
		{
			g_debug ("mime: %s", mimes[i]);
		}
		*/
		
		if (strcmp (lang, "C") == 0)
		{
			lm = SC_LANGUAGE_MANAGER (sc_lm_c_new ());
			g_hash_table_insert (self->priv->lmanagers,
					     g_strdup (lang),
					     lm);
		}
		else
		{
			g_debug ("no lm: %s", lang);
		}
	}
	
	return lm;
}

static void
document_enable (ScPlugin *self, GeditDocument *doc)
{
	ScLanguageManager *lm;
	GtkSourceLanguage *language;
	GeditTab *tab;
	GeditView *view;
	GtkSourceCompletion *comp;
	GtkSourceCompletionProvider *prov;
	
	language = gtk_source_buffer_get_language (GTK_SOURCE_BUFFER (doc));
	if (!language)
	{
		if (self->priv->current_lm)
		{
			sc_language_manager_set_active (self->priv->current_lm, FALSE);
			self->priv->current_lm = NULL;
		} 
		return;
	}
	
	lm = get_language_manager (self, gtk_source_language_get_name (language));
	
	if (!lm)
	{
		if (self->priv->current_lm)
		{
			sc_language_manager_set_active (self->priv->current_lm, FALSE);
			self->priv->current_lm = NULL;
		} 
		return;
	}
	
	if (self->priv->current_lm != NULL && self->priv->current_lm != lm)
	{
		sc_language_manager_set_active (self->priv->current_lm, FALSE);
	}
	
	self->priv->current_lm = lm;
	
	sc_language_manager_activate_document (lm, doc);
	
	if (document_get_provider_symbols(doc))
	{
		//This document has the providers
		return;
	}
	
	tab = gedit_tab_get_from_document (doc);
	view = gedit_tab_get_view (tab);
	comp = gtk_source_view_get_completion (GTK_SOURCE_VIEW (view));

	prov = GTK_SOURCE_COMPLETION_PROVIDER(sc_provider_csymbols_new (doc, lm));
	gtk_source_completion_add_provider (comp, prov, NULL);
	g_object_set_data (G_OBJECT (doc), SC_PROVIDER_SYMBOLS_KEY, prov);
	g_object_unref (prov);

	prov = GTK_SOURCE_COMPLETION_PROVIDER(sc_provider_project_csymbols_new (doc, lm));
	gtk_source_completion_add_provider (comp, prov, NULL);
	g_object_set_data (G_OBJECT (doc), SC_PROVIDER_PROJECT_SYMBOLS_KEY, prov);
	g_object_unref (prov);	

	prov = GTK_SOURCE_COMPLETION_PROVIDER(sc_provider_csymbols_goto_new (doc, lm));
	gtk_source_completion_add_provider (comp, prov, NULL);
	g_object_set_data (G_OBJECT (doc), SC_PROVIDER_SYMBOLS_GOTO_KEY, prov);
	g_object_unref (prov);

	g_signal_connect (view, "key-press-event",
			  G_CALLBACK (view_key_press_event_cb),
			  self);
}

static void
document_disable (ScPlugin *self, GeditDocument *doc)
{
	
	GeditTab *tab = gedit_tab_get_from_document (doc);
	GeditView *view = gedit_tab_get_view (tab);
	GtkSourceCompletion *completion = gtk_source_view_get_completion (GTK_SOURCE_VIEW (view));
	GtkSourceCompletionProvider *prov;
	
	prov = document_get_provider_symbols (doc);
	gtk_source_completion_remove_provider (completion, prov, NULL);
	prov = document_get_provider_symbols_goto (doc);
	gtk_source_completion_remove_provider (completion, prov, NULL);
	prov = document_get_provider_project_symbols (doc);
	gtk_source_completion_remove_provider (completion, prov, NULL);

	g_signal_handlers_disconnect_by_func (view,
					      G_CALLBACK (view_key_press_event_cb),
					      self);
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	GList *docs, *l;
	GeditDocument *doc;
	ScPlugin * self = SC_PLUGIN(plugin);

	gedit_debug (DEBUG_PLUGINS);
	
	self->priv->gedit_window = window;
		
	create_panel (plugin, window);
	
	g_signal_connect (window, "active-tab-changed",
			  G_CALLBACK (tab_changed_cb),
			  self);
	
	g_signal_connect (window, "active-tab-state-changed",
			  G_CALLBACK (tab_state_changed_cb),
			  self);

	/* Adding the menu */
	self->priv->menu = sc_menu_new (window);
	sc_menu_enable (self->priv->menu);
		
	docs = gedit_window_get_documents (window);
	for (l = docs; l != NULL; l = g_list_next (l))
	{
		doc = GEDIT_DOCUMENT (l->data);
		document_enable (self, doc);
	}
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{
	
	GeditPanel *side_panel;
	GeditDocument *doc;
	GList *docs, *l;
	ScPlugin *self = SC_PLUGIN(plugin);

	gedit_debug (DEBUG_PLUGINS);
	g_signal_handlers_disconnect_by_func (window,
					      G_CALLBACK (tab_changed_cb),
					      self);

	g_signal_handlers_disconnect_by_func (window,
					      G_CALLBACK (tab_state_changed_cb),
					      self);
	side_panel = gedit_window_get_side_panel (window);
	gedit_panel_remove_item (side_panel, self->priv->panel);

	sc_menu_disable (self->priv->menu);
	self->priv->menu = NULL;

	docs = gedit_window_get_documents (window);
	for (l = docs; l != NULL; l = g_list_next (l))
	{
		doc = GEDIT_DOCUMENT (l->data);
		document_disable (self, doc);
	}
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

