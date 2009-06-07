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

GdkPixbuf*
sc_utils_symbol_pixbuf_new (gchar *type)
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


