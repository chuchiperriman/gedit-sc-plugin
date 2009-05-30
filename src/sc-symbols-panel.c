/*
 * sc-symbols-panel.c
 * 
 * Copyright (C) 2007 - Micah Carrick
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
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h> 
#endif 

#include <string.h>

#include "sc-symbols-panel.h"
#include "sc-symbol.h"

#include <gedit/gedit-utils.h>
#include <gedit/gedit-debug.h>
#include <gedit/gedit-plugin.h>
#include <gedit/gedit-statusbar.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#define SC_SYMBOLS_PANEL_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), \
        SC_TYPE_SYMBOLS_PANEL, \
        ScSymbolsPanelPrivate))
                                                       
/* enum for tree model columns, not all is currently used */
enum    
{
        SYM_ICON,
        SYM_NAME,
        SYM_FILE,
        SYM_LINE,
        SYM_LANGUAGE,
        SYM_DOCUMENT,
        N_COLUMNS
};

struct _ScSymbolsPanelPrivate
{
        GtkWidget       *tree_view;
        GtkWidget       *label;
        GeditWindow     *window;
        gint            status_context;
};

G_DEFINE_TYPE (ScSymbolsPanel, sc_symbols_panel, GTK_TYPE_VBOX)

enum
{
        PROP_0,
        PROP_WINDOW
};

static void
on_row_activated (              GtkTreeView *tree_view,
                                GtkTreePath *path,
                                GtkTreeViewColumn *column,
                                gpointer user_data);

static void
sc_symbols_panel_finalize (GObject *object)
{
        G_OBJECT_CLASS (sc_symbols_panel_parent_class)->finalize (object);
}

static void 
sc_symbols_panel_class_init (ScSymbolsPanelClass *klass)
{       
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = sc_symbols_panel_finalize;

        g_type_class_add_private (object_class, 
                        sizeof(ScSymbolsPanelPrivate));
}

/*
 *      Return the PixBuf that represents the specified type of symbol, or NULL
 */         
static GdkPixbuf *
get_symbol_pixbuf (gchar *type)
{
        GdkPixbuf               *pixbuf=NULL;
        gchar                   *path;
        gchar                   *filename;
        GError                  *error = NULL;
        
        filename = g_strdup_printf ("symbol-%s.png", type);
        
        path = NULL;
                        
        if (path)
        {
                pixbuf = gdk_pixbuf_new_from_file(path, &error);
                g_free (path);
        }
        
        if (error)
        {
                g_warning ("Could not load pixbuf: %s\n", error->message);
                g_error_free(error);
        }
        
        g_free (filename);
        
        
        return pixbuf;
}

/*
 *      Add a single symbol to the treeview. Parents rows are added as the type
 *      whenever they aren't already present.
 */ 
static void
add_symbol (ScSymbolsPanel *panel,  ScSymbol *symbol)
{
        GtkTreeIter             iter;
        GtkTreeIter             parent;
        GtkTreeStore            *store;
        GtkWidget               *tree_view;
        GdkPixbuf               *pixbuf;
        gint                    i;
        gboolean                valid = TRUE;
        gboolean                no_parent = TRUE;
        gchar                   *markup;
        GeditDocument		*document;
        
        tree_view = panel->priv->tree_view;
        document = gedit_window_get_active_document (panel->priv->window);
        
        /* we could do bold here */
        markup = g_strdup_printf ("<b>%s</b>", symbol->type);
        markup[3] = g_ascii_toupper (markup[3]);
        
        pixbuf = get_symbol_pixbuf (symbol->type);
        
        store = GTK_TREE_STORE (gtk_tree_view_get_model(GTK_TREE_VIEW (tree_view)));
        
        /* check for existing parent iter */
        
        for (i=0; (valid && no_parent); i++)
        {
                gchar *path_string = g_strdup_printf("%d", i);
                valid = gtk_tree_model_get_iter_from_string (GTK_TREE_MODEL (store), 
                                &parent, 
                                path_string); 
                if (valid)
                {
                        gchar *val;
                        gtk_tree_model_get (GTK_TREE_MODEL (store), 
                                        &parent, 
                                        SYM_NAME, 
                                        &val, 
                                        -1);
                        if (g_ascii_strcasecmp(val, markup) == 0) 
                        {
                                no_parent = FALSE;
                        }
                        g_free (val);
                }
                
                g_free (path_string);
        }
        
        /* create the parent if it wasn't already found */
        if (no_parent)
        {
                gtk_tree_store_append (store, &parent, NULL);
                gtk_tree_store_set (store, &parent,
                                SYM_ICON, pixbuf,
                                SYM_NAME, markup,
                                -1);                                     
        }
        
        /* add the new symbol */
        gtk_tree_store_append (store, &iter, &parent);
        gtk_tree_store_set (store, &iter,
                        SYM_ICON, pixbuf,
                        SYM_NAME, symbol->name,
                        SYM_FILE, symbol->file,
                        SYM_LINE, symbol->line,
                        SYM_LANGUAGE, symbol->language,
                        SYM_DOCUMENT, document,
                        -1);
                        
        if (pixbuf) g_object_unref (pixbuf);
        
        g_free (markup);
        
        // gtk_tree_view_expand_all (GTK_TREE_VIEW (tree_view));
}

