/* 
 *  sc-provider-csymbols.c - Type here a short description of your plugin
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

#include "sc-provider-csymbols.h"
#include <gtksourceview/gtksourcecompletion.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_PROVIDER_CSYMBOLS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_PROVIDER_CSYMBOLS, ScProviderCsymbolsPrivate))

static void	 sc_provider_csymbols_iface_init	(GtkSourceCompletionProviderIface *iface);

struct _ScProviderCsymbolsPrivate
{
	GdkPixbuf *icon;
	GdkPixbuf *proposal_icon;
};

G_DEFINE_TYPE_WITH_CODE (ScProviderCsymbols,
			 sc_provider_csymbols,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_SOURCE_COMPLETION_PROVIDER,
				 		sc_provider_csymbols_iface_init))

static const gchar * 
sc_provider_csymbols_get_name (GtkSourceCompletionProvider *self)
{
	return "Symbols";
}

static GdkPixbuf * 
sc_provider_csymbols_get_icon (GtkSourceCompletionProvider *self)
{
	return SC_PROVIDER_CSYMBOLS (self)->priv->icon;
}


static GList *
append_item (GList *list, const gchar *name, GdkPixbuf *icon, const gchar *info)
{
	GtkSourceCompletionItem *prop;
	
	prop = gtk_source_completion_item_new (name, name, icon, info);
	return g_list_append (list, prop);
}

static GList *
sc_provider_csymbols_get_proposals (GtkSourceCompletionProvider *base,
                                 GtkTextIter                 *iter)
{
	ScProviderCsymbols *provider = SC_PROVIDER_CSYMBOLS (base);
	GList *list = NULL;
	
	list = append_item (list, "aa", provider->priv->proposal_icon, "Info proposal 1.1");
	list = append_item (list, "ab", provider->priv->proposal_icon, "Info proposal 1.2");
	list = append_item (list, "bc", provider->priv->proposal_icon, "Info proposal 1.3");
	list = append_item (list, "bd", provider->priv->proposal_icon, "Info proposal 1.3");
	
	return list;
}

static gboolean
sc_provider_csymbols_filter_proposal (GtkSourceCompletionProvider *provider,
                                   GtkSourceCompletionProposal *proposal,
                                   GtkTextIter                 *iter,
                                   const gchar                 *criteria)
{
	const gchar *label;
	
	label = gtk_source_completion_proposal_get_label (proposal);
	return g_str_has_prefix (label, criteria);
}

static const gchar *
sc_provider_csymbols_get_capabilities (GtkSourceCompletionProvider *provider)
{
	return GTK_SOURCE_COMPLETION_CAPABILITY_INTERACTIVE ","
	       GTK_SOURCE_COMPLETION_CAPABILITY_AUTOMATIC;
}

static void 
sc_provider_csymbols_finalize (GObject *object)
{
	ScProviderCsymbols *provider = SC_PROVIDER_CSYMBOLS (object);
	
	if (provider->priv->icon != NULL)
	{
		g_object_unref (provider->priv->icon);
	}
	
	if (provider->priv->proposal_icon != NULL)
	{
		g_object_unref (provider->priv->proposal_icon);
	}

	G_OBJECT_CLASS (sc_provider_csymbols_parent_class)->finalize (object);
}

static void 
sc_provider_csymbols_class_init (ScProviderCsymbolsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = sc_provider_csymbols_finalize;
	
	g_type_class_add_private (object_class, sizeof(ScProviderCsymbolsPrivate));
}

static void
sc_provider_csymbols_iface_init (GtkSourceCompletionProviderIface *iface)
{
	iface->get_name = sc_provider_csymbols_get_name;
	iface->get_icon = sc_provider_csymbols_get_icon;

	iface->get_proposals = sc_provider_csymbols_get_proposals;
	iface->filter_proposal = sc_provider_csymbols_filter_proposal;
	iface->get_capabilities = sc_provider_csymbols_get_capabilities;
}

static void 
sc_provider_csymbols_init (ScProviderCsymbols * self)
{
	GtkIconTheme *theme;
	gint width;
	
	self->priv = SC_PROVIDER_CSYMBOLS_GET_PRIVATE (self);
	
	theme = gtk_icon_theme_get_default ();

	gtk_icon_size_lookup (GTK_ICON_SIZE_MENU, &width, NULL);
	self->priv->proposal_icon = gtk_icon_theme_load_icon (theme,
	                                                      GTK_STOCK_YES,
	                                                      width,
	                                                      GTK_ICON_LOOKUP_USE_BUILTIN,
	                                                      NULL);
}

ScProviderCsymbols *
sc_provider_csymbols_new ()
{
	ScProviderCsymbols *ret = g_object_new (SC_TYPE_PROVIDER_CSYMBOLS, NULL);
	return ret;
}
