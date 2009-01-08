 /* gsc-proposal-csymbol.c - Type here a short description of your plugin
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

#include "gsc-proposal-csymbol.h"

struct _GscProposalCsymbolPrivate
{
	gchar *dummy;
};

G_DEFINE_TYPE(GscProposalCsymbol, gsc_proposal_csymbol, GSC_TYPE_PROPOSAL);

#define GSC_PROPOSAL_CSYMBOL_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GSC_TYPE_PROPOSAL_CSYMBOL, GscProposalCsymbolPrivate))
 
static gboolean
gsc_proposal_csymbol_apply(GscProposal* proposal, GtkTextView *view)
{
	/*Do the work when the user selects this proposal*/
	return TRUE;
}

static void
gsc_proposal_csymbol_finalize (GObject *object)
{
	G_OBJECT_CLASS (gsc_proposal_csymbol_parent_class)->finalize (object);
}

static void
gsc_proposal_csymbol_class_init (GscProposalCsymbolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (object_class, sizeof(GscProposalCsymbolPrivate));
	
	GscProposalClass *proposal_class = GSC_PROPOSAL_CLASS (klass);
	proposal_class->apply = gsc_proposal_csymbol_apply;
}

static void
gsc_proposal_csymbol_init (GscProposalCsymbol *self)
{
	self->priv = GSC_PROPOSAL_CSYMBOL_GET_PRIVATE (self);
}

GscProposal*
gsc_proposal_csymbol_new (const gchar *label,
				      const gchar *info,
				      GdkPixbuf *icon)
{
	GscProposalCsymbol *self = GSC_PROPOSAL_CSYMBOL (g_object_new (GSC_TYPE_PROPOSAL_CSYMBOL, 
								 "label", label,
								 "info", info,
								 "icon", icon,
								 NULL));
	return GSC_PROPOSAL (self);
}


