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
#include "../src/sc-utils.h"

static const gchar* texts[]= {
	"    siiii if (a == b) {",
	"        siiii if (a == b) {    ",
	"siii if (a == b)\n{",
	"if (a == b) {   sisisi; ",
	"if (a == b) {   nooooo;} ",
	"if (a == b) {   siiii;}{ ",
	NULL
};

static gchar*
get_indent (const gchar *string)
{
	/* Print all uppercase-only words. */
	GRegex *regex;
	GMatchInfo *match_info;
	gchar *word = NULL;

	regex = g_regex_new ("^\\s*", 0, 0, NULL);
	g_regex_match (regex, string, 0, &match_info);
	while (g_match_info_matches (match_info))
	{
		word = g_match_info_fetch (match_info, 0);
		break;
	}
	g_match_info_free (match_info);
	g_regex_unref (regex);

	return word;
}

static void
regexp_test ()
{
	gint i;
	gchar *indent;
	const gchar *text = texts[0];
	for (i = 1; text != NULL; i++)
	{
		g_debug ("new line");
		if (g_regex_match_simple ("{\\.*[^}]*\\.*$",
					  text,
					  0,
					  0))
		{
			printf ("%s\n", text);
			indent = get_indent (text);
			if (indent)
			{
				printf ("%s    print();\n", indent);
				g_free (indent);
			}
		}
		else
		{
			printf ("%s (NOT)\n", text);
		}
		text = texts[i];
	}
}

int
main (int argc, char *argv[])
{
	GList	*symbols;
	GList	*l;
	ScSymbol	*s;
	gchar *sc_tags;
	
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

	g_debug ("============= generating SC_TAGS ==============");

	sc_tags = sc_ctags_build_project_sctags ("..", TRUE);
	if (sc_tags)
	{
		g_debug ("File SC_TAGS created: %s", sc_tags);
		g_free (sc_tags);
	}
	else
	{
		g_debug ("Error creating SC_TAGS");
	}
	
	gchar *pro = sc_utils_get_project_dir ("test-ctags.c");

	g_debug ("Project dir: %s", pro);

	g_free(pro);

	regexp_test ();
	
	return 0;
}
