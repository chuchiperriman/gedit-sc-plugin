/* 
 * sc-menu.h --- Source Code Menu Manager
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

#ifndef __SC_MENU_H__
#define __SC_MENU_H__

#include <gtk/gtk.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

/* TODO reformat this defines */
#define SC_TYPE_MENU                                                    \
   (sc_menu_get_type())
#define SC_MENU(obj)                                                    \
   (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                  \
                                SC_TYPE_MENU,                           \
                                ScMenu))
#define SC_MENU_CLASS(klass)                                            \
   (G_TYPE_CHECK_CLASS_CAST ((klass),                                   \
                             SC_TYPE_MENU,                              \
                             ScMenuClass))
#define SC_IS_MENU(obj)                                                 \
   (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                  \
                                SC_TYPE_MENU))
#define SC_IS_MENU_CLASS(klass)                                         \
   (G_TYPE_CHECK_CLASS_TYPE ((klass),                                   \
                             SC_TYPE_MENU))
#define SC_MENU_GET_CLASS(obj)                                          \
   (G_TYPE_INSTANCE_GET_CLASS ((obj),                                   \
                               SC_TYPE_MENU,                            \
                               ScMenuClass))


typedef struct _ScMenu      ScMenu;
typedef struct _ScMenuPrivate      ScMenuPrivate;
typedef struct _ScMenuClass ScMenuClass;

struct _ScMenuClass
{
    GObjectClass parent_class;
};

struct _ScMenu
{
	GObject parent;
	ScMenuPrivate *priv;
};

GType sc_menu_get_type (void) G_GNUC_CONST;

ScMenu		*sc_menu_new			(GeditWindow *window);
void		 sc_menu_enable			(ScMenu *self);
void		 sc_menu_disable		(ScMenu *self);
void		 sc_menu_insert_main_actions	(ScMenu *self, const GtkActionEntry *action_entries, gint n_entries);

G_END_DECLS

#endif /* __SC_MENU_H__ */
