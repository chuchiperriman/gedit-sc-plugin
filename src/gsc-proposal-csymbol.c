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
#include <gtksourcecompletion/gsc-utils.h>
#include "gsc-proposal-csymbol.h"

struct _GscProposalCsymbolPrivate
{
	gchar *prototype;
	gchar *dummy;
};

G_DEFINE_TYPE(GscProposalCsymbol, gsc_proposal_csymbol, GSC_TYPE_PROPOSAL);

#define GSC_PROPOSAL_CSYMBOL_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GSC_TYPE_PROPOSAL_CSYMBOL, GscProposalCsymbolPrivate))

/*FIXME Poner global porque se usa también en proposal-symgoto*/
#define INFO_TMPL "<b>File:</b> %s\n<b>Type:</b> %s\n<b>Line:</b> %d\n<b>Signature:</b> %s"
 
static gboolean
gsc_proposal_csymbol_apply(GscProposal* proposal, GtkTextView *view)
{
	gchar *temp = g_strdup_printf ("%s (",
				       gsc_proposal_get_label (proposal));
	gsc_replace_actual_word (view,
				temp);
	g_free (temp);
	/*TODO Poner el cursor en el primer parámetro*/
	return FALSE;
}

static void gsc_proposal_csymbol_finalize(GObject *object)
{
	GscProposalCsymbol *self = GSC_PROPOSAL_CSYMBOL (object);
	g_free (self->priv->prototype);
	G_OBJECT_CLASS(gsc_proposal_csymbol_parent_class)->finalize(object);
}

static void
gsc_proposal_csymbol_class_init (GscProposalCsymbolClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (object_class, sizeof(GscProposalCsymbolPrivate));
	
	GscProposalClass *proposal_class = GSC_PROPOSAL_CLASS (klass);
	proposal_class->apply = gsc_proposal_csymbol_apply;
	object_class->finalize = gsc_proposal_csymbol_finalize;
}

static void
gsc_proposal_csymbol_init (GscProposalCsymbol *self)
{
	self->priv = GSC_PROPOSAL_CSYMBOL_GET_PRIVATE (self);
}

const gchar*
gsc_proposal_csymbol_get_prototype (GscProposal *prop)
{
	g_return_val_if_fail (GSC_IS_PROPOSAL_CSYMBOL (prop), NULL);
	GscProposalCsymbol *self = GSC_PROPOSAL_CSYMBOL (prop);
	return self->priv->prototype;
}

GscProposal*
gsc_proposal_csymbol_new (GeditWindow *window,
			  Symbol *symbol)
{
	gchar		*info;
	
	g_return_val_if_fail (symbol != NULL, NULL);
	
	info = g_strdup_printf (INFO_TMPL, 
				symbol->file,
				symbol->type,
				symbol->line,
				symbol->signature);
	
	GscProposalCsymbol *self = GSC_PROPOSAL_CSYMBOL (g_object_new (GSC_TYPE_PROPOSAL_CSYMBOL, 
								 "label", symbol->name,
								 "info", info,
								 "icon", NULL,
								 NULL));
	self->priv->prototype = g_strdup_printf ("%s %s", symbol->name, symbol->signature);
	return GSC_PROPOSAL (self);
}


