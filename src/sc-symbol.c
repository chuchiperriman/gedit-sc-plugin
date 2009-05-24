/* sc-symbol.c */

#include "sc-symbol.h"

G_DEFINE_TYPE (ScSymbol, sc_symbol, G_TYPE_OBJECT)

#define GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SC_TYPE_SYMBOL, ScSymbolPrivate))

typedef struct _ScSymbolPrivate ScSymbolPrivate;

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
	ScSymbolPrivate *priv = GET_PRIVATE (self);
	priv->name = g_strdup (name);
	/*TODO and the finalize*/
	return self;
}


