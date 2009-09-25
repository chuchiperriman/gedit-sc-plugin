/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * sc-language-manager.h
 *
 * Copyright (C) 2009 Jesús Barbero Rodríguez <chuchiperriman@gmail.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA.
 */

#ifndef __SC_LANGUAGE_MANAGER_H__
#define __SC_LANGUAGE_MANAGER_H__

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

#define SC_TYPE_LANGUAGE_MANAGER		(sc_language_manager_get_type ())
#define SC_LANGUAGE_MANAGER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_LANGUAGE_MANAGER, ScLanguageManager))
#define SC_IS_LANGUAGE_MANAGER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_LANGUAGE_MANAGER))
#define SC_LANGUAGE_MANAGER_GET_INTERFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), SC_TYPE_LANGUAGE_MANAGER, ScLanguageManagerIface))

typedef struct _ScLanguageManager		ScLanguageManager;
typedef struct _ScLanguageManagerIface		ScLanguageManagerIface;

struct _ScLanguageManagerIface
{
	GTypeInterface parent;
	
	/* Interface functions */
	const gchar 	*(*get_language)		(ScLanguageManager	*lm);
	void		 (*set_active)			(ScLanguageManager	*lm,
						 	 gboolean		 active);
	void		 (*activate_document)		(ScLanguageManager	*lm,
							 GeditDocument		*doc);
	GList		*(*get_document_symbols)	(ScLanguageManager	*lm);
	GList		*(*get_project_symbols)		(ScLanguageManager	*lm);
};

GType 			 sc_language_manager_get_type 	(void) G_GNUC_CONST;

const gchar		*sc_language_manager_get_language	(ScLanguageManager	*lm);

void			 sc_language_manager_set_active		(ScLanguageManager	*lm,
								 gboolean		 active);
								 
void			 sc_language_manager_activate_document	(ScLanguageManager	*lm,
								 GeditDocument		*doc);

GList			*sc_language_manager_get_document_symbols (ScLanguageManager	*lm);

GList			*sc_language_manager_get_project_symbols (ScLanguageManager	*lm);


G_END_DECLS

#endif /* __SC_LANGUAGE_MANAGER_H__ */