static void
sc_symbols_panel_init (ScSymbolsPanel *panel)
{
        GtkWidget               *sw;
        GtkTreeStore            *store;
        GtkCellRenderer         *renderer;
        GtkTreeViewColumn       *col;

        panel->priv = SC_SYMBOLS_PANEL_GET_PRIVATE (panel);
        
        /* create some nice padding */
        
        gtk_box_set_spacing (GTK_BOX (panel), 2);
        gtk_container_set_border_width (GTK_CONTAINER (panel), 5);
        
        /* create and pack tree view */
                            
        sw = gtk_scrolled_window_new (NULL, NULL);

        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                        GTK_POLICY_AUTOMATIC,
                        GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                     GTK_SHADOW_IN);
        gtk_box_pack_start (GTK_BOX (panel), sw, TRUE, TRUE, 0);
        
        panel->priv->tree_view = gtk_tree_view_new ();
        
        gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (panel->priv->tree_view), 
                        FALSE);
        gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (panel->priv->tree_view), 
                        TRUE);
        gtk_tree_view_set_headers_clickable (GTK_TREE_VIEW (panel->priv->tree_view), 
                        TRUE);
                        
        /* TODO: Learn the new GtkTooltop API */
              // g_object_set (panel->priv->tree_view, "has-tooltip", TRUE, NULL);
        
        gtk_container_add (GTK_CONTAINER (sw), panel->priv->tree_view);
        
        /* create and pack label */
        
        panel->priv->label = gtk_label_new ("");
        gtk_misc_set_alignment (GTK_MISC (panel->priv->label), 0.0, 0.5);
              gtk_box_pack_start (GTK_BOX (panel),
                        panel->priv->label,
                        FALSE,
                        TRUE,
                        0);
                                
        /* create model */
        
        store = gtk_tree_store_new (N_COLUMNS, 
                        GDK_TYPE_PIXBUF, 
                        G_TYPE_STRING,
                        G_TYPE_STRING,
                        G_TYPE_INT,
                        G_TYPE_STRING,
                        G_TYPE_OBJECT); 
                        
        gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (store), 1,
                        GTK_SORT_ASCENDING);
                                                         
        /* main column */
        
        col = gtk_tree_view_column_new();
        gtk_tree_view_column_set_title(col, "Symbols");
    
        renderer = gtk_cell_renderer_pixbuf_new();
        gtk_tree_view_column_pack_start(col, renderer, FALSE);
        gtk_tree_view_column_set_attributes(col, 
                        renderer, 
                        "pixbuf", 
                        SYM_ICON,
                        NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_column_pack_start(col, renderer, TRUE);
        gtk_tree_view_column_set_attributes(col, 
                        renderer,
                        "markup", 
                        SYM_NAME,
                        NULL);
                        
        gtk_tree_view_append_column( GTK_TREE_VIEW (panel->priv->tree_view), col);
        
        /* additional columns */
        
        /*
        if (panel->priv->show_line_number_column)   
        {
                col = gtk_tree_view_column_new();
                gtk_tree_view_column_set_title(col, "Line");
                renderer = gtk_cell_renderer_text_new();
                gtk_tree_view_column_pack_start(col, renderer, TRUE);
                gtk_tree_view_column_set_attributes(col, 
                                renderer,
                                "text", 
                                SYM_LINE,
                                NULL);
                                
                gtk_tree_view_append_column( GTK_TREE_VIEW (panel->priv->tree_view), col);
        }
        */  
                                                  
        gtk_tree_view_set_model (GTK_TREE_VIEW (panel->priv->tree_view), 
                        GTK_TREE_MODEL (store));
        
        /* attach signal callbacks for tree view */
        
        g_signal_connect_after (panel->priv->tree_view,
                        "row-activated",
                        G_CALLBACK (on_row_activated),
                        panel);

        g_object_unref (store);     
}

