 /* gtksourcecompletiontrigger-members.c - Type here a short description of your trigger
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
#include "gtksourcecompletiontrigger-members.h"

struct _GtkSourceCompletionTriggerMembersPrivate {
	GtkSourceCompletionCompletion *completion;
	gint init_offset;
};

#define SC_TRIGGER_MEMBERS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SC_TYPE_TRIGGER_MEMBERS, GtkSourceCompletionTriggerMembersPrivate))

enum  {
	SC_TRIGGER_MEMBERS_DUMMY_PROPERTY,
};

static const gchar* gtk_source_completion_trigger_members_real_get_name(GtkSourceCompletionTrigger* base);
static gboolean gtk_source_completion_trigger_members_real_activate (GtkSourceCompletionTrigger* base);
static gboolean gtk_source_completion_trigger_members_real_deactivate (GtkSourceCompletionTrigger* base);

static gpointer gtk_source_completion_trigger_members_parent_class = NULL;
static GtkSourceCompletionTriggerIface* gtk_source_completion_trigger_members_parent_iface = NULL;


static const gchar* gtk_source_completion_trigger_members_real_get_name(GtkSourceCompletionTrigger *self)
{
	return SC_TRIGGER_MEMBERS_NAME;
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
view_delete_cd (GtkTextBuffer *textbuffer,
	       GtkTextIter   *start,
	       GtkTextIter   *end,
	       gpointer       user_data)
{
	GtkSourceCompletionTriggerMembers *self = SC_TRIGGER_MEMBERS (user_data);
	if (GTK_WIDGET_VISIBLE (self->priv->completion) && 
	    gtk_source_completion_completion_get_active_trigger(self->priv->completion) == SC_TRIGGER (self))
	{
		gint offset = gtk_text_iter_get_line_offset (start);
		if (offset < self->priv->init_offset)
		{
			gtk_source_completion_completion_finish_completion (self->priv->completion);
		}
		else
		{
			gchar *temp;
			GtkTextIter init_iter = *start;
			gtk_text_iter_set_line_offset (&init_iter, self->priv->init_offset);
			
			/*Filter the current proposals */
			temp = gtk_text_iter_get_text (&init_iter, start);
			gtk_source_completion_completion_filter_proposals (self->priv->completion,
							 symbols_filter_func,
							 temp);
			g_free (temp);
		}
		
	}
}

static void
view_insert_text_cb (GtkTextBuffer *buffer,
		     GtkTextIter *location,
		     gchar *text,
		     gint len,
		     gpointer user_data)
{
	GtkTextIter iter = *location;
	gchar *ch;
	gchar *temp;
	gboolean found = FALSE;
	
	GtkSourceCompletionTriggerMembers *self = SC_TRIGGER_MEMBERS (user_data);
	if (g_strcmp0 (text, ">") == 0)
	{
		if (gtk_text_iter_backward_chars (&iter, 2) &&
		    gtk_text_iter_backward_char (location))
		{
			ch = gtk_text_buffer_get_text (buffer,
						       &iter,
						       location,
						       FALSE);
			if (g_strcmp0 (ch, "-") == 0)
				found = TRUE;
		}
	}
	else if (g_strcmp0 (text, ".") == 0)
	{
		found = TRUE;
	}
	else if (GTK_WIDGET_VISIBLE (self->priv->completion) && 
		 gtk_source_completion_completion_get_active_trigger(self->priv->completion) == SC_TRIGGER (self))
	{
		if (gtk_source_completion_char_is_separator (g_utf8_get_char (text)) ||
		    gtk_text_iter_get_line_offset (location) < self->priv->init_offset)
		{
			gtk_source_completion_completion_finish_completion (self->priv->completion);
		}
		else
		{
			GtkTextIter init_iter = *location;
			gtk_text_iter_set_line_offset (&init_iter, self->priv->init_offset);
			
			/*Filter the current proposals */
			temp = gtk_text_iter_get_text (&init_iter, location);
			gtk_source_completion_completion_filter_proposals (self->priv->completion,
							 symbols_filter_func,
							 temp);
			g_free (temp);
		}
	}
	
	if (found)
	{
		self->priv->init_offset = gtk_text_iter_get_line_offset (location);
		gtk_source_completion_completion_trigger_event (self->priv->completion,
					      SC_TRIGGER (self));
	}
}

