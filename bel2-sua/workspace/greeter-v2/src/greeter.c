/*
 ============================================================================
 Name        : template.c
 Author      : Alija Sabic
 Version     :
 Copyright   : Free for all.
 Description : Hello World in C, Gtk-style
 ============================================================================
 */
#include <gtk/gtk.h>
#include <string.h>

struct widgets {
	GtkWidget *label_output;
	GtkWidget *input_entry;
};

static void ok_clicked(GtkWidget *widget, gpointer data) {
	gchar *buffer1, *buffer2;

	// obtain references to the widgets passed as a generic data pointer
	struct widgets *w = (struct widgets*) data;

	// obtain text from the entry box
	buffer1 = (gchar*) gtk_entry_get_text(GTK_ENTRY(w->input_entry));
	// allocate memory for the final text
	buffer2 = g_malloc(sizeof(gchar) * (strlen(buffer1) + 7));
	// assemble the final text
	sprintf(buffer2, "Hello %s!", buffer1);
	// write the final text to the label on top
	gtk_label_set_text(GTK_LABEL(w->label_output), buffer2);
	// free the memory
	g_free(buffer2);
}

static void clr_clicked(GtkWidget *widget, gpointer data) {

	// obtain references to the widgets passed as a generic data pointer
	struct widgets *w = (struct widgets*) data;

	// clear the entry box
	gtk_entry_set_text(GTK_ENTRY(w->input_entry), "");
	// put the placeholder text into the entry box
	gtk_entry_set_placeholder_text(GTK_ENTRY(w->input_entry), "e.g., Muster");
	// clear the label
	gtk_label_set_text(GTK_LABEL(w->label_output), "Hello ?");
}

static void activate(GtkApplication* app, gpointer user_data) {
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *label_name;
	GtkWidget *clr_button, *ok_button;

	// obtain references to the widgets passed as a generic data pointer
	struct widgets *w = (struct widgets*) user_data;

	// create the window and set a title
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Welcome");

	// create a grid to be used as layout container
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	// output label
	w->label_output = gtk_label_new("Hello?");
	gtk_grid_attach(GTK_GRID(grid), w->label_output, 0, 0, 2, 1);

	// name label
	label_name = gtk_label_new("Name:");
	gtk_grid_attach(GTK_GRID(grid), label_name, 0, 1, 1, 1);

	// text entry
	w->input_entry = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), w->input_entry, 1, 1, 1, 1);
	// connect a signal when ENTER is hit -> invoke ok_clicked() callback
	// and pass a generic pointer to the struct containing references to some widgets
	g_signal_connect(w->input_entry, "activate", G_CALLBACK(ok_clicked), (gpointer)w);

	// CLEAR button
	clr_button = gtk_button_new_with_mnemonic("_Clear");
	gtk_grid_attach(GTK_GRID(grid), clr_button, 0, 2, 1, 1);
	// connect a signal when the button is clicked -> invoke clr_clicked() callback
	// and pass a generic pointer to the struct containing references to some widgets
	g_signal_connect(clr_button, "clicked", G_CALLBACK(clr_clicked), (gpointer)w);

	// OKAY button
	ok_button = gtk_button_new_with_mnemonic("_Okay");
	gtk_grid_attach(GTK_GRID(grid), ok_button, 1, 2, 1, 1);
	// connect a signal when the button is clicked -> invoke cok_clicked() callback
	// and pass a generic pointer to the struct containing references to some widgets
	g_signal_connect(ok_button, "clicked", G_CALLBACK(ok_clicked), (gpointer)w);

	gtk_widget_show_all(window);
}

int main (int argc, char **argv) {
	GtkApplication *app;
	int status;

	// we need some memory for the widgets struct
	struct widgets *w = g_malloc(sizeof(struct widgets));

	// create a threaded application
	app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer)w);
	// run the application -> emits an "activate" signal
	status = g_application_run (G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	// free the memory for the widgets struct
	g_free(w);
	w = NULL;

	return status;
}
