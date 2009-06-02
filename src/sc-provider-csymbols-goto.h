/* 
 *  sc-provider-csymbols-goto.h - Type here a short description of your plugin
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
#ifndef __SC_PROVIDER_CSYMBOLS_GOTO_H__
#define __SC_PROVIDER_CSYMBOLS_GOTO_H__

#include <glib.h>
#include <glib-object.h>
#include <gtksourceview/gtksourcecompletionprovider.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

#define SC_TYPE_PROVIDER_CSYMBOLS_GOTO (sc_provider_csymbols_goto_get_type ())
#define SC_PROVIDER_CSYMBOLS_GOTO(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_PROVIDER_CSYMBOLS_GOTO, ScProviderCsymbolsGoto))
#define SC_PROVIDER_CSYMBOLS_GOTO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_PROVIDER_CSYMBOLS_GOTO, ScProviderCsymbolsGotoClass))
#define SC_IS_PROVIDER_CSYMBOLS_GOTO(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_PROVIDER_CSYMBOLS_GOTO))
#define SC_IS_PROVIDER_CSYMBOLS_GOTO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_PROVIDER_CSYMBOLS_GOTO))
#define SC_PROVIDER_CSYMBOLS_GOTO_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_PROVIDER_CSYMBOLS_GOTO, ScProviderCsymbolsGotoClass))

#define SC_PROVIDER_CSYMBOLS_GOTO_NAME "ScProviderCsymbolsGoto"

typedef struct _ScProviderCsymbolsGoto ScProviderCsymbolsGoto;
typedef struct _ScProviderCsymbolsGotoPrivate ScProviderCsymbolsGotoPrivate;
typedef struct _ScProviderCsymbolsGotoClass ScProviderCsymbolsGotoClass;

struct _ScProviderCsymbolsGoto
{
	GObject parent;
	
	ScProviderCsymbolsGotoPrivate *priv;
};

struct _ScProviderCsymbolsGotoClass
{
	GObjectClass parent;
};

GType		 sc_provider_csymbols_goto_get_type	(void) G_GNUC_CONST;

ScProviderCsymbolsGoto *sc_provider_csymbols_goto_new (GeditDocument *document);

G_END_DECLS

#endif
