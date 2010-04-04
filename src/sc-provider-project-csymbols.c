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
#include "chcompletionutils.h"
#include <glib/gi18n-lib.h>
#include <gtksourceview/gtksourcecompletion.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_PROVIDER_PROJECT_CSYMBOLS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, ScProviderProjectCsymbolsPrivate))

static void	 sc_provider_project_csymbols_iface_init	(GtkSourceCompletionProviderIface *iface);

struct _ScProviderProjectCsymbolsPrivate
{
	GdkPixbuf *provider_icon;
	GeditDocument *document;
	ScLanguageManager *lm;
	GList *populate_iter;
	GList *symbols;
	guint idle_id;
	guint cancel_id;
	GtkSourceCompletionContext *context;
	gchar *word;
};

G_DEFINE_TYPE_WITH_CODE (ScProviderProjectCsymbols,
			 sc_provider_project_csymbols,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_SOURCE_COMPLETION_PROVIDER,
				 		sc_provider_project_csymbols_iface_init))

static gchar * 
sc_provider_project_csymbols_get_name (GtkSourceCompletionProvider *self)
{
	return g_strdup (_("Project Symbols"));
}

static GdkPixbuf * 
sc_provider_project_csymbols_get_icon (GtkSourceCompletionProvider *self)
{
	return SC_PROVIDER_PROJECT_CSYMBOLS (self)->priv->provider_icon;
}

static gboolean
sc_provider_project_csymbols_match (GtkSourceCompletionProvider	*provider,
				    GtkSourceCompletionContext	*context)
{
	return TRUE;
}

static void
population_finished (ScProviderProjectCsymbols *self)
{
	if (self->priv->idle_id != 0)
	{
		g_source_remove (self->priv->idle_id);
		self->priv->idle_id = 0;
	}
	
	if (self->priv->context != NULL)
	{
		if (self->priv->cancel_id)
		{
			g_signal_handler_disconnect (self->priv->context, 
			                             self->priv->cancel_id);
			self->priv->cancel_id = 0;
		}
	
		g_object_unref (self->priv->context);
		self->priv->context = NULL;
	}
	
	self->priv->symbols = NULL;
	if (self->priv->word)
	{
		g_free (self->priv->word);
		self->priv->word = NULL;
	}
}

static gboolean
add_in_idle (ScProviderProjectCsymbols *self)
{
	guint idx = 0;
	GList *ret = NULL;
	gboolean finished;
	
	if (self->priv->populate_iter == NULL)
		self->priv->populate_iter = self->priv->symbols;
		
	while (idx < 100 && 
	       self->priv->populate_iter)
	{
		ScSymbol *s = (ScSymbol*)self->priv->populate_iter->data;
		
		if (self->priv->word == NULL ||
		    (g_str_has_prefix (s->name, self->priv->word) && g_utf8_collate (s->name, self->priv->word) != 0))
		{
			GtkSourceCompletionProposal *proposal = sc_utils_symbol_to_proposal ((ScSymbol*)self->priv->populate_iter->data);
			ret = g_list_prepend (ret, proposal);
		}
		
		self->priv->populate_iter = g_list_next (self->priv->populate_iter);
		++idx;
	}
	
	ret = g_list_reverse (ret);
	finished = self->priv->populate_iter == NULL;
	
	gtk_source_completion_context_add_proposals (self->priv->context,
	                                             GTK_SOURCE_COMPLETION_PROVIDER (self),
	                                             ret,
	                                             finished);
	
	if (finished)
	{
		population_finished (self);
	}

	return !finished;
}

static void
sc_provider_project_csymbols_populate (GtkSourceCompletionProvider	*base,
				       GtkSourceCompletionContext	*context)
{
	ScProviderProjectCsymbols *self = SC_PROVIDER_PROJECT_CSYMBOLS (base);
	
	self->priv->word = ch_completion_get_word (GTK_SOURCE_BUFFER (self->priv->document));
	
	if (self->priv->word)
	{
		if ((gtk_source_completion_context_get_activation (context) & GTK_SOURCE_COMPLETION_ACTIVATION_USER_REQUESTED) == GTK_SOURCE_COMPLETION_ACTIVATION_USER_REQUESTED
		    || g_utf8_strlen (self->priv->word, -1) > 2)
		{
			self->priv->symbols = sc_language_manager_get_project_symbols (self->priv->lm);
		}
		else
		{
			g_free (self->priv->word);
			self->priv->word = NULL;
		}
	}
	
	self->priv->cancel_id = 
		g_signal_connect_swapped (context, 
			                  "cancelled", 
			                   G_CALLBACK (population_finished), 
			                   self);

	self->priv->context = g_object_ref (context);
	
	/* Do first right now */
	if (add_in_idle (self))
	{
		self->priv->idle_id = g_idle_add ((GSourceFunc)add_in_idle,
		                                  self);
	}
}

static void 
sc_provider_project_csymbols_finalize (GObject *object)
{
	ScProviderProjectCsymbols *provider = SC_PROVIDER_PROJECT_CSYMBOLS (object);
	
	if (provider->priv->provider_icon != NULL)
	{
		g_object_unref (provider->priv->provider_icon);
	}
	
	g_object_unref (provider->priv->lm);
	
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

	iface->populate = sc_provider_project_csymbols_populate;
	iface->match = sc_provider_project_csymbols_match;
}

static void 
sc_provider_project_csymbols_init (ScProviderProjectCsymbols * self)
{
	self->priv = SC_PROVIDER_PROJECT_CSYMBOLS_GET_PRIVATE (self);
	self->priv->provider_icon = sc_utils_get_theme_pixbuf (SC_APP_ICON_NAME);
}

ScProviderProjectCsymbols *
sc_provider_project_csymbols_new (GeditDocument *document,
				  ScLanguageManager *lm)
{
	ScProviderProjectCsymbols *self = g_object_new (SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, NULL);
	self->priv->document = document;
	self->priv->lm = g_object_ref (lm);
	return self;
}
