/*
 * gtksourcecompletionproposal-symgoto.h - Type here a short description of your plugin
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

#ifndef __SC_PROPOSAL_SYMGOTO_H__
#define __SC_PROPOSAL_SYMGOTO_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>
#include <gtksourceview/gtksourcecompletionproposal.h>
#include "gtksourcecompletionprovider-csymbols.h"

G_BEGIN_DECLS

#define SC_TYPE_PROPOSAL_SYMGOTO (gtk_source_completion_proposal_symgoto_get_type ())
#define SC_PROPOSAL_SYMGOTO(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_PROPOSAL_SYMGOTO, GtkSourceCompletionProposalSymgoto))
#define SC_PROPOSAL_SYMGOTO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_PROPOSAL_SYMGOTO, GtkSourceCompletionProposalSymgotoClass))
#define SC_IS_PROPOSAL_SYMGOTO(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_PROPOSAL_SYMGOTO))
#define SC_IS_PROPOSAL_SYMGOTO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_PROPOSAL_SYMGOTO))
#define SC_PROPOSAL_SYMGOTO_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_PROPOSAL_SYMGOTO, GtkSourceCompletionProposalSymgotoClass))

typedef struct _GtkSourceCompletionProposalSymgoto GtkSourceCompletionProposalSymgoto;
typedef struct _GtkSourceCompletionProposalSymgotoClass GtkSourceCompletionProposalSymgotoClass;
typedef struct _GtkSourceCompletionProposalSymgotoPrivate GtkSourceCompletionProposalSymgotoPrivate;

struct _GtkSourceCompletionProposalSymgoto {
	GtkSourceCompletionProposal parent;
	GtkSourceCompletionProposalSymgotoPrivate *priv;	
};

struct _GtkSourceCompletionProposalSymgotoClass {
	GtkSourceCompletionProposalClass parent;
};

GType			 gtk_source_completion_proposal_symgoto_get_type	();

GtkSourceCompletionProposal		*gtk_source_completion_proposal_symgoto_new	(GeditWindow *window,
							 Symbol *symbol);

G_END_DECLS

#endif
