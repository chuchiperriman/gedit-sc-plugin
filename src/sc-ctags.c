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
 *      Free strings used by Symbol struct while parsing
 */ 
void
sc_ctags_symbol_free (Symbol *symbol)
{
        if (symbol->name != NULL) g_free (symbol->name);
        if (symbol->file != NULL) g_free (symbol->file);
        if (symbol->type != NULL) g_free (symbol->type);
        if (symbol->language != NULL) g_free (symbol->language);
        if (symbol->signature != NULL) g_free (symbol->signature);
        
        g_slice_free (Symbol, symbol);
}

/*
 *      This function needs work. Used to be custom parsed for ex searches
 *      which could potentially contain tabs, however, I've decided that isn't
 *      really needed. But, it's still using Symbol. We could just pass back
 *      **fields array.
 */ 
Symbol*
sc_ctags_symbol_new_from_line (gchar *line)
{
	Symbol	*symbol;
	gchar	**splits;
	gchar	**fields;

	/* name, file, ex search, type, line:n  language:x */

	symbol = g_slice_new0 (Symbol);

	fields = g_strsplit (line, "\t", 0);
	if (fields[0] == NULL) return NULL;

	symbol->name = g_strdup (fields[0]);

	symbol->file = g_path_get_basename (fields[1]);
	symbol->type = g_strdup (fields[3]);

	splits = g_strsplit (fields[4], ":", 0);
	symbol->line = g_ascii_strtod (splits[1], NULL);
	g_strfreev (splits);

	splits = g_strsplit (fields[5], ":", 0);
	symbol->language = g_strdup (splits[1]);
	g_strfreev (splits);
	
	if (fields[6] != NULL)
	{
		splits = g_strsplit (fields[6], ":", 0);
		symbol->signature = g_strdup (splits[1]);
		g_strfreev (splits);
	}

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
	gchar	**lines = NULL;
	gint	i;
	GList	*list = NULL;
	Symbol	*symbol;
	
	
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


