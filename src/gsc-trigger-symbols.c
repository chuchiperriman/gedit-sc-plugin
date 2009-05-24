 /* gtksourcecompletiontrigger-symbols.c - Type here a short description of your trigger
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
#include <gtksourceview/gtksourcecompletionutils.h>

#include "gtksourcecompletiontrigger-symbols.h"

struct _GtkSourceCompletionTriggerSymbolsPrivate {
	GtkSourceCompletionCompletion *comp;
	gint init_offset;
};

#define SC_TRIGGER_SYMBOLS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SC_TYPE_TRIGGER_SYMBOLS, GtkSourceCompletionTriggerSymbolsPrivate))

enum  {
	SC_TRIGGER_SYMBOLS_DUMMY_PROPERTY,
};

static const gchar* gtk_source_completion_trigger_symbols_real_get_name(GtkSourceCompletionTrigger* base);
static gboolean gtk_source_completion_trigger_symbols_real_activate (GtkSourceCompletionTrigger* base);
static gboolean gtk_source_completion_trigger_symbols_real_deactivate (GtkSourceCompletionTrigger* base);

static gpointer gtk_source_completion_trigger_symbols_parent_class = NULL;
static GtkSourceCompletionTriggerIface* gtk_source_completion_trigger_symbols_parent_iface = NULL;


static const gchar* gtk_source_completion_trigger_symbols_real_get_name(GtkSourceCompletionTrigger *base)
{
	return SC_TRIGGER_SYMBOLS_NAME;
}

static gboolean
gtk_source_completion_trigger_symbols_real_activate (GtkSourceCompletionTrigger* base)
{
	g_debug("Activating Symbols trigger");
	return TRUE;
}

static gboolean
gtk_source_completion_trigger_symbols_real_deactivate (GtkSourceCompletionTrigger* base)
{
	g_debug("Deactivating Symbols trigger");
	return FALSE;
}

static void gtk_source_completion_trigger_symbols_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gtk_source_completion_trigger_symbols_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}

static void gtk_source_completion_trigger_symbols_init (GtkSourceCompletionTriggerSymbols * self)
{
	self->priv = g_new0(GtkSourceCompletionTriggerSymbolsPrivate, 1);
	self->priv->init_offset = -1;
	g_debug("Init Symbols trigger");
}

static void gtk_source_completion_trigger_symbols_finalize(GObject *object)
{
	g_debug("Finish Symbols trigger");
	GtkSourceCompletionTriggerSymbols *self;
	self = SC_TRIGGER_SYMBOLS(object);
	G_OBJECT_CLASS(gtk_source_completion_trigger_symbols_parent_class)->finalize(object);
}

static void gtk_source_completion_trigger_symbols_class_init (GtkSourceCompletionTriggerSymbolsClass * klass)
{
	gtk_source_completion_trigger_symbols_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gtk_source_completion_trigger_symbols_get_property;
	G_OBJECT_CLASS (klass)->set_property = gtk_source_completion_trigger_symbols_set_property;
	G_OBJECT_CLASS (klass)->finalize = gtk_source_completion_trigger_symbols_finalize;
}

static void gtk_source_completion_trigger_symbols_interface_init (GtkSourceCompletionTriggerIface * iface)
{
	gtk_source_completion_trigger_symbols_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_name = gtk_source_completion_trigger_symbols_real_get_name;
	iface->activate = gtk_source_completion_trigger_symbols_real_activate;
	iface->deactivate = gtk_source_completion_trigger_symbols_real_deactivate;
}

GType gtk_source_completion_trigger_symbols_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GtkSourceCompletionTriggerSymbolsClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gtk_source_completion_trigger_symbols_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GtkSourceCompletionTriggerSymbols), 0, (GInstanceInitFunc) gtk_source_completion_trigger_symbols_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GtkSourceCompletionTriggerSymbols", &g_define_type_info, 0);
		static const GInterfaceInfo gtk_source_completion_trigger_symbols_info = { (GInterfaceInitFunc) gtk_source_completion_trigger_symbols_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, SC_TYPE_TRIGGER, &gtk_source_completion_trigger_symbols_info);
	}
	return g_define_type_id;
}

static gboolean
symbols_filter_func (GtkSourceCompletionProposal *proposal,
		     gpointer user_data)
{
	const gchar *label = gtk_source_completion_proposal_get_label (proposal);
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
	g_return_if_fail (SC_IS_TRIGGER_SYMBOLS (user_data));
	
	gchar *filter, *temp;
	GtkTextIter iter = *location;
	
	
	GtkSourceCompletionTriggerSymbols *self = SC_TRIGGER_SYMBOLS (user_data);
	
	/*FIXME Configure these symbols*/
	if (g_strcmp0 (text, ",") == 0)
	{
		self->priv->init_offset = gtk_text_iter_get_line_offset (location);
		gtk_source_completion_completion_trigger_event (self->priv->comp,
					   SC_TRIGGER (self));
	}
	else
	{
		if (GTK_WIDGET_VISIBLE (self->priv->comp) && 
		    gtk_source_completion_completion_get_active_trigger(self->priv->comp) == SC_TRIGGER (self))
		{
			if (gtk_source_completion_char_is_separator (g_utf8_get_char (text)) ||
			    gtk_text_iter_get_line_offset (location) < self->priv->init_offset)
			{
				gtk_source_completion_completion_finish_completion (self->priv->comp);
			}
			else
			{
				GtkTextView *view = gtk_source_completion_completion_get_view (self->priv->comp);
				GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
				/*Filter the current proposals */
				while (gtk_text_iter_get_line_offset (&iter) > self->priv->init_offset + 1)
				{
					gtk_text_iter_backward_char (&iter);
				}
				
				temp = gtk_text_buffer_get_text (buffer,
								   &iter,
								   location,
								   FALSE);
				filter = g_strconcat (temp, text, NULL);
				g_free (temp);
				gtk_source_completion_completion_filter_proposals (self->priv->comp,
								      symbols_filter_func,
								      filter);
				g_free (filter);
			}
		}
	}
}

/**
 * gtk_source_completion_trigger_symbols_new:
 *
 * Returns The new #GtkSourceCompletionTriggerSymbols
 *
 */
GtkSourceCompletionTriggerSymbols*
gtk_source_completion_trigger_symbols_new(GtkSourceCompletionCompletion *comp)
{
	GtkSourceCompletionTriggerSymbols *self = SC_TRIGGER_SYMBOLS (g_object_new (SC_TYPE_TRIGGER_SYMBOLS, NULL));
	self->priv->comp = comp;
	GtkTextView *view = gtk_source_completion_completion_get_view (comp);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
	/*FIXME disconnect on destroy*/
	g_signal_connect(buffer,"insert-text",G_CALLBACK(insert_text_cb), self);
	return self;
}

