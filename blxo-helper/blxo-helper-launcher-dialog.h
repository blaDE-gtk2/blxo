/*-
 * Copyright (c) 2003-2006 Benedikt Meurer <benny@xfce.org>.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __BLXO_HELPER_LAUNCHER_DIALOG_H__
#define __BLXO_HELPER_LAUNCHER_DIALOG_H__

#include <blxo-helper/blxo-helper.h>

G_BEGIN_DECLS

typedef struct _BlxoHelperLauncherDialogClass BlxoHelperLauncherDialogClass;
typedef struct _BlxoHelperLauncherDialog      BlxoHelperLauncherDialog;

#define BLXO_TYPE_HELPER_LAUNCHER_DIALOG             (blxo_helper_launcher_dialog_get_type ())
#define BLXO_HELPER_LAUNCHER_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_HELPER_LAUNCHER_DIALOG, BlxoHelperLauncherDialog))
#define BLXO_HELPER_LAUNCHER_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_HELPER_LAUNCHER_DIALOG, BlxoHelperLauncherDialogClass))
#define BLXO_IS_HELPER_LAUNCHER_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_HELPER_LAUNCHER_DIALOG))
#define BLXO_IS_HELPER_LAUNCHER_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_HELPER_LAUNCHER_DIALOG))
#define BLXO_HELPER_LAUNCHER_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXOT_TYPE_HELPER_LAUNCHER_DIALOg, BlxoHelperLauncherDialogClass))

GType             blxo_helper_launcher_dialog_get_type     (void) G_GNUC_CONST;

GtkWidget        *blxo_helper_launcher_dialog_new          (BlxoHelperCategory              category) G_GNUC_MALLOC;

BlxoHelperCategory blxo_helper_launcher_dialog_get_category (const BlxoHelperLauncherDialog *launcher_dialog);
void              blxo_helper_launcher_dialog_set_category (BlxoHelperLauncherDialog       *launcher_dialog,
                                                           BlxoHelperCategory              category);

G_END_DECLS

#endif /* !__BLXO_HELPER_LAUNCHER_DIALOG_H__ */
