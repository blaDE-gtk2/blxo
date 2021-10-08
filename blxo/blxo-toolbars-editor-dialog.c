/*-
 * Copyright (c) 2004-2006 os-cillation e.K.
 * Copyright (c) 2003      Marco Pesenti Gritti
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

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <libbladeutil/libbladeutil.h>

#include <blxo/blxo-gtk-extensions.h>
#include <blxo/blxo-private.h>
#include <blxo/blxo-string.h>
#include <blxo/blxo-toolbars-editor.h>
#include <blxo/blxo-toolbars-editor-dialog.h>
#include <blxo/blxo-alias.h>

/**
 * SECTION: blxo-toolbars-editor-dialog
 * @title: BlxoToolbarsEditorDialog
 * @short_description: Dialog to edit toolbars
 * @include: blxo/blxo.h
 * @see_also: #BlxoToolbarsEditor
 *
 * Provides an easy-to-use wrapper for the #BlxoToolbarsEditor widget.
 **/



static void blxo_toolbars_editor_dialog_add_toolbar  (BlxoToolbarsEditorDialog      *dialog);



struct _BlxoToolbarsEditorDialogPrivate
{
  GtkWidget *editor;
};



G_DEFINE_TYPE_WITH_PRIVATE (BlxoToolbarsEditorDialog, blxo_toolbars_editor_dialog, GTK_TYPE_DIALOG)



static void
blxo_toolbars_editor_dialog_class_init (BlxoToolbarsEditorDialogClass *klass)
{
}



static void
blxo_toolbars_editor_dialog_init (BlxoToolbarsEditorDialog *dialog)
{
  GtkWidget *button;
  GtkWidget *align;
  GtkWidget *hbox;
  GtkWidget *image;
  GtkWidget *label;

  dialog->priv = blxo_toolbars_editor_dialog_get_instance_private (dialog);

  gtk_window_set_default_size (GTK_WINDOW (dialog), -1, 300);

  dialog->priv->editor = g_object_new (BLXO_TYPE_TOOLBARS_EDITOR, NULL);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), dialog->priv->editor, TRUE, TRUE, 0);
  gtk_widget_show (dialog->priv->editor);

  button = gtk_button_new ();
  g_signal_connect_swapped (G_OBJECT (button), "clicked",
                            G_CALLBACK (blxo_toolbars_editor_dialog_add_toolbar), dialog);
  gtk_box_pack_start (GTK_BOX (blxo_gtk_dialog_get_action_area (GTK_DIALOG (dialog))), button, FALSE, TRUE, 0);
  gtk_widget_show (button);

  align = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
  gtk_container_add (GTK_CONTAINER (button), align);
  gtk_widget_show (align);

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER (align), hbox);
  gtk_widget_show (hbox);

  image = gtk_image_new_from_icon_name ("list-add", GTK_ICON_SIZE_BUTTON);
  gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);
  gtk_widget_show (image);

  label = gtk_label_new_with_mnemonic (_("_Add a new toolbar"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  button = gtk_button_new_with_mnemonic ("_Close");
  image = gtk_image_new_from_icon_name ("window-close", GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image (GTK_BUTTON (button), image);
  g_signal_connect_swapped (G_OBJECT (button), "clicked",
                            G_CALLBACK (gtk_widget_destroy), dialog);
  gtk_box_pack_start (GTK_BOX (blxo_gtk_dialog_get_action_area (GTK_DIALOG (dialog))), button, FALSE, TRUE, 0);
  gtk_widget_show (button);
}



static void
blxo_toolbars_editor_dialog_add_toolbar (BlxoToolbarsEditorDialog *dialog)
{
  BlxoToolbarsModel *model;
  gchar            *name;

  model = blxo_toolbars_editor_get_model (BLXO_TOOLBARS_EDITOR (dialog->priv->editor));
  if (G_LIKELY (model != NULL))
    {
      name = g_strdup_printf ("blxo-toolbar-%ld-%d", (glong) getpid (), (gint) time (NULL));
      blxo_toolbars_model_add_toolbar (model, -1, name);
      g_free (name);
    }
}



/**
 * blxo_toolbars_editor_dialog_new_with_model:
 * @ui_manager  : A #GtkUIManager.
 * @model       : An #BlxoToolbarsModel.
 *
 * Creates a new #BlxoToolbarsEditorDialog that is associated with
 * @ui_manager and @model.
 *
 * Returns: A new #BlxoToolbarsEditorDialog.
 **/
GtkWidget*
blxo_toolbars_editor_dialog_new_with_model (GtkUIManager     *ui_manager,
                                           BlxoToolbarsModel *model)
{
  BlxoToolbarsEditorDialog *dialog;

  g_return_val_if_fail (GTK_IS_UI_MANAGER (ui_manager), NULL);
  g_return_val_if_fail (BLXO_IS_TOOLBARS_MODEL (model), NULL);

  dialog = g_object_new (BLXO_TYPE_TOOLBARS_EDITOR_DIALOG, NULL);
  g_object_set (G_OBJECT (dialog->priv->editor),
                "ui-manager", ui_manager,
                "model", model,
                NULL);

  return GTK_WIDGET (dialog);
}



#define __BLXO_TOOLBARS_EDITOR_DIALOG_C__
#include <blxo/blxo-aliasdef.c>
