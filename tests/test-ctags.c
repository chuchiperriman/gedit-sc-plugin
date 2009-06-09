/*
 * main.c
 * Copyright (C) perriman 2007 <chuchiperriman@gmail.com>
 * 
 * main.c is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * main.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with main.c.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include "../src/sc-symbol.h"
#include "../src/sc-ctags.h"

int
main (int argc, char *argv[])
{
	GList	*symbols;
	GList	*l;
	ScSymbol	*s;
	
	gtk_init_check(&argc, &argv);
	
	g_debug ("============= file ============");
	symbols = sc_ctags_exec_get_symbols (CTAGS_EXEC_FILE, "../src/sc-plugin.c");
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = (ScSymbol*)l->data;
		g_debug ("name: %s\n sign: %s", s->name, s->signature);
		g_object_unref (s);
	}
	g_list_free (symbols);
	
	g_debug ("============= folder ============");
	symbols = sc_ctags_exec_get_symbols (CTAGS_EXEC_PROJECT, "../src");
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = (ScSymbol*)l->data;
		g_debug ("name: %s\n sign: %s", s->name, s->signature);
		g_object_unref (s);
	}
	g_list_free (symbols);
	return 0;
}
