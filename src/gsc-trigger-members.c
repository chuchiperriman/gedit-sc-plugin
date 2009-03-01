 /* gsc-trigger-members.c - Type here a short description of your trigger
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
#include "gsc-trigger-members.h"

struct _GscTriggerMembersPrivate {
	GscCompletion *completion;
};

#define GSC_TRIGGER_MEMBERS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GSC_TYPE_TRIGGER_MEMBERS, GscTriggerMembersPrivate))

enum  {
	GSC_TRIGGER_MEMBERS_DUMMY_PROPERTY,
};

static const gchar* gsc_trigger_members_real_get_name(GscTrigger* base);
static gboolean gsc_trigger_members_real_activate (GscTrigger* base);
static gboolean gsc_trigger_members_real_deactivate (GscTrigger* base);

static gpointer gsc_trigger_members_parent_class = NULL;
static GscTriggerIface* gsc_trigger_members_parent_iface = NULL;


static const gchar* gsc_trigger_members_real_get_name(GscTrigger *self)
{
	return GSC_TRIGGER_MEMBERS_NAME;
}

static void
view_insert_text_cb (GtkTextBuffer *buffer,
		     GtkTextIter *iter,
		     gchar *text,
		     gint len,
		     gpointer user_data)
{
	GtkTextIter iter2 = *iter;
	gchar *ch;
	gboolean found = FALSE;
	
	GscTriggerMembers *self = GSC_TRIGGER_MEMBERS (user_data);
	if (g_strcmp0 (text, ">") == 0)
	{
		if (gtk_text_iter_backward_chars (&iter2, 2) &&
		    gtk_text_iter_backward_char (iter))
		{
			ch = gtk_text_buffer_get_text (buffer,
						       &iter2,
						       iter,
						       FALSE);
			if (g_strcmp0 (ch, "-") == 0)
				found = TRUE;
		}
	}
	else if (g_strcmp0 (text, ".") == 0)
	{
		found = TRUE;
	}
	
	if (found)
	{
		gsc_completion_trigger_event (self->priv->completion,
					      GSC_TRIGGER (self));
	}
}

static gboolean
gsc_trigger_members_real_activate (GscTrigger* base)
{
	g_debug("Activating Members trigger");
	GscTriggerMembers *self = GSC_TRIGGER_MEMBERS(base);
	
	GtkTextView *view = gsc_completion_get_view (self->priv->completion);
	
	g_signal_connect_after (gtk_text_view_get_buffer (view),
				"insert-text",
				G_CALLBACK (view_insert_text_cb),
				self);

	return TRUE;
}

static gboolean
gsc_trigger_members_real_deactivate (GscTrigger* base)
{
	g_debug("Deactivating Members trigger");
	GscTriggerMembers *self = GSC_TRIGGER_MEMBERS(base);
	return FALSE;
}

static void gsc_trigger_members_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gsc_trigger_members_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}

static void gsc_trigger_members_init (GscTriggerMembers * self)
{
	self->priv = g_new0(GscTriggerMembersPrivate, 1);
	g_debug("Init Members trigger");
}

static void gsc_trigger_members_finalize(GObject *object)
{
	g_debug("Finish Members trigger");
	GscTriggerMembers *self;
	self = GSC_TRIGGER_MEMBERS(object);
	G_OBJECT_CLASS(gsc_trigger_members_parent_class)->finalize(object);
}

static void gsc_trigger_members_class_init (GscTriggerMembersClass * klass)
{
	gsc_trigger_members_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gsc_trigger_members_get_property;
	G_OBJECT_CLASS (klass)->set_property = gsc_trigger_members_set_property;
	G_OBJECT_CLASS (klass)->finalize = gsc_trigger_members_finalize;
}

static void gsc_trigger_members_interface_init (GscTriggerIface * iface)
{
	gsc_trigger_members_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_name = gsc_trigger_members_real_get_name;
	iface->activate = gsc_trigger_members_real_activate;
	iface->deactivate = gsc_trigger_members_real_deactivate;
}

GType gsc_trigger_members_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GscTriggerMembersClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gsc_trigger_members_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GscTriggerMembers), 0, (GInstanceInitFunc) gsc_trigger_members_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GscTriggerMembers", &g_define_type_info, 0);
		static const GInterfaceInfo gsc_trigger_members_info = { (GInterfaceInitFunc) gsc_trigger_members_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GSC_TYPE_TRIGGER, &gsc_trigger_members_info);
	}
	return g_define_type_id;
}

/**
 * gsc_trigger_members_new:
 *
 * Returns The new #GscTriggerMembers
 *
 */
GscTriggerMembers*
gsc_trigger_members_new(GscCompletion *completion)
{
	GscTriggerMembers *self = GSC_TRIGGER_MEMBERS (g_object_new (GSC_TYPE_TRIGGER_MEMBERS, NULL));
	self->priv->completion = completion;
	
	return self;
}

