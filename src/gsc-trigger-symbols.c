 /* gsc-trigger-symbols.c - Type here a short description of your trigger
 *
 * Copyright (C) 2009 - perriman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <glib/gprintf.h>
#include <string.h>
#include <ctype.h>
#include "gsc-trigger-symbols.h"

struct _GscTriggerSymbolsPrivate {
	GscManager *manager;
};

#define GSC_TRIGGER_SYMBOLS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GSC_TYPE_TRIGGER_SYMBOLS, GscTriggerSymbolsPrivate))

enum  {
	GSC_TRIGGER_SYMBOLS_DUMMY_PROPERTY,
};

static const gchar* gsc_trigger_symbols_real_get_name(GscTrigger* base);
static gboolean gsc_trigger_symbols_real_activate (GscTrigger* base);
static gboolean gsc_trigger_symbols_real_deactivate (GscTrigger* base);

static gpointer gsc_trigger_symbols_parent_class = NULL;
static GscTriggerIface* gsc_trigger_symbols_parent_iface = NULL;


static const gchar* gsc_trigger_symbols_real_get_name(GscTrigger *base)
{
	return GSC_TRIGGER_SYMBOLS_NAME;
}

static gboolean
gsc_trigger_symbols_real_activate (GscTrigger* base)
{
	g_debug("Activating Symbols trigger");
	GscTriggerSymbols *self = GSC_TRIGGER_SYMBOLS(base);

	return TRUE;
}

static gboolean
gsc_trigger_symbols_real_deactivate (GscTrigger* base)
{
	g_debug("Deactivating Symbols trigger");
	GscTriggerSymbols *self = GSC_TRIGGER_SYMBOLS(base);
	return FALSE;
}

static void gsc_trigger_symbols_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gsc_trigger_symbols_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}

static void gsc_trigger_symbols_init (GscTriggerSymbols * self)
{
	self->priv = g_new0(GscTriggerSymbolsPrivate, 1);
	g_debug("Init Symbols trigger");
}

static void gsc_trigger_symbols_finalize(GObject *object)
{
	g_debug("Finish Symbols trigger");
	GscTriggerSymbols *self;
	self = GSC_TRIGGER_SYMBOLS(object);
	G_OBJECT_CLASS(gsc_trigger_symbols_parent_class)->finalize(object);
}

static void gsc_trigger_symbols_class_init (GscTriggerSymbolsClass * klass)
{
	gsc_trigger_symbols_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gsc_trigger_symbols_get_property;
	G_OBJECT_CLASS (klass)->set_property = gsc_trigger_symbols_set_property;
	G_OBJECT_CLASS (klass)->finalize = gsc_trigger_symbols_finalize;
}

static void gsc_trigger_symbols_interface_init (GscTriggerIface * iface)
{
	gsc_trigger_symbols_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_name = gsc_trigger_symbols_real_get_name;
	iface->activate = gsc_trigger_symbols_real_activate;
	iface->deactivate = gsc_trigger_symbols_real_deactivate;
}

GType gsc_trigger_symbols_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GscTriggerSymbolsClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gsc_trigger_symbols_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GscTriggerSymbols), 0, (GInstanceInitFunc) gsc_trigger_symbols_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GscTriggerSymbols", &g_define_type_info, 0);
		static const GInterfaceInfo gsc_trigger_symbols_info = { (GInterfaceInitFunc) gsc_trigger_symbols_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GSC_TYPE_TRIGGER, &gsc_trigger_symbols_info);
	}
	return g_define_type_id;
}

static gboolean
symbols_filter_func (GscProposal *proposal,
	     gpointer user_data)
{
	const gchar *label = gsc_proposal_get_label (proposal);
	const gchar *text = (const gchar*)user_data;
	return g_str_has_prefix (label, text);
}

static void
insert_text_cb (GtkTextBuffer *textbuffer,
		GtkTextIter   *location,
		gchar         *text,
		gint           len,
		gpointer       user_data)
{
	g_return_if_fail (GSC_IS_TRIGGER_SYMBOLS (user_data));
	
	GscTriggerSymbols *self = GSC_TRIGGER_SYMBOLS (user_data);
	
	/*FIXME Configure these symbols*/
	if (g_strcmp0 (text, ".") == 0)
	{
		gsc_manager_trigger_event (self->priv->manager,
					   GSC_TRIGGER_SYMBOLS_NAME);
		g_debug ("members completion");
	}
	else if (g_strcmp0 (text, "(") == 0)
	{
		g_debug ("show function calltip");
	}
	else if (g_strcmp0 (text, "{") == 0)
	{
		gsc_manager_trigger_event (self->priv->manager,
					   GSC_TRIGGER_SYMBOLS_NAME);
		g_debug ("actions completion");
	}
	else
	{
		if (gsc_manager_is_visible (self->priv->manager) && 
		    gsc_manager_get_active_trigger(self->priv->manager) == GSC_TRIGGER (self))
		{
			/*Filter the current proposals */
			gsc_manager_filter_current_proposals (self->priv->manager,
							      symbols_filter_func,
							      "Gsc");
		}
	}
}

/**
 * gsc_trigger_symbols_new:
 *
 * Returns The new #GscTriggerSymbols
 *
 */
GscTriggerSymbols*
gsc_trigger_symbols_new(GscManager *manager)
{
	GscTriggerSymbols *self = GSC_TRIGGER_SYMBOLS (g_object_new (GSC_TYPE_TRIGGER_SYMBOLS, NULL));
	self->priv->manager = manager;
	GtkTextView *view = gsc_manager_get_view (manager);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
	/*FIXME disconnect on destroy*/
	g_signal_connect(buffer,"insert-text",G_CALLBACK(insert_text_cb), self);
	return self;
}

