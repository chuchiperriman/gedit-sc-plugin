/*
 * sourcecompletion-plugin.c - Adds (auto)completion support to gedit
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

#include "sourcecompletion-plugin.h"

#include <gdk/gdk.h>
#include <glib/gi18n-lib.h>
#include <gedit/gedit-debug.h>
#include <gconf/gconf-client.h>
#include <gtksourcecompletion/gsc-trigger.h>
#include <gtksourcecompletion/gsc-trigger-customkey.h>
#include <gtksourcecompletion/gsc-trigger-autowords.h>

#include "gsc-trigger-symbols.h"
#include "gsc-provider-csymbols.h"

#define SOURCECOMPLETION_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), TYPE_SOURCECOMPLETION_PLUGIN, SourcecompletionPluginPrivate))

struct _SourcecompletionPluginPrivate
{
	GeditWindow *gedit_window;
};

GEDIT_PLUGIN_REGISTER_TYPE (SourcecompletionPlugin, sourcecompletion_plugin)

static void
sourcecompletion_plugin_init (SourcecompletionPlugin *plugin)
{
	plugin->priv = SOURCECOMPLETION_PLUGIN_GET_PRIVATE (plugin);
	gedit_debug_message (DEBUG_PLUGINS,
			     "Sourcecompletion initializing");
}

static void
sourcecompletion_plugin_finalize (GObject *object)
{
	gedit_debug_message (DEBUG_PLUGINS,
			     "Sourcecompletion finalizing");
	SourcecompletionPlugin * dw_plugin = (SourcecompletionPlugin*)object;
	G_OBJECT_CLASS (sourcecompletion_plugin_parent_class)->finalize (object);
}


static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	SourcecompletionPlugin * dw_plugin = (SourcecompletionPlugin*)plugin;
	dw_plugin->priv->gedit_window = window;
	gedit_debug (DEBUG_PLUGINS);
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{
	gedit_debug (DEBUG_PLUGINS);
}

static void
impl_update_ui (GeditPlugin *plugin,
		GeditWindow *window)
{
	SourcecompletionPlugin *self = (SourcecompletionPlugin*)plugin;
	self->priv->gedit_window = window;
	gedit_debug (DEBUG_PLUGINS);
	
	GtkTextView* view = GTK_TEXT_VIEW(gedit_window_get_active_view(window));
	if (view!=NULL)
	{
		GscManager *comp = gsc_manager_get_from_view(view);
		if (comp == NULL)
		{
			comp = gsc_manager_new(GTK_TEXT_VIEW(view));
			gsc_manager_activate(comp);
		}

		if (gsc_manager_get_trigger(comp,GSC_TRIGGER_SYMBOLS_NAME) == NULL)
		{
			GscTriggerSymbols *trigger = gsc_trigger_symbols_new(comp);
			gsc_manager_register_trigger(comp,GSC_TRIGGER(trigger));
			g_object_unref(trigger);
		}
		
		/*FIXME create user-request trigger if doesn't exists*/
		
		if (gsc_manager_get_provider(comp,GSC_PROVIDER_CSYMBOLS_NAME) == NULL)
		{
			GscProviderCsymbols *prov = gsc_provider_csymbols_new (comp);
			gsc_manager_register_provider(comp,
						      GSC_PROVIDER (prov),
						      GSC_TRIGGER_SYMBOLS_NAME);
						      
			if (gsc_manager_get_provider(comp, USER_REQUEST_TRIGGER_NAME) == NULL)
			{
				gsc_manager_register_provider(comp,
							      GSC_PROVIDER (prov),
							      USER_REQUEST_TRIGGER_NAME);
			}
			g_object_unref(prov);
		}
	}
}

static void
sourcecompletion_plugin_class_init (SourcecompletionPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = sourcecompletion_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
	plugin_class->update_ui = impl_update_ui;

	g_type_class_add_private (object_class, 
				  sizeof (SourcecompletionPluginPrivate));
}
