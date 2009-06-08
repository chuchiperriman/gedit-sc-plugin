/*
 * sc-utils.h 
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

#ifndef __SC_UTILS_H__
#define __SC_UTILS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SC_APP_ICON_NAME "gedit-sc-plugin"

GdkPixbuf	*sc_utils_get_theme_pixbuf 	(gchar *name);
GdkPixbuf	*sc_utils_get_symbol_pixbuf	(gchar *type);

G_END_DECLS

#endif /* __SC_PLUGIN_H__ */