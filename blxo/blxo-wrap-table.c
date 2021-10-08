/*-
 * Copyright (c) 2000      Ramiro Estrugo <ramiro@eazel.com>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <blxo/blxo-config.h>
#include <blxo/blxo-private.h>
#include <blxo/blxo-string.h>
#include <blxo/blxo-wrap-table.h>
#include <blxo/blxo-alias.h>

/**
 * SECTION: blxo-wrap-table
 * @title: BlxoWrapTable
 * @short_description: A container widget with automatic child layouting
 * @include: blxo/blxo.h
 *
 * The #BlxoWrapTable class layouts its children automatically
 * according to the available width allocated to the container
 * class.
 *
 * Since: 0.3.1
 **/



/* Property identifiers */
enum
{
  PROP_0,
  PROP_COL_SPACING,
  PROP_ROW_SPACING,
  PROP_HOMOGENEOUS,
};



static void blxo_wrap_table_get_property       (GObject            *object,
                                               guint               prop_id,
                                               GValue             *value,
                                               GParamSpec         *pspec);
static void blxo_wrap_table_set_property       (GObject            *object,
                                               guint               prop_id,
                                               const GValue       *value,
                                               GParamSpec         *pspec);
static void blxo_wrap_table_size_request       (GtkWidget          *widget,
                                               GtkRequisition     *requisition);
static void blxo_wrap_table_size_allocate      (GtkWidget          *widget,
                                               GtkAllocation      *allocation);
static void blxo_wrap_table_add                (GtkContainer       *container,
                                               GtkWidget          *widget);
static void blxo_wrap_table_remove             (GtkContainer       *container,
                                               GtkWidget          *widget);
static void blxo_wrap_table_forall             (GtkContainer       *container,
                                               gboolean            include_internals,
                                               GtkCallback         callback,
                                               gpointer            callback_data);
static void blxo_wrap_table_layout             (BlxoWrapTable       *table);
static gint blxo_wrap_table_get_max_child_size (const BlxoWrapTable *table,
                                               gint               *max_width_return,
                                               gint               *max_height_return);
static gint blxo_wrap_table_get_num_fitting    (gint                available,
                                               gint                spacing,
                                               gint                max_child_size);



struct _BlxoWrapTablePrivate
{
  /* the list of child widgets */
  GList *children;

  /* configurable parameters */
  guint  col_spacing;
  guint  row_spacing;
  guint  homogeneous : 1;

  /* the estimated number of columns */
  gint   num_cols;
};



G_DEFINE_TYPE_WITH_PRIVATE (BlxoWrapTable, blxo_wrap_table, GTK_TYPE_CONTAINER)



static void
blxo_wrap_table_class_init (BlxoWrapTableClass *klass)
{
  GtkContainerClass *gtkcontainer_class;
  GtkWidgetClass    *gtkwidget_class;
  GObjectClass      *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->get_property = blxo_wrap_table_get_property;
  gobject_class->set_property = blxo_wrap_table_set_property;

  gtkwidget_class = GTK_WIDGET_CLASS (klass);
  gtkwidget_class->size_request = blxo_wrap_table_size_request;
  gtkwidget_class->size_allocate = blxo_wrap_table_size_allocate;

  gtkcontainer_class = GTK_CONTAINER_CLASS (klass);
  gtkcontainer_class->add = blxo_wrap_table_add;
  gtkcontainer_class->remove = blxo_wrap_table_remove;
  gtkcontainer_class->forall = blxo_wrap_table_forall;

  /* initialize the library's i18n support */
  _blxo_i18n_init ();

  /**
   * BlxoWrapTable::col-spacing:
   *
   * The amount of space between two consecutive columns.
   *
   * Since: 0.3.1
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_COL_SPACING,
                                   g_param_spec_uint ("col-spacing",
                                                      _("Column spacing"),
                                                      _("The amount of space between two consecutive columns"),
                                                      0, G_MAXUINT, 0,
                                                      BLXO_PARAM_READWRITE));

  /**
   * BlxoWrapTable::row-spacing:
   *
   * The amount of space between two consecutive rows.
   *
   * Since: 0.3.1
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_ROW_SPACING,
                                   g_param_spec_uint ("row-spacing",
                                                      _("Row spacing"),
                                                      _("The amount of space between two consecutive rows"),
                                                      0, G_MAXUINT, 0,
                                                      BLXO_PARAM_READWRITE));

  /**
   * BlxoWrapTable::homogeneous:
   *
   * Whether the children should be all the same size.
   *
   * Since: 0.3.1
   **/
  g_object_class_install_property (gobject_class,
                                   PROP_HOMOGENEOUS,
                                   g_param_spec_boolean ("homogeneous",
                                                         _("Homogeneous"),
                                                         _("Whether the children should be all the same size"),
                                                         FALSE,
                                                         BLXO_PARAM_READWRITE));
}



