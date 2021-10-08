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

#ifndef __BLXO_HELPER_CHOOSER_H__
#define __BLXO_HELPER_CHOOSER_H__

#include <blxo-helper/blxo-helper.h>

G_BEGIN_DECLS

#define BLXO_TYPE_HELPER_CHOOSER            (blxo_helper_chooser_get_type ())
#define BLXO_HELPER_CHOOSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_HELPER_CHOOSER, BlxoHelperChooser))
#define BLXO_HELPER_CHOOSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_HELPER_CHOOSER, BlxoHelperChooserClass))
#define BLXO_IS_HELPER_CHOOSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_HELPER_CHOOSER))
#define BLXO_IS_HELPER_CHOOSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_HELPER_CHOOSER))
#define BLXO_HELPER_CHOOSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_HELPER_CHOOSER, BlxoHelperChooserClass))

typedef struct _BlxoHelperChooserClass BlxoHelperChooserClass;
typedef struct _BlxoHelperChooser      BlxoHelperChooser;

GType              blxo_helper_chooser_get_type      (void) G_GNUC_CONST;

GtkWidget         *blxo_helper_chooser_new           (BlxoHelperCategory       category) G_GNUC_MALLOC;

BlxoHelperCategory  blxo_helper_chooser_get_category  (const BlxoHelperChooser *chooser);
void               blxo_helper_chooser_set_category  (BlxoHelperChooser       *chooser,
                                                     BlxoHelperCategory       category);

gboolean           blxo_helper_chooser_get_is_valid  (const BlxoHelperChooser *chooser);

G_END_DECLS

#endif /* !__BLXO_HELPER_CHOOSER_H__ */
