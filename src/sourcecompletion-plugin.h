/*
 * sourcecompletion-plugin.h - Adds (auto)completion support to gedit
 *
 * Copyright (C) 2007 - chuchiperriman
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

#ifndef __SOURCECOMPLETION_PLUGIN_H__
#define __SOURCECOMPLETION_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define TYPE_SOURCECOMPLETION_PLUGIN		(sourcecompletion_plugin_get_type ())
#define SOURCECOMPLETION_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_SOURCECOMPLETION_PLUGIN, SourcecompletionPlugin))
#define SOURCECOMPLETION_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), TYPE_SOURCECOMPLETION_PLUGIN, SourcecompletionPluginClass))
#define IS_SOURCECOMPLETION_PLUGIN(o)	(G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_SOURCECOMPLETION_PLUGIN))
#define IS_SOURCECOMPLETION_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_SOURCECOMPLETION_PLUGIN))
#define SOURCECOMPLETION_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_SOURCECOMPLETION_PLUGIN, SourcecompletionPluginClass))

/* Private structure type */
typedef struct _SourcecompletionPluginPrivate	SourcecompletionPluginPrivate;

/*
 * Main object structure
 */
typedef struct _SourcecompletionPlugin		SourcecompletionPlugin;

struct _SourcecompletionPlugin
{
	GeditPlugin parent_instance;

	/*< private >*/
	SourcecompletionPluginPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _SourcecompletionPluginClass	SourcecompletionPluginClass;

struct _SourcecompletionPluginClass
{
	GeditPluginClass parent_class;
};

/*
 * Public methods
 */
GType	sourcecompletion_plugin_get_type	(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_gedit_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SOURCECOMPLETION_PLUGIN_H__ */
