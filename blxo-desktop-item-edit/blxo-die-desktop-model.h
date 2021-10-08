/*-
 * Copyright (c) 2006 Benedikt Meurer <benny@xfce.org>.
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

#ifndef __BLXO_DIE_DESKTOP_MODEL_H__
#define __BLXO_DIE_DESKTOP_MODEL_H__

#include <blxo/blxo.h>

G_BEGIN_DECLS;

typedef struct _BlxoDieDesktopModelClass BlxoDieDesktopModelClass;
typedef struct _BlxoDieDesktopModel      BlxoDieDesktopModel;

#define BLXO_DIE_TYPE_DESKTOP_MODEL            (blxo_die_desktop_model_get_type ())
#define BLXO_DIE_DESKTOP_MODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_DIE_TYPE_DESKTOP_MODEL, BlxoDieDesktopModel))
#define BLXO_DIE_DESKTOP_MODEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_DIE_TYPE_DESKTOP_MODEL, BlxoDieDesktopModelClass))
#define BLXO_DIE_IS_DESKTOP_MODEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_DIE_TYPE_DESKTOP_MODEL))
#define BLXO_DIE_IS_DESKTOP_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_DIE_TYPE_DESKTOP_MODEL))
#define BLXO_DIE_DESKTOP_MODEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_DIE_TYPE_DESKTOP_MODEL, BlxoDieDesktopModelClass))

/**
 * BlxoDieDesktopModelColumn:
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_ABSTRACT : the column with the markup text for the renderer.
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_COMMAND  : the column with the application command.
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_COMMENT  : the column with the application comment.
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_ICON     : the column with the application icon.
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_NAME     : the column with the application name.
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_SNOTIFY  : the column with the applications StartupNotify setting.
 * @BLXO_DIE_DESKTOP_MODEL_COLUMN_TERMINAL : the column with the applications Terminal setting.
 *
 * The columns provided by the #BlxoDieDesktopModel.
 **/
typedef enum /*< enum >*/
{
  BLXO_DIE_DESKTOP_MODEL_COLUMN_ABSTRACT,
  BLXO_DIE_DESKTOP_MODEL_COLUMN_COMMAND,
  BLXO_DIE_DESKTOP_MODEL_COLUMN_COMMENT,
  BLXO_DIE_DESKTOP_MODEL_COLUMN_ICON,
  BLXO_DIE_DESKTOP_MODEL_COLUMN_NAME,
  BLXO_DIE_DESKTOP_MODEL_COLUMN_SNOTIFY,
  BLXO_DIE_DESKTOP_MODEL_COLUMN_TERMINAL,
  BLXO_DIE_DESKTOP_MODEL_N_COLUMNS,
} BlxoDieDesktopModelColumn;

GType               blxo_die_desktop_model_get_type    (void) G_GNUC_CONST;

BlxoDieDesktopModel *blxo_die_desktop_model_new         (void) G_GNUC_MALLOC;

gboolean            blxo_die_desktop_model_match_func  (GtkEntryCompletion *completion,
                                                       const gchar        *key,
                                                       GtkTreeIter        *iter,
                                                       gpointer            user_data);

G_END_DECLS;

#endif /* !__BLXO_DIE_DESKTOP_MODEL_H__ */