static void
blxo_wrap_table_init (BlxoWrapTable *table)
{
  /* grab a pointer on the private data */
  table->priv = blxo_wrap_table_get_instance_private (table);

  /* we don't provide our own window */
  gtk_widget_set_has_window (GTK_WIDGET (table), FALSE);
}



static void
blxo_wrap_table_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (object);

  switch (prop_id)
    {
    case PROP_COL_SPACING:
      g_value_set_uint (value, blxo_wrap_table_get_col_spacing (table));
      break;

    case PROP_ROW_SPACING:
      g_value_set_uint (value, blxo_wrap_table_get_row_spacing (table));
      break;

    case PROP_HOMOGENEOUS:
      g_value_set_boolean (value, blxo_wrap_table_get_homogeneous (table));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
blxo_wrap_table_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (object);

  switch (prop_id)
    {
    case PROP_COL_SPACING:
      blxo_wrap_table_set_col_spacing (table, g_value_get_uint (value));
      break;

    case PROP_ROW_SPACING:
      blxo_wrap_table_set_row_spacing (table, g_value_get_uint (value));
      break;

    case PROP_HOMOGENEOUS:
      blxo_wrap_table_set_homogeneous (table, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
blxo_wrap_table_size_request (GtkWidget      *widget,
                             GtkRequisition *requisition)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (widget);
  gint          max_width = 0;
  gint          max_height = 0;
  gint          num_children;
  gint          num_cols;
  gint          num_rows = 0;
  GtkAllocation allocation;

  /* determine the max size request */
  num_children = blxo_wrap_table_get_max_child_size (table, &max_width, &max_height);
  gtk_widget_get_allocation (widget, &allocation);

  /* check if we have any visible children */
  if (G_LIKELY (num_children > 0))
    {
      num_cols = blxo_wrap_table_get_num_fitting (allocation.width
                                                 - gtk_container_get_border_width (GTK_CONTAINER (widget)) * 2,
                                                 table->priv->col_spacing, max_width);
      if (G_LIKELY (num_cols > 0))
        num_rows = num_children / num_cols;
      num_rows = MAX (num_rows, 1);

      if ((num_children % num_rows) > 0)
        ++num_rows;

      requisition->width = -1;
      requisition->height = (num_rows * max_height)
                         + (num_rows - 1) * table->priv->col_spacing
                         + gtk_container_get_border_width (GTK_CONTAINER (widget)) * 2;
    }
  else
    {
      requisition->width = 0;
      requisition->height = 0;
    }
}



static void
blxo_wrap_table_size_allocate (GtkWidget     *widget,
                              GtkAllocation *allocation)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (widget);

  /* setup the new allocation */
  gtk_widget_set_allocation (widget, allocation);

  /* layout the children */
  blxo_wrap_table_layout (table);
}



static void
blxo_wrap_table_add (GtkContainer *container,
                    GtkWidget    *widget)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (container);

  /* take over ownership */
  gtk_widget_set_parent (widget, GTK_WIDGET (table));

  /* add the child to our internal list */
  table->priv->children = g_list_append (table->priv->children, widget);

  /* realize the widget if required */
  if (gtk_widget_get_realized (GTK_WIDGET (container)))
    gtk_widget_realize (widget);

  /* map the widget if required */
  if (gtk_widget_get_visible (GTK_WIDGET (container)) && gtk_widget_get_visible (widget))
    {
      if (gtk_widget_get_mapped (GTK_WIDGET (container)))
        gtk_widget_map (widget);
    }

  /* queue a resize on the table */
  gtk_widget_queue_resize (GTK_WIDGET (container));
}



static void
blxo_wrap_table_remove (GtkContainer *container,
                       GtkWidget    *widget)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (container);
  gboolean      widget_was_visible;

  /* check if the widget was visible */
  widget_was_visible = gtk_widget_get_visible (widget);

  /* unparent and remove the widget */
  gtk_widget_unparent (widget);
  table->priv->children = g_list_remove (table->priv->children, widget);

  /* schedule a resize if the widget was visible */
  if (G_LIKELY (widget_was_visible))
    gtk_widget_queue_resize (GTK_WIDGET (table));
}



static void
blxo_wrap_table_forall (GtkContainer *container,
                       gboolean      include_internals,
                       GtkCallback   callback,
                       gpointer      callback_data)
{
  BlxoWrapTable *table = BLXO_WRAP_TABLE (container);
  GList        *next;
  GList        *node;

  for (node = table->priv->children; node != NULL; node = next)
    {
      /* verify that we have a valid widget for the node */
      g_assert (GTK_IS_WIDGET (node->data));

      /* remember a pointer to the next node */
      next = node->next;

      /* invoke the callback for this widget */
      (*callback) (GTK_WIDGET (node->data), callback_data);
    }
}



