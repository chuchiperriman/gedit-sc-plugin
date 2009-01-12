 /* gsc-proposal-symgoto.c - Type here a short description of your plugin
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

#include "gsc-proposal-symgoto.h"

struct _GscProposalSymgotoPrivate
{
	GeditWindow	*gedit_win;
	gchar 		*file;
	gint		line;
};

G_DEFINE_TYPE(GscProposalSymgoto, gsc_proposal_symgoto, GSC_TYPE_PROPOSAL);

#define GSC_PROPOSAL_SYMGOTO_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GSC_TYPE_PROPOSAL_SYMGOTO, GscProposalSymgotoPrivate))

/*FIXME Poner global porque se usa también en proposal-symgoto*/
#define INFO_TMPL "<b>File:</b> %s\n<b>Type:</b> %s\n<b>Line:</b> %d"
 
static gboolean
gsc_proposal_symgoto_apply(GscProposal* proposal, GtkTextView *view)
{
	gchar *uri;
	gchar *dirname;
	GscProposalSymgoto *self = GSC_PROPOSAL_SYMGOTO (proposal);
	GeditDocument *doc;
	
	doc = gedit_window_get_active_document(self->priv->gedit_win);

	if (doc == NULL) 
		return FALSE;
	
	uri = gedit_document_get_uri_for_display (doc);
	if (uri == NULL)
		return FALSE;
	
	dirname = g_path_get_dirname (uri);
	g_free (uri);
	uri = g_strdup_printf ("file://%s/%s", dirname, self->priv->file);
	g_free (dirname);
	
	gedit_commands_load_uri(self->priv->gedit_win,
				uri,
				NULL,
				self->priv->line);
	g_free (uri);
	return TRUE;
}

static void
gsc_proposal_symgoto_finalize (GObject *object)
{
	GscProposalSymgoto *self = GSC_PROPOSAL_SYMGOTO (object);
	g_free (self->priv->file);
	G_OBJECT_CLASS (gsc_proposal_symgoto_parent_class)->finalize (object);
}

static void
gsc_proposal_symgoto_class_init (GscProposalSymgotoClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (object_class, sizeof(GscProposalSymgotoPrivate));
	
	GscProposalClass *proposal_class = GSC_PROPOSAL_CLASS (klass);
	proposal_class->apply = gsc_proposal_symgoto_apply;
}

static void
gsc_proposal_symgoto_init (GscProposalSymgoto *self)
{
	self->priv = GSC_PROPOSAL_SYMGOTO_GET_PRIVATE (self);
	self->priv->gedit_win = NULL;
	self->priv->file = NULL;
}

GscProposal*
gsc_proposal_symgoto_new (GeditWindow *window,
			  Symbol *symbol)
{
	gchar		*info;
	
	g_return_val_if_fail (symbol != NULL, NULL);
	
	info = g_strdup_printf (INFO_TMPL, 
				symbol->file,
				symbol->type,
				symbol->line);
							
	GscProposalSymgoto *self = GSC_PROPOSAL_SYMGOTO (g_object_new (GSC_TYPE_PROPOSAL_SYMGOTO, 
								 "label", symbol->name,
								 "info", info,
								 "icon", NULL,
								 NULL));
	g_free (info);
	self->priv->gedit_win = window;
	self->priv->file = g_strdup (symbol->file);
	self->priv->line = symbol->line;
	return GSC_PROPOSAL (self);
}


