/* 
 *  gsc-provider-csymbols.h - Type here a short description of your plugin
 *
 *  Copyright (C) 2009 - perriman
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
#ifndef __CSYMBOLS_PROVIDER_H__
#define __CSYMBOLS_PROVIDER_H__

#include <glib.h>
#include <glib-object.h>
#include <gtksourcecompletion/gsc-provider.h>
#include <gtksourcecompletion/gsc-completion.h>

G_BEGIN_DECLS

#define GSC_TYPE_PROVIDER_CSYMBOLS (gsc_provider_csymbols_get_type ())
#define GSC_PROVIDER_CSYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSC_TYPE_PROVIDER_CSYMBOLS, GscProviderCsymbols))
#define GSC_PROVIDER_CSYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GSC_TYPE_PROVIDER_CSYMBOLS, GscProviderCsymbolsClass))
#define GSC_IS_PROVIDER_CSYMBOLS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSC_TYPE_PROVIDER_CSYMBOLS))
#define GSC_IS_PROVIDER_CSYMBOLS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSC_TYPE_PROVIDER_CSYMBOLS))
#define GSC_PROVIDER_CSYMBOLS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GSC_TYPE_PROVIDER_CSYMBOLS, GscProviderCsymbolsClass))

#define GSC_PROVIDER_CSYMBOLS_NAME "GscProviderCsymbols"
#define GSC_PROVIDER_CSYMBOLS_GOTO_NAME "GscProviderCsymbolsGoto"
#define GSC_PROVIDER_CSYMBOLS_GLOBAL_GOTO_NAME "GscProviderCsymbolsGlobalGoto"
#define GSC_PROVIDER_CSYMBOLS_MEMBERS_NAME "GscProviderCsymbolsMembers"

typedef struct _GscProviderCsymbols GscProviderCsymbols;
typedef struct _GscProviderCsymbolsClass GscProviderCsymbolsClass;
typedef struct _GscProviderCsymbolsPrivate GscProviderCsymbolsPrivate;

struct _GscProviderCsymbols {
	GObject parent;
	GscProviderCsymbolsPrivate *priv;	
};

struct _GscProviderCsymbolsClass {
	GObjectClass parent;
};

typedef enum
{
	SYMBOLS_TYPE,
	GOTO_TYPE,
	MEMBERS_TYPE,
	GLOBAL_GOTO_TYPE
} CSymbolsType;

typedef struct 
{
        gchar           *name;
        gchar           *type;
        gchar           *file;
        gchar           *language;
        gint            line;
        gchar		*signature;
} Symbol;

GType gsc_provider_csymbols_get_type ();

GscProviderCsymbols* 	gsc_provider_csymbols_new	(GscCompletion *comp,
							 GeditWindow *gedit_win,
							 CSymbolsType type);

G_END_DECLS

#endif