static void
blxo_wrap_table_layout (BlxoWrapTable *table)
{
  GtkRequisition child_requisition;
  GtkAllocation  child_allocation;
  GtkWidget     *child;
  GList         *lp;
  gint           x0, x1, x, y;
  gint           num_children;
  gint           num_cols;
  gint           max_height;
  gint           max_width;
  GtkAllocation  allocation;

  /* determine the number of visible children and the max size */
  num_children = blxo_wrap_table_get_max_child_size (table, &max_width, &max_height);
  if (G_UNLIKELY (num_children <= 0))
    return;

  gtk_widget_get_allocation (GTK_WIDGET (table), &allocation);

  /* determine the number of columns */
  num_cols = blxo_wrap_table_get_num_fitting (allocation.width
                                             - gtk_container_get_border_width (GTK_CONTAINER (table)) * 2,
                                             table->priv->col_spacing, max_width);

  /* verify that the number of columns match */
  if (G_UNLIKELY (num_cols != table->priv->num_cols))
    {
      table->priv->num_cols = num_cols;
      gtk_widget_queue_resize (GTK_WIDGET (table));
      return;
    }

  /* determine the horizontal bounds */
  x0 = allocation.x + gtk_container_get_border_width (GTK_CONTAINER (table));
  x1 = x0 + allocation.width - gtk_container_get_border_width (GTK_CONTAINER (table));

  /* initialize the position */
  x = x0;
  y = allocation.y + gtk_container_get_border_width (GTK_CONTAINER (table));

  /* allocate space to all visible children */
  for (lp = table->priv->children; lp != NULL; lp = lp->next)
    {
      /* allocate space only for visible children */
      child = GTK_WIDGET (lp->data);
      if (G_UNLIKELY (!gtk_widget_get_visible (child)))
        continue;

      /* initialize the child position */
      child_allocation.x = x;
      child_allocation.y = y;

      /* check if we should layout the children homogeneously */
      if (G_LIKELY (table->priv->homogeneous))
        {
          child_allocation.width = max_width;
          child_allocation.height = max_height;

          /* check if we're wrapping */
          if (G_UNLIKELY ((x + max_width) > x1))
            {
              x = x0 + table->priv->col_spacing + max_width;
              y += table->priv->row_spacing + max_height;
              child_allocation.x = x0;
              child_allocation.y = y;
            }
          else
            {
              x += table->priv->col_spacing + max_width;
            }
        }
      else
        {
          gtk_widget_size_request (child, &child_requisition);

          child_allocation.width = child_requisition.width;
          child_allocation.height = child_requisition.height;

          g_assert (child_allocation.width <= max_width);
          g_assert (child_allocation.height <= max_height);

          if (G_UNLIKELY ((x + max_width) > x1))
            {
              x = x0 + table->priv->col_spacing + max_width;
              y += table->priv->row_spacing + max_height;
              child_allocation.x = x0;
              child_allocation.y = y;
            }
          else
            {
              x += table->priv->col_spacing + max_width;
            }
        }

      /* allocate the space to the child */
      gtk_widget_size_allocate (child, &child_allocation);
    }
}



static gint
blxo_wrap_table_get_max_child_size (const BlxoWrapTable *table,
                                   gint               *max_width_return,
                                   gint               *max_height_return)
{
  GtkRequisition child_requisition;
  GtkWidget     *child;
  GList         *lp;
  gint           max_width = 0;
  gint           max_height = 0;
  gint           num_children = 0;

  for (lp = table->priv->children; lp != NULL; lp = lp->next)
    {
      child = GTK_WIDGET (lp->data);
      if (gtk_widget_get_visible (child))
        {
          gtk_widget_size_request (child, &child_requisition);
          if (child_requisition.width > max_width)
            max_width = child_requisition.width;
          if (child_requisition.height > max_height)
            max_height = child_requisition.height;

          /* we count only visible children */
          ++num_children;
        }
    }

  /* use atleast one pixel if we have visible childrens */
  if (G_LIKELY (num_children > 0))
    {
      if (G_UNLIKELY (max_width < 1))
        max_width = 1;
      if (G_UNLIKELY (max_height < 1))
        max_height = 1;
    }

  /* return the determined values */
  if (G_LIKELY (max_width_return != NULL))
    *max_width_return = max_width;
  if (G_LIKELY (max_height_return != NULL))
    *max_height_return = max_height;

  return num_children;
}



static gint
blxo_wrap_table_get_num_fitting (gint available,
                                gint spacing,
                                gint max_child_size)
{
  gint num;

  g_return_val_if_fail (spacing >= 0, 0);
  g_return_val_if_fail (max_child_size > 0, 0);

  /* verify that available is atleast 0 */
  if (G_UNLIKELY (available < 0))
    available = 0;

  /* determine the num */
  num = (available + spacing) / (max_child_size + spacing);

  /* verify that num is atleast 1 */
  if (G_UNLIKELY (num < 1))
    num = 1;

  return num;
}



