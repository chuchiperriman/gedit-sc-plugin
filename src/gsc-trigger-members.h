/*
 * gtksourcecompletiontrigger-members.h - Type here a short description of your trigger
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

#ifndef __TRIGGER_MEMBERS_H__
#define __TRIGGER_MEMBERS_H__

#include <glib.h>
#include <glib-object.h>
#include <gtksourceview/gtksourcecompletiontrigger.h>
#include <gtksourceview/gtksourcecompletioncompletion.h>

G_BEGIN_DECLS

#define SC_TYPE_TRIGGER_MEMBERS (gtk_source_completion_trigger_members_get_type ())
#define SC_TRIGGER_MEMBERS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_TRIGGER_MEMBERS, GtkSourceCompletionTriggerMembers))
#define SC_TRIGGER_MEMBERS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_TRIGGER_MEMBERS, GtkSourceCompletionTriggerMembersClass))
#define SC_IS_TRIGGER_MEMBERS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_TRIGGER_MEMBERS))
#define SC_IS_TRIGGER_MEMBERS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_TRIGGER_MEMBERS))
#define SC_TRIGGER_MEMBERS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_TRIGGER_MEMBERS, GtkSourceCompletionTriggerMembersClass))

#define SC_TRIGGER_MEMBERS_NAME "GtkSourceCompletionTriggerMembers"

typedef struct _GtkSourceCompletionTriggerMembers GtkSourceCompletionTriggerMembers;
typedef struct _GtkSourceCompletionTriggerMembersClass GtkSourceCompletionTriggerMembersClass;
typedef struct _GtkSourceCompletionTriggerMembersPrivate GtkSourceCompletionTriggerMembersPrivate;

struct _GtkSourceCompletionTriggerMembers {
	GObject parent;
	GtkSourceCompletionTriggerMembersPrivate *priv;	
};

struct _GtkSourceCompletionTriggerMembersClass {
	GObjectClass parent;
};

GtkSourceCompletionTriggerMembers* 
gtk_source_completion_trigger_members_new(GtkSourceCompletionCompletion *completion);

GType gtk_source_completion_trigger_members_get_type ();

G_END_DECLS

#endif
