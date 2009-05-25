/*
 * sc-ctags.c
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

#include "sc-ctags.h"

/*
 *      This function needs work. Used to be custom parsed for ex searches
 *      which could potentially contain tabs, however, I've decided that isn't
 *      really needed. But, it's still using Symbol. We could just pass back
 *      **fields array.
 */ 
ScSymbol*
sc_ctags_symbol_new_from_line (gchar *line)
{
	ScSymbol	*symbol;
	gchar		**splits_line, **splits_lang, **splits_sign = NULL;
	gchar		**fields;
	gchar		*sign = NULL;

	/* name, file, ex search, type, line:n  language:x */

	fields = g_strsplit (line, "\t", 0);
	if (!fields[0]) return NULL;

	splits_line = g_strsplit (fields[4], ":", 0);
	splits_lang = g_strsplit (fields[5], ":", 0);
	
	if (fields[6])
	{
		splits_sign = g_strsplit (fields[6], ":", 0);
		sign = g_strdup (splits_sign[1]);
		g_strfreev (splits_sign);
	}
	
	symbol = sc_symbol_new (fields[0],
				fields[3],
				g_path_get_basename (fields[1]),
				splits_lang[1], 
				g_ascii_strtod (splits_line[1], NULL),
				sign);
	
	g_strfreev (splits_line);
	g_strfreev (splits_lang);
	if (sign) g_free (sign);

	g_strfreev (fields);

	return symbol;
}

/*
*      execute the ctags command and return stdout
*/ 
gchar *
sc_ctags_exec (const gchar *exec, const gchar *filename)
{
	gchar	*output;       
	gchar	*command;       
	GError	*error = NULL;  

	/* build ctags command */

	g_return_val_if_fail (filename != NULL, NULL);

	command = g_strdup_printf (exec, 
		        filename);

	/* execute command */
	g_spawn_command_line_sync (command, &output, NULL, NULL, &error);
	if (error)
	{
		g_critical ("Could not execute ctags: %s\n", error->message);
		g_error_free (error);
	}

	g_free (command);
	return output;
}

GList* 
sc_ctags_get_symbols_from_string	(const gchar *ctags_output)
{
	gchar		**lines = NULL;
	gint		i;
	GList		*list = NULL;
	ScSymbol	*symbol;
	
	
	if (!ctags_output)
		return NULL;
		
	lines = g_strsplit (ctags_output, "\n", 0);
	for (i=0; lines[i]; i++)
	{
		symbol = sc_ctags_symbol_new_from_line (lines[i]);
		if (symbol)
		{
			list = g_list_append (list, symbol);
		}
	}
	g_strfreev (lines);
	
	return list;
}

GList* 
sc_ctags_exec_get_symbols	(const gchar *exec, 
				 const gchar *filename)
{
	gchar	*output =NULL;
	GList	*list = NULL;
	
	output = sc_ctags_exec (exec, filename);
	
	if (!output)
		return NULL;
	
	list = sc_ctags_get_symbols_from_string (output);
	
	g_free (output);
	
	return list;
}


