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
	/*
	GtkTextView* view = GTK_TEXT_VIEW(gedit_window_get_active_view(window));
	if (view!=NULL)
	{
		GscManager *comp = gsc_manager_get_from_view(view);
		if (comp==NULL)
		{
			comp = gsc_manager_new(GTK_TEXT_VIEW(view));
			GValue value = {0,};
			g_value_init(&value,G_TYPE_STRING);
			g_value_set_string(&value,dw_plugin->priv->conf->si_keys);
			g_object_set (comp,
				      "info-keys",
				      dw_plugin->priv->conf->si_keys,
				      NULL);
		}

		if (gsc_manager_get_provider(comp,GSC_DOCUMENTWORDS_PROVIDER_NAME)==NULL)
		{
			
			GscTriggerCustomkey *ck_trigger = gsc_trigger_customkey_new(comp,
					OPEN_DOCS_TRIGGER_NAME, 
					dw_plugin->priv->conf->od_keys);
			gsc_manager_register_trigger(comp,GSC_TRIGGER(ck_trigger));
			g_object_unref(ck_trigger);
			
			if (dw_plugin->priv->conf->ac_enabled)
			{
				GscTrigger *ur_trigger = 
					gsc_manager_get_trigger(comp,USER_REQUEST_TRIGGER_NAME);
				if (ur_trigger==NULL)
				{
					ur_trigger = GSC_TRIGGER(gsc_trigger_customkey_new(
							comp,
							USER_REQUEST_TRIGGER_NAME,
							dw_plugin->priv->conf->ure_keys));
					gsc_manager_register_trigger(comp,ur_trigger);
					g_object_unref(ur_trigger);
				}
				GscTriggerAutowords *ac_trigger = gsc_trigger_autowords_new(comp);
				gsc_manager_register_trigger(comp,GSC_TRIGGER(ac_trigger));
				g_object_unref(ac_trigger);
				gsc_trigger_autowords_set_delay(ac_trigger,dw_plugin->priv->conf->ac_delay);
				GscDocumentwordsProvider *dw  = gsc_documentwords_provider_new(view);
				gsc_manager_register_provider(comp,GSC_PROVIDER(dw),GSC_TRIGGER_AUTOWORDS_NAME);
				gsc_manager_register_provider(comp,GSC_PROVIDER(dw),USER_REQUEST_TRIGGER_NAME);
				g_object_unref(dw);
			}
			
			
			if (dw_plugin->priv->conf->open_enabled)
			{
				GscGeditopendocProvider* od = gsc_geditopendoc_provider_new(window);
				gsc_manager_register_provider(comp,GSC_PROVIDER(od),OPEN_DOCS_TRIGGER_NAME);
				g_object_unref(od);
			}
			
			if (dw_plugin->priv->conf->recent_enabled)
			{
				GscGeditrecentProvider *grp = gsc_geditrecent_provider_new(window);
				gsc_manager_register_provider(comp,GSC_PROVIDER(grp),OPEN_DOCS_TRIGGER_NAME);
				g_object_unref(grp);
			}
			
			g_object_set (comp,
				      "autoselect", dw_plugin->priv->conf->autoselect_enabled,
				      NULL);
			
			gsc_manager_activate(comp);
		}

	}
	*/
}

static GtkWidget*
create_configure_dialog(GeditPlugin *plugin)
{
	return NULL;
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
	plugin_class->create_configure_dialog = create_configure_dialog;

	g_type_class_add_private (object_class, 
				  sizeof (SourcecompletionPluginPrivate));
}
