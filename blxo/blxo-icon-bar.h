/*-
 * Copyright (c) 2004 os-cillation e.K.
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

#ifndef __BLXO_ICON_BAR_H__
#define __BLXO_ICON_BAR_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BLXO_TYPE_ICON_BAR             (blxo_icon_bar_get_type ())
#define BLXO_ICON_BAR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_ICON_BAR, BlxoIconBar))
#define BLXO_ICON_BAR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_ICON_BAR, BlxoIconBarClass))
#define BLXO_IS_ICON_BAR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_ICON_BAR))
#define BLXO_IS_ICON_BAR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((obj), BLXO_TYPE_ICON_BAR))
#define BLXO_ICON_BAR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_ICON_BAR, BlxoIconBarClass))

typedef struct _BlxoIconBarPrivate BlxoIconBarPrivate;
typedef struct _BlxoIconBarClass   BlxoIconBarClass;
typedef struct _BlxoIconBar        BlxoIconBar;

/**
 * BlxoIconBarClass:
 * @set_scroll_adjustments : Used internally to make the BlxoIconBar scrollable.
 * @selection_changed      : This signal is emitted whenever the currently selected icon changes.
 *
 * The #BlxoIconBar class contains only private fields and should not
 * be directly accessed.
 **/
struct _BlxoIconBarClass
{
  GtkContainerClass __parent__;

  /* signals */
  void  (*set_scroll_adjustments)  (BlxoIconBar    *icon_bar,
                                    GtkAdjustment *hadjustment,
                                    GtkAdjustment *vadjustment);
  void  (*selection_changed)       (BlxoIconBar *icon_bar);

  /*< private >*/
  void  (*reserved1) (void);
  void  (*reserved2) (void);
  void  (*reserved3) (void);
  void  (*reserved4) (void);
};

/**
 * BlxoIconBar:
 *
 * The #BlxoIconBar struct contains only private fields and should not
 * be directly accessed.
 **/
struct _BlxoIconBar
{
  GtkContainer       __parent__;

  /*< private >*/
  BlxoIconBarPrivate *priv;
};

GType           blxo_icon_bar_get_type           (void) G_GNUC_CONST;

GtkWidget      *blxo_icon_bar_new                (void);
GtkWidget      *blxo_icon_bar_new_with_model     (GtkTreeModel   *model);

GtkTreeModel   *blxo_icon_bar_get_model          (BlxoIconBar     *icon_bar);
void            blxo_icon_bar_set_model          (BlxoIconBar     *icon_bar,
                                                 GtkTreeModel   *model);

gint            blxo_icon_bar_get_pixbuf_column  (BlxoIconBar     *icon_bar);
void            blxo_icon_bar_set_pixbuf_column  (BlxoIconBar     *icon_bar,
                                                 gint            column);

gint            blxo_icon_bar_get_text_column    (BlxoIconBar     *icon_bar);
void            blxo_icon_bar_set_text_column    (BlxoIconBar     *icon_bar,
                                                 gint            column);

GtkOrientation  blxo_icon_bar_get_orientation    (BlxoIconBar     *icon_bar);
void            blxo_icon_bar_set_orientation    (BlxoIconBar     *icon_bar,
                                                 GtkOrientation  orientation);

gint            blxo_icon_bar_get_active         (BlxoIconBar     *icon_bar);
void            blxo_icon_bar_set_active         (BlxoIconBar     *icon_bar,
                                                 gint            idx);

gboolean        blxo_icon_bar_get_active_iter    (BlxoIconBar     *icon_bar,
                                                 GtkTreeIter    *iter);
void            blxo_icon_bar_set_active_iter    (BlxoIconBar     *icon_bar,
                                                 GtkTreeIter    *iter);

G_END_DECLS

#endif /* !__BLXO_ICON_BAR_H__ */
