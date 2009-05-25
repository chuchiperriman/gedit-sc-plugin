/* sc-symbol.c */

#include "sc-symbol.h"

G_DEFINE_TYPE (ScSymbol, sc_symbol, G_TYPE_OBJECT)

#define GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SC_TYPE_SYMBOL, ScSymbolPrivate))

struct _ScSymbolPrivate {
	gchar	*name;
        gchar	*type;
        gchar	*file;
        gchar	*language;
        gint	 line;
        gchar	*signature;
};

static void
sc_symbol_finalize (GObject *object)
{
	ScSymbol *self = SC_SYMBOL (object);
	if (self->priv->name) g_free (self->priv->name);
	if (self->priv->file) g_free (self->priv->file);
	if (self->priv->type) g_free (self->priv->type);
	if (self->priv->language) g_free (self->priv->language);
	if (self->priv->signature) g_free (self->priv->signature);
        
	G_OBJECT_CLASS (sc_symbol_parent_class)->finalize (object);
}

static void
sc_symbol_class_init (ScSymbolClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (klass, sizeof (ScSymbolPrivate));
	object_class->finalize = sc_symbol_finalize;
}

static void
sc_symbol_init (ScSymbol *self)
{
	self->priv = GET_PRIVATE (self);
	self->priv->name = NULL;
	self->priv->file = NULL;
	self->priv->type = NULL;
	self->priv->language = NULL;
	self->priv->signature = NULL;
}

ScSymbol*
sc_symbol_new	(const gchar *name,
		 const gchar *type,
		 const gchar *file,
		 const gchar *language,
		 const gint line,
		 const gchar *signature)
{
	ScSymbol *self = g_object_new (SC_TYPE_SYMBOL, NULL);
	self->priv->name = g_strdup (name);
	self->priv->type = g_strdup (type);
	self->priv->file = g_strdup (file);
	self->priv->language = g_strdup (language);
	self->priv->line = line;
	self->priv->signature = g_strdup (signature);
	return self;
}

const gchar*
sc_symbol_get_name	(ScSymbol *self)
{
	return self->priv->name;
}
void
sc_symbol_set_name	(ScSymbol *self, const gchar *name)
{
	self->priv->name = g_strdup (name);
}
const gchar*
sc_symbol_get_type_name	(ScSymbol *self)
{
	return self->priv->type;
}
void
sc_symbol_set_type_name	(ScSymbol *self, const gchar *type)
{
	self->priv->type = g_strdup (type);
}
const gchar*
sc_symbol_get_file	(ScSymbol *self)
{
	return self->priv->file;
}
void
sc_symbol_set_file	(ScSymbol *self, const gchar *file)
{
	self->priv->file = g_strdup (file);
}
const gchar*
sc_symbol_get_language	(ScSymbol *self)
{
	return self->priv->language;
}
void
sc_symbol_set_language	(ScSymbol *self, const gchar *language)
{
	self->priv->language = g_strdup (language);
}
const gint
sc_symbol_get_line	(ScSymbol *self)
{
	return self->priv->line;
}
void
sc_symbol_set_line	(ScSymbol *self, gint line)
{
	self->priv->line = line;
}
const gchar*
sc_symbol_get_signature	(ScSymbol *self)
{
	return self->priv->signature;
}
void
sc_symbol_set_signature	(ScSymbol *self, const gchar *signature)
{
	self->priv->signature = g_strdup (signature);
}


