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

typedef struct {
  GObject parent;
} ScSymbol;

typedef struct {
  GObjectClass parent_class;
} ScSymbolClass;

GType sc_symbol_get_type (void);

ScSymbol	*sc_symbol_new		(const gchar *name,
					 const gchar *type,
					 const gchar *file,
					 const gchar *language,
					 const gint line,
					 const gchar *signature);

G_END_DECLS

#endif /* _SC_SYMBOL */

