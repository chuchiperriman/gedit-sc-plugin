/*
 * sc-ctags.h
 *
 * Copyright (C) 2009 - chuchiperriman
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

#include <gtk/gtk.h>

#define CTAGS_EXEC_FILE "sh -c \"ctags -n --fields=-k-f-s-t+K+l+n+S -f - %s\""
#define CTAGS_EXEC_PROJECT "sh -c \"ctags -n --fields=-k-f-s-t+K+l+n+S -f - %s/*.[ch]\""
#define SYMBOL_TEMPLATE "<b>File:</b> %s\n<b>Type:</b> %s\n<b>Line:</b> %d"

typedef struct 
{
        gchar           *name;
        gchar           *type;
        gchar           *file;
        gchar           *language;
        gint            line;
        gchar		*signature;
} Symbol; 

void	 sc_ctags_symbol_free			(Symbol *symbol);

Symbol	*sc_ctags_symbol_new_from_line		(gchar *line);

gchar	*sc_ctags_exec				(const gchar *exec,
						 const gchar *filename);

GList	*sc_ctags_get_symbols_from_string	(const gchar *ctags_output);

GList	*sc_ctags_exec_get_symbols		(const gchar *exec,
						 const gchar *filename);



