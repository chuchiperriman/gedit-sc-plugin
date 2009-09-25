/* 
 *  sc-provider-csymbols.h - Type here a short description of your plugin
 *
 *  Copyright (C) 2008 - perriman
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __SC_PROVIDER_CSYMBOLS_H__
#define __SC_PROVIDER_CSYMBOLS_H__

#include <glib.h>
#include <glib-object.h>
#include <gtksourceview/gtksourcecompletionprovider.h>
#include <gedit/gedit-plugin.h>
#include "sc-language-manager.h"

G_BEGIN_DECLS

#define SC_TYPE_PROVIDER_CSYMBOLS (sc_provider_csymbols_get_type ())
#define SC_PROVIDER_CSYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_PROVIDER_CSYMBOLS, ScProviderCsymbols))
#define SC_PROVIDER_CSYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_PROVIDER_CSYMBOLS, ScProviderCsymbolsClass))
#define SC_IS_PROVIDER_CSYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_PROVIDER_CSYMBOLS))
#define SC_IS_PROVIDER_CSYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_PROVIDER_CSYMBOLS))
#define SC_PROVIDER_CSYMBOLS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_PROVIDER_CSYMBOLS, ScProviderCsymbolsClass))

#define SC_PROVIDER_CSYMBOLS_NAME "ScProviderCsymbols"

typedef struct _ScProviderCsymbols ScProviderCsymbols;
typedef struct _ScProviderCsymbolsPrivate ScProviderCsymbolsPrivate;
typedef struct _ScProviderCsymbolsClass ScProviderCsymbolsClass;

struct _ScProviderCsymbols
{
	GObject parent;
	
	ScProviderCsymbolsPrivate *priv;
};

struct _ScProviderCsymbolsClass
{
	GObjectClass parent;
};

GType		 sc_provider_csymbols_get_type	(void) G_GNUC_CONST;

ScProviderCsymbols *sc_provider_csymbols_new (GeditDocument *document,
					      ScLanguageManager *lm);

G_END_DECLS

#endif
