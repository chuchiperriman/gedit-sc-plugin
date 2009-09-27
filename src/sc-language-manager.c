/*
 * sclanguagemanager.c
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

#include "sc-language-manager.h"

static GSList*
sc_language_manager_get_language_ids_default (ScLanguageManager *lm)
{
	return NULL;
}

static void
sc_language_manager_set_active_default (ScLanguageManager	*lm,
					gboolean		 active)
{
	return;
}

static void
sc_language_manager_activate_document_default (ScLanguageManager	*lm,
					       GeditDocument		*doc)
{
	return;
}

static GList*
sc_language_manager_get_document_symbols_default (ScLanguageManager	*lm)
{
	return NULL;
}

static GList*
sc_language_manager_get_project_symbols_default (ScLanguageManager	*lm)
{
	return NULL;
}

static void 
sc_language_manager_init (ScLanguageManagerIface *iface)
{
	static gboolean initialized = FALSE;
	
	iface->get_language_ids = sc_language_manager_get_language_ids_default;
	iface->set_active = sc_language_manager_set_active_default;
	iface->activate_document = sc_language_manager_activate_document_default;
	iface->get_document_symbols = sc_language_manager_get_document_symbols_default;
	iface->get_project_symbols = sc_language_manager_get_project_symbols_default;
	
	if (!initialized)
	{
		initialized = TRUE;
	}
}

GType 
sc_language_manager_get_type ()
{
	static GType sc_language_manager_type_id = 0;
	
	if (!sc_language_manager_type_id)
	{
		static const GTypeInfo g_define_type_info =
		{
			sizeof (ScLanguageManagerIface),
			(GBaseInitFunc) sc_language_manager_init, 
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL
		};
		
		sc_language_manager_type_id = 
			g_type_register_static (G_TYPE_INTERFACE,
						"ScLanguageManager",
						&g_define_type_info,
						0);

		g_type_interface_add_prerequisite (sc_language_manager_type_id,
		                                   G_TYPE_OBJECT);
	}
	
	return sc_language_manager_type_id;
}

GSList*
sc_language_manager_get_language_ids (ScLanguageManager *lm)
{
	g_return_val_if_fail (SC_IS_LANGUAGE_MANAGER (lm), NULL);	
	return SC_LANGUAGE_MANAGER_GET_INTERFACE (lm)->get_language_ids (lm);
}

void
sc_language_manager_set_active (ScLanguageManager	*lm,
				gboolean 		 active)
{
	g_return_if_fail (SC_IS_LANGUAGE_MANAGER (lm));
	
	SC_LANGUAGE_MANAGER_GET_INTERFACE (lm)->set_active (lm, active);
}

void
sc_language_manager_activate_document	(ScLanguageManager	*lm,
					 GeditDocument		*doc)
{
	g_return_if_fail (SC_IS_LANGUAGE_MANAGER (lm));
	g_return_if_fail (GEDIT_IS_DOCUMENT (doc));
	
	SC_LANGUAGE_MANAGER_GET_INTERFACE (lm)->activate_document (lm, doc);
}

GList*
sc_language_manager_get_document_symbols (ScLanguageManager	*lm)
{
	g_return_val_if_fail (SC_IS_LANGUAGE_MANAGER (lm), NULL);
	
	return SC_LANGUAGE_MANAGER_GET_INTERFACE (lm)->get_document_symbols (lm);
}

GList*
sc_language_manager_get_project_symbols (ScLanguageManager	*lm)
{
	g_return_val_if_fail (SC_IS_LANGUAGE_MANAGER (lm), NULL);
	
	return SC_LANGUAGE_MANAGER_GET_INTERFACE (lm)->get_project_symbols (lm);
}



