/*
 * gsc-proposal-csymbol.h - Type here a short description of your plugin
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

#ifndef __GSC_PROPOSAL_CSYMBOL_H__
#define __GSC_PROPOSAL_CSYMBOL_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>
#include <gtksourcecompletion/gsc-proposal.h>
#include "gsc-provider-csymbols.h"

G_BEGIN_DECLS

#define GSC_TYPE_PROPOSAL_CSYMBOL (gsc_proposal_csymbol_get_type ())
#define GSC_PROPOSAL_CSYMBOL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSC_TYPE_PROPOSAL_CSYMBOL, GscProposalCsymbol))
#define GSC_PROPOSAL_CSYMBOL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GSC_TYPE_PROPOSAL_CSYMBOL, GscProposalCsymbolClass))
#define GSC_IS_PROPOSAL_CSYMBOL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSC_TYPE_PROPOSAL_CSYMBOL))
#define GSC_IS_PROPOSAL_CSYMBOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSC_TYPE_PROPOSAL_CSYMBOL))
#define GSC_PROPOSAL_CSYMBOL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GSC_TYPE_PROPOSAL_CSYMBOL, GscProposalCsymbolClass))

typedef struct _GscProposalCsymbol GscProposalCsymbol;
typedef struct _GscProposalCsymbolClass GscProposalCsymbolClass;
typedef struct _GscProposalCsymbolPrivate GscProposalCsymbolPrivate;

struct _GscProposalCsymbol {
	GscProposal parent;
	GscProposalCsymbolPrivate *priv;	
};

struct _GscProposalCsymbolClass {
	GscProposalClass parent;
};

GType			 gsc_proposal_csymbol_get_type	();

GscProposal		*gsc_proposal_csymbol_new		(GeditWindow *window,
			  					 Symbol *symbol);

G_END_DECLS

#endif
