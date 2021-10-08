/*-
 * Copyright (c) 2004-2006 os-cillation e.K.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <blxo/blxo-config.h>
#include <blxo/blxo-icon-bar.h>
#include <blxo/blxo-marshal.h>
#include <blxo/blxo-private.h>
#include <blxo/blxo-string.h>
#include <blxo/blxo-alias.h>

/**
 * SECTION: blxo-icon-bar
 * @title: BlxoIconBar
 * @short_description: A widget for displaying icon bars
 * @include: blxo/blxo.h
 * @see_also: <ulink url="http://library.gnome.org/devel/gtk/stable/GtkTreeModel.html">
 *            GtkTreeModel</ulink>,
 *            <ulink url="http://library.gnome.org/devel/gtk/stable/TreeWidget.html">
 *            Tree and List Widget Overview</ulink>
 *
 * A widget that displays any object that implements the #GtkTreeModel interface
 * in an icon bar.
 *
 * <example>
 * <title>
 *   Creating a new <structname>BlxoIconBar</structname> with a <structname>GtkListStore</structname>
 * </title>
 * <programlisting>
 * enum
 * {
 *   PIXBUF_COLUMN,
 *   STRING_COLUMN,
 *   N_COLUMNS,
 * };
 *
 * {
 *   GtkListStore *store;
 *   GtkWidget    *bar;
 *
 *   // make a new list store
 *   store = gtk_list_store_new (N_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING);
 *
 *   // fill the store with data
 *   fill_store (store);
 *
 *   // create the icon bar
 *   bar = blxo_icon_bar_new_with_model (GTK_TREE_MODEL (store));
 *   blxo_icon_bar_set_pixbuf_column (BLXO_ICON_BAR (bar), PIXBUF_COLUMN);
 *   blxo_icon_bar_set_text_column (BLXO_ICON_BAR (bar), TEXT_COLUMN);
 *   gtk_widget_show (bar);
 *
 *   // the icon bar keeps a reference on the store now
 *   g_object_unref (G_OBJECT (store));
 *
 *   // add the bar to your GUI now...
 * }
 * </programlisting>
 * </example>
 **/



#define MINIMUM_ICON_ITEM_WIDTH 32
#define ICON_TEXT_PADDING 1

#define BLXO_ICON_BAR_VALID_MODEL_AND_COLUMNS(obj) ((obj)->priv->model != NULL && \
                                                   (obj)->priv->pixbuf_column != -1 && \
                                                   (obj)->priv->text_column != -1)



typedef struct _BlxoIconBarItem BlxoIconBarItem;

enum
{
  PROP_0,
  PROP_ORIENTATION,
  PROP_PIXBUF_COLUMN,
  PROP_TEXT_COLUMN,
  PROP_MODEL,
  PROP_ACTIVE,
};

enum
{
  SELECTION_CHANGED,
  LAST_SIGNAL,
};



static void            blxo_icon_bar_destroy               (GtkObject        *object);
static void            blxo_icon_bar_finalize              (GObject          *object);
static void            blxo_icon_bar_get_property          (GObject          *object,
                                                           guint             prop_id,
                                                           GValue           *value,
                                                           GParamSpec       *pspec);
static void            blxo_icon_bar_set_property          (GObject          *object,
                                                           guint             prop_id,
                                                           const GValue     *value,
                                                           GParamSpec       *pspec);
static void            blxo_icon_bar_style_set             (GtkWidget        *widget,
                                                           GtkStyle         *previous_style);
static void            blxo_icon_bar_realize               (GtkWidget        *widget);
static void            blxo_icon_bar_unrealize             (GtkWidget        *widget);
static void            blxo_icon_bar_size_request          (GtkWidget        *widget,
                                                           GtkRequisition   *requisition);
static void            blxo_icon_bar_size_allocate         (GtkWidget        *widget,
                                                           GtkAllocation    *allocation);
static gboolean        blxo_icon_bar_expose                (GtkWidget        *widget,
                                                           GdkEventExpose   *expose);
static gboolean        blxo_icon_bar_leave                 (GtkWidget        *widget,
                                                           GdkEventCrossing *event);
static gboolean        blxo_icon_bar_motion                (GtkWidget        *widget,
                                                           GdkEventMotion   *event);
static gboolean        blxo_icon_bar_button_press          (GtkWidget        *widget,
                                                           GdkEventButton   *event);
static void            blxo_icon_bar_set_adjustments       (BlxoIconBar       *icon_bar,
                                                           GtkAdjustment    *hadj,
                                                           GtkAdjustment    *vadj);
static void            blxo_icon_bar_adjustment_changed    (GtkAdjustment    *adjustment,
                                                           BlxoIconBar       *icon_bar);
static void            blxo_icon_bar_invalidate            (BlxoIconBar       *icon_bar);
static BlxoIconBarItem *blxo_icon_bar_get_item_at_pos       (BlxoIconBar       *icon_bar,
                                                           gint              x,
                                                           gint              y);
static void            blxo_icon_bar_queue_draw_item       (BlxoIconBar       *icon_bar,
                                                           BlxoIconBarItem   *item);
static void            blxo_icon_bar_paint_item            (BlxoIconBar       *icon_bar,
                                                           BlxoIconBarItem   *item,
                                                           GdkRectangle     *area);
static void            blxo_icon_bar_calculate_item_size   (BlxoIconBar       *icon_bar,
                                                           BlxoIconBarItem   *item);
static void            blxo_icon_bar_update_item_text      (BlxoIconBar       *icon_bar,
                                                           BlxoIconBarItem   *item);
static GdkPixbuf      *blxo_icon_bar_get_item_icon         (BlxoIconBar       *icon_bar,
                                                           BlxoIconBarItem   *item);
static BlxoIconBarItem *blxo_icon_bar_item_new              (void);
static void            blxo_icon_bar_item_free             (BlxoIconBarItem   *item);
static void            blxo_icon_bar_item_invalidate       (BlxoIconBarItem   *item);
static void            blxo_icon_bar_build_items           (BlxoIconBar       *icon_bar);
static void            blxo_icon_bar_row_changed           (GtkTreeModel     *model,
                                                           GtkTreePath      *path,
                                                           GtkTreeIter      *iter,
                                                           BlxoIconBar       *icon_bar);
static void            blxo_icon_bar_row_inserted          (GtkTreeModel     *model,
                                                           GtkTreePath      *path,
                                                           GtkTreeIter      *iter,
                                                           BlxoIconBar       *icon_bar);
static void            blxo_icon_bar_row_deleted           (GtkTreeModel     *model,
                                                           GtkTreePath      *path,
                                                           GtkTreeIter      *iter,
                                                           BlxoIconBar       *icon_bar);
static void            blxo_icon_bar_rows_reordered        (GtkTreeModel     *model,
                                                           GtkTreePath      *path,
                                                           GtkTreeIter      *iter,
                                                           gint             *new_order,
                                                           BlxoIconBar       *icon_bar);



struct _BlxoIconBarItem
{
  GtkTreeIter iter;
  gint        index;

  gint        width;
  gint        height;

  gint        pixbuf_width;
  gint        pixbuf_height;

  gint        layout_width;
  gint        layout_height;
};

struct _BlxoIconBarPrivate
{
  GdkWindow      *bin_window;

  gint            width;
  gint            height;

  gint            pixbuf_column;
  gint            text_column;

  BlxoIconBarItem *active_item;
  BlxoIconBarItem *cursor_item;
  GList          *items;
  gint            item_width;
  gint            item_height;

  GtkAdjustment  *hadjustment;
  GtkAdjustment  *vadjustment;

  GtkOrientation  orientation;

  GtkTreeModel   *model;

  PangoLayout    *layout;
};



static guint icon_bar_signals[LAST_SIGNAL];



