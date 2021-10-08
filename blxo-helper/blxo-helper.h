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

#ifndef __BLXO_HELPER_H__
#define __BLXO_HELPER_H__

#include <blxo/blxo.h>

G_BEGIN_DECLS

typedef enum /*< enum,prefix=BLXO_HELPER >*/
{
  BLXO_HELPER_WEBBROWSER,        /*< nick=WebBrowser >*/
  BLXO_HELPER_MAILREADER,        /*< nick=MailReader >*/
  BLXO_HELPER_FILEMANAGER,       /*< nick=FileManager >*/
  BLXO_HELPER_TERMINALEMULATOR,  /*< nick=TerminalEmulator >*/
  BLXO_HELPER_N_CATEGORIES,      /*< skip >*/
} BlxoHelperCategory;

typedef struct _BlxoHelperClass BlxoHelperClass;
typedef struct _BlxoHelper      BlxoHelper;

#define BLXO_TYPE_HELPER            (blxo_helper_get_type ())
#define BLXO_HELPER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_HELPER, BlxoHelper))
#define BLXO_HELPER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_HELPER, BlxoHelperClass))
#define BLXO_IS_HELPER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_HELPER))
#define BLXO_IS_HELPER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_HELPER))
#define BLXO_HELPER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_HELPER, BlxoHelperClass))

GType              blxo_helper_get_type      (void) G_GNUC_CONST;
BlxoHelperCategory  blxo_helper_get_category  (const BlxoHelper   *helper);
const gchar       *blxo_helper_get_id        (const BlxoHelper   *helper);
const gchar       *blxo_helper_get_name      (const BlxoHelper   *helper);
const gchar       *blxo_helper_get_icon      (const BlxoHelper   *helper);
const gchar       *blxo_helper_get_command   (const BlxoHelper   *helper);
gboolean           blxo_helper_execute       (BlxoHelper         *helper,
                                             GdkScreen         *screen,
                                             const gchar       *parameter,
                                             GError           **error);


#define BLXO_TYPE_HELPER_DATABASE             (blxo_helper_database_get_type ())
#define BLXO_HELPER_DATABASE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_TYPE_HELPER_DATABASE, BlxoHelperDatabase))
#define BLXO_HELPER_DATABASE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_TYPE_HELPER_DATABASE, BlxoHelperDatabaseClass))
#define BLXO_IS_HELPER_DATABASE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_TYPE_HELPER_DATABASE))
#define BLXO_IS_HELPER_DATABASE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_TYPE_HELPER_DATABASE))
#define BLXO_HELPER_DATABASE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_TYPE_HELPER_DATABASE, BlxoHelperDatabaseClass))

typedef struct _BlxoHelperDatabaseClass BlxoHelperDatabaseClass;
typedef struct _BlxoHelperDatabase      BlxoHelperDatabase;

GType               blxo_helper_database_get_type        (void) G_GNUC_CONST;
BlxoHelperDatabase  *blxo_helper_database_get             (void);
BlxoHelper          *blxo_helper_database_get_default     (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category);
gboolean            blxo_helper_database_set_default     (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category,
                                                         BlxoHelper         *helper,
                                                         GError           **error);
gboolean            blxo_helper_database_clear_default   (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category,
                                                         GError           **error);
GList              *blxo_helper_database_get_all         (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category);
BlxoHelper          *blxo_helper_database_get_custom      (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category);
void                blxo_helper_database_set_custom      (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category,
                                                         const gchar       *command);
gboolean            blxo_helper_database_get_dismissed   (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category);
gboolean            blxo_helper_database_set_dismissed   (BlxoHelperDatabase *database,
                                                         BlxoHelperCategory  category,
                                                         gboolean           dismissed);

G_END_DECLS

#endif /* !__BLXO_HELPER_H__ */
