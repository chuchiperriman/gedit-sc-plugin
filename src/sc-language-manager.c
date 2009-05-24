/* sc-language-manager.c */

#include "sc-language-manager.h"

G_DEFINE_TYPE (ScLanguageManager, sc_language_manager, G_TYPE_OBJECT)

#define GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SC_TYPE_LANGUAGE_MANAGER, ScLanguageManagerPrivate))

typedef struct _ScLanguageManagerPrivate ScLanguageManagerPrivate;

struct _ScLanguageManagerPrivate {
    int dummy;
};

static void
sc_language_manager_finalize (GObject *object)
{
  G_OBJECT_CLASS (sc_language_manager_parent_class)->finalize (object);
}


static void
sc_language_manager_class_init (ScLanguageManagerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (ScLanguageManagerPrivate));

  object_class->finalize = sc_language_manager_finalize;
}

static void
sc_language_manager_init (ScLanguageManager *self)
{
}

ScLanguageManager*
sc_language_manager_new (void)
{
  return g_object_new (SC_TYPE_LANGUAGE_MANAGER, NULL);
}


