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

#if !defined (BLXO_INSIDE_BLXO_H) && !defined (BLXO_COMPILATION)
#error "Only <blxo/blxo.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef __BLXO_ICON_CHOOSER_DIALOG_H__
#define __BLXO_ICON_CHOOSER_DIALOG_H__

#include <blxo/blxo-config.h>

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _BlxoIconChooserDialogPrivate BlxoIconChooserDialogPrivate;
typedef struct _BlxoIconChooserDialogClass   BlxoIconChooserDialogClass;
typedef struct _BlxoIconChooserDialog        BlxoIconChooserDialog;

#define BLXO_TYPE_ICON_CHOOSER_DIALOG            (blxo_icon_chooser_dialog_get_type ())
#define BLXO_ICON_CHOOSER_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_ICON_CHOOSER_DIALOG, BlxoIconChooserDialog))
#define BLXO_ICON_CHOOSER_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_ICON_CHOOSER_DIALOG, BlxoIconChooserDialogClass))
#define BLXO_IS_ICON_CHOOSER_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_ICON_CHOOSER_DIALOG))
#define BLXO_IS_ICON_CHOOSER_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_ICON_CHOOSER_DIALOG))
#define BLXO_ICON_CHOOSER_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_ICON_CHOOSER_DIALOG, BlxoIconChooserDialogClass))

struct _BlxoIconChooserDialogClass
{
  /*< private >*/
  GtkDialogClass __parent__;

  /* reserved for future expansion */
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
  void (*reserved4) (void);
  void (*reserved5) (void);
  void (*reserved6) (void);
};

/**
 * BlxoIconChooserDialog:
 *
 * The #BlxoIconChooserDialog class provides an easy to use dialog to ask
 * the user to select either a named icon from the selected icon theme,
 * or an image file from the local file system.
 **/
struct _BlxoIconChooserDialog
{
  /*< private >*/
  GtkDialog __parent__;
};

GType      blxo_icon_chooser_dialog_get_type (void) G_GNUC_CONST;

GtkWidget *blxo_icon_chooser_dialog_new      (const gchar          *title,
                                             GtkWindow            *parent,
                                             const gchar          *first_button_text,
                                             ...) G_GNUC_MALLOC G_GNUC_NULL_TERMINATED G_GNUC_WARN_UNUSED_RESULT;

gchar     *blxo_icon_chooser_dialog_get_icon (BlxoIconChooserDialog *icon_chooser_dialog) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;
gboolean   blxo_icon_chooser_dialog_set_icon (BlxoIconChooserDialog *icon_chooser_dialog,
                                             const gchar          *icon);

G_END_DECLS

#endif /* !__BLXO_ICON_CHOOSER_DIALOG_H__ */