G_DEFINE_TYPE_WITH_PRIVATE (BlxoIconBar, blxo_icon_bar, GTK_TYPE_CONTAINER)



static void
blxo_icon_bar_class_init (BlxoIconBarClass *klass)
{
  GtkObjectClass *gtkobject_class;
  GtkWidgetClass *gtkwidget_class;
  GObjectClass   *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = blxo_icon_bar_finalize;
  gobject_class->get_property = blxo_icon_bar_get_property;
  gobject_class->set_property = blxo_icon_bar_set_property;

  gtkobject_class = GTK_OBJECT_CLASS (klass);
  gtkobject_class->destroy = blxo_icon_bar_destroy;

  gtkwidget_class = GTK_WIDGET_CLASS (klass);
  gtkwidget_class->style_set = blxo_icon_bar_style_set;
  gtkwidget_class->realize = blxo_icon_bar_realize;
  gtkwidget_class->unrealize = blxo_icon_bar_unrealize;
  gtkwidget_class->size_request = blxo_icon_bar_size_request;
  gtkwidget_class->size_allocate = blxo_icon_bar_size_allocate;
  gtkwidget_class->expose_event = blxo_icon_bar_expose;
  gtkwidget_class->leave_notify_event = blxo_icon_bar_leave;
  gtkwidget_class->motion_notify_event = blxo_icon_bar_motion;
  gtkwidget_class->button_press_event = blxo_icon_bar_button_press;

  klass->set_scroll_adjustments = blxo_icon_bar_set_adjustments;

  /**
   * BlxoIconBar:orientation:
   *
   * The orientation of the icon bar.
   *
   * Default value: %GTK_ORIENTATION_VERTICAL
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_ORIENTATION,
                                   g_param_spec_enum ("orientation",
                                                      _("Orientation"),
                                                      _("The orientation of the iconbar"),
                                                      GTK_TYPE_ORIENTATION,
                                                      GTK_ORIENTATION_VERTICAL,
                                                      BLXO_PARAM_READWRITE));

  /**
   * BlxoIconBar:pixbuf-column:
   *
   * The ::pixbuf-column property contains the number of the model column
   * containing the pixbufs which are displyed. The pixbuf column must be
   * of type #GDK_TYPE_PIXBUF. Setting this property to -1 turns off the
   * display of pixbufs.
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_PIXBUF_COLUMN,
                                   g_param_spec_int ("pixbuf-column",
                                                     _("Pixbuf column"),
                                                     _("Model column used to retrieve the icon pixbuf from"),
                                                     -1, G_MAXINT, -1,
                                                     BLXO_PARAM_READWRITE));

  /**
   * BlxoIconBar:text-column:
   *
   * The ::text-column property contains the number of the model column
   * containing the texts which are displayed. The text column must be
   * of type #G_TYPE_STRING. If this property is set to -1, no texts
   * are displayed.
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_TEXT_COLUMN,
                                   g_param_spec_int ("text-column",
                                                     _("Text column"),
                                                     _("Model column used to retrieve the text from"),
                                                     -1, G_MAXINT, -1,
                                                     BLXO_PARAM_READWRITE));

  /**
   * BlxoIconBar:model:
   *
   * The model for the icon bar.
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_MODEL,
                                   g_param_spec_object ("model",
                                                        _("Icon Bar Model"),
                                                        _("Model for the icon bar"),
                                                        GTK_TYPE_TREE_MODEL,
                                                        BLXO_PARAM_READWRITE));

  /**
   * BlxoIconBar:active:
   *
   * The item which is currently active.
   *
   * Allowed values: >= -1
   *
   * Default value: -1
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_ACTIVE,
                                   g_param_spec_int ("active",
                                                     _("Active"),
                                                     _("Active item index"),
                                                     -1, G_MAXINT, -1,
                                                     BLXO_PARAM_READWRITE));

  gtk_widget_class_install_style_property (gtkwidget_class,
                                           g_param_spec_boxed ("active-item-fill-color",
                                                               _("Active item fill color"),
                                                               _("Active item fill color"),
                                                               GDK_TYPE_COLOR,
                                                               BLXO_PARAM_READABLE));

  gtk_widget_class_install_style_property (gtkwidget_class,
                                           g_param_spec_boxed ("active-item-border-color",
                                                               _("Active item border color"),
                                                               _("Active item border color"),
                                                               GDK_TYPE_COLOR,
                                                               BLXO_PARAM_READABLE));

  gtk_widget_class_install_style_property (gtkwidget_class,
                                           g_param_spec_boxed ("active-item-text-color",
                                                               _("Active item text color"),
                                                               _("Active item text color"),
                                                               GDK_TYPE_COLOR,
                                                               BLXO_PARAM_READABLE));

  gtk_widget_class_install_style_property (gtkwidget_class,
                                           g_param_spec_boxed ("cursor-item-fill-color",
                                                               _("Cursor item fill color"),
                                                               _("Cursor item fill color"),
                                                               GDK_TYPE_COLOR,
                                                               BLXO_PARAM_READABLE));

  gtk_widget_class_install_style_property (gtkwidget_class,
                                           g_param_spec_boxed ("cursor-item-border-color",
                                                               _("Cursor item border color"),
                                                               _("Cursor item border color"),
                                                               GDK_TYPE_COLOR,
                                                               BLXO_PARAM_READABLE));

  gtk_widget_class_install_style_property (gtkwidget_class,
                                           g_param_spec_boxed ("cursor-item-text-color",
                                                               _("Cursor item text color"),
                                                               _("Cursor item text color"),
                                                               GDK_TYPE_COLOR,
                                                               BLXO_PARAM_READABLE));

  /**
   * BlxoIconBar::set-scroll-adjustments:
   * @icon_bar    : The #BlxoIconBar.
   * @hadjustment : The horizontal adjustment.
   * @vadjustment : The vertical adjustment.
   *
   * Used internally to make the #BlxoIconBar scrollable.
   **/
  gtkwidget_class->set_scroll_adjustments_signal =
    g_signal_new (I_("set-scroll-adjustments"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (BlxoIconBarClass, set_scroll_adjustments),
                  NULL, NULL,
                  _blxo_marshal_VOID__OBJECT_OBJECT,
                  G_TYPE_NONE, 2,
                  GTK_TYPE_ADJUSTMENT,
                  GTK_TYPE_ADJUSTMENT);

  /**
   * BlxoIconBar::selection-changed:
   * @icon_bar  : The #BlxoIconBar.
   *
   * This signal is emitted whenever the currently selected icon
   * changes.
   **/
  icon_bar_signals[SELECTION_CHANGED] =
    g_signal_new (I_("selection-changed"),
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_FIRST,
                  G_STRUCT_OFFSET (BlxoIconBarClass, selection_changed),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);
}



static void
blxo_icon_bar_init (BlxoIconBar *icon_bar)
{
  icon_bar->priv = blxo_icon_bar_get_instance_private (icon_bar);

  icon_bar->priv->orientation = GTK_ORIENTATION_VERTICAL;
  icon_bar->priv->pixbuf_column = -1;
  icon_bar->priv->text_column = -1;

  icon_bar->priv->layout = gtk_widget_create_pango_layout (GTK_WIDGET (icon_bar), NULL);
  pango_layout_set_width (icon_bar->priv->layout, -1);

  gtk_widget_set_can_focus (GTK_WIDGET (icon_bar), FALSE);

  blxo_icon_bar_set_adjustments (icon_bar, NULL, NULL);
}



static void
blxo_icon_bar_destroy (GtkObject *object)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (object);

  blxo_icon_bar_set_model (icon_bar, NULL);

  (*GTK_OBJECT_CLASS (blxo_icon_bar_parent_class)->destroy) (object);
}



static void
blxo_icon_bar_finalize (GObject *object)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (object);

  g_object_unref (G_OBJECT (icon_bar->priv->layout));

  (*G_OBJECT_CLASS (blxo_icon_bar_parent_class)->finalize) (object);
}



