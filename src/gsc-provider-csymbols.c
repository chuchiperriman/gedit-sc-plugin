/* 
 *  gsc-provider-csymbols.c - Type here a short description of your plugin
 *
 *  Copyright (C) 2009 - perriman
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gsc-provider-csymbols.h"

#define CTAGS_EXEC "sh -c \"ctags -n --fields=-k-f-s-t+K+l+n -f - %s/*.[ch]\""
#define INFO_TMPL "<b>File:</b> %s\n<b>Type:</b> %s\n<b>Line:</b> %d"

struct _GscProviderCsymbolsPrivate {
	GtkTextView	*view;
};

#define GSC_PROVIDER_CSYMBOLS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_GSC_PROVIDER_CSYMBOLS, GscProviderCsymbolsPrivate))

/* just used to keep the population code readable */
typedef struct 
{
        gchar           *name;
        gchar           *type;
        gchar           *file;
        gchar           *language;
        gint            line;
} Symbol;

static const gchar* 
gsc_provider_csymbols_real_get_name (GscProvider* self);
static GList* 
gsc_provider_csymbols_real_get_proposals (GscProvider* base,
						     GscTrigger *trigger);
static void 
gsc_provider_csymbols_real_finish (GscProvider* base);

static gpointer 
gsc_provider_csymbols_parent_class = NULL;
static GscProviderIface* 
gsc_provider_csymbols_parent_iface = NULL;

/*
 *      Free strings used by Symbol struct while parsing
 */ 
static void
symbol_free (Symbol *symbol)
{
        if (symbol->name != NULL) g_free (symbol->name);
        if (symbol->file != NULL) g_free (symbol->file);
        if (symbol->type != NULL) g_free (symbol->type);
        if (symbol->language != NULL) g_free (symbol->language);
        
        g_slice_free (Symbol, symbol);
}

/*
 *      This function needs work. Used to be custom parsed for ex searches
 *      which could potentially contain tabs, however, I've decided that isn't
 *      really needed. But, it's still using Symbol. We could just pass back
 *      **fields array.
 */ 
static Symbol*
parse_line (gchar *line)
{
	Symbol                  *symbol;
	gchar                   **splits;
	gchar                   **fields;

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

	g_strfreev (fields);

	return symbol;
}

/*
 *      execute the ctags command and return stdout
 */ 
static gchar *
exec_ctags (gchar *folder)
{
	gchar                   *output;       
	gchar                   *command;       
	GError                  *error = NULL;  

	/* build ctags command */

	g_return_val_if_fail (folder != NULL, NULL);

	command = g_strdup_printf (CTAGS_EXEC, 
		        folder);

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

static const gchar* 
gsc_provider_csymbols_real_get_name (GscProvider* self)
{
	return GSC_PROVIDER_CSYMBOLS_NAME;
}

static GList* 
gsc_provider_csymbols_real_get_proposals (GscProvider* base,
					  GscTrigger *trigger)
{

	gchar			*output =NULL;
	gchar			*info = NULL;
	gchar			*current_word;
	gchar			**lines = NULL;
	gint			i;
	GList			*list = NULL;
	Symbol			*symbol;
	GscProposal	 	*prop;
	GscProviderCsymbols	*self;
	
	/* execute command */

	output = exec_ctags ("/home/perriman/dev/git/gedit-sc-plugin/src");
	
	if (output != NULL)
	{
		self = GSC_PROVIDER_CSYMBOLS (base);
		current_word = gsc_get_last_word_and_iter(self->priv->view,
							  NULL,
							  NULL);
		lines = g_strsplit (output, "\n", 0);
		for (i=0; lines[i]; i++)
		{
			symbol = parse_line (lines[i]);
			if (symbol != NULL)
			{
				if (g_str_has_prefix (symbol->name, current_word))
				{
					info = g_strdup_printf (INFO_TMPL, 
								symbol->file,
								symbol->type,
								symbol->line);
			
					prop = gsc_proposal_new(symbol->name,
								info,
								NULL);
					g_free (info);
					gsc_proposal_set_page_name (prop, "Symbols");
					list = g_list_append (list, prop);
				}
				symbol_free (symbol);
			}
		}
		g_strfreev (lines);
		g_free (output);
		g_free (current_word);
		output = NULL;
	}
	else
	{
		g_debug ("no output");
	}
	
	return list;
}

static void 
gsc_provider_csymbols_real_finish (GscProvider* base)
{

}

static void 
gsc_provider_csymbols_finalize(GObject *object)
{
	GscProviderCsymbols *self;
	self = GSC_PROVIDER_CSYMBOLS(object);
	G_OBJECT_CLASS(gsc_provider_csymbols_parent_class)->finalize(object);
}


static void 
gsc_provider_csymbols_class_init (GscProviderCsymbolsClass *klass)
{
	gsc_provider_csymbols_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->finalize = gsc_provider_csymbols_finalize;
}

static void
gsc_provider_csymbols_interface_init (GscProviderIface *iface)
{
	gsc_provider_csymbols_parent_iface = g_type_interface_peek_parent (iface);
	
	iface->get_name = gsc_provider_csymbols_real_get_name;
	iface->get_proposals = gsc_provider_csymbols_real_get_proposals;
	iface->finish = gsc_provider_csymbols_real_finish;
}


static void 
gsc_provider_csymbols_init (GscProviderCsymbols * self)
{
	self->priv = g_new0(GscProviderCsymbolsPrivate, 1);
}

GType gsc_provider_csymbols_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GscProviderCsymbolsClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gsc_provider_csymbols_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GscProviderCsymbols), 0, (GInstanceInitFunc) gsc_provider_csymbols_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GscProviderCsymbols", &g_define_type_info, 0);
		static const GInterfaceInfo gsc_provider_info = { (GInterfaceInitFunc) gsc_provider_csymbols_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GSC_TYPE_PROVIDER, &gsc_provider_info);
	}
	return g_define_type_id;
}


GscProviderCsymbols*
gsc_provider_csymbols_new(GscManager *manager)
{
	GscProviderCsymbols *self = GSC_PROVIDER_CSYMBOLS (g_object_new (GSC_TYPE_PROVIDER_CSYMBOLS, NULL));
	self->priv->view = gsc_manager_get_view (manager);
	return self;
}

