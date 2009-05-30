/*
 * gtksourcecompletiontrigger-symbols.h - Type here a short description of your trigger
 *
 * Copyright (C) 2009 - perriman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __TRIGGER_SYMBOLS_H__
#define __TRIGGER_SYMBOLS_H__

#include <glib.h>
#include <glib-object.h>
#include <gtksourceview/gtksourcecompletiontrigger.h>
#include <gtksourceview/gtksourcecompletioncompletion.h>

G_BEGIN_DECLS

#define SC_TYPE_TRIGGER_SYMBOLS (gtk_source_completion_trigger_symbols_get_type ())
#define SC_TRIGGER_SYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_TRIGGER_SYMBOLS, GtkSourceCompletionTriggerSymbols))
#define SC_TRIGGER_SYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_TRIGGER_SYMBOLS, GtkSourceCompletionTriggerSymbolsClass))
#define SC_IS_TRIGGER_SYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_TRIGGER_SYMBOLS))
#define SC_IS_TRIGGER_SYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_TRIGGER_SYMBOLS))
#define SC_TRIGGER_SYMBOLS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_TRIGGER_SYMBOLS, GtkSourceCompletionTriggerSymbolsClass))

#define SC_TRIGGER_SYMBOLS_NAME "GtkSourceCompletionTriggerSymbols"

typedef struct _GtkSourceCompletionTriggerSymbols GtkSourceCompletionTriggerSymbols;
typedef struct _GtkSourceCompletionTriggerSymbolsClass GtkSourceCompletionTriggerSymbolsClass;
typedef struct _GtkSourceCompletionTriggerSymbolsPrivate GtkSourceCompletionTriggerSymbolsPrivate;

struct _GtkSourceCompletionTriggerSymbols {
	GObject parent;
	GtkSourceCompletionTriggerSymbolsPrivate *priv;	
};

struct _GtkSourceCompletionTriggerSymbolsClass {
	GObjectClass parent;
};

GtkSourceCompletionTriggerSymbols* 
gtk_source_completion_trigger_symbols_new(GtkSourceCompletionCompletion *completion);

GType gtk_source_completion_trigger_symbols_get_type ();

G_END_DECLS

#endif