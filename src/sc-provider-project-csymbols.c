/* 
 *  sc-provider-project-csymbols.c - Type here a short description of your plugin
 *
 *  Copyright (C) 2008 - perriman
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "sc-provider-project-csymbols.h"
#include "sc-symbol.h"
#include "sc-utils.h"
#include "sc-ctags.h"
#include <glib/gi18n-lib.h>
#include <gtksourceview/gtksourcecompletion.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_PROVIDER_PROJECT_CSYMBOLS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, ScProviderProjectCsymbolsPrivate))

static void	 sc_provider_project_csymbols_iface_init	(GtkSourceCompletionProviderIface *iface);

struct _ScProviderProjectCsymbolsPrivate
{
	GdkPixbuf *provider_icon;
	GeditDocument *document;
};

G_DEFINE_TYPE_WITH_CODE (ScProviderProjectCsymbols,
			 sc_provider_project_csymbols,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_SOURCE_COMPLETION_PROVIDER,
				 		sc_provider_project_csymbols_iface_init))

static const gchar * 
sc_provider_project_csymbols_get_name (GtkSourceCompletionProvider *self)
{
	return _("Project Symbols");
}

static GdkPixbuf * 
sc_provider_project_csymbols_get_icon (GtkSourceCompletionProvider *self)
{
	return SC_PROVIDER_PROJECT_CSYMBOLS (self)->priv->provider_icon;
}

static GList *
sc_provider_project_csymbols_get_proposals (GtkSourceCompletionProvider *base,
                                 GtkTextIter                 *iter)
{
	ScProviderProjectCsymbols *self = SC_PROVIDER_PROJECT_CSYMBOLS (base);
	GList *symbols = NULL;
	gchar *sctags, *project_dir;
	
	gchar *uri = gedit_document_get_uri_for_display (self->priv->document);
	if (g_file_test (uri, G_FILE_TEST_EXISTS))
	{
		project_dir = sc_utils_get_project_dir (uri);
		if (project_dir != NULL)
		{
			sctags = sc_ctags_build_project_sctags (project_dir, FALSE);
			if (sctags != NULL)
			{
				symbols = sc_ctags_get_symbols_from_sctags (sctags);
				g_free (sctags);
			}
			g_free (project_dir);
		}
	}
	
	if (symbols)
	{
		symbols = sc_utils_symbols_to_proposals_without_dup (symbols);
	}

	g_free (uri);
	
	return symbols;
}

static gboolean
sc_provider_project_csymbols_filter_proposal (GtkSourceCompletionProvider *provider,
                                   GtkSourceCompletionProposal *proposal,
                                   GtkTextIter                 *iter,
                                   const gchar                 *criteria)
{
	const gchar *label;
	
	label = gtk_source_completion_proposal_get_label (proposal);
	return g_str_has_prefix (label, criteria);
}

static const gchar *
sc_provider_project_csymbols_get_capabilities (GtkSourceCompletionProvider *provider)
{
	return GTK_SOURCE_COMPLETION_CAPABILITY_AUTOMATIC;
}

static void 
sc_provider_project_csymbols_finalize (GObject *object)
{
	ScProviderProjectCsymbols *provider = SC_PROVIDER_PROJECT_CSYMBOLS (object);
	
	if (provider->priv->provider_icon != NULL)
	{
		g_object_unref (provider->priv->provider_icon);
	}
	
	G_OBJECT_CLASS (sc_provider_project_csymbols_parent_class)->finalize (object);
}

static void 
sc_provider_project_csymbols_class_init (ScProviderProjectCsymbolsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = sc_provider_project_csymbols_finalize;
	
	g_type_class_add_private (object_class, sizeof(ScProviderProjectCsymbolsPrivate));
}

static void
sc_provider_project_csymbols_iface_init (GtkSourceCompletionProviderIface *iface)
{
	iface->get_name = sc_provider_project_csymbols_get_name;
	iface->get_icon = sc_provider_project_csymbols_get_icon;

	iface->get_proposals = sc_provider_project_csymbols_get_proposals;
	iface->filter_proposal = sc_provider_project_csymbols_filter_proposal;
	iface->get_capabilities = sc_provider_project_csymbols_get_capabilities;
}

static void 
sc_provider_project_csymbols_init (ScProviderProjectCsymbols * self)
{
	self->priv = SC_PROVIDER_PROJECT_CSYMBOLS_GET_PRIVATE (self);
	self->priv->provider_icon = sc_utils_get_theme_pixbuf (SC_APP_ICON_NAME);
}

ScProviderProjectCsymbols *
sc_provider_project_csymbols_new (GeditDocument *document)
{
	ScProviderProjectCsymbols *self = g_object_new (SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, NULL);
	self->priv->document = document;
	return self;
}