static void
blxo_icon_bar_get_property (GObject          *object,
                           guint             prop_id,
                           GValue           *value,
                           GParamSpec       *pspec)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (object);

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      g_value_set_enum (value, icon_bar->priv->orientation);
      break;

    case PROP_PIXBUF_COLUMN:
      g_value_set_int (value, icon_bar->priv->pixbuf_column);
      break;

    case PROP_TEXT_COLUMN:
      g_value_set_int (value, icon_bar->priv->text_column);
      break;

    case PROP_MODEL:
      g_value_set_object (value, icon_bar->priv->model);
      break;

    case PROP_ACTIVE:
      g_value_set_int (value, blxo_icon_bar_get_active (icon_bar));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
blxo_icon_bar_set_property (GObject          *object,
                           guint             prop_id,
                           const GValue     *value,
                           GParamSpec       *pspec)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (object);

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      blxo_icon_bar_set_orientation (icon_bar, g_value_get_enum (value));
      break;

    case PROP_PIXBUF_COLUMN:
      blxo_icon_bar_set_pixbuf_column (icon_bar, g_value_get_int (value));
      break;

    case PROP_TEXT_COLUMN:
      blxo_icon_bar_set_text_column (icon_bar, g_value_get_int (value));
      break;

    case PROP_MODEL:
      blxo_icon_bar_set_model (icon_bar, g_value_get_object (value));
      break;

    case PROP_ACTIVE:
      blxo_icon_bar_set_active (icon_bar, g_value_get_int (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
blxo_icon_bar_style_set (GtkWidget *widget,
                        GtkStyle  *previous_style)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (widget);

  (*GTK_WIDGET_CLASS (blxo_icon_bar_parent_class)->style_set) (widget, previous_style);

  if (gtk_widget_get_realized (widget))
    {
      gdk_window_set_background (icon_bar->priv->bin_window,
                                 &gtk_widget_get_style (widget)->base[gtk_widget_get_state (widget)]);
    }
}



static void
blxo_icon_bar_realize (GtkWidget *widget)
{
  GdkWindowAttr attributes;
  BlxoIconBar   *icon_bar = BLXO_ICON_BAR (widget);
  gint          attributes_mask;
  GtkAllocation allocation;

  gtk_widget_set_realized (widget, TRUE);
  gtk_widget_get_allocation (widget, &allocation);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = allocation.x;
  attributes.y = allocation.y;
  attributes.width = allocation.width;
  attributes.height = allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);
  attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK;
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  gtk_widget_set_window (widget, gdk_window_new (gtk_widget_get_parent_window (widget),
                                                 &attributes, attributes_mask));
  gdk_window_set_user_data (gtk_widget_get_window (widget), widget);

  attributes.x = 0;
  attributes.y = 0;
  attributes.width = MAX (icon_bar->priv->width, allocation.width);
  attributes.height = MAX (icon_bar->priv->height, allocation.height);
  attributes.event_mask = (GDK_SCROLL_MASK
                           | GDK_EXPOSURE_MASK
                           | GDK_LEAVE_NOTIFY_MASK
                           | GDK_POINTER_MOTION_MASK
                           | GDK_BUTTON_PRESS_MASK
                           | GDK_BUTTON_RELEASE_MASK
                           | GDK_KEY_PRESS_MASK
                           | GDK_KEY_RELEASE_MASK)
                          | gtk_widget_get_events (widget);
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  icon_bar->priv->bin_window = gdk_window_new (gtk_widget_get_window (widget),
                                               &attributes, attributes_mask);
  gdk_window_set_user_data (icon_bar->priv->bin_window, widget);

  gtk_widget_set_style (widget, gtk_style_attach (gtk_widget_get_style (widget), gtk_widget_get_window (widget)));
  gdk_window_set_background (gtk_widget_get_window (widget), &gtk_widget_get_style (widget)->base[gtk_widget_get_state (widget)]);
  gdk_window_set_background (icon_bar->priv->bin_window, &gtk_widget_get_style (widget)->base[gtk_widget_get_state (widget)]);
  gdk_window_show (icon_bar->priv->bin_window);
}



static void
blxo_icon_bar_unrealize (GtkWidget *widget)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (widget);

  gdk_window_set_user_data (icon_bar->priv->bin_window, NULL);
  gdk_window_destroy (icon_bar->priv->bin_window);
  icon_bar->priv->bin_window = NULL;

  /* GtkWidget::unrealize destroys children and widget->window */
  (*GTK_WIDGET_CLASS (blxo_icon_bar_parent_class)->unrealize) (widget);
}



static void
blxo_icon_bar_size_request (GtkWidget      *widget,
                           GtkRequisition *requisition)
{
  BlxoIconBarItem *item;
  BlxoIconBar     *icon_bar = BLXO_ICON_BAR (widget);
  GList          *lp;
  gint            n = 0;
  gint            max_width = 0;
  gint            max_height = 0;

  if (!BLXO_ICON_BAR_VALID_MODEL_AND_COLUMNS (icon_bar)
      || icon_bar->priv->items == NULL)
    {
      icon_bar->priv->width = requisition->width = 0;
      icon_bar->priv->height = requisition->height = 0;
      return;
    }

  /* calculate max item size */
  for (lp = icon_bar->priv->items; lp != NULL; ++n, lp = lp->next)
    {
      item = lp->data;
      blxo_icon_bar_calculate_item_size (icon_bar, item);

      if (item->width > max_width)
        max_width = item->width;
      if (item->height > max_height)
        max_height = item->height;
    }

  icon_bar->priv->item_width = max_width;
  icon_bar->priv->item_height = max_height;

  if (icon_bar->priv->orientation == GTK_ORIENTATION_VERTICAL)
    {
      icon_bar->priv->width = requisition->width = icon_bar->priv->item_width;
      icon_bar->priv->height = requisition->height = icon_bar->priv->item_height * n;
    }
  else
    {
      icon_bar->priv->width = requisition->width = icon_bar->priv->item_width * n;
      icon_bar->priv->height = requisition->height = icon_bar->priv->item_height;
    }
}



static void
blxo_icon_bar_size_allocate (GtkWidget     *widget,
                            GtkAllocation *allocation)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (widget);

  gtk_widget_set_allocation (widget, allocation);

  if (gtk_widget_get_realized (widget))
    {
      gdk_window_move_resize (gtk_widget_get_window (widget),
                              allocation->x,
                              allocation->y,
                              allocation->width,
                              allocation->height);
      gdk_window_resize (icon_bar->priv->bin_window,
                         MAX (icon_bar->priv->width, allocation->width),
                         MAX (icon_bar->priv->height, allocation->height));
    }

  gtk_adjustment_set_page_size (icon_bar->priv->hadjustment, allocation->width);
  gtk_adjustment_set_page_increment (icon_bar->priv->hadjustment, allocation->width * 0.9);
  gtk_adjustment_set_step_increment (icon_bar->priv->hadjustment, allocation->width * 0.1);
  gtk_adjustment_set_lower (icon_bar->priv->hadjustment, 0);
  gtk_adjustment_set_upper (icon_bar->priv->hadjustment, MAX (allocation->width, icon_bar->priv->width));
  gtk_adjustment_changed (icon_bar->priv->hadjustment);

  gtk_adjustment_set_page_size (icon_bar->priv->vadjustment, allocation->height);
  gtk_adjustment_set_page_increment (icon_bar->priv->vadjustment, allocation->height * 0.9);
  gtk_adjustment_set_step_increment (icon_bar->priv->vadjustment, allocation->height * 0.1);
  gtk_adjustment_set_lower (icon_bar->priv->vadjustment, 0);
  gtk_adjustment_set_upper (icon_bar->priv->vadjustment, MAX (allocation->height, icon_bar->priv->height));
  gtk_adjustment_changed (icon_bar->priv->vadjustment);

  if (icon_bar->priv->orientation == GTK_ORIENTATION_VERTICAL)
    {
      icon_bar->priv->width = MAX (icon_bar->priv->width, allocation->width);
      icon_bar->priv->item_width = icon_bar->priv->width;
    }
  else
    {
      icon_bar->priv->height = MAX (icon_bar->priv->height, allocation->height);
      icon_bar->priv->item_height = icon_bar->priv->height;
    }
}



