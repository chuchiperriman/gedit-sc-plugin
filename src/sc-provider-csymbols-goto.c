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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "sc-provider-csymbols-goto.h"
#include "sc-symbol.h"
#include "sc-utils.h"
#include "sc-ctags.h"
#include "chcompletionutils.h"
#include <glib/gi18n-lib.h>
#include <gtksourceview/gtksourcecompletion.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_PROVIDER_CSYMBOLS_GOTO_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_PROVIDER_CSYMBOLS_GOTO, ScProviderCsymbolsGotoPrivate))
#define SC_SYMBOL_KEY "sc-symbol"

static void	 sc_provider_csymbols_goto_iface_init	(GtkSourceCompletionProviderIface *iface);

struct _ScProviderCsymbolsGotoPrivate
{
	GdkPixbuf *provider_icon;
	GeditDocument *document;
	ScLanguageManager *lm;
};

G_DEFINE_TYPE_WITH_CODE (ScProviderCsymbolsGoto,
			 sc_provider_csymbols_goto,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_SOURCE_COMPLETION_PROVIDER,
				 		sc_provider_csymbols_goto_iface_init))

static const gchar * 
sc_provider_csymbols_goto_get_name (GtkSourceCompletionProvider *self)
{
	return _("File Symbols Goto");
}

static GdkPixbuf * 
sc_provider_csymbols_goto_get_icon (GtkSourceCompletionProvider *self)
{
	return SC_PROVIDER_CSYMBOLS_GOTO (self)->priv->provider_icon;
}

static void
sc_provider_csymbols_goto_populate (GtkSourceCompletionProvider	*base,
				    GtkSourceCompletionContext	*context)
{
	ScProviderCsymbolsGoto *self = SC_PROVIDER_CSYMBOLS_GOTO (base);
	GList *list = NULL, *symbols = NULL, *l;
	GtkSourceCompletionItem *prop;
	ScSymbol *s;
	GdkPixbuf *icon;
	gchar *info;
	gchar *word;
	
	symbols = sc_language_manager_get_document_symbols (self->priv->lm);
	
	if (!symbols)
	{
		gtk_source_completion_context_add_proposals (context,
							     base,
							     NULL,
							     TRUE);
		return;	
	}
	
	word = ch_completion_get_word (GTK_SOURCE_BUFFER (self->priv->document));
	
	if (!word)
	{
		gtk_source_completion_context_add_proposals (context,
							     base,
							     NULL,
							     TRUE);
		return;
	}
	
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = SC_SYMBOL (l->data);
		if (g_utf8_strlen (word, -1) > 2 && g_str_has_prefix (s->name, word))
		{
			info = g_strdup_printf (_("<b>Name:</b> %s\n<b>Type:</b> %s\n<b>Line:</b> %d\n<b>Signature:</b> %s"),
						s->name, 
						s->type, 
						s->line, 
						s->signature != NULL ? s->signature : "");
	
			icon = sc_utils_get_symbol_pixbuf (s->type);
			prop = gtk_source_completion_item_new (s->name, s->name, icon, info);
			g_object_set_data_full (G_OBJECT (prop), SC_SYMBOL_KEY, g_object_ref (s), g_object_unref);
			list = g_list_append (list, prop);
			g_free (info);
			g_object_unref (icon);
		}
	}

	g_free (word);
	
	gtk_source_completion_context_add_proposals (context,
						     base,
						     list,
						     TRUE);
	g_list_free (list);
}

static gboolean
sc_provider_csymbols_goto_match (GtkSourceCompletionProvider	*provider,
				 GtkSourceCompletionContext	*context)
{
	return TRUE;
}

static gboolean
sc_provider_csymbols_goto_get_interactive (GtkSourceCompletionProvider *provider)
{
	return FALSE;
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
	
	g_object_unref (provider->priv->lm);
	
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

	iface->populate = sc_provider_csymbols_goto_populate;
	iface->match = sc_provider_csymbols_goto_match;
	iface->get_interactive = sc_provider_csymbols_goto_get_interactive;
	iface->activate_proposal = sc_provider_csymbols_goto_activate_proposal;
}

static void 
sc_provider_csymbols_goto_init (ScProviderCsymbolsGoto * self)
{
	self->priv = SC_PROVIDER_CSYMBOLS_GOTO_GET_PRIVATE (self);
	self->priv->provider_icon = sc_utils_get_theme_pixbuf (SC_APP_ICON_NAME);
}

ScProviderCsymbolsGoto *
sc_provider_csymbols_goto_new (GeditDocument *document,
			       ScLanguageManager *lm)
{
	ScProviderCsymbolsGoto *self = g_object_new (SC_TYPE_PROVIDER_CSYMBOLS_GOTO, NULL);
	self->priv->document = document;
	self->priv->lm = g_object_ref (lm);
	return self;
}
