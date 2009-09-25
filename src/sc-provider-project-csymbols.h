/* 
 *  sc-provider-project-csymbols.h - Type here a short description of your plugin
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
#ifndef __SC_PROVIDER_PROJECT_CSYMBOLS_H__
#define __SC_PROVIDER_PROJECT_CSYMBOLS_H__

#include <glib.h>
#include <glib-object.h>
#include <gtksourceview/gtksourcecompletionprovider.h>
#include <gedit/gedit-plugin.h>
#include "sc-language-manager.h"

G_BEGIN_DECLS

#define SC_TYPE_PROVIDER_PROJECT_CSYMBOLS (sc_provider_project_csymbols_get_type ())
#define SC_PROVIDER_PROJECT_CSYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, ScProviderProjectCsymbols))
#define SC_PROVIDER_PROJECT_CSYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, ScProviderProjectCsymbolsClass))
#define SC_IS_PROVIDER_PROJECT_CSYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS))
#define SC_IS_PROVIDER_PROJECT_CSYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS))
#define SC_PROVIDER_PROJECT_CSYMBOLS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_PROVIDER_PROJECT_CSYMBOLS, ScProviderProjectCsymbolsClass))

#define SC_PROVIDER_PROJECT_CSYMBOLS_NAME "ScProviderProjectCsymbols"

typedef struct _ScProviderProjectCsymbols ScProviderProjectCsymbols;
typedef struct _ScProviderProjectCsymbolsPrivate ScProviderProjectCsymbolsPrivate;
typedef struct _ScProviderProjectCsymbolsClass ScProviderProjectCsymbolsClass;

struct _ScProviderProjectCsymbols
{
	GObject parent;
	
	ScProviderProjectCsymbolsPrivate *priv;
};

struct _ScProviderProjectCsymbolsClass
{
	GObjectClass parent;
};

GType		 sc_provider_project_csymbols_get_type	(void) G_GNUC_CONST;

ScProviderProjectCsymbols *sc_provider_project_csymbols_new (GeditDocument *document,
							     ScLanguageManager *lm);

G_END_DECLS

#endif