/**
 * blxo_wrap_table_new:
 * @homogeneous : %TRUE if all children are to be given equal space allotments.
 *
 * Allocates a new #BlxoWrapTable.
 *
 * Returns: the newly allocated #BlxoWrapTable.
 *
 * Since: 0.3.1
 **/
GtkWidget*
blxo_wrap_table_new (gboolean homogeneous)
{
  return g_object_new (BLXO_TYPE_WRAP_TABLE,
                       "homogeneous", homogeneous,
                       NULL);
}



/**
 * blxo_wrap_table_get_col_spacing:
 * @table : an #BlxoWrapTable.
 *
 * Returns the amount of space between consecutive
 * columns in @table.
 *
 * Returns: the amount of space between consecutive columns.
 *
 * Since: 0.3.1
 **/
guint
blxo_wrap_table_get_col_spacing (const BlxoWrapTable *table)
{
  g_return_val_if_fail (BLXO_IS_WRAP_TABLE (table), 0);
  return table->priv->col_spacing;
}



/**
 * blxo_wrap_table_set_col_spacing:
 * @table       : an #BlxoWrapTable.
 * @col_spacing : the new column spacing.
 *
 * Sets the amount of space between consecutive
 * columns in @table to @col_spacing.
 *
 * Since: 0.3.1
 **/
void
blxo_wrap_table_set_col_spacing (BlxoWrapTable *table,
                                guint         col_spacing)
{
  g_return_if_fail (BLXO_IS_WRAP_TABLE (table));

  if (G_LIKELY (table->priv->col_spacing != col_spacing))
    {
      table->priv->col_spacing = col_spacing;
      gtk_widget_queue_resize (GTK_WIDGET (table));
      g_object_notify (G_OBJECT (table), "col-spacing");
    }
}



/**
 * blxo_wrap_table_get_row_spacing:
 * @table : an #BlxoWrapTable.
 *
 * Returns the amount of space between consecutive
 * rows in @table.
 *
 * Returns: the amount of space between consecutive rows in @table.
 *
 * Since: 0.3.1
 **/
guint
blxo_wrap_table_get_row_spacing (const BlxoWrapTable *table)
{
  g_return_val_if_fail (BLXO_IS_WRAP_TABLE (table), 0);
  return table->priv->row_spacing;
}



/**
 * blxo_wrap_table_set_row_spacing:
 * @table       : an #BlxoWrapTable.
 * @row_spacing : the new row spacing.
 *
 * Sets the amount of spacing between consecutive
 * rows in @table to @row_spacing.
 *
 * Since: 0.3.1
 **/
void
blxo_wrap_table_set_row_spacing (BlxoWrapTable *table,
                                guint         row_spacing)
{
  g_return_if_fail (BLXO_IS_WRAP_TABLE (table));

  if (G_LIKELY (table->priv->row_spacing != row_spacing))
    {
      table->priv->row_spacing = row_spacing;
      gtk_widget_queue_resize (GTK_WIDGET (table));
      g_object_notify (G_OBJECT (table), "row-spacing");
    }
}



/**
 * blxo_wrap_table_get_homogeneous:
 * @table : an #BlxoWrapTable.
 *
 * Returns whether the table cells are all constrained
 * to the same width and height.
 *
 * Returns: %TRUE if the cells are all constrained to the same size.
 *
 * Since: 0.3.1
 **/
gboolean
blxo_wrap_table_get_homogeneous (const BlxoWrapTable *table)
{
  g_return_val_if_fail (BLXO_IS_WRAP_TABLE (table), FALSE);
  return table->priv->homogeneous;
}



/**
 * blxo_wrap_table_set_homogeneous:
 * @table       : an #BlxoWrapTable.
 * @homogeneous : Set to %TRUE to ensure all @table cells are the same size.
 *                Set to %FALSE if this is not your desired behaviour.
 *
 * Changes the homogenous property of @table cells, ie. whether all cells
 * are an equal size or not.
 *
 * Since: 0.3.1
 **/
void
blxo_wrap_table_set_homogeneous (BlxoWrapTable *table,
                                gboolean      homogeneous)
{
  g_return_if_fail (BLXO_IS_WRAP_TABLE (table));

  if (G_LIKELY (table->priv->homogeneous != homogeneous))
    {
      table->priv->homogeneous = homogeneous;
      gtk_widget_queue_resize (GTK_WIDGET (table));
      g_object_notify (G_OBJECT (table), "homogeneous");
    }
}



#define __BLXO_WRAP_TABLE_C__
#include <blxo/blxo-aliasdef.c>
