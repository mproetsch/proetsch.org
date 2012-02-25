//menubar.c: compile with gcc -o x.out x `pkg-config --libs --cflags gtk+-2.0`

#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
	
	GtkWidget *window;
	GtkWidget *vbox;
	
	GtkWidget *menubar;
	GtkWidget *filemenu;
	GtkWidget *file;
	GtkWidget *quit;
	GtkWidget *start;
	
	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);
	gtk_window_set_title(GTK_WINDOW(window), "menu");
	
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	menubar = gtk_menu_bar_new();
	filemenu = gtk_menu_new();
	
	file = gtk_menu_item_new_with_label("File");
	quit = gtk_menu_item_new_with_label("Quit");
	start = gtk_menu_item_new_with_label("Start");
	
	//Set the submenu of "file" to "filemenu", so that when the user clicks "file",
	//they actually open up the contents of the "filemenu" submenu
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	
	//Add GtkWidgets to filemenu
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), start);
	
	//Add GtkMenuItem file to GtkMenu menubar
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	
	//idk wat dis is
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 3);
	
	//Explanation:  filemenu is the submenu of file...
	//				file was originally a "gtk_menu_item" for menubar
	//				and now this gtk_menu_item has a submenu.
	//				quit is the member of this submenu
	//				so when you click "File", you're clicking the file submenu of
	//				menubar, and inside this file submenu is another submenu called
	//				filemenu, which has elements "quit".and "start"
		
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
		G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(quit), "activate",
		G_CALLBACK(gtk_main_quit), NULL);
		
	gtk_widget_show_all(window);
	
	gtk_main();
	
	return 0;
}
