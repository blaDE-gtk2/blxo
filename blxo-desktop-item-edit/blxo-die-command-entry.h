/*-
 * Copyright (c) 2006 Benedikt Meurer <benny@xfce.org>.
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

#ifndef __BLXO_DIE_COMMAND_ENTRY_H__
#define __BLXO_DIE_COMMAND_ENTRY_H__

#include <blxo/blxo.h>

G_BEGIN_DECLS;

typedef struct _BlxoDieCommandEntryClass BlxoDieCommandEntryClass;
typedef struct _BlxoDieCommandEntry      BlxoDieCommandEntry;

#define BLXO_DIE_TYPE_COMMAND_ENTRY            (blxo_die_command_entry_get_type ())
#define BLXO_DIE_COMMAND_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_DIE_TYPE_COMMAND_ENTRY, BlxoDieCommandEntry))
#define BLXO_DIE_COMMAND_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_DIE_TYPE_COMMAND_ENTRY, BlxoDieCommandEntryClass))
#define BLXO_DIE_IS_COMMAND_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_DIE_TYPE_COMMAND_ENTRY))
#define BLXO_DIE_IS_COMMAND_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_DIE_TYPE_COMMAND_ENTRY))
#define BLXO_DIE_COMMAND_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_DIE_TYPE_COMMAND_ENTRY, BlxoDieCommandEntryClass))

GType        blxo_die_command_entry_get_type (void) G_GNUC_CONST;

GtkWidget   *blxo_die_command_entry_new      (void) G_GNUC_MALLOC;

const gchar *blxo_die_command_entry_get_text (BlxoDieCommandEntry *command_entry);
void         blxo_die_command_entry_set_text (BlxoDieCommandEntry *command_entry,
                                             const gchar        *text);

G_END_DECLS;

#endif /* !__BLXO_DIE_COMMAND_ENTRY_H__ */
