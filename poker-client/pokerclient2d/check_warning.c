/* *
 * Copyright (C) 2004, 2005, 2006 Mekensleep <licensing@mekensleep.com>
 *                                24 rue vieille du temple, 75004 Paris
 *
 * This software's license gives you freedom; you can copy, convey,
 * propagate, redistribute and/or modify this program under the terms of
 * the GNU Affero General Public License (AGPL) as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version of the AGPL published by the FSF.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program in a file in the toplevel directory called
 * "AGPLv3".  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Henry Pr�cheur <henry@precheur.org>
 *
 */

#include <gtk/gtk.h>
#include <glade/glade.h>
#include "gui.h"
#include "interface_io.h"
#include "dispatcher.h"

static GtkWidget*	g_check_warning_window;
static gboolean		g_check_warning_window_shown = 0;
static GtkLayout*	g_screen = 0;

void	on_check_warning_fold_button_clicked(GtkWidget *widget, gpointer user_data)
{
  (void) widget;
  (void) user_data;

  g_message("fold");
  set_string("check_warning");
  set_string("fold");
  flush_io_channel();
  if (g_screen)
    gtk_widget_hide_all(g_check_warning_window);
}

void	on_check_warning_check_button_clicked(GtkWidget *widget, gpointer user_data)
{
  (void) widget;
  (void) user_data;

  g_message("check");
  set_string("check_warning");
  set_string("check");
  flush_io_channel();
  if (g_screen)
    gtk_widget_hide_all(g_check_warning_window);
}

void	on_check_warning_cancel_button_clicked(GtkWidget *widget, gpointer user_data)
{
  (void) widget;
  (void) user_data;

  g_message("cancel");
  set_string("check_warning");
  set_string("cancel");
  flush_io_channel();
  if (g_screen)
    gtk_widget_hide_all(g_check_warning_window);
}

int	handle_check_warning(GladeXML* g_glade_xml, GtkLayout* screen, int init)
{
  if (init)
    {
      g_screen = screen;
      g_check_warning_window = glade_xml_get_widget(g_glade_xml,
						    "check_warning_window");
      g_assert(g_check_warning_window);
      set_nil_draw_focus(g_check_warning_window);
      if(screen) gtk_layout_put(screen, g_check_warning_window, 0, 0);
      GUI_BRANCH(g_glade_xml, on_check_warning_fold_button_clicked);
      GUI_BRANCH(g_glade_xml, on_check_warning_check_button_clicked);
      GUI_BRANCH(g_glade_xml, on_check_warning_cancel_button_clicked);
    }

  
  if (screen != NULL || !g_check_warning_window_shown)
    {
      gui_center(g_check_warning_window, screen);
      g_check_warning_window_shown = 1;
    }

  return TRUE;
}
