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

#ifndef __BLXO_DIE_COMMAND_MODEL_H__
#define __BLXO_DIE_COMMAND_MODEL_H__

#include <blxo/blxo.h>

G_BEGIN_DECLS;

typedef struct _BlxoDieCommandModelClass BlxoDieCommandModelClass;
typedef struct _BlxoDieCommandModel      BlxoDieCommandModel;

#define BLXO_DIE_TYPE_COMMAND_MODEL            (blxo_die_command_model_get_type ())
#define BLXO_DIE_COMMAND_MODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BLXO_DIE_TYPE_COMMAND_MODEL, BlxoDieCommandModel))
#define BLXO_DIE_COMMAND_MODEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BLXO_DIE_TYPE_COMMAND_MODEL, BlxoDieCommandModelClass))
#define BLXO_DIE_IS_COMMAND_MODEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BLXO_DIE_TYPE_COMMAND_MODEL))
#define BLXO_DIE_IS_COMMAND_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BLXO_DIE_TYPE_COMMAND_MODEL))
#define BLXO_DIE_COMMAND_MODEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BLXO_DIE_TYPE_COMMAND_MODEL, BlxoDieCommandModelClass))

/**
 * BlxoDieCommandModelColumn:
 * @BLXO_DIE_COMMAND_MODEL_COLUMN_NAME : the column with the file name.
 *
 * The columns provided by the #BlxoDieCommandModel.
 **/
typedef enum /*< enum >*/
{
  BLXO_DIE_COMMAND_MODEL_COLUMN_NAME,
  BLXO_DIE_COMMAND_MODEL_N_COLUMNS,
} BlxoDieCommandModelColumn;

GType               blxo_die_command_model_get_type (void) G_GNUC_CONST;

BlxoDieCommandModel *blxo_die_command_model_new      (void) G_GNUC_MALLOC;

G_END_DECLS;

#endif /* !__BLXO_DIE_COMMAND_MODEL_H__ */
