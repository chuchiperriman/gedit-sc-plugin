/* 
 *  sc-provider-csymbols-goto.c - Type here a short description of your plugin
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

#include "sc-provider-csymbols-goto.h"
#include "sc-symbol.h"
#include "sc-ctags.h"
#include <gtksourceview/gtksourcecompletion.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_PROVIDER_CSYMBOLS_GOTO_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_PROVIDER_CSYMBOLS_GOTO, ScProviderCsymbolsGotoPrivate))
#define SC_SYMBOL_KEY "sc-symbol"
#define INFO_TMPL "<b>Name:</b> %s\n<b>Type:</b> %s\n<b>Line:</b> %d\n<b>Signature:</b> %s"

static void	 sc_provider_csymbols_goto_iface_init	(GtkSourceCompletionProviderIface *iface);

struct _ScProviderCsymbolsGotoPrivate
{
	GdkPixbuf *provider_icon;
	GeditDocument *document;
};

G_DEFINE_TYPE_WITH_CODE (ScProviderCsymbolsGoto,
			 sc_provider_csymbols_goto,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_SOURCE_COMPLETION_PROVIDER,
				 		sc_provider_csymbols_goto_iface_init))

static const gchar * 
sc_provider_csymbols_goto_get_name (GtkSourceCompletionProvider *self)
{
	return "File Symbols Goto";
}

static GdkPixbuf * 
sc_provider_csymbols_goto_get_icon (GtkSourceCompletionProvider *self)
{
	return SC_PROVIDER_CSYMBOLS_GOTO (self)->priv->provider_icon;
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
sc_provider_csymbols_goto_get_proposals (GtkSourceCompletionProvider *base,
                                 GtkTextIter                 *iter)
{
	ScProviderCsymbolsGoto *self = SC_PROVIDER_CSYMBOLS_GOTO (base);
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
	
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = SC_SYMBOL (l->data);
		info = g_strdup_printf (INFO_TMPL, 
					s->name, 
					s->type, 
					s->line, 
					s->signature != NULL ? s->signature : "");
		
		icon = get_symbol_pixbuf (s->type);
		prop = gtk_source_completion_item_new (s->name, s->name, icon, info);
		g_object_set_data_full (G_OBJECT (prop), SC_SYMBOL_KEY, g_object_ref (s), g_object_unref);
		list = g_list_append (list, prop);
		g_free (info);
		g_object_unref (icon);
		/*TODO We must free the symbols?*/
	}
	
	return list;
}

static gboolean
sc_provider_csymbols_goto_filter_proposal (GtkSourceCompletionProvider *provider,
                                   GtkSourceCompletionProposal *proposal,
                                   GtkTextIter                 *iter,
                                   const gchar                 *criteria)
{
	const gchar *label;
	
	label = gtk_source_completion_proposal_get_label (proposal);
	return g_str_has_prefix (label, criteria);
}

static const gchar *
sc_provider_csymbols_goto_get_capabilities (GtkSourceCompletionProvider *provider)
{
	return GTK_SOURCE_COMPLETION_CAPABILITY_INTERACTIVE ","
	       GTK_SOURCE_COMPLETION_CAPABILITY_AUTOMATIC;
}

static gboolean
sc_provider_csymbols_goto_activate_proposal (GtkSourceCompletionProvider	*provider,
					     GtkSourceCompletionProposal	*proposal,
					     GtkTextIter			*iter)
{
	ScProviderCsymbolsGoto *self = SC_PROVIDER_CSYMBOLS_GOTO (provider);
	GeditTab *tab = gedit_tab_get_from_document (self->priv->document);
	GeditView *view = gedit_tab_get_view (tab);
	ScSymbol *s = SC_SYMBOL (g_object_get_data (G_OBJECT (proposal), SC_SYMBOL_KEY));
	gedit_document_goto_line (self->priv->document, s->line - 1);
        gedit_view_scroll_to_cursor (view);
	return TRUE;
}

static void 
sc_provider_csymbols_goto_finalize (GObject *object)
{
	ScProviderCsymbolsGoto *provider = SC_PROVIDER_CSYMBOLS_GOTO (object);
	
	if (provider->priv->provider_icon != NULL)
	{
		g_object_unref (provider->priv->provider_icon);
	}
	
	G_OBJECT_CLASS (sc_provider_csymbols_goto_parent_class)->finalize (object);
}

static void 
sc_provider_csymbols_goto_class_init (ScProviderCsymbolsGotoClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = sc_provider_csymbols_goto_finalize;
	
	g_type_class_add_private (object_class, sizeof(ScProviderCsymbolsGotoPrivate));
}

static void
sc_provider_csymbols_goto_iface_init (GtkSourceCompletionProviderIface *iface)
{
	iface->get_name = sc_provider_csymbols_goto_get_name;
	iface->get_icon = sc_provider_csymbols_goto_get_icon;

	iface->get_proposals = sc_provider_csymbols_goto_get_proposals;
	iface->filter_proposal = sc_provider_csymbols_goto_filter_proposal;
	iface->get_capabilities = sc_provider_csymbols_goto_get_capabilities;
	iface->activate_proposal = sc_provider_csymbols_goto_activate_proposal;
}

static void 
sc_provider_csymbols_goto_init (ScProviderCsymbolsGoto * self)
{
	GtkIconTheme *theme;
	gint width;
	
	self->priv = SC_PROVIDER_CSYMBOLS_GOTO_GET_PRIVATE (self);
	
	theme = gtk_icon_theme_get_default ();

	gtk_icon_size_lookup (GTK_ICON_SIZE_MENU, &width, NULL);
}

ScProviderCsymbolsGoto *
sc_provider_csymbols_goto_new (GeditDocument *document)
{
	ScProviderCsymbolsGoto *self = g_object_new (SC_TYPE_PROVIDER_CSYMBOLS_GOTO, NULL);
	self->priv->document = document;
	return self;
}
