/*-
 * Copyright (c) 2004-2006  os-cillation e.K.
 * Copyright (c) 2002,2004  Anders Carlsson <andersca@gnu.org>
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

#ifndef __BLXO_ICON_VIEW_H__
#define __BLXO_ICON_VIEW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoIconViewPrivate    BlxoIconViewPrivate;
typedef struct _BlxoIconViewClass      BlxoIconViewClass;
typedef struct _BlxoIconView           BlxoIconView;

#define BLXO_TYPE_ICON_VIEW            (blxo_icon_view_get_type ())
#define BLXO_ICON_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_ICON_VIEW, BlxoIconView))
#define BLXO_ICON_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_ICON_VIEW, BlxoIconViewClass))
#define BLXO_IS_ICON_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_ICON_VIEW))
#define BLXO_IS_ICON_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_ICON_VIEW))
#define BLXO_ICON_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_ICON_VIEW, BlxoIconViewClass))

/**
 * BlxoIconViewForeachFunc:
 * @icon_view : an #BlxoIconView.
 * @path      : the current path.
 * @user_data : the user data supplied to blxo_icon_view_selected_foreach().
 *
 * Callback function prototype, invoked for every selected path in the
 * @icon_view. See blxo_icon_view_selected_foreach() for details.
 **/
typedef void (*BlxoIconViewForeachFunc) (BlxoIconView *icon_view,
                                        GtkTreePath *path,
                                        gpointer     user_data);

/**
 * BlxoIconViewSearchEqualFunc:
 * @model       : the #GtkTreeModel being searched.
 * @column      : the search column set by blxo_icon_view_set_search_column().
 * @key         : the key string to compare with.
 * @iter        : the #GtkTreeIter of the current item.
 * @search_data : user data from blxo_icon_view_set_search_equal_func().
 *
 * A function used for checking whether a row in @model matches a search key string
 * entered by the user. Note the return value is reversed from what you would normally
 * expect, though it has some similarity to strcmp() returning 0 for equal strings.
 *
 * Returns: %FALSE if the row matches, %TRUE otherwise.
 **/
typedef gboolean (*BlxoIconViewSearchEqualFunc) (GtkTreeModel *model,
                                                gint          column,
                                                const gchar  *key,
                                                GtkTreeIter  *iter,
                                                gpointer      search_data);

/**
 * BlxoIconViewSearchPositionFunc:
 * @icon_view     : an #BlxoIconView.
 * @search_dialog : the search dialog window to place.
 * @user_data     : user data from blxo_icon_view_set_search_position_func().
 *
 * A function used to place the @search_dialog for the @icon_view.
 **/
typedef void (*BlxoIconViewSearchPositionFunc) (BlxoIconView *icon_view,
                                               GtkWidget   *search_dialog,
                                               gpointer     user_data);

/**
 * BlxoIconViewDropPosition:
 * @BLXO_ICON_VIEW_NO_DROP    : no drop indicator.
 * @BLXO_ICON_VIEW_DROP_INTO  : drop indicator on an item.
 * @BLXO_ICON_VIEW_DROP_LEFT  : drop indicator on the left of an item.
 * @BLXO_ICON_VIEW_DROP_RIGHT : drop indicator on the right of an item.
 * @BLXO_ICON_VIEW_DROP_ABOVE : drop indicator above an item.
 * @BLXO_ICON_VIEW_DROP_BELOW : drop indicator below an item.
 *
 * Specifies whether to display the drop indicator,
 * i.e. where to drop into the icon view.
 **/
typedef enum
{
  BLXO_ICON_VIEW_NO_DROP,
  BLXO_ICON_VIEW_DROP_INTO,
  BLXO_ICON_VIEW_DROP_LEFT,
  BLXO_ICON_VIEW_DROP_RIGHT,
  BLXO_ICON_VIEW_DROP_ABOVE,
  BLXO_ICON_VIEW_DROP_BELOW
} BlxoIconViewDropPosition;

/**
 * BlxoIconViewLayoutMode:
 * @BLXO_ICON_VIEW_LAYOUT_ROWS : layout items in rows.
 * @BLXO_ICON_VIEW_LAYOUT_COLS : layout items in columns.
 *
 * Specifies the layouting mode of an #BlxoIconView. @BLXO_ICON_VIEW_LAYOUT_ROWS
 * is the default, which lays out items vertically in rows from top to bottom.
 * @BLXO_ICON_VIEW_LAYOUT_COLS lays out items horizontally in columns from left
 * to right.
 **/
typedef enum
{
  BLXO_ICON_VIEW_LAYOUT_ROWS,
  BLXO_ICON_VIEW_LAYOUT_COLS
} BlxoIconViewLayoutMode;

