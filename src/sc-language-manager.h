/* sc-language-manager.h */

#ifndef _SC_LANGUAGE_MANAGER
#define _SC_LANGUAGE_MANAGER

#include <glib-object.h>

G_BEGIN_DECLS

#define SC_TYPE_LANGUAGE_MANAGER sc_language_manager_get_type()

#define SC_LANGUAGE_MANAGER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SC_TYPE_LANGUAGE_MANAGER, ScLanguageManager))
#define SC_LANGUAGE_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SC_TYPE_LANGUAGE_MANAGER, ScLanguageManagerClass))
#define SC_IS_LANGUAGE_MANAGER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SC_TYPE_LANGUAGE_MANAGER))
#define SC_IS_LANGUAGE_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SC_TYPE_LANGUAGE_MANAGER))
#define SC_LANGUAGE_MANAGER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), SC_TYPE_LANGUAGE_MANAGER, ScLanguageManagerClass))

typedef struct {
  GObject parent;
} ScLanguageManager;

typedef struct {
  GObjectClass parent_class;
} ScLanguageManagerClass;

GType sc_language_manager_get_type (void);

ScLanguageManager	*sc_language_manager_new	(void);

G_END_DECLS

#endif /* _SC_LANGUAGE_MANAGER */

