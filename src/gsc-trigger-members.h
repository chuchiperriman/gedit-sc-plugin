/*
 * gsc-trigger-members.h - Type here a short description of your trigger
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
#include <gtksourcecompletion/gsc-trigger.h>
#include <gtksourcecompletion/gsc-completion.h>

G_BEGIN_DECLS

#define GSC_TYPE_TRIGGER_MEMBERS (gsc_trigger_members_get_type ())
#define GSC_TRIGGER_MEMBERS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSC_TYPE_TRIGGER_MEMBERS, GscTriggerMembers))
#define GSC_TRIGGER_MEMBERS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GSC_TYPE_TRIGGER_MEMBERS, GscTriggerMembersClass))
#define GSC_IS_TRIGGER_MEMBERS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSC_TYPE_TRIGGER_MEMBERS))
#define GSC_IS_TRIGGER_MEMBERS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSC_TYPE_TRIGGER_MEMBERS))
#define GSC_TRIGGER_MEMBERS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GSC_TYPE_TRIGGER_MEMBERS, GscTriggerMembersClass))

#define GSC_TRIGGER_MEMBERS_NAME "GscTriggerMembers"

typedef struct _GscTriggerMembers GscTriggerMembers;
typedef struct _GscTriggerMembersClass GscTriggerMembersClass;
typedef struct _GscTriggerMembersPrivate GscTriggerMembersPrivate;

struct _GscTriggerMembers {
	GObject parent;
	GscTriggerMembersPrivate *priv;	
};

struct _GscTriggerMembersClass {
	GObjectClass parent;
};

GscTriggerMembers* 
gsc_trigger_members_new(GscCompletion *completion);

GType gsc_trigger_members_get_type ();

G_END_DECLS

#endif