/**
 * BlxoIconView:
 *
 * #BlxoIconView provides an alternative view on a list model.
 * It displays the model as a grid of icons with labels. Like
 * #GtkTreeView, it allows to select one or multiple items
 * (depending on the selection mode, see blxo_icon_view_set_selection_mode()).
 * In addition to selection with the arrow keys, #BlxoIconView supports
 * rubberband selection, which is controlled by dragging the pointer.
 **/
struct _BlxoIconView
{
  GtkContainer        __parent__;

  /*< private >*/
  BlxoIconViewPrivate *priv;
};

struct _BlxoIconViewClass
{
  GtkContainerClass __parent__;

  /* virtual methods */
  void     (*set_scroll_adjustments)    (BlxoIconView     *icon_view,
                                         GtkAdjustment   *hadjustment,
                                         GtkAdjustment   *vadjustment);

  /* signals */
  void     (*item_activated)            (BlxoIconView     *icon_view,
                                         GtkTreePath     *path);
  void     (*selection_changed)         (BlxoIconView     *icon_view);

  /* Key binding signals */
  void     (*select_all)                (BlxoIconView    *icon_view);
  void     (*unselect_all)              (BlxoIconView    *icon_view);
  void     (*select_cursor_item)        (BlxoIconView    *icon_view);
  void     (*toggle_cursor_item)        (BlxoIconView    *icon_view);
  gboolean (*move_cursor)               (BlxoIconView    *icon_view,
                                         GtkMovementStep step,
                                         gint            count);
  gboolean (*activate_cursor_item)      (BlxoIconView    *icon_view);
  gboolean (*start_interactive_search)  (BlxoIconView    *icon_view);

  /*< private >*/
  void (*reserved0) (void);
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
  void (*reserved4) (void);
  void (*reserved5) (void);
  void (*reserved6) (void);
  void (*reserved7) (void);
  void (*reserved8) (void);
  void (*reserved9) (void);
};

GType                 blxo_icon_view_get_type                  (void) G_GNUC_CONST;

GtkWidget            *blxo_icon_view_new                       (void);
GtkWidget            *blxo_icon_view_new_with_model            (GtkTreeModel             *model);

