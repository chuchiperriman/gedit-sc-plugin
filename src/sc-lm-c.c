/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * sc-lm-c.c
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#include "sc-lm-c.h"
#include "sc-ctags.h"
#include "sc-utils.h"

#define SC_LM_C_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_LM_C, ScLmCPrivate))

#define LANGUAGES "text/x-c;text/x-csrc;text/x-c++hdr;text/x-chdr"

static void	 generate_tags_cb	(GtkAction * action, GeditWindow * window);

static const GtkActionEntry action_entries[] =
{
	{"SourceCodeAction", NULL, N_("Source Code")},
	{ "GenerateTagsAction",
	  NULL,
	  N_("Generate Project Tags"),
	  NULL,
	  N_("Generate the tags symbols file for the project of the current document"),
	  G_CALLBACK (generate_tags_cb) }
};

struct _ScLmCPrivate
{
	GList *symbols;
	GList *project_symbols;
	GSList *language_ids;
};

static void sc_lm_c_iface_init (gpointer g_iface, gpointer iface_data);

G_DEFINE_TYPE_WITH_CODE (ScLmC, 
			 sc_lm_c, 
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (SC_TYPE_LANGUAGE_MANAGER,
			 			sc_lm_c_iface_init))

static void
generate_tags_cb (GtkAction * action,
		  GeditWindow * window)
{
	g_debug ("Generate tags action");
}

static GSList*
build_language_ids ()
{
	gchar **language_ids = g_strsplit (LANGUAGES, ";", 0);
	GSList *list = NULL;
	guint i;

	for (i = 0; language_ids[i] != NULL; i++)
	{
		list = g_slist_append (list, language_ids[i]);
	}
	
	g_free (language_ids);
	
	return list;
}

static void 
clean_symbols (GList *symbols)
{
	if (!symbols)
		return
	
	g_list_foreach (symbols, (GFunc)g_object_unref, NULL);
	
	g_list_free (symbols);
}

static void
sc_lm_c_enable (ScLanguageManager *self)
{

}

static void
sc_lm_c_disable (ScLanguageManager *self)
{
	
}

static GSList*
sc_lm_c_get_language_ids_impl (ScLanguageManager *self)
{
	/*TODO*/
	return NULL;
}

static void
sc_lm_c_set_active_impl (ScLanguageManager	*self,
			 gboolean		 active)
{
	if (active)
		sc_lm_c_enable (self);
	else
		sc_lm_c_disable (self);
}

static void
sc_lm_c_activate_document_impl (ScLanguageManager	*lm,
				GeditDocument		*doc)
{
	ScLmC *self = SC_LM_C (lm);
	gchar *uri, *project_dir, *sctags;
	
	clean_symbols (self->priv->symbols);
	self->priv->symbols = NULL;
	clean_symbols (self->priv->project_symbols);
	self->priv->project_symbols = NULL;
	
	
	uri = gedit_document_get_uri_for_display (doc);
	if (g_file_test (uri, G_FILE_TEST_EXISTS))
	{
		/*Loading document symbols*/
		self->priv->symbols = sc_ctags_exec_get_symbols (CTAGS_EXEC_FILE, uri);

		/*Loading project symbols*/		
		project_dir = sc_utils_get_project_dir (uri);
		if (project_dir != NULL)
		{
			sctags = sc_ctags_build_project_sctags (project_dir, FALSE);
			if (sctags != NULL)
			{
				self->priv->project_symbols = sc_ctags_get_symbols_from_sctags (sctags);
				g_free (sctags);
			}
			g_free (project_dir);
		}
	}
}

static GList*
sc_lm_c_get_document_symbols_impl (ScLanguageManager	*lm)
{
	ScLmC *self = SC_LM_C (lm);
	return self->priv->symbols;
}

static GList*
sc_lm_c_get_project_symbols_impl (ScLanguageManager	*lm)
{
	ScLmC *self = SC_LM_C (lm);
	return self->priv->project_symbols;
}

static void
sc_lm_c_iface_init (gpointer g_iface, 
					   gpointer iface_data)
{
	ScLanguageManagerIface *iface = (ScLanguageManagerIface *)g_iface;
	
	/* Interface data getter implementations */
	iface->get_language_ids = sc_lm_c_get_language_ids_impl;
	iface->set_active = sc_lm_c_set_active_impl;
	iface->activate_document = sc_lm_c_activate_document_impl;
	iface->get_document_symbols = sc_lm_c_get_document_symbols_impl;
	iface->get_project_symbols = sc_lm_c_get_project_symbols_impl;
}

static void
sc_lm_c_finalize (GObject *object)
{
	ScLmC *self = SC_LM_C(object);
	
	clean_symbols (self->priv->symbols);
	clean_symbols (self->priv->project_symbols);
	
	g_slist_free (self->priv->language_ids);

	G_OBJECT_CLASS (sc_lm_c_parent_class)->finalize (object);
}

static void
sc_lm_c_class_init (ScLmCClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = sc_lm_c_finalize;

	g_type_class_add_private (object_class, sizeof(ScLmCPrivate));
}

static void
sc_lm_c_init (ScLmC *self)
{
	self->priv = SC_LM_C_GET_PRIVATE (self);
	self->priv->symbols = NULL;
	self->priv->project_symbols = NULL;
	self->priv->language_ids = build_language_ids ();
}

ScLmC *
sc_lm_c_new ()
{
	return g_object_new (SC_TYPE_LM_C, 
			     NULL);
}

