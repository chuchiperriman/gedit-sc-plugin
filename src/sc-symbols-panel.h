/*
 * sc-symbols-panel.h
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

#ifndef __SC_SYMBOLS_PANEL_H__
#define __SC_SYMBOLS_PANEL_H__

#include <gtk/gtk.h>
#include <gedit/gedit-window.h>

G_BEGIN_DECLS

#define SC_TYPE_SYMBOLS_PANEL              (sc_symbols_panel_get_type())
#define SC_SYMBOLS_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SC_TYPE_SYMBOLS_PANEL, ScSymbolsPanel))
#define SC_SYMBOLS_PANEL_CONST(obj)        (G_TYPE_CHECK_INSTANCE_CAST((obj), SC_TYPE_SYMBOLS_PANEL, ScSymbolsPanel const))
#define SC_SYMBOLS_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SC_TYPE_SYMBOLS_PANEL, ScSymbolsPanelClass))
#define SC_IS_SYMBOLS_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SC_TYPE_SYMBOLS_PANEL))
#define SC_IS_SYMBOLS_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_SYMBOLS_PANEL))
#define SC_SYMBOLS_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SC_TYPE_SYMBOLS_PANEL, ScSymbolsPanelClass))

typedef struct _ScSymbolsPanelPrivate ScSymbolsPanelPrivate;

typedef struct _ScSymbolsPanel ScSymbolsPanel;

struct _ScSymbolsPanel 
{
	GtkVBox vbox;                   /* parent type */

	ScSymbolsPanelPrivate *priv;
};

typedef struct _ScSymbolsPanelClass ScSymbolsPanelClass;

struct _ScSymbolsPanelClass 
{
	GtkVBoxClass parent_class;      /* parent clase */
};

/* public methods */

GType 
sc_symbols_panel_register_type (GTypeModule *module);
							
GType 
sc_symbols_panel_get_type (void) G_GNUC_CONST;

GtkWidget* 
sc_symbols_panel_new (GeditWindow *window);

void sc_symbols_panel_populate (GtkWidget *panel, GList *symbols);

G_END_DECLS

#endif  /* __SC_SYMBOLS_PANEL_H__  */
