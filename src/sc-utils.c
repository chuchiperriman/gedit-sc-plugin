/*
 * sc-utils.c
 *
 * Copyright (C) 2007 - chuchiperriman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "sc-utils.h"
#include <glib/gi18n-lib.h>
#include <gtksourceview/gtksourcecompletionitem.h>

#define SC_STOCK_ICONS "sc-stock-icons"

static gboolean icons_init = FALSE;

/*
static const gchar* ICONS[] = {
	"symbol-class.png",
	"symbol-constant.png",
	"symbol-define.png",
	"symbol-enumerator.png",
	"symbol-function.png",
	"symbol-macro.png",
	"symbol-member.png",
	"symbol-method.png",
	"symbol-struct.png",
	"symbol-typedef.png",
	"symbol-variable.png",
	NULL
};
*/

static void
sc_utils_icons_init ()
{
	GtkIconTheme *theme = gtk_icon_theme_get_default ();
	gtk_icon_theme_append_search_path (theme, ICON_DIR);
	icons_init = TRUE;
}

GdkPixbuf*
sc_utils_get_theme_pixbuf (gchar *name)
{
	GtkIconTheme 		*icon_theme;
	GdkPixbuf               *pixbuf=NULL;
	GError                  *error = NULL;

	if (!icons_init)
	{
		sc_utils_icons_init ();
	}
	icon_theme = gtk_icon_theme_get_default ();
	pixbuf = gtk_icon_theme_load_icon (icon_theme,
					   name, /* icon name */
					   24, /* size */
					   0,  /* flags */
					   &error);
	
	if (!pixbuf)
	{
		g_warning (_("Couldn't load icon: %s"), error->message);
		g_error_free (error);
	}
        return pixbuf;
}

GdkPixbuf*
sc_utils_get_symbol_pixbuf (gchar *type)
{
	GdkPixbuf		*pixbuf;
	gchar                   *name;

	name = g_strdup_printf ("symbol-%s", type);
	pixbuf = sc_utils_get_theme_pixbuf (name);
	g_free (name);
	return pixbuf;
}

static GFile*
sc_utils_search_configure (GFile *parent)
{
	GFile *reparent;
	GFile *child;
	gboolean exists;
	
	if (parent == NULL)
		return NULL;

	/* Search for the configure or configure.ac or configure.in */
	child = g_file_get_child (parent, "configure.ac");
	exists = g_file_query_exists (child, NULL);
	if (!exists)
	{
		g_object_unref (child);
		child = g_file_get_child (parent, "configure.in");
		exists = g_file_query_exists (child, NULL);
	}
	if (!exists)
	{
		g_object_unref (child);
		child = g_file_get_child (parent, "configure");
		exists = g_file_query_exists (child, NULL);
	}
	if (!exists)
	{
		g_object_unref (child);
		reparent = g_file_get_parent (parent);
		child = sc_utils_search_configure (reparent);
		g_object_unref (reparent);
		return child;
	}
	else
	{
		g_object_ref (parent);
		return parent;
	}
}

gchar*
sc_utils_get_project_dir (const gchar *path)
{
	GFile *file = g_file_new_for_path (path);
	GFile *parent = g_file_get_parent (file);
	GFile *child;
	gchar *res = NULL;

	if (parent != NULL)
	{
		child = sc_utils_search_configure (parent);
		g_object_unref (parent);
		if (child != NULL)
		{
			res = g_file_get_path (child);
			g_object_unref (child);
		}
	}

	g_object_unref (file);
	return res;
}

GtkSourceCompletionItem*
sc_utils_symbol_to_proposal (ScSymbol *s)
{
	GdkPixbuf *icon;
	gchar *info = NULL;
	GtkSourceCompletionItem *prop;

	g_return_val_if_fail (s != NULL, NULL);
	
	if (s->signature)
	{
		info = g_strdup_printf ("<b>%s</b> %s", s->name, s->signature);
	}
	icon = sc_utils_get_symbol_pixbuf (s->type);
	prop = gtk_source_completion_item_new (s->name, s->name, icon, info);
	g_free (info);
	g_object_unref (icon);

	return prop;
}

/**
   Free all the symbols and the symbols list.
   Return a list of GtkSourceCompletionItem
 */
GList*
sc_utils_symbols_to_proposals (GList *symbols)
{
	GList *list = NULL, *l;
	GtkSourceCompletionItem *prop;
	ScSymbol *s;

	if (symbols == NULL)
		return NULL;
	
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = SC_SYMBOL (l->data);
		prop = sc_utils_symbol_to_proposal (s);
		list = g_list_append (list, prop);
		g_object_unref (s);
	}
	g_list_free (symbols);
	return list;
}

/**
   Free all the symbols and the symbols list.
   Inserts only the first ocurrence of a symbol name
   Return a list of GtkSourceCompletionItem
 */
GList*
sc_utils_symbols_to_proposals_without_dup (GList *symbols)
{
	GHashTable *table;
	GList *list = NULL, *l;
	GtkSourceCompletionItem *prop;
	ScSymbol *s;
	gpointer *temp;

	if (symbols == NULL)
		return NULL;

	table = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, g_object_unref);
	
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = SC_SYMBOL (l->data);
		temp = g_hash_table_lookup (table, s->name);
		if (temp == NULL)
		{
			prop = sc_utils_symbol_to_proposal (s);
			list = g_list_append (list, prop);
			g_hash_table_insert (table, s->name, s);
			/*This symbol will be freed with the table*/
		}
		else
		{
			g_object_unref (s);
		}
	}
	g_hash_table_unref (table);
	g_list_free (symbols);
	return list;
}
