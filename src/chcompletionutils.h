/*
 * chcompletionutils.h
 *
 * Copyright (C) 2007 -2009 Jesús Barbero Rodríguez <chuchiperriman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA.
 */
 
#ifndef __CH_COMPLETION_UTILS_H__
#define __CH_COMPLETION_UTILS_H__

#include <gtksourceview/gtksourceview.h>

G_BEGIN_DECLS

gboolean	 ch_completion_is_separator		(gunichar         ch);

gchar		*ch_completion_get_word_iter		(GtkSourceBuffer *source_buffer, 
							 GtkTextIter     *current,
							 GtkTextIter     *start_word, 
							 GtkTextIter     *end_word);

gchar		*ch_completion_get_word			(GtkSourceBuffer *text_view);

void		 ch_completion_replace_word		(GtkSourceBuffer *source_buffer,
							 GtkTextIter     *iter,
							 const gchar     *text,
							 gint             len);

void		 ch_completion_replace_current_word	(GtkSourceBuffer *source_buffer, 
							 const gchar     *text,
							 gint             len);

void		 ch_completion_move_to_iter		(GtkWindow       *window,
							 GtkSourceView   *view,
							 GtkTextIter     *iter);

void		 ch_completion_move_to_cursor		(GtkWindow       *window,
							 GtkSourceView   *view);

G_END_DECLS

#endif /* __CH_COMPLETION_UTILS_H__ */
