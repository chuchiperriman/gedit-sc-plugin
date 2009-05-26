/* sc-symbol.h */

#ifndef _SC_SYMBOL
#define _SC_SYMBOL

#include <glib-object.h>

G_BEGIN_DECLS

#define SC_TYPE_SYMBOL sc_symbol_get_type()
#define SC_SYMBOL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_SYMBOL, ScSymbol))
#define SC_SYMBOL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_SYMBOL, ScSymbolClass))
#define SC_IS_SYMBOL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_SYMBOL))
#define SC_IS_SYMBOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_SYMBOL))
#define SC_SYMBOL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_SYMBOL, ScSymbolClass))

typedef struct _ScSymbolClass ScSymbolClass;
typedef struct _ScSymbolPrivate ScSymbolPrivate;
typedef struct _ScSymbol ScSymbol;

struct _ScSymbol {
	GObject parent;
	ScSymbolPrivate *priv;
};

struct _ScSymbolClass {
	GObjectClass parent_class;
} _ScSymbolClass;

GType		 sc_symbol_get_type		(void);

ScSymbol	*sc_symbol_new			(const gchar *name,
						 const gchar *type,
						 const gchar *file,
						 const gchar *language,
						 const gint line,
						 const gchar *signature);

const gchar	*sc_symbol_get_name		(ScSymbol *self);
void		 sc_symbol_set_name		(ScSymbol *self, const gchar *name);
const gchar	*sc_symbol_get_type_name	(ScSymbol *self);
void		 sc_symbol_set_type_name	(ScSymbol *self, const gchar *type_name);
const gchar	*sc_symbol_get_file		(ScSymbol *self);
void		 sc_symbol_set_file		(ScSymbol *self, const gchar *file);
const gchar	*sc_symbol_get_language		(ScSymbol *self);
void		 sc_symbol_set_language		(ScSymbol *self, const gchar *language);
const gint	 sc_symbol_get_line		(ScSymbol *self);
void		 sc_symbol_set_line		(ScSymbol *self, const gint line);
const gchar	*sc_symbol_get_signature	(ScSymbol *self);
void		 sc_symbol_set_signature	(ScSymbol *self, const gchar *signature);

G_END_DECLS

#endif /* _SC_SYMBOL */