static gboolean
blxo_icon_bar_expose (GtkWidget      *widget,
                     GdkEventExpose *expose)
{
  BlxoIconBarItem *item;
  GdkRectangle    area;
  BlxoIconBar     *icon_bar = BLXO_ICON_BAR (widget);
  GList          *lp;

  if (expose->window != icon_bar->priv->bin_window)
    return FALSE;

  for (lp = icon_bar->priv->items; lp != NULL; lp = lp->next)
    {
      item = lp->data;

      if (icon_bar->priv->orientation == GTK_ORIENTATION_VERTICAL)
        {
          area.x = 0;
          area.y = item->index * icon_bar->priv->item_height;
        }
      else
        {
          area.x = item->index * icon_bar->priv->item_width;
          area.y = 0;
        }

      area.width = icon_bar->priv->item_width;
      area.height = icon_bar->priv->item_height;

      if (gdk_region_rect_in (expose->region, &area) != GDK_OVERLAP_RECTANGLE_OUT)
        blxo_icon_bar_paint_item (icon_bar, item, &expose->area);
    }

  return TRUE;
}



static gboolean
blxo_icon_bar_leave (GtkWidget        *widget,
                    GdkEventCrossing *event)
{
  BlxoIconBar *icon_bar = BLXO_ICON_BAR (widget);

  if (icon_bar->priv->cursor_item != NULL)
    {
      blxo_icon_bar_queue_draw_item (icon_bar, icon_bar->priv->cursor_item);
      icon_bar->priv->cursor_item = NULL;
    }

  return FALSE;
}



static gboolean
blxo_icon_bar_motion (GtkWidget      *widget,
                     GdkEventMotion *event)
{
  BlxoIconBarItem *item;
  BlxoIconBar     *icon_bar = BLXO_ICON_BAR (widget);

  item = blxo_icon_bar_get_item_at_pos (icon_bar, event->x, event->y);
  if (item != NULL && icon_bar->priv->cursor_item != item)
    {
      if (icon_bar->priv->cursor_item != NULL)
        blxo_icon_bar_queue_draw_item (icon_bar, icon_bar->priv->cursor_item);
      icon_bar->priv->cursor_item = item;
      blxo_icon_bar_queue_draw_item (icon_bar, item);
    }
  else if (icon_bar->priv->cursor_item != NULL
        && icon_bar->priv->cursor_item != item)
    {
      blxo_icon_bar_queue_draw_item (icon_bar, icon_bar->priv->cursor_item);
      icon_bar->priv->cursor_item = NULL;
    }

  return TRUE;
}



static gboolean
blxo_icon_bar_button_press (GtkWidget      *widget,
                           GdkEventButton *event)
{
  BlxoIconBarItem  *item;
  BlxoIconBar      *icon_bar = BLXO_ICON_BAR (widget);

  if (!gtk_widget_has_focus (widget))
    gtk_widget_grab_focus (widget);

  if (event->button == 1 && event->type == GDK_BUTTON_PRESS)
    {
      item = blxo_icon_bar_get_item_at_pos (icon_bar, event->x, event->y);
      if (G_LIKELY (item != NULL && item != icon_bar->priv->active_item))
        blxo_icon_bar_set_active (icon_bar, item->index);
    }

  return TRUE;
}



static void
blxo_icon_bar_set_adjustments (BlxoIconBar    *icon_bar,
                              GtkAdjustment *hadj,
                              GtkAdjustment *vadj)
{
  gboolean need_adjust = FALSE;

  if (hadj != NULL)
    g_return_if_fail (GTK_IS_ADJUSTMENT (hadj));
  else
    hadj = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

  if (vadj != NULL)
    g_return_if_fail (GTK_IS_ADJUSTMENT (vadj));
  else
    vadj = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

  if (icon_bar->priv->hadjustment && (icon_bar->priv->hadjustment != hadj))
    {
      g_signal_handlers_disconnect_matched (icon_bar->priv->hadjustment, G_SIGNAL_MATCH_DATA,
                                            0, 0, NULL, NULL, icon_bar);
      g_object_unref (icon_bar->priv->hadjustment);
    }

  if (icon_bar->priv->vadjustment && (icon_bar->priv->vadjustment != vadj))
    {
      g_signal_handlers_disconnect_matched (icon_bar->priv->vadjustment, G_SIGNAL_MATCH_DATA,
                                            0, 0, NULL, NULL, icon_bar);
      g_object_unref (icon_bar->priv->vadjustment);
    }

  if (icon_bar->priv->hadjustment != hadj)
    {
      icon_bar->priv->hadjustment = hadj;
      g_object_ref (icon_bar->priv->hadjustment);
      gtk_object_sink (GTK_OBJECT (icon_bar->priv->hadjustment));

      g_signal_connect (icon_bar->priv->hadjustment, "value_changed",
                        G_CALLBACK (blxo_icon_bar_adjustment_changed), icon_bar);
      need_adjust = TRUE;
    }

  if (icon_bar->priv->vadjustment != vadj)
    {
      icon_bar->priv->vadjustment = vadj;
      g_object_ref (icon_bar->priv->vadjustment);
      gtk_object_sink (GTK_OBJECT (icon_bar->priv->vadjustment));

      g_signal_connect (icon_bar->priv->vadjustment, "value_changed",
                        G_CALLBACK (blxo_icon_bar_adjustment_changed), icon_bar);
      need_adjust = TRUE;
    }

  if (need_adjust)
    blxo_icon_bar_adjustment_changed (NULL, icon_bar);
}



static void
blxo_icon_bar_adjustment_changed (GtkAdjustment *adjustment,
                                 BlxoIconBar    *icon_bar)
{
  if (gtk_widget_get_realized (GTK_WIDGET (icon_bar)))
    {
      gdk_window_move (icon_bar->priv->bin_window,
                       - gtk_adjustment_get_value (icon_bar->priv->hadjustment),
                       - gtk_adjustment_get_value (icon_bar->priv->vadjustment));

      gdk_window_process_updates (icon_bar->priv->bin_window, TRUE);
    }
}



static void
blxo_icon_bar_invalidate (BlxoIconBar *icon_bar)
{
  g_list_foreach (icon_bar->priv->items, (GFunc) (void (*)(void)) blxo_icon_bar_item_invalidate, NULL);
  gtk_widget_queue_resize (GTK_WIDGET (icon_bar));
}



static BlxoIconBarItem*
blxo_icon_bar_get_item_at_pos (BlxoIconBar *icon_bar,
                              gint        x,
                              gint        y)
{
  GList *lp;

  if (G_UNLIKELY (icon_bar->priv->item_height == 0))
    return NULL;

  if (icon_bar->priv->orientation == GTK_ORIENTATION_VERTICAL)
    lp = g_list_nth (icon_bar->priv->items, y / icon_bar->priv->item_height);
  else
    lp = g_list_nth (icon_bar->priv->items, x / icon_bar->priv->item_width);

  return (lp != NULL) ? lp->data : NULL;
}



