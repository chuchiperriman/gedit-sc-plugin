/*
 * gsc-trigger-symbols.h - Type here a short description of your trigger
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
#include <gtksourcecompletion/gsc-trigger.h>
#include <gtksourcecompletion/gsc-manager.h>

G_BEGIN_DECLS

#define GSC_TYPE_TRIGGER_SYMBOLS (gsc_trigger_symbols_get_type ())
#define GSC_TRIGGER_SYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSC_TYPE_TRIGGER_SYMBOLS, GscTriggerSymbols))
#define GSC_TRIGGER_SYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GSC_TYPE_TRIGGER_SYMBOLS, GscTriggerSymbolsClass))
#define GSC_IS_TRIGGER_SYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSC_TYPE_TRIGGER_SYMBOLS))
#define GSC_IS_TRIGGER_SYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSC_TYPE_TRIGGER_SYMBOLS))
#define GSC_TRIGGER_SYMBOLS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GSC_TYPE_TRIGGER_SYMBOLS, GscTriggerSymbolsClass))

#define GSC_TRIGGER_SYMBOLS_NAME "GscTriggerSymbols"

typedef struct _GscTriggerSymbols GscTriggerSymbols;
typedef struct _GscTriggerSymbolsClass GscTriggerSymbolsClass;
typedef struct _GscTriggerSymbolsPrivate GscTriggerSymbolsPrivate;

struct _GscTriggerSymbols {
	GObject parent;
	GscTriggerSymbolsPrivate *priv;	
};

struct _GscTriggerSymbolsClass {
	GObjectClass parent;
};

GscTriggerSymbols* 
gsc_trigger_symbols_new(GscManager *completion);

GType gsc_trigger_symbols_get_type ();

G_END_DECLS

#endif