GtkTreeModel         *blxo_icon_view_get_model                 (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_model                 (BlxoIconView              *icon_view,
                                                               GtkTreeModel             *model);

GtkOrientation        blxo_icon_view_get_orientation           (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_orientation           (BlxoIconView              *icon_view,
                                                               GtkOrientation            orientation);

gint                  blxo_icon_view_get_columns               (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_columns               (BlxoIconView              *icon_view,
                                                               gint                      columns);

gint                  blxo_icon_view_get_item_width            (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_item_width            (BlxoIconView              *icon_view,
                                                               gint                      item_width);

gint                  blxo_icon_view_get_spacing               (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_spacing               (BlxoIconView              *icon_view,
                                                               gint                      spacing);

gint                  blxo_icon_view_get_row_spacing           (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_row_spacing           (BlxoIconView              *icon_view,
                                                               gint                      row_spacing);

gint                  blxo_icon_view_get_column_spacing        (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_column_spacing        (BlxoIconView              *icon_view,
                                                               gint                      column_spacing);

gint                  blxo_icon_view_get_margin                (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_margin                (BlxoIconView              *icon_view,
                                                               gint                      margin);

GtkSelectionMode      blxo_icon_view_get_selection_mode        (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_selection_mode        (BlxoIconView              *icon_view,
                                                               GtkSelectionMode          mode);

BlxoIconViewLayoutMode blxo_icon_view_get_layout_mode           (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_layout_mode           (BlxoIconView              *icon_view,
                                                               BlxoIconViewLayoutMode     layout_mode);

gboolean              blxo_icon_view_get_single_click          (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_single_click          (BlxoIconView              *icon_view,
                                                               gboolean                  single_click);

guint                 blxo_icon_view_get_single_click_timeout  (const BlxoIconView        *icon_view);
void                  blxo_icon_view_set_single_click_timeout  (BlxoIconView              *icon_view,
                                                               guint                     single_click_timeout);

void                  blxo_icon_view_widget_to_icon_coords     (const BlxoIconView        *icon_view,
                                                               gint                      wx,
                                                               gint                      wy,
                                                               gint                     *ix,
                                                               gint                     *iy);
void                  blxo_icon_view_icon_to_widget_coords     (const BlxoIconView        *icon_view,
                                                               gint                      ix,
                                                               gint                      iy,
                                                               gint                     *wx,
                                                               gint                     *wy);

GtkTreePath          *blxo_icon_view_get_path_at_pos           (const BlxoIconView        *icon_view,
                                                               gint                      x,
                                                               gint                      y);
gboolean              blxo_icon_view_get_item_at_pos           (const BlxoIconView        *icon_view,
                                                               gint                      x,
                                                               gint                      y,
                                                               GtkTreePath             **path,
                                                               GtkCellRenderer         **cell);

gboolean              blxo_icon_view_get_visible_range         (const BlxoIconView        *icon_view,
                                                               GtkTreePath             **start_path,
                                                               GtkTreePath             **end_path);

void                  blxo_icon_view_selected_foreach          (BlxoIconView              *icon_view,
                                                               BlxoIconViewForeachFunc    func,
                                                               gpointer                  data);
void                  blxo_icon_view_select_path               (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);
void                  blxo_icon_view_unselect_path             (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);
gboolean              blxo_icon_view_path_is_selected          (const BlxoIconView        *icon_view,
                                                               GtkTreePath              *path);
GList                *blxo_icon_view_get_selected_items        (const BlxoIconView        *icon_view);
void                  blxo_icon_view_select_all                (BlxoIconView              *icon_view);
void                  blxo_icon_view_unselect_all              (BlxoIconView              *icon_view);
void                  blxo_icon_view_selection_invert          (BlxoIconView              *icon_view);
void                  blxo_icon_view_item_activated            (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);

gint                  blxo_icon_view_get_item_column           (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);
gint                  blxo_icon_view_get_item_row              (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);

gboolean              blxo_icon_view_get_cursor                (const BlxoIconView        *icon_view,
                                                               GtkTreePath             **path,
                                                               GtkCellRenderer         **cell);
void                  blxo_icon_view_set_cursor                (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path,
                                                               GtkCellRenderer          *cell,
                                                               gboolean                  start_editing);

void                  blxo_icon_view_scroll_to_path            (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path,
                                                               gboolean                  use_align,
                                                               gfloat                    row_align,
                                                               gfloat                    col_align);

/* Drag-and-Drop support */
void                  blxo_icon_view_enable_model_drag_source  (BlxoIconView              *icon_view,
                                                               GdkModifierType           start_button_mask,
                                                               const GtkTargetEntry     *targets,
                                                               gint                      n_targets,
                                                               GdkDragAction             actions);
void                  blxo_icon_view_enable_model_drag_dest    (BlxoIconView              *icon_view,
                                                               const GtkTargetEntry     *targets,
                                                               gint                      n_targets,
                                                               GdkDragAction             actions);
void                  blxo_icon_view_unset_model_drag_source   (BlxoIconView              *icon_view);
void                  blxo_icon_view_unset_model_drag_dest     (BlxoIconView              *icon_view);
void                  blxo_icon_view_set_reorderable           (BlxoIconView              *icon_view,
                                                               gboolean                  reorderable);
gboolean              blxo_icon_view_get_reorderable           (BlxoIconView              *icon_view);


/* These are useful to implement your own custom stuff. */
void                  blxo_icon_view_set_drag_dest_item        (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path,
                                                               BlxoIconViewDropPosition   pos);
void                  blxo_icon_view_get_drag_dest_item        (BlxoIconView              *icon_view,
                                                               GtkTreePath             **path,
                                                               BlxoIconViewDropPosition  *pos);
gboolean              blxo_icon_view_get_dest_item_at_pos      (BlxoIconView              *icon_view,
                                                               gint                      drag_x,
                                                               gint                      drag_y,
                                                               GtkTreePath             **path,
                                                               BlxoIconViewDropPosition  *pos);
#if GTK_CHECK_VERSION (3, 0, 0)
cairo_surface_t      *blxo_icon_view_create_drag_icon          (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);
#else
GdkPixmap            *blxo_icon_view_create_drag_icon          (BlxoIconView              *icon_view,
                                                               GtkTreePath              *path);
#endif


/* Interactive search support */
gboolean                      blxo_icon_view_get_enable_search         (const BlxoIconView            *icon_view);
void                          blxo_icon_view_set_enable_search         (BlxoIconView                  *icon_view,
                                                                       gboolean                      enable_search);
gint                          blxo_icon_view_get_search_column         (const BlxoIconView            *icon_view);
void                          blxo_icon_view_set_search_column         (BlxoIconView                  *icon_view,
                                                                       gint                          search_column);
BlxoIconViewSearchEqualFunc    blxo_icon_view_get_search_equal_func     (const BlxoIconView            *icon_view);
void                          blxo_icon_view_set_search_equal_func     (BlxoIconView                  *icon_view,
                                                                       BlxoIconViewSearchEqualFunc    search_equal_func,
                                                                       gpointer                      search_equal_data,
                                                                       GDestroyNotify                search_equal_destroy);
BlxoIconViewSearchPositionFunc blxo_icon_view_get_search_position_func  (const BlxoIconView            *icon_view);
void                          blxo_icon_view_set_search_position_func  (BlxoIconView                  *icon_view,
                                                                       BlxoIconViewSearchPositionFunc search_position_func,
                                                                       gpointer                      search_position_data,
                                                                       GDestroyNotify                search_position_destroy);

G_END_DECLS

#endif /* __BLXO_ICON_VIEW_H__ */
