/*
 * gtksourcecompletionproposal-csymbol.h - Type here a short description of your plugin
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

#ifndef __SC_PROPOSAL_CSYMBOL_H__
#define __SC_PROPOSAL_CSYMBOL_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>
#include <gtksourceview/gtksourcecompletionproposal.h>
#include "gtksourcecompletionprovider-csymbols.h"

G_BEGIN_DECLS

#define SC_TYPE_PROPOSAL_CSYMBOL (gtk_source_completion_proposal_csymbol_get_type ())
#define SC_PROPOSAL_CSYMBOL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_PROPOSAL_CSYMBOL, GtkSourceCompletionProposalCsymbol))
#define SC_PROPOSAL_CSYMBOL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_PROPOSAL_CSYMBOL, GtkSourceCompletionProposalCsymbolClass))
#define SC_IS_PROPOSAL_CSYMBOL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_PROPOSAL_CSYMBOL))
#define SC_IS_PROPOSAL_CSYMBOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_PROPOSAL_CSYMBOL))
#define SC_PROPOSAL_CSYMBOL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_PROPOSAL_CSYMBOL, GtkSourceCompletionProposalCsymbolClass))

typedef struct _GtkSourceCompletionProposalCsymbol GtkSourceCompletionProposalCsymbol;
typedef struct _GtkSourceCompletionProposalCsymbolClass GtkSourceCompletionProposalCsymbolClass;
typedef struct _GtkSourceCompletionProposalCsymbolPrivate GtkSourceCompletionProposalCsymbolPrivate;

struct _GtkSourceCompletionProposalCsymbol {
	GtkSourceCompletionProposal parent;
	GtkSourceCompletionProposalCsymbolPrivate *priv;	
};

struct _GtkSourceCompletionProposalCsymbolClass {
	GtkSourceCompletionProposalClass parent;
};

GType			 gtk_source_completion_proposal_csymbol_get_type	();

GtkSourceCompletionProposal		*gtk_source_completion_proposal_csymbol_new		(GeditWindow *window,
			  					 Symbol *symbol);

const gchar		*gtk_source_completion_proposal_csymbol_get_prototype 	(GtkSourceCompletionProposal *prop);
G_END_DECLS

#endif
