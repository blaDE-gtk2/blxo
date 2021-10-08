/*-
 * Copyright (c) 2005-2006 Benedikt Meurer <benny@xfce.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#if !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file is not part of the public API."
#endif

#ifndef __BLXO_ICON_CHOOSER_MODEL_H__
#define __BLXO_ICON_CHOOSER_MODEL_H__

#include <blxo/blxo-config.h>

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoIconChooserModelClass BlxoIconChooserModelClass;
typedef struct _BlxoIconChooserModel      BlxoIconChooserModel;

#define BLXO_TYPE_ICON_CHOOSER_MODEL             (blxo_icon_chooser_model_get_type ())
#define BLXO_ICON_CHOOSER_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_ICON_CHOOSER_MODEL, BlxoIconChooserModel))
#define BLXO_ICON_CHOOSER_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_ICON_CHOOSER_MODEL, BlxoIconChooserModelClass))
#define BLXO_IS_ICON_CHOOSER_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_ICON_CHOOSER_MODEL))
#define BLXO_IS_ICON_CHOOSER_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_ICON_CHOOSER_MODEL))
#define BLXO_ICON_CHOOSER_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_ICON_CHOOSER_MODEL, BlxoIconChooserModelClass))

/**
 * BlxoIconChooserContexts:
 *
 * The list of default contexts for the icon themes
 * according to the Icon Naming Spec, Version 0.7.
 **/
typedef enum
{
  /* the contexts provided by the model */
  BLXO_ICON_CHOOSER_CONTEXT_ACTIONS,
  BLXO_ICON_CHOOSER_CONTEXT_ANIMATIONS,
  BLXO_ICON_CHOOSER_CONTEXT_APPLICATIONS,
  BLXO_ICON_CHOOSER_CONTEXT_CATEGORIES,
  BLXO_ICON_CHOOSER_CONTEXT_DEVICES,
  BLXO_ICON_CHOOSER_CONTEXT_EMBLEMS,
  BLXO_ICON_CHOOSER_CONTEXT_EMOTES,
  BLXO_ICON_CHOOSER_CONTEXT_MIME_TYPES,
  BLXO_ICON_CHOOSER_CONTEXT_PLACES,
  BLXO_ICON_CHOOSER_CONTEXT_STATUS,
  BLXO_ICON_CHOOSER_CONTEXT_STOCK,
  BLXO_ICON_CHOOSER_CONTEXT_OTHER,
  BLXO_ICON_CHOOSER_N_CONTEXTS,

  /* not provided by the model (plus separators before them) */
  BLXO_ICON_CHOOSER_CONTEXT_ALL  = BLXO_ICON_CHOOSER_CONTEXT_OTHER + 2,
  BLXO_ICON_CHOOSER_CONTEXT_FILE = BLXO_ICON_CHOOSER_CONTEXT_OTHER + 4,
} BlxoIconChooserContext;

/**
 * BlxoIconChooserModelColumns:
 * @BLXO_ICON_CHOOSER_MODEL_COLUMN_CONTEXT      : the context of the icon.
 * @BLXO_ICON_CHOOSER_MODEL_COLUMN_ICON_NAME    : the name of the icon.
 * @BLXO_ICON_CHOOSER_MODEL_N_COLUMNS           : the number of columns.
 *
 * The columns provided by the #BlxoIconChooserModel.
 **/
typedef enum
{
  BLXO_ICON_CHOOSER_MODEL_COLUMN_CONTEXT,
  BLXO_ICON_CHOOSER_MODEL_COLUMN_ICON_NAME,
  BLXO_ICON_CHOOSER_MODEL_N_COLUMNS,
} BlxoIconChooserModelColumn;

G_GNUC_INTERNAL GType                  blxo_icon_chooser_model_get_type                (void) G_GNUC_CONST;

G_GNUC_INTERNAL BlxoIconChooserModel   *_blxo_icon_chooser_model_get_for_widget         (GtkWidget           *widget) G_GNUC_WARN_UNUSED_RESULT;
G_GNUC_INTERNAL BlxoIconChooserModel   *_blxo_icon_chooser_model_get_for_icon_theme     (GtkIconTheme        *icon_theme) G_GNUC_WARN_UNUSED_RESULT;

G_GNUC_INTERNAL gboolean               _blxo_icon_chooser_model_get_iter_for_icon_name (BlxoIconChooserModel *model,
                                                                                       GtkTreeIter         *iter,
                                                                                       const gchar         *icon_name);

G_END_DECLS

#endif /* !__BLXO_ICON_CHOOSER_MODEL_H__ */
