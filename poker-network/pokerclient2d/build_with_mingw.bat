%POK3DROOT%/underware/envwin32/MinGW/bin/gcc.exe -o _pokerinterface2_4.dll blind.c buy_in.c cashier.c chat.c check_warning.c chooser.c credits.c dispatcher.c gui.c hand_history.c lobby.c login.c menu.c message_box.c muck.c outfits.c python.c sit_actions.c tournaments.c util.c yesno_message.c -DPYTHON_VERSION=\"2_4\" -DVERSION_NAME(a)=a##2_4 -IC:/POK3DROOT/underware/envwin32/python/include -IC:/GTK/include/libglade-2.0 -IC:/GTK/include/gtk-2.0 -IC:/GTK/include/libxml2 -IC:/GTK/lib/gtk-2.0/include -IC:/GTK/include/atk-1.0 -IC:/GTK/include/cairo -IC:/GTK/include/pango-1.0 -IC:/GTK/include/glib-2.0 -IC:/GTK/lib/glib-2.0/include -IC:/GTK/include/libxml2 -IC:/GTK/include -shared -LC:/POK3DROOT/underware/envwin32/python/libs -lpython24 -LC:/GTK/lib -lglib-2.0 -lgtk-win32-2.0 -lglade-2.0 -lgobject-2.0 -lintl -liconv -lgdk-win32-2.0 -lgdk_pixbuf-2.0 -lxml2