static gboolean
gtk_source_completion_trigger_members_real_activate (GtkSourceCompletionTrigger* base)
{
	g_debug("Activating Members trigger");
	GtkSourceCompletionTriggerMembers *self = SC_TRIGGER_MEMBERS(base);
	
	GtkTextView *view = gtk_source_completion_completion_get_view (self->priv->completion);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (view);
	g_signal_connect_after (buffer,
				"insert-text",
				G_CALLBACK (view_insert_text_cb),
				self);
	g_signal_connect_after (buffer,
				"delete-range",
				G_CALLBACK (view_delete_cd),
				self);
	return TRUE;
}

static gboolean
gtk_source_completion_trigger_members_real_deactivate (GtkSourceCompletionTrigger* base)
{
	g_debug("Deactivating Members trigger");
	return FALSE;
}

static void gtk_source_completion_trigger_members_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gtk_source_completion_trigger_members_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}

static void gtk_source_completion_trigger_members_init (GtkSourceCompletionTriggerMembers * self)
{
	self->priv = g_new0(GtkSourceCompletionTriggerMembersPrivate, 1);
	self->priv->init_offset = -1;
	g_debug("Init Members trigger");
}

static void gtk_source_completion_trigger_members_finalize(GObject *object)
{
	g_debug("Finish Members trigger");
	GtkSourceCompletionTriggerMembers *self;
	self = SC_TRIGGER_MEMBERS(object);
	G_OBJECT_CLASS(gtk_source_completion_trigger_members_parent_class)->finalize(object);
}

static void gtk_source_completion_trigger_members_class_init (GtkSourceCompletionTriggerMembersClass * klass)
{
	gtk_source_completion_trigger_members_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gtk_source_completion_trigger_members_get_property;
	G_OBJECT_CLASS (klass)->set_property = gtk_source_completion_trigger_members_set_property;
	G_OBJECT_CLASS (klass)->finalize = gtk_source_completion_trigger_members_finalize;
}

static void gtk_source_completion_trigger_members_interface_init (GtkSourceCompletionTriggerIface * iface)
{
	gtk_source_completion_trigger_members_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_name = gtk_source_completion_trigger_members_real_get_name;
	iface->activate = gtk_source_completion_trigger_members_real_activate;
	iface->deactivate = gtk_source_completion_trigger_members_real_deactivate;
}

GType gtk_source_completion_trigger_members_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GtkSourceCompletionTriggerMembersClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gtk_source_completion_trigger_members_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GtkSourceCompletionTriggerMembers), 0, (GInstanceInitFunc) gtk_source_completion_trigger_members_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GtkSourceCompletionTriggerMembers", &g_define_type_info, 0);
		static const GInterfaceInfo gtk_source_completion_trigger_members_info = { (GInterfaceInitFunc) gtk_source_completion_trigger_members_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, SC_TYPE_TRIGGER, &gtk_source_completion_trigger_members_info);
	}
	return g_define_type_id;
}

/**
 * gtk_source_completion_trigger_members_new:
 *
 * Returns The new #GtkSourceCompletionTriggerMembers
 *
 */
GtkSourceCompletionTriggerMembers*
gtk_source_completion_trigger_members_new(GtkSourceCompletionCompletion *completion)
{
	GtkSourceCompletionTriggerMembers *self = SC_TRIGGER_MEMBERS (g_object_new (SC_TYPE_TRIGGER_MEMBERS, NULL));
	self->priv->completion = completion;
	
	return self;
}