static void
blxo_icon_bar_queue_draw_item (BlxoIconBar     *icon_bar,
                              BlxoIconBarItem *item)
{
  GdkRectangle area;

  if (gtk_widget_get_realized (GTK_WIDGET (icon_bar)))
    {
      if (icon_bar->priv->orientation == GTK_ORIENTATION_VERTICAL)
        {
          area.x = 0;
          area.y = icon_bar->priv->item_height * item->index;
        }
      else
        {
          area.x = icon_bar->priv->item_width * item->index;
          area.y = 0;
        }

      area.width = icon_bar->priv->item_width;
      area.height = icon_bar->priv->item_height;

      gdk_window_invalidate_rect (icon_bar->priv->bin_window, &area, TRUE);
    }
}



static void
blxo_icon_bar_paint_item (BlxoIconBar     *icon_bar,
                         BlxoIconBarItem *item,
                         GdkRectangle   *area)
{
  GdkPixbuf    *pixbuf;
  GdkColor     *border_color;
  GdkColor     *fill_color;
  GdkColor     *text_color;
  GdkGC        *gc;
  gint          focus_width;
  gint          focus_pad;
  gint          x, y;
  gint          px, py;
  gint          lx, ly;

  if (!BLXO_ICON_BAR_VALID_MODEL_AND_COLUMNS (icon_bar))
    return;

  gtk_widget_style_get (GTK_WIDGET (icon_bar),
                        "focus-line-width", &focus_width,
                        "focus-padding", &focus_pad,
                        NULL);

  /* calculate pixbuf/layout location */
  if (icon_bar->priv->orientation == GTK_ORIENTATION_VERTICAL)
    {
      x = 0;
      y = icon_bar->priv->item_height * item->index;

      px = (icon_bar->priv->item_width - item->pixbuf_width) / 2 + focus_pad + focus_width;
      py = (icon_bar->priv->item_height - (item->pixbuf_height + item->layout_height + ICON_TEXT_PADDING)) / 2
         + icon_bar->priv->item_height * item->index + focus_pad + focus_width;
      lx = (icon_bar->priv->item_width - (item->layout_width + ICON_TEXT_PADDING)) / 2 + focus_pad;
      ly = py + item->pixbuf_height + ICON_TEXT_PADDING;
    }
  else
    {
      x = icon_bar->priv->item_width * item->index;
      y = 0;

      px = (icon_bar->priv->item_width - item->pixbuf_width) / 2 + focus_pad + focus_width
         + icon_bar->priv->item_width * item->index;
      py = (icon_bar->priv->item_height - (item->pixbuf_height + item->layout_height)) / 2
          + focus_pad + focus_width;
      lx = (icon_bar->priv->item_width - (item->layout_width)) / 2 + x;
      ly = py + item->pixbuf_height + ICON_TEXT_PADDING;
    }

  if (icon_bar->priv->active_item == item)
    {
      gtk_widget_style_get (GTK_WIDGET (icon_bar),
                            "active-item-fill-color", &fill_color,
                            "active-item-border-color", &border_color,
                            NULL);

      if (fill_color == NULL)
        {
          fill_color = gdk_color_copy (&gtk_widget_get_style (GTK_WIDGET (icon_bar))->base[GTK_STATE_SELECTED]);
          gdk_color_parse ("#c1d2ee", fill_color);
        }

      if (border_color == NULL)
        {
          border_color = gdk_color_copy (&gtk_widget_get_style (GTK_WIDGET (icon_bar))->base[GTK_STATE_SELECTED]);
          gdk_color_parse ("#316ac5", border_color);
        }

      gc = gdk_gc_new (icon_bar->priv->bin_window);
      gdk_gc_set_clip_rectangle (gc, area);
      gdk_gc_set_rgb_fg_color (gc, fill_color);
      gdk_draw_rectangle (icon_bar->priv->bin_window, gc, TRUE,
                          x + focus_pad + focus_width,
                          y + focus_pad + focus_width,
                          icon_bar->priv->item_width - 2 * (focus_width + focus_pad) + 1,
                          icon_bar->priv->item_height - 2 * (focus_width + focus_pad) + 1);
      gdk_gc_set_rgb_fg_color (gc, border_color);
      gdk_gc_set_line_attributes (gc, focus_width, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
      gdk_draw_rectangle (icon_bar->priv->bin_window, gc, FALSE,
                          x + focus_pad + focus_width / 2,
                          y + focus_pad + focus_width / 2,
                          icon_bar->priv->item_width - (2 * focus_pad + focus_width) + 1,
                          icon_bar->priv->item_height - (2 * focus_pad + focus_width) + 1);
      gdk_color_free (border_color);
      gdk_color_free (fill_color);
      g_object_unref (gc);
    }
  else if (icon_bar->priv->cursor_item == item)
    {
      gtk_widget_style_get (GTK_WIDGET (icon_bar),
                            "cursor-item-fill-color", &fill_color,
                            "cursor-item-border-color", &border_color,
                            NULL);

      if (fill_color == NULL)
        {
          fill_color = gdk_color_copy (&gtk_widget_get_style (GTK_WIDGET (icon_bar))->base[GTK_STATE_SELECTED]);
          gdk_color_parse ("#e0e8f6", fill_color);
        }

      if (border_color == NULL)
        {
          border_color = gdk_color_copy (&gtk_widget_get_style (GTK_WIDGET (icon_bar))->base[GTK_STATE_SELECTED]);
          gdk_color_parse ("#98b4e2", border_color);
        }

      gc = gdk_gc_new (icon_bar->priv->bin_window);
      gdk_gc_set_clip_rectangle (gc, area);
      gdk_gc_set_rgb_fg_color (gc, fill_color);
      gdk_draw_rectangle (icon_bar->priv->bin_window, gc, TRUE,
                          x + focus_pad + focus_width,
                          y + focus_pad + focus_width,
                          icon_bar->priv->item_width - 2 * (focus_width + focus_pad) + 1,
                          icon_bar->priv->item_height - 2 * (focus_width + focus_pad) + 1);
      gdk_gc_set_rgb_fg_color (gc, border_color);
      gdk_gc_set_line_attributes (gc, focus_width, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
      gdk_draw_rectangle (icon_bar->priv->bin_window, gc, FALSE,
                          x + focus_pad + focus_width / 2,
                          y + focus_pad + focus_width / 2,
                          icon_bar->priv->item_width - (2 * focus_pad + focus_width) + 1,
                          icon_bar->priv->item_height - (2 * focus_pad + focus_width) + 1);
      gdk_color_free (border_color);
      gdk_color_free (fill_color);
      g_object_unref (gc);
    }

  if (icon_bar->priv->pixbuf_column != -1)
    {
      pixbuf = blxo_icon_bar_get_item_icon (icon_bar, item);
      if (G_LIKELY (pixbuf != NULL))
        {
          gdk_draw_pixbuf (icon_bar->priv->bin_window, NULL, pixbuf, 0, 0,
                           px, py, item->pixbuf_width, item->pixbuf_height,
                           GDK_RGB_DITHER_NORMAL, item->pixbuf_width,
                           item->pixbuf_height);
          g_object_unref (pixbuf);
        }
    }

  if (icon_bar->priv->text_column != -1)
    {
      blxo_icon_bar_update_item_text (icon_bar, item);

      if (icon_bar->priv->active_item == item)
        {
          gtk_widget_style_get (GTK_WIDGET (icon_bar),
                                "active-item-text-color", &text_color,
                                NULL);

          if (text_color == NULL)
            {
              text_color = gdk_color_copy (&gtk_widget_get_style (GTK_WIDGET (icon_bar))->base[GTK_STATE_SELECTED]);
              gdk_color_parse ("#000000", text_color);
            }

          gc = gdk_gc_new (GDK_DRAWABLE (icon_bar->priv->bin_window));
          gdk_gc_copy (gc, gtk_widget_get_style (GTK_WIDGET (icon_bar))->text_gc[GTK_STATE_SELECTED]);
          gdk_gc_set_clip_rectangle (gc, area);
          gdk_gc_set_rgb_fg_color (gc, text_color);
          gdk_draw_layout (icon_bar->priv->bin_window, gc, lx, ly, icon_bar->priv->layout);
          g_object_unref (G_OBJECT (gc));
          gdk_color_free (text_color);
        }
      else if (icon_bar->priv->cursor_item == item)
        {
          gtk_widget_style_get (GTK_WIDGET (icon_bar),
                                "cursor-item-text-color", &text_color,
                                NULL);

          if (text_color == NULL)
            {
              text_color = gdk_color_copy (&gtk_widget_get_style (GTK_WIDGET (icon_bar))->base[GTK_STATE_SELECTED]);
              gdk_color_parse ("#000000", text_color);
            }

          gc = gdk_gc_new (GDK_DRAWABLE (icon_bar->priv->bin_window));
          gdk_gc_copy (gc, gtk_widget_get_style (GTK_WIDGET (icon_bar))->text_gc[GTK_STATE_SELECTED]);
          gdk_gc_set_clip_rectangle (gc, area);
          gdk_gc_set_rgb_fg_color (gc, text_color);
          gdk_draw_layout (icon_bar->priv->bin_window, gc, lx, ly, icon_bar->priv->layout);
          g_object_unref (G_OBJECT (gc));
          gdk_color_free (text_color);
        }
      else
        {
          gtk_paint_layout (gtk_widget_get_style (GTK_WIDGET (icon_bar)),
                            icon_bar->priv->bin_window,
                            GTK_STATE_NORMAL, TRUE, area,
                            GTK_WIDGET (icon_bar), "icon_bar",
                            lx, ly, icon_bar->priv->layout);
        }
    }
}



static void
blxo_icon_bar_calculate_item_size (BlxoIconBar      *icon_bar,
                                  BlxoIconBarItem  *item)
{
  GdkPixbuf *pixbuf;
  gint       focus_width;
  gint       focus_pad;

  if (G_LIKELY (item->width != -1 && item->height != -1))
    return;

  gtk_widget_style_get (GTK_WIDGET (icon_bar),
                        "focus-line-width", &focus_width,
                        "focus-padding", &focus_pad,
                        NULL);

  if (icon_bar->priv->pixbuf_column != -1)
    {
      pixbuf = blxo_icon_bar_get_item_icon (icon_bar, item);
      if (G_LIKELY (pixbuf != NULL))
        {
          item->pixbuf_width = gdk_pixbuf_get_width (pixbuf);
          item->pixbuf_height = gdk_pixbuf_get_height (pixbuf);
          g_object_unref (G_OBJECT (pixbuf));
        }
      else
        {
          item->pixbuf_width = 0;
          item->pixbuf_height = 0;
        }
    }
  else
    {
      item->pixbuf_width = 0;
      item->pixbuf_height = 0;
    }

  if (icon_bar->priv->text_column != -1)
    {
      blxo_icon_bar_update_item_text (icon_bar, item);
      pango_layout_get_pixel_size (icon_bar->priv->layout,
                                   &item->layout_width,
                                   &item->layout_height);
    }
  else
    {
      item->layout_width = 0;
      item->layout_height = 0;
    }

  item->width = MAX (item->layout_width, item->pixbuf_width) + 2 * ICON_TEXT_PADDING
              + 2 * (focus_width + focus_pad);
  item->height = item->layout_height + 2 * (focus_width + focus_pad + ICON_TEXT_PADDING)
               + item->pixbuf_height;
}



static void
blxo_icon_bar_update_item_text (BlxoIconBar     *icon_bar,
                               BlxoIconBarItem *item)
{
  GtkTreePath *path;
  GtkTreeIter  iter;
  gchar       *text;

  if ((gtk_tree_model_get_flags (icon_bar->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST) == 0)
    {
      path = gtk_tree_path_new_from_indices (item->index, -1);
      gtk_tree_model_get_iter (icon_bar->priv->model, &iter, path);
      gtk_tree_path_free (path);
    }
  else
    {
      iter = item->iter;
    }

  gtk_tree_model_get (icon_bar->priv->model, &iter,
                      icon_bar->priv->text_column, &text,
                      -1);
  pango_layout_set_text (icon_bar->priv->layout, text, -1);
  g_free (text);
}



static GdkPixbuf*
blxo_icon_bar_get_item_icon (BlxoIconBar      *icon_bar,
                            BlxoIconBarItem  *item)
{
  GtkTreePath *path;
  GtkTreeIter  iter;
  GdkPixbuf   *pixbuf;

  if ((gtk_tree_model_get_flags (icon_bar->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST) == 0)
    {
      path = gtk_tree_path_new_from_indices (item->index, -1);
      gtk_tree_model_get_iter (icon_bar->priv->model, &iter, path);
      gtk_tree_path_free (path);
    }
  else
    {
      iter = item->iter;
    }

  gtk_tree_model_get (icon_bar->priv->model, &iter,
                      icon_bar->priv->pixbuf_column, &pixbuf,
                      -1);

  return pixbuf;
}



static BlxoIconBarItem*
blxo_icon_bar_item_new (void)
{
  BlxoIconBarItem *item;

  item = g_slice_new0 (BlxoIconBarItem);
  item->width = -1;
  item->height = -1;

  return item;
}



static void
blxo_icon_bar_item_free (BlxoIconBarItem *item)
{
  g_slice_free (BlxoIconBarItem, item);
}



static void
blxo_icon_bar_item_invalidate (BlxoIconBarItem *item)
{
  item->width = -1;
  item->height = -1;
}



static void
blxo_icon_bar_build_items (BlxoIconBar *icon_bar)
{
  BlxoIconBarItem *item;
  GtkTreeIter     iter;
  GList          *items = NULL;
  gint            i = 0;

  if (!gtk_tree_model_get_iter_first (icon_bar->priv->model, &iter))
    return;

  do
    {
      item = blxo_icon_bar_item_new ();
      item->iter = iter;
      item->index = i++;

      items = g_list_prepend (items, item);
    }
  while (gtk_tree_model_iter_next (icon_bar->priv->model, &iter));

  icon_bar->priv->items = g_list_reverse (items);
}



static void
blxo_icon_bar_row_changed (GtkTreeModel *model,
                          GtkTreePath  *path,
                          GtkTreeIter  *iter,
                          BlxoIconBar   *icon_bar)
{
  BlxoIconBarItem  *item;
  gint             idx;

  idx = gtk_tree_path_get_indices (path)[0];
  item = g_list_nth (icon_bar->priv->items, idx)->data;
  blxo_icon_bar_item_invalidate (item);
  gtk_widget_queue_resize (GTK_WIDGET (icon_bar));
}



static void
blxo_icon_bar_row_inserted (GtkTreeModel *model,
                           GtkTreePath  *path,
                           GtkTreeIter  *iter,
                           BlxoIconBar   *icon_bar)
{
  BlxoIconBarItem  *item;
  GList           *lp;
  gint             idx;

  idx = gtk_tree_path_get_indices (path)[0];
  item = blxo_icon_bar_item_new ();

  if ((gtk_tree_model_get_flags (icon_bar->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST) != 0)
    item->iter = *iter;
  item->index = idx;

  icon_bar->priv->items = g_list_insert (icon_bar->priv->items, item, idx);

  for (lp = g_list_nth (icon_bar->priv->items, idx + 1); lp != NULL; lp = lp->next)
    {
      item = lp->data;
      item->index++;
    }

  gtk_widget_queue_resize (GTK_WIDGET (icon_bar));
}



static void
blxo_icon_bar_row_deleted (GtkTreeModel *model,
                          GtkTreePath  *path,
                          GtkTreeIter  *iter,
                          BlxoIconBar   *icon_bar)
{
  BlxoIconBarItem *item;
  gboolean        active = FALSE;
  GList          *lnext;
  GList          *lp;
  gint            idx;

  idx = gtk_tree_path_get_indices (path)[0];
  lp = g_list_nth (icon_bar->priv->items, idx);
  item = lp->data;

  if (item == icon_bar->priv->active_item)
    {
      icon_bar->priv->active_item = NULL;
      active = TRUE;
    }

  if (item == icon_bar->priv->cursor_item)
    icon_bar->priv->cursor_item = NULL;

  blxo_icon_bar_item_free (item);

  for (lnext = lp->next; lnext != NULL; lnext = lnext->next)
    {
      item = lnext->data;
      item->index--;
    }

  icon_bar->priv->items = g_list_delete_link (icon_bar->priv->items, lp);

  if (active && icon_bar->priv->items != NULL)
    icon_bar->priv->active_item = icon_bar->priv->items->data;

  gtk_widget_queue_resize (GTK_WIDGET (icon_bar));

  if (active)
    blxo_icon_bar_set_active (icon_bar, -1);
}



static void
blxo_icon_bar_rows_reordered (GtkTreeModel *model,
                             GtkTreePath  *path,
                             GtkTreeIter  *iter,
                             gint         *new_order,
                             BlxoIconBar   *icon_bar)
{
  BlxoIconBarItem **item_array;
  GList           *items = NULL;
  GList           *lp;
  gint            *inverted_order;
  gint             length;
  gint             i;

  length = gtk_tree_model_iter_n_children (model, NULL);
  inverted_order = g_newa (gint, length);

  /* invert the array */
  for (i = 0; i < length; ++i)
    inverted_order[new_order[i]] = i;

  item_array = g_newa (BlxoIconBarItem *, length);
  for (i = 0, lp = icon_bar->priv->items; lp != NULL; ++i, lp = lp->next)
    item_array[inverted_order[i]] = lp->data;

  for (i = 0; i < length; ++i)
    {
      item_array[i]->index = i;
      items = g_list_append (items, item_array[i]);
    }

  g_list_free (icon_bar->priv->items);
  icon_bar->priv->items = g_list_reverse (items);

  gtk_widget_queue_draw (GTK_WIDGET (icon_bar));
}



/**
 * blxo_icon_bar_new:
 *
 * Creates a new #BlxoIconBar without model.
 *
 * Returns: a newly allocated #BlxoIconBar.
 **/
GtkWidget*
blxo_icon_bar_new (void)
{
  return g_object_new (BLXO_TYPE_ICON_BAR, NULL);
}



/**
 * blxo_icon_bar_new_with_model:
 * @model : A #GtkTreeModel.
 *
 * Creates a new #BlxoIconBar and associates it with
 * @model.
 *
 * Returns: a newly allocated #BlxoIconBar, which is associated with @model.
 **/
GtkWidget*
blxo_icon_bar_new_with_model (GtkTreeModel *model)
{
  g_return_val_if_fail (GTK_IS_TREE_MODEL (model), NULL);

  return g_object_new (BLXO_TYPE_ICON_BAR,
                       "model", model,
                       NULL);
}



/**
 * blxo_icon_bar_get_model:
 * @icon_bar  : A #BlxoIconBar.
 *
 * Returns the model the #BlxoIconBar is based on. Returns %NULL if
 * the model is unset.
 *
 * Returns: A #GtkTreeModel, or %NULL if none is currently being used.
 **/
GtkTreeModel*
blxo_icon_bar_get_model (BlxoIconBar *icon_bar)
{
  g_return_val_if_fail (BLXO_IS_ICON_BAR (icon_bar), NULL);
  return icon_bar->priv->model;
}



/**
 * blxo_icon_bar_set_model:
 * @icon_bar  : A #BlxoIconBar.
 * @model     : A #GtkTreeModel or %NULL.
 *
 * Sets the model for a #BlxoIconBar. If the @icon_bar already has a model
 * set, it will remove it before settings the new model. If @model is %NULL,
 * then it will unset the old model.
 **/
void
blxo_icon_bar_set_model (BlxoIconBar    *icon_bar,
                        GtkTreeModel  *model)
{
  GType pixbuf_column_type;
  GType text_column_type;
  gint  active = -1;

  g_return_if_fail (BLXO_IS_ICON_BAR (icon_bar));
  g_return_if_fail (GTK_IS_TREE_MODEL (model) || model == NULL);

  if (G_UNLIKELY (model == icon_bar->priv->model))
    return;

  if (model != NULL)
    {
      g_return_if_fail (gtk_tree_model_get_flags (model) & GTK_TREE_MODEL_LIST_ONLY);

      if (icon_bar->priv->pixbuf_column != -1)
        {
          pixbuf_column_type = gtk_tree_model_get_column_type (model, icon_bar->priv->pixbuf_column);
          g_return_if_fail (pixbuf_column_type == GDK_TYPE_PIXBUF);
        }

      if (icon_bar->priv->text_column != -1)
        {
          text_column_type = gtk_tree_model_get_column_type (model, icon_bar->priv->text_column);
          g_return_if_fail (text_column_type == G_TYPE_STRING);
        }
    }

  if (icon_bar->priv->model)
    {
      g_signal_handlers_disconnect_by_func (icon_bar->priv->model,
                                            blxo_icon_bar_row_changed,
                                            icon_bar);
      g_signal_handlers_disconnect_by_func (icon_bar->priv->model,
                                            blxo_icon_bar_row_inserted,
                                            icon_bar);
      g_signal_handlers_disconnect_by_func (icon_bar->priv->model,
                                            blxo_icon_bar_row_deleted,
                                            icon_bar);
      g_signal_handlers_disconnect_by_func (icon_bar->priv->model,
                                            blxo_icon_bar_rows_reordered,
                                            icon_bar);

      g_object_unref (G_OBJECT (icon_bar->priv->model));

      g_list_foreach (icon_bar->priv->items, (GFunc) (void (*)(void)) blxo_icon_bar_item_free, NULL);
      g_list_free (icon_bar->priv->items);
      icon_bar->priv->active_item = NULL;
      icon_bar->priv->cursor_item = NULL;
      icon_bar->priv->items = NULL;
    }

  icon_bar->priv->model = model;

  if (model != NULL)
    {
      g_object_ref (G_OBJECT (model));

      g_signal_connect (G_OBJECT (model), "row-changed",
                        G_CALLBACK (blxo_icon_bar_row_changed), icon_bar);
      g_signal_connect (G_OBJECT (model), "row-inserted",
                        G_CALLBACK (blxo_icon_bar_row_inserted), icon_bar);
      g_signal_connect (G_OBJECT (model), "row-deleted",
                        G_CALLBACK (blxo_icon_bar_row_deleted), icon_bar);
      g_signal_connect (G_OBJECT (model), "rows-reordered",
                        G_CALLBACK (blxo_icon_bar_rows_reordered), icon_bar);

      blxo_icon_bar_build_items (icon_bar);

      if (icon_bar->priv->items != NULL)
        active = ((BlxoIconBarItem *) icon_bar->priv->items->data)->index;
    }

  blxo_icon_bar_invalidate (icon_bar);

  g_object_notify (G_OBJECT (icon_bar), "model");

  blxo_icon_bar_set_active (icon_bar, active);
}



/**
 * blxo_icon_bar_get_pixbuf_column:
 * @icon_bar  : An #BlxoIconBar.
 *
 * Returns the column with pixbufs for @icon_bar.
 *
 * Returns: the pixbuf column, or -1 if it's unset.
 **/
gint
blxo_icon_bar_get_pixbuf_column (BlxoIconBar *icon_bar)
{
  g_return_val_if_fail (BLXO_IS_ICON_BAR (icon_bar), -1);
  return icon_bar->priv->pixbuf_column;
}



/**
 * blxo_icon_bar_set_pixbuf_column:
 * @icon_bar  : An #BlxoIconBar.
 * @column    : A column in the currently used model.
 *
 * Sets the column with pixbufs for @icon_bar to be @column. The pixbuf
 * column must be of type #GDK_TYPE_PIXBUF.
 **/
void
blxo_icon_bar_set_pixbuf_column (BlxoIconBar *icon_bar,
                                gint        column)
{
  GType pixbuf_column_type;

  g_return_if_fail (BLXO_IS_ICON_BAR (icon_bar));

  if (column == icon_bar->priv->pixbuf_column)
    return;

  if (column == -1)
    {
      icon_bar->priv->pixbuf_column = -1;
    }
  else
    {
      if (icon_bar->priv->model != NULL)
        {
          pixbuf_column_type = gtk_tree_model_get_column_type (icon_bar->priv->model, column);
          g_return_if_fail (pixbuf_column_type == GDK_TYPE_PIXBUF);
        }

      icon_bar->priv->pixbuf_column = column;
    }

  blxo_icon_bar_invalidate (icon_bar);

  g_object_notify (G_OBJECT (icon_bar), "pixbuf-column");
}



/**
 * blxo_icon_bar_get_text_column:
 * @icon_bar  : An #BlxoIconBar.
 *
 * Returns the column with text for @icon_bar.
 *
 * Returns: the text column, or -1 if it's unset.
 **/
gint
blxo_icon_bar_get_text_column (BlxoIconBar *icon_bar)
{
  g_return_val_if_fail (BLXO_IS_ICON_BAR (icon_bar), -1);
  return icon_bar->priv->text_column;
}



/**
 * blxo_icon_bar_set_text_column:
 * @icon_bar  : An #BlxoIconBar.
 * @column    : A column in the currently used model or -1 to
 *              use no text in @icon_bar.
 *
 * Sets the column with text for @icon_bar to be @column. The
 * text column must be of type #G_TYPE_STRING.
 **/
void
blxo_icon_bar_set_text_column (BlxoIconBar *icon_bar,
                              gint        column)
{
  GType text_column_type;

  g_return_if_fail (BLXO_IS_ICON_BAR (icon_bar));

  if (column == icon_bar->priv->text_column)
    return;

  if (column == -1)
    {
      icon_bar->priv->text_column = -1;
    }
  else
    {
      if (icon_bar->priv->model != NULL)
        {
          text_column_type = gtk_tree_model_get_column_type (icon_bar->priv->model, column);
          g_return_if_fail (text_column_type == G_TYPE_STRING);
        }

      icon_bar->priv->text_column = column;
    }

  blxo_icon_bar_invalidate (icon_bar);

  g_object_notify (G_OBJECT (icon_bar), "text-column");
}



/**
 * blxo_icon_bar_get_orientation:
 * @icon_bar  : An #BlxoIconBar.
 *
 * Retrieves the current orientation of the toolbar. See
 * blxo_icon_bar_set_orientation().
 *
 * Returns: The orientation of @icon_bar.
 **/
GtkOrientation
blxo_icon_bar_get_orientation (BlxoIconBar *icon_bar)
{
  g_return_val_if_fail (BLXO_IS_ICON_BAR (icon_bar), GTK_ORIENTATION_VERTICAL);
  return icon_bar->priv->orientation;
}



/**
 * blxo_icon_bar_set_orientation:
 * @icon_bar    : An #BlxoIconBar.
 * @orientation : A new #GtkOrientation.
 *
 * Sets whether the @icon_bar should appear horizontally
 * or vertically.
 **/
void
blxo_icon_bar_set_orientation (BlxoIconBar    *icon_bar,
                              GtkOrientation orientation)
{
  g_return_if_fail (BLXO_IS_ICON_BAR (icon_bar));

  if (icon_bar->priv->orientation != orientation)
    {
      icon_bar->priv->orientation = orientation;
      gtk_widget_queue_resize (GTK_WIDGET (icon_bar));
      g_object_notify (G_OBJECT (icon_bar), "orientation");
    }
}



/**
 * blxo_icon_bar_get_active:
 * @icon_bar  : An #BlxoIconBar.
 *
 * Returns the index of the currently active item, or -1 if there's no
 * active item.
 *
 * Returns: An integer which is the index of the currently active item,
 *          or -1 if there's no active item.
 **/
gint
blxo_icon_bar_get_active (BlxoIconBar *icon_bar)
{
  g_return_val_if_fail (BLXO_IS_ICON_BAR (icon_bar), -1);

  return (icon_bar->priv->active_item != NULL)
        ? icon_bar->priv->active_item->index
        : -1;
}



/**
 * blxo_icon_bar_set_active:
 * @icon_bar  : An #BlxoIconBar.
 * @idx       : An index in the model passed during construction,
 *              or -1 to have no active item.
 *
 * Sets the active item of @icon_bar to be the item at @idx.
 **/
void
blxo_icon_bar_set_active (BlxoIconBar *icon_bar,
                         gint        idx)
{
  g_return_if_fail (BLXO_IS_ICON_BAR (icon_bar));
  g_return_if_fail (idx == -1 || g_list_nth (icon_bar->priv->items, idx) != NULL);

  if ((icon_bar->priv->active_item == NULL && idx == -1)
      || (icon_bar->priv->active_item != NULL && idx == icon_bar->priv->active_item->index))
    return;

  if (G_UNLIKELY (idx >= 0))
    icon_bar->priv->active_item = g_list_nth (icon_bar->priv->items, idx)->data;
  else
    icon_bar->priv->active_item = NULL;

  g_signal_emit (G_OBJECT (icon_bar), icon_bar_signals[SELECTION_CHANGED], 0);
  g_object_notify (G_OBJECT (icon_bar), "active");
  gtk_widget_queue_draw (GTK_WIDGET (icon_bar));
}



/**
 * blxo_icon_bar_get_active_iter:
 * @icon_bar  : An #BlxoIconBar.
 * @iter      : An uninitialized #GtkTreeIter.
 *
 * Sets @iter to point to the current active item, if it exists.
 *
 * Returns: %TRUE if @iter was set.
 **/
gboolean
blxo_icon_bar_get_active_iter (BlxoIconBar  *icon_bar,
                              GtkTreeIter *iter)
{
  BlxoIconBarItem *item;
  GtkTreePath    *path;

  g_return_val_if_fail (BLXO_IS_ICON_BAR (icon_bar), FALSE);
  g_return_val_if_fail (iter != NULL, FALSE);

  item = icon_bar->priv->active_item;
  if (item == NULL)
    return FALSE;

  if ((gtk_tree_model_get_flags (icon_bar->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST) == 0)
    {
      path = gtk_tree_path_new_from_indices (item->index, -1);
      gtk_tree_model_get_iter (icon_bar->priv->model, iter, path);
      gtk_tree_path_free (path);
    }
  else
    {
      *iter = item->iter;
    }

  return TRUE;
}



/**
 * blxo_icon_bar_set_active_iter:
 * @icon_bar  : An #BlxoIconBar.
 * @iter      : The #GtkTreeIter.
 *
 * Sets the current active item to be the one referenced by @iter. @iter
 * must correspond to a path of depth one.
 *
 * This can only be called if @icon_bar is associated with #GtkTreeModel.
 **/
void
blxo_icon_bar_set_active_iter (BlxoIconBar  *icon_bar,
                              GtkTreeIter *iter)
{
  GtkTreePath *path;

  g_return_if_fail (BLXO_IS_ICON_BAR (icon_bar));
  g_return_if_fail (icon_bar->priv->model != NULL);
  g_return_if_fail (iter != NULL);

  path = gtk_tree_model_get_path (icon_bar->priv->model, iter);
  if (G_LIKELY (path != NULL))
    {
      blxo_icon_bar_set_active (icon_bar, gtk_tree_path_get_indices (path)[0]);
      gtk_tree_path_free (path);
    }
}



#define __BLXO_ICON_BAR_C__
#include <blxo/blxo-aliasdef.c>
