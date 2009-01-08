/* 
 *  gsc-provider-csymbols.c - Type here a short description of your plugin
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

#include "gsc-provider-csymbols.h"

struct _GscProviderCsymbolsPrivate {
	
};

#define GSC_PROVIDER_CSYMBOLS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_GSC_PROVIDER_CSYMBOLS, GscProviderCsymbolsPrivate))

static const gchar* 
gsc_provider_csymbols_real_get_name (GscProvider* self);
static GList* 
gsc_provider_csymbols_real_get_proposals (GscProvider* base,
						     GscTrigger *trigger);
static void 
gsc_provider_csymbols_real_finish (GscProvider* base);

static gpointer 
gsc_provider_csymbols_parent_class = NULL;
static GscProviderIface* 
gsc_provider_csymbols_parent_iface = NULL;


static const gchar* 
gsc_provider_csymbols_real_get_name (GscProvider* self)
{
	return GSC_PROVIDER_CSYMBOLS_NAME;
}

static GList* 
gsc_provider_csymbols_real_get_proposals (GscProvider* base,
						GscTrigger *trigger)
{
	return NULL;
}

static void 
gsc_provider_csymbols_real_finish (GscProvider* base)
{

}

static void 
gsc_provider_csymbols_finalize(GObject *object)
{
	GscProviderCsymbols *self;
	self = GSC_PROVIDER_CSYMBOLS(object);
	G_OBJECT_CLASS(gsc_provider_csymbols_parent_class)->finalize(object);
}


static void 
gsc_provider_csymbols_class_init (GscProviderCsymbolsClass *klass)
{
	gsc_provider_csymbols_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->finalize = gsc_provider_csymbols_finalize;
}

static void
gsc_provider_csymbols_interface_init (GscProviderIface *iface)
{
	gsc_provider_csymbols_parent_iface = g_type_interface_peek_parent (iface);
	
	iface->get_name = gsc_provider_csymbols_real_get_name;
	iface->get_proposals = gsc_provider_csymbols_real_get_proposals;
	iface->finish = gsc_provider_csymbols_real_finish;
}


static void 
gsc_provider_csymbols_init (GscProviderCsymbols * self)
{
	self->priv = g_new0(GscProviderCsymbolsPrivate, 1);
}

GType gsc_provider_csymbols_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GscProviderCsymbolsClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gsc_provider_csymbols_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GscProviderCsymbols), 0, (GInstanceInitFunc) gsc_provider_csymbols_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GscProviderCsymbols", &g_define_type_info, 0);
		static const GInterfaceInfo gsc_provider_info = { (GInterfaceInitFunc) gsc_provider_csymbols_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GSC_TYPE_PROVIDER, &gsc_provider_info);
	}
	return g_define_type_id;
}


GscProviderCsymbols*
gsc_provider_csymbols_new()
{
	return GSC_PROVIDER_CSYMBOLS (g_object_new (GSC_TYPE_PROVIDER_CSYMBOLS, NULL));
}

