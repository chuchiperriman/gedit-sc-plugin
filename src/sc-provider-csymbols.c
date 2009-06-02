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
#include "sc-symbol.h"
#include "sc-ctags.h"
#include <gtksourceview/gtksourcecompletion.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_PROVIDER_CSYMBOLS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_PROVIDER_CSYMBOLS, ScProviderCsymbolsPrivate))

static void	 sc_provider_csymbols_iface_init	(GtkSourceCompletionProviderIface *iface);

struct _ScProviderCsymbolsPrivate
{
	GdkPixbuf *provider_icon;
	GeditDocument *document;
};

G_DEFINE_TYPE_WITH_CODE (ScProviderCsymbols,
			 sc_provider_csymbols,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_SOURCE_COMPLETION_PROVIDER,
				 		sc_provider_csymbols_iface_init))

static const gchar * 
sc_provider_csymbols_get_name (GtkSourceCompletionProvider *self)
{
	return "File Symbols";
}

static GdkPixbuf * 
sc_provider_csymbols_get_icon (GtkSourceCompletionProvider *self)
{
	return SC_PROVIDER_CSYMBOLS (self)->priv->provider_icon;
}

/* TODO Do better because we load a new pixbuf in all the proposals
 *      Return the PixBuf that represents the specified type of symbol, or NULL
 */         
static GdkPixbuf *
get_symbol_pixbuf (gchar *type)
{
        GdkPixbuf               *pixbuf=NULL;
        gchar                   *path;
        GError                  *error = NULL;
        
        path = g_strdup_printf (ICON_DIR"/symbol-%s.png", type);
        
        if (g_file_test (path, G_FILE_TEST_EXISTS))
        {
                pixbuf = gdk_pixbuf_new_from_file(path, &error);
        }
        
        if (error)
        {
                g_warning ("Could not load pixbuf: %s\n", error->message);
                g_error_free(error);
        }
        
        g_free (path);
        
        
        return pixbuf;
}

static GList *
sc_provider_csymbols_get_proposals (GtkSourceCompletionProvider *base,
                                 GtkTextIter                 *iter)
{
	ScProviderCsymbols *self = SC_PROVIDER_CSYMBOLS (base);
	GList *list = NULL, *symbols = NULL, *l;
	GtkSourceCompletionItem *prop;
	ScSymbol *s;
	GdkPixbuf *icon;
	gchar *info;
	
	gchar *uri = gedit_document_get_uri_for_display (self->priv->document);
	if (g_file_test (uri, G_FILE_TEST_EXISTS))
	{
		symbols = sc_ctags_exec_get_symbols (CTAGS_EXEC_FILE, uri);
	}
	
	if (symbols)
	{
		for (l = symbols; l != NULL; l = g_list_next (l))
		{
			s = SC_SYMBOL (l->data);
			info = NULL;
			if (s->signature)
			{
				info = g_strdup_printf ("<b>%s</b> %s", s->name, s->signature);
			}
			icon = get_symbol_pixbuf (s->type);
			prop = gtk_source_completion_item_new (s->name, s->name, icon, info);
			list = g_list_append (list, prop);
			g_free (info);
			g_object_unref (icon);
			g_object_unref (s);
		}
		g_list_free (symbols);
	}
	
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
	
	if (provider->priv->provider_icon != NULL)
	{
		g_object_unref (provider->priv->provider_icon);
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
}

ScProviderCsymbols *
sc_provider_csymbols_new (GeditDocument *document)
{
	ScProviderCsymbols *self = g_object_new (SC_TYPE_PROVIDER_CSYMBOLS, NULL);
	self->priv->document = document;
	return self;
}
