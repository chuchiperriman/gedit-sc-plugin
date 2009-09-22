/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * sc-lm-c.h
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

#ifndef __SC_LM_C_H__
#define __SC_LM_C_H__

#include <glib-object.h>
#include "sc-language-manager.h"

G_BEGIN_DECLS

#define SC_TYPE_LM_C			(sc_lm_c_get_type ())
#define SC_LM_C(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_LM_C, ScLmC))
#define SC_LM_C_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_LM_C, ScLmC const))
#define SC_LM_C_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_LM_C, ScLmCClass))
#define SC_IS_LM_C(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_LM_C))
#define SC_IS_LM_C_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_LM_C))
#define SC_LM_C_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_LM_C, ScLmCClass))

typedef struct _ScLmC		ScLmC;
typedef struct _ScLmCClass	ScLmCClass;
typedef struct _ScLmCPrivate	ScLmCPrivate;

struct _ScLmC {
	GObject parent;
	
	ScLmCPrivate *priv;
};

struct _ScLmCClass {
	GObjectClass parent_class;
};

GType 			 sc_lm_c_get_type 		(void) G_GNUC_CONST;

ScLmC			*sc_lm_c_new	 		(void);

G_END_DECLS

#endif /* __SC_LM_C_H__ */
