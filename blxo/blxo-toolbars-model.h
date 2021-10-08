/*-
 * Copyright (c) 2004 os-cillation e.K.
 * Copyright (c) 2003 Marco Pesenti Gritti
 *
 * Written by Benedikt Meurer <benny@xfce.org>.
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

#if !defined (BLXO_INSIDE_BLXO_H) && !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __BLXO_TOOLBARS_MODEL_H__
#define __BLXO_TOOLBARS_MODEL_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BLXO_TYPE_TOOLBARS_MODEL             (blxo_toolbars_model_get_type ())
#define BLXO_TOOLBARS_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_TOOLBARS_MODEL, BlxoToolbarsModel))
#define BLXO_TOOLBARS_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_TOOLBARS_MODEL, BlxoToolbarsModelClass))
#define BLXO_IS_TOOLBARS_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_TOOLBARS_MODEL))
#define BLXO_IS_TOOLBARS_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_TOOLBARS_MODEL))
#define BLXO_TOOLBARS_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_TOOLBARS_MODEL, BlxoToolbarsModelClass))

typedef struct _BlxoToolbarsModelPrivate BlxoToolbarsModelPrivate;
typedef struct _BlxoToolbarsModelClass   BlxoToolbarsModelClass;
typedef struct _BlxoToolbarsModel        BlxoToolbarsModel;

/**
 * BlxoToolbarsModelFlags:
 * @BLXO_TOOLBARS_MODEL_NOT_REMOVABLE     : Not possible to remove items from the model.
 * @BLXO_TOOLBARS_MODEL_ACCEPT_ITEMS_ONLY : Only accept new item, dragging items around
 *                                         is disabled.
 * @BLXO_TOOLBARS_MODEL_OVERRIDE_STYLE    : If set, the #BlxoToolbarsModel
 *                                         accepts a custom #GtkToolbarStyle. See
 *                                         blxo_toolbars_model_set_style().
 *
 * Flags available for #BlxoToolbarsModel.
 */
typedef enum
{
  BLXO_TOOLBARS_MODEL_NOT_REMOVABLE      = 1 << 0,
  BLXO_TOOLBARS_MODEL_ACCEPT_ITEMS_ONLY  = 1 << 1,
  BLXO_TOOLBARS_MODEL_OVERRIDE_STYLE     = 1 << 2,
} BlxoToolbarsModelFlags;


/**
 * BLXO_TOOLBARS_ITEM_TYPE:
 *
 * The string used for drag-and-drop in the toolbars editor/model.
 **/
#define BLXO_TOOLBARS_ITEM_TYPE "application/x-blxo-toolbars-item"

struct _BlxoToolbarsModelClass
{
  GObjectClass  __parent__;

  /* Virtual Table */
  gboolean     (*add_item)  (BlxoToolbarsModel *model,
                             gint              toolbar_position,
                             gint              item_position,
                             const gchar      *id,
                             const gchar      *type);

  void   (*reserved1)  (void);
  void   (*reserved2)  (void);
  void   (*reserved3)  (void);
  void   (*reserved4)  (void);

  /* Signals */
  void   (*item_added)        (BlxoToolbarsModel *model,
                               gint              toolbar_position,
                               gint              item_position);
  void   (*item_removed)      (BlxoToolbarsModel *model,
                               gint              toolbar_position,
                               gint              item_position);
  void   (*toolbar_added)     (BlxoToolbarsModel *model,
                               gint              toolbar_position);
  void   (*toolbar_changed)   (BlxoToolbarsModel *model,
                               gint              toolbar_position);
  void   (*toolbar_removed)   (BlxoToolbarsModel *model,
                               gint              toolbar_position);
  gchar *(*get_item_type)     (BlxoToolbarsModel *model,
                               GdkAtom           dnd_type);
  gchar *(*get_item_id)       (BlxoToolbarsModel *model,
                               const gchar      *type,
                               const gchar      *data);
  gchar *(*get_item_data)     (BlxoToolbarsModel *model,
                               const gchar      *type,
                               const gchar      *id);

  void   (*reserved5)  (void);
  void   (*reserved6)  (void);
  void   (*reserved7)  (void);
  void   (*reserved8)  (void);
};

/**
 * BlxoToolbarsModel:
 *
 * The #BlxoToolbarsModel struct contains only private fields and should
 * not be directly accessed.
 **/
struct _BlxoToolbarsModel
{
  GObject                  __parent__;

  /*< private >*/
  BlxoToolbarsModelPrivate *priv;
};


GType                 blxo_toolbars_model_get_type       (void) G_GNUC_CONST;
BlxoToolbarsModel     *blxo_toolbars_model_new            (void);

void                  blxo_toolbars_model_set_actions    (BlxoToolbarsModel      *model,
                                                         gchar                **actions,
                                                         guint                  n_actions);
gchar               **blxo_toolbars_model_get_actions    (BlxoToolbarsModel      *model);

gboolean              blxo_toolbars_model_load_from_file (BlxoToolbarsModel      *model,
                                                         const gchar           *filename,
                                                         GError               **error);
gboolean              blxo_toolbars_model_save_to_file   (BlxoToolbarsModel      *model,
                                                         const gchar           *filename,
                                                         GError               **error);

BlxoToolbarsModelFlags blxo_toolbars_model_get_flags      (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position);
void                  blxo_toolbars_model_set_flags      (BlxoToolbarsModel      *model,
                                                         BlxoToolbarsModelFlags  flags,
                                                         gint                   toolbar_position);

GtkToolbarStyle       blxo_toolbars_model_get_style      (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position);
void                  blxo_toolbars_model_set_style      (BlxoToolbarsModel      *model,
                                                         GtkToolbarStyle        style,
                                                         gint                   toolbar_position);
void                  blxo_toolbars_model_unset_style    (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position);

gchar                *blxo_toolbars_model_get_item_type  (BlxoToolbarsModel      *model,
                                                         GdkAtom                dnd_type);
gchar                *blxo_toolbars_model_get_item_id    (BlxoToolbarsModel      *model,
                                                         const gchar           *type,
                                                         const gchar           *name);
gchar                *blxo_toolbars_model_get_item_data  (BlxoToolbarsModel      *model,
                                                         const gchar           *type,
                                                         const gchar           *id);

gboolean              blxo_toolbars_model_add_item       (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position,
                                                         gint                   item_position,
                                                         const gchar           *id,
                                                         const gchar           *type);
void                  blxo_toolbars_model_add_separator  (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position,
                                                         gint                   item_position);
gint                  blxo_toolbars_model_add_toolbar    (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position,
                                                         const gchar           *name);

void                  blxo_toolbars_model_move_item      (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position,
                                                         gint                   item_position,
                                                         gint                   new_toolbar_position,
                                                         gint                   new_item_position);

void                  blxo_toolbars_model_remove_item    (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position,
                                                         gint                   item_position);
void                  blxo_toolbars_model_remove_toolbar (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position);

gint                  blxo_toolbars_model_n_items        (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position);
void                  blxo_toolbars_model_item_nth       (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position,
                                                         gint                   item_position,
                                                         gboolean              *is_separator,
                                                         const gchar          **id,
                                                         const gchar          **type);

gint                  blxo_toolbars_model_n_toolbars     (BlxoToolbarsModel      *model);
const gchar          *blxo_toolbars_model_toolbar_nth    (BlxoToolbarsModel      *model,
                                                         gint                   toolbar_position);

G_END_DECLS

#endif /* !__BLXO_TOOLBARS_MODEL_H__ */
