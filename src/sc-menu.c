/* 
 * sc-menu.c --- Source Code Menu Manager
 * 
 * Copyright  (C)  2009  Jesús Barbero Rodríguez <chuchiperriman@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "sc-menu.h"
#include <glib/gi18n-lib.h>

#define SC_MENU_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_MENU, ScMenuPrivate))
#define WINDOW_DATA_KEY "SourceCodeWindowData"

struct _ScMenuPrivate
{
	GeditWindow *window;
};

typedef struct
{
	ScMenu *menu;
        GtkActionGroup *action_group;
        guint           ui_id;
} WindowData;

static void sc_menu_dispose (GObject *object);
static void sc_menu_finalize (GObject *object);
static void generate_tags_cb (GtkAction * action, GeditWindow * window);

G_DEFINE_TYPE (ScMenu, sc_menu, G_TYPE_OBJECT);

static void
window_data_free (WindowData *data)
{
        g_return_if_fail (data != NULL);

        g_object_unref (data->action_group);
        g_free (data);
}

static void
sc_menu_class_init (ScMenuClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;

    gobject_class->dispose = sc_menu_dispose;
    gobject_class->finalize = sc_menu_finalize;

    g_type_class_add_private (gobject_class, sizeof(ScMenuPrivate));
}

static void
sc_menu_init (ScMenu *self)
{
	self->priv = SC_MENU_GET_PRIVATE (self);
}

static void
sc_menu_dispose (GObject *object)
{
    /* ScMenu *self = (ScMenu *)object; */

    G_OBJECT_CLASS (sc_menu_parent_class)->dispose (object);
}

static void
sc_menu_finalize (GObject *object)
{
    /* ScMenu *self = (ScMenu *)object; */

    g_signal_handlers_destroy (object);
    G_OBJECT_CLASS (sc_menu_parent_class)->finalize (object);
}

ScMenu*
sc_menu_new (GeditWindow *window)
{
	ScMenu *self = SC_MENU(g_object_new (SC_TYPE_MENU,NULL));
	self->priv->window = window;
	return self;
}

static const gchar *ui_str =
        "<ui>"
        "  <menubar name='MenuBar'>"
	"    <placeholder name='ExtraMenu_1'>"
	"      <menu name='SourceCodeMenu' action='SourceCodeAction'>"
	"        <menuitem name='GenerateTags' action='GenerateTagsAction'/>"
	"        <placeholder name='ScMainPlaceholder'/>"
	"      </menu>"
	"    </placeholder>"
        "  </menubar>"
        "</ui>";

static const GtkActionEntry action_entries[] =
        {
		{"SourceCodeAction", NULL, N_("Source Code")},
		{ "GenerateTagsAction",
		  NULL,
		  N_("Generate Project Tags"),
		  NULL,
		  N_("Generate the tags symbols file for the project of the current document"),
		  G_CALLBACK (generate_tags_cb) }
        };

static WindowData*
get_plugin_data (GeditWindow * window)
{
        return (WindowData *) (g_object_get_data (G_OBJECT (window), WINDOW_DATA_KEY));
}

static void
generate_tags_cb (GtkAction * action,
		  GeditWindow * window)
{
	g_debug ("Generate tags action");
}

void
sc_menu_enable (ScMenu *self)
{
	GtkUIManager *manager;
        WindowData *data;

        data = g_new (WindowData, 1);
        manager = gedit_window_get_ui_manager (self->priv->window);

	data->menu = self;
        data->action_group = gtk_action_group_new ("SourceCodeActions");
        gtk_action_group_set_translation_domain (data->action_group,
                                                 GETTEXT_PACKAGE);
        gtk_action_group_add_actions (data->action_group,
                                      action_entries,
                                      G_N_ELEMENTS (action_entries),
                                      self->priv->window);

        gtk_ui_manager_insert_action_group (manager, data->action_group, -1);

        data->ui_id = gtk_ui_manager_add_ui_from_string (manager, ui_str,
                                                         -1, NULL);

        g_object_set_data_full (G_OBJECT (self->priv->window),
                                WINDOW_DATA_KEY,
                                data,
                                (GDestroyNotify) window_data_free);
}

void
sc_menu_disable (ScMenu *self)
{
	GtkUIManager *manager;
        WindowData *data;

        manager = gedit_window_get_ui_manager (self->priv->window);

        data = get_plugin_data (self->priv->window);
        g_return_if_fail (data != NULL);

        gtk_ui_manager_remove_ui (manager, data->ui_id);
        gtk_ui_manager_remove_action_group (manager, data->action_group);

        g_object_set_data (G_OBJECT (self->priv->window), WINDOW_DATA_KEY, NULL);
}

void
sc_menu_insert_main_actions (ScMenu *self, const GtkActionEntry *entries, gint n_entries)
{
	GtkUIManager *manager;
        WindowData *data;
	gint i;

	g_return_if_fail (entries != NULL);
	
        manager = gedit_window_get_ui_manager (self->priv->window);
	data = get_plugin_data (self->priv->window);
	g_return_if_fail (data != NULL);

        gtk_action_group_add_actions (data->action_group,
                                      entries,
                                      n_entries,
                                      self->priv->window);
	for (i=0;i < n_entries;i++)
	{
		gtk_ui_manager_add_ui (manager, data->ui_id,
				       "/MenuBar/ExtraMenu_1/SourceCodeMenu/ScMainPlaceholder",
				       entries[i].name,
				       entries[i].name,
				       GTK_UI_MANAGER_MENUITEM,
				       FALSE);
	}
}
