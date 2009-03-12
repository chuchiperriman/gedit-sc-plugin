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
#include "gsc-trigger-members.h"
#include "gsc-provider-csymbols.h"

#define SOURCECOMPLETION_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), TYPE_SOURCECOMPLETION_PLUGIN, SourcecompletionPluginPrivate))
#define GOTO_SYMBOLS_TRIGGER_NAME "Goto Symbols Trigger"

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
		GscCompletion *comp = gsc_completion_get_from_view(view);
		if (comp == NULL)
		{
			comp = GSC_COMPLETION (gsc_completion_new(GTK_TEXT_VIEW(view)));
			gsc_completion_set_active (comp, TRUE);
		}

		GscTrigger *trigger = gsc_completion_get_trigger(comp,GSC_TRIGGER_SYMBOLS_NAME);
		if (trigger == NULL)
		{
			trigger = GSC_TRIGGER (gsc_trigger_symbols_new(comp));
			gsc_completion_register_trigger(comp,GSC_TRIGGER(trigger));
			g_object_unref(trigger);
		}
	
		GscTrigger *goto_trigger = gsc_completion_get_trigger (comp, GOTO_SYMBOLS_TRIGGER_NAME);
		if (goto_trigger == NULL)
		{
			goto_trigger = GSC_TRIGGER (gsc_trigger_customkey_new(comp,
							GOTO_SYMBOLS_TRIGGER_NAME, 
							"<Control>o"));
			gsc_completion_register_trigger(comp,GSC_TRIGGER(goto_trigger));
			g_object_unref(goto_trigger);
		}
		GscTrigger *ur_trigger = gsc_completion_get_trigger(comp, USER_REQUEST_TRIGGER_NAME);
		if (ur_trigger == NULL)
		{
			ur_trigger = GSC_TRIGGER (gsc_trigger_customkey_new(comp,
						USER_REQUEST_TRIGGER_NAME, 
						"<Control>space"));
			gsc_completion_register_trigger(comp,GSC_TRIGGER(ur_trigger));
			g_object_unref(ur_trigger);
		}
		
		GscTrigger *m_trigger = gsc_completion_get_trigger(comp, GSC_TRIGGER_MEMBERS_NAME);
		if (m_trigger == NULL)
		{
			m_trigger = GSC_TRIGGER (gsc_trigger_members_new(comp));
			gsc_completion_register_trigger(comp,GSC_TRIGGER(m_trigger));
			g_object_unref(m_trigger);
		}
		
		/*FIXME create user-request trigger if doesn't exists*/
		GscProviderCsymbols *goto_prov;
		GscProviderCsymbols *ggoto_prov;
		GscProviderCsymbols *csym_prov;
		GscProviderCsymbols *mem_prov;
		
		if (gsc_completion_get_provider (comp, GSC_PROVIDER_CSYMBOLS_GOTO_NAME) == NULL)
		{
			goto_prov = gsc_provider_csymbols_new (comp,
							       window,
							       GOTO_TYPE);
			gsc_completion_register_provider(comp,
							 GSC_PROVIDER (goto_prov),
							 GSC_TRIGGER (goto_trigger));
			g_object_unref(goto_prov);
		}

		if (gsc_completion_get_provider (comp, GSC_PROVIDER_CSYMBOLS_NAME) == NULL)
		{
			csym_prov = gsc_provider_csymbols_new (comp,
							       window,
							       SYMBOLS_TYPE);
			gsc_completion_register_provider(comp,
						 	 GSC_PROVIDER (csym_prov),
							 ur_trigger);
			g_object_unref(csym_prov);
		}
		
		if (gsc_completion_get_provider (comp, GSC_PROVIDER_CSYMBOLS_MEMBERS_NAME) == NULL)
		{
			mem_prov = gsc_provider_csymbols_new (comp,
							       window,
							       MEMBERS_TYPE);
			gsc_completion_register_provider(comp,
						 	 GSC_PROVIDER (mem_prov),
							 m_trigger);
			g_object_unref(mem_prov);
		}
		
		if (gsc_completion_get_provider (comp, GSC_PROVIDER_CSYMBOLS_GLOBAL_GOTO_NAME) == NULL)
		{
			ggoto_prov = gsc_provider_csymbols_new (comp,
							       window,
							       GLOBAL_GOTO_TYPE);
			gsc_completion_register_provider(comp,
						 	 GSC_PROVIDER (ggoto_prov),
							 goto_trigger);
			g_object_unref(ggoto_prov);
		}
/*
		gsc_completion_register_provider(comp,
					      GSC_PROVIDER (prov),
					      GSC_TRIGGER (trigger));
		gsc_completion_register_provider(comp,
					      GSC_PROVIDER (prov),
					      ur_trigger);
*/
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
