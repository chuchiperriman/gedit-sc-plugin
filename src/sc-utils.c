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
sc_utils_get_symbol_pixbuf (gchar *type)
{
	GtkIconTheme 		*icon_theme;
	GdkPixbuf               *pixbuf=NULL;
	gchar                   *name;
	GError                  *error = NULL;

	if (!icons_init)
	{
		sc_utils_icons_init ();
	}
	name = g_strdup_printf ("symbol-%s", type);
	icon_theme = gtk_icon_theme_get_default ();
	pixbuf = gtk_icon_theme_load_icon (icon_theme,
					   name, /* icon name */
					   24, /* size */
					   0,  /* flags */
					   &error);
	g_free (name);
	
	if (!pixbuf)
	{
		g_warning ("Couldn't load icon: %s", error->message);
		g_error_free (error);
	}
	
        return pixbuf;
}