GtkWidget* 
sc_symbols_panel_new (GeditWindow *window)
{
        ScSymbolsPanel *self;
        self = g_object_new (SC_TYPE_SYMBOLS_PANEL, NULL);
        self->priv->window = window;

        return GTK_WIDGET (self);
}

void
sc_symbols_panel_populate (GtkWidget *widget, GList *symbols)
{
	GtkTreeModel            *model;
	ScSymbolsPanel		*self;
	GList			*l;
	ScSymbol		*s;
	
	g_return_if_fail (SC_IS_SYMBOLS_PANEL (widget));
	
	self = SC_SYMBOLS_PANEL (widget);
	
        g_return_if_fail (GEDIT_IS_WINDOW (self->priv->window));
        g_return_if_fail (GTK_IS_TREE_VIEW (self->priv->tree_view));
        
        /* clear data */
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (self->priv->tree_view));
        gtk_tree_store_clear (GTK_TREE_STORE (model));
        gtk_label_set_text (GTK_LABEL (self->priv->label), "");
	
	if (symbols == NULL)
		return;
	
	for (l = symbols; l != NULL; l = g_list_next (l))
	{
		s = SC_SYMBOL (l->data);
		add_symbol (self, s);
		g_object_unref (s);
	}
	g_list_free (symbols);
	
	gtk_tree_view_expand_all (GTK_TREE_VIEW (self->priv->tree_view));
}

/*TODO Do it in the plugin
static void 
on_active_tab_state_changed (   GeditWindow *window, gpointer user_data)
{
        // file has been saved. re-populate symbols 
        
        if (user_data == NULL) return;
        
        ScSymbolsPanel *panel;
        
        panel = SC_SYMBOLS_PANEL (user_data);
        
        g_return_if_fail (SC_IS_SYMBOLS_PANEL (panel));
        g_return_if_fail (GEDIT_IS_WINDOW (panel->priv->window));
        g_return_if_fail (GTK_IS_TREE_VIEW (panel->priv->tree_view));
        
        populate_symbols (panel);
}
*/
static void
on_row_activated (              GtkTreeView *tree_view,
                                GtkTreePath *path,
                                GtkTreeViewColumn *column,
                                gpointer user_data)
{

        GtkTreeIter                     iter;
        GtkTreeModel                    *model;
        GeditDocument                   *document;
        GeditTab                        *tab;
        ScSymbolsPanel   *panel;
        GeditView                       *view;
        gchar                           *name;
        gint                            line;
        
        
        panel = SC_SYMBOLS_PANEL (user_data);
        model = gtk_tree_view_get_model (tree_view);
        gtk_tree_model_get_iter (model, &iter, path);
        
        g_return_if_fail (&iter != NULL);
        /* expand/collapse row if it has children (and return) */
        if (gtk_tree_model_iter_has_child (model, &iter))
        {
                        if (gtk_tree_view_row_expanded (tree_view, path))
                        {
                                gtk_tree_view_collapse_row (tree_view, path);
                        }
                        else
                        {
                                gtk_tree_view_expand_row (tree_view, path, FALSE);
                        }
                        return;
        }
                   
        /* otherwise, show the file name and line number and flash statusbar */
                                              
        gtk_tree_model_get (model, &iter, 
                        SYM_NAME, &name, 
                        SYM_LINE, &line,
                        SYM_DOCUMENT, &document, -1);

        if (GEDIT_IS_DOCUMENT (document)) 
        {
                tab = gedit_tab_get_from_document (document);
                gedit_window_set_active_tab (panel->priv->window, tab); 
                view = gedit_window_get_active_view (panel->priv->window);
                gedit_document_goto_line (document, line - 1);
                gedit_view_scroll_to_cursor (view);
        }
        
        if (name) g_free (name); 
}



