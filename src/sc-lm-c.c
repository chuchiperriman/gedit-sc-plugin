/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * sc-lm-c.c
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#include "sc-lm-c.h"

#define SC_LM_C_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SC_TYPE_LM_C, ScLmCPrivate))

struct _ScLmCPrivate
{
	gchar *label;
	gchar *markup;
	gchar *text;
	gchar *info;
	GdkPixbuf *icon;
};

static void sc_lm_c_iface_init (gpointer g_iface, gpointer iface_data);

G_DEFINE_TYPE_WITH_CODE (ScLmC, 
			 sc_lm_c, 
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (SC_TYPE_LANGUAGE_MANAGER,
			 			sc_lm_c_iface_init))

static const gchar *
sc_lm_c_get_language_impl (ScLanguageManager *self)
{
	return NULL;
}

static void
sc_lm_c_set_active_impl (ScLanguageManager	*self,
			 gboolean		 active)
{

}

static void
sc_lm_c_iface_init (gpointer g_iface, 
					   gpointer iface_data)
{
	ScLanguageManagerIface *iface = (ScLanguageManagerIface *)g_iface;
	
	/* Interface data getter implementations */
	iface->get_language = sc_lm_c_get_language_impl;
	iface->set_active = sc_lm_c_set_active_impl;
}

static void
sc_lm_c_finalize (GObject *object)
{
	ScLmC *self = SC_LM_C(object);
	
	g_free (self->priv->label);
	g_free (self->priv->markup);
	g_free (self->priv->text);

	g_free (self->priv->info);
	
	if (self->priv->icon != NULL)
	{
		g_object_unref (self->priv->icon);
	}

	G_OBJECT_CLASS (sc_lm_c_parent_class)->finalize (object);
}

static void
sc_lm_c_class_init (ScLmCClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = sc_lm_c_finalize;

	g_type_class_add_private (object_class, sizeof(ScLmCPrivate));
}

static void
sc_lm_c_init (ScLmC *self)
{
	self->priv = SC_LM_C_GET_PRIVATE (self);
}

ScLmC *
sc_lm_c_new ()
{
	return g_object_new (SC_TYPE_LM_C, 
			     NULL);
}

