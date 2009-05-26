/* sc-symbol.c */

#include "sc-symbol.h"

G_DEFINE_TYPE (ScSymbol, sc_symbol, G_TYPE_OBJECT)

static void
sc_symbol_finalize (GObject *object)
{
	ScSymbol *self = SC_SYMBOL (object);
	if (self->name) g_free (self->name);
	if (self->file) g_free (self->file);
	if (self->type) g_free (self->type);
	if (self->language) g_free (self->language);
	if (self->signature) g_free (self->signature);
        
	G_OBJECT_CLASS (sc_symbol_parent_class)->finalize (object);
}

static void
sc_symbol_class_init (ScSymbolClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = sc_symbol_finalize;
}

static void
sc_symbol_init (ScSymbol *self)
{
	self->name = NULL;
	self->file = NULL;
	self->type = NULL;
	self->language = NULL;
	self->signature = NULL;
}

ScSymbol*
sc_symbol_new	(const gchar *name,
		 const gchar *type,
		 const gchar *file,
		 const gchar *language,
		 const gint line,
		 const gchar *signature)
{
	ScSymbol *self = SC_SYMBOL (g_object_new (SC_TYPE_SYMBOL, NULL));
	self->name = g_strdup (name);
	self->type = g_strdup (type);
	self->file = g_strdup (file);
	self->language = g_strdup (language);
	self->line = line;
	if (signature)
		self->signature = g_strdup (signature);
	return self;
}



