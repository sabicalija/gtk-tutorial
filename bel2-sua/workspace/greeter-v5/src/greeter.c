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

static void ok_clicked (GtkWidget *widget, gpointer data)
{
    gchar *buffer1, *buffer2;

    // obtain references to the widgets passed as a generic data pointer
    struct widgets *w = (struct widgets*) data;

    // obtain text from the entry box
    buffer1 = (gchar*) gtk_entry_get_text (GTK_ENTRY (w->input_entry));
    // allocate memory for the final text
    buffer2 = g_malloc (sizeof (gchar) * (strlen (buffer1) + 7));
    // assemble the final text
    sprintf (buffer2, "Hello %s!", buffer1);
    // write the final text to the label on top
    gtk_label_set_text (GTK_LABEL (w->label_output), buffer2);
    // free the memory
    g_free (buffer2);
}

static void clr_clicked (GtkWidget *widget, gpointer data)
{

    // obtain references to the widgets passed as a generic data pointer
    struct widgets *w = (struct widgets*) data;

    // clear the entry box
    gtk_entry_set_text (GTK_ENTRY (w->input_entry), "");
    // put the placeholder text into the entry box
    gtk_entry_set_placeholder_text (GTK_ENTRY (w->input_entry), "e.g., Muster");
    // clear the label
    gtk_label_set_text (GTK_LABEL (w->label_output), "Hello ?");
}

static void apply_css (GtkWidget *widget, GtkStyleProvider *provider)
{
    gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
    if (GTK_IS_CONTAINER (widget)) {
        gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
    }
}

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label_name;
    GtkWidget *clr_button, *ok_button;
    GtkWidget *headerbar;
    GtkStyleContext *context;
    GtkStyleProvider *provider;
    GtkWidget *box;

    // obtain references to the widgets passed as a generic data pointer
    struct widgets *w = (struct widgets*) user_data;

    // create the window and set a title and icon
    window = gtk_application_window_new (app);
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_window_set_default_icon_from_file ("img/ico.png", NULL);
    // gtk_window_set_title(GTK_WINDOW(window), "Welcome");
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

    // create a grid to be used as layout container
    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);
    gtk_container_add (GTK_CONTAINER (window), grid);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    // output label using fancy font from Google Fonts
    // NOTE - the box is only used to make the label more
    //        convenient selectable using the CSS - see below
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    w->label_output = gtk_label_new ("Hello ?");
    // name the öabel so that we can reference it from the CSS file
    gtk_widget_set_name (w->label_output, "label_output");
    gtk_box_pack_start (GTK_BOX (box), w->label_output, TRUE, TRUE, 0);
    gtk_widget_set_size_request (w->label_output, 400, 50);
    gtk_grid_attach (GTK_GRID (grid), box, 0, 0, 2, 1);

    // name label - label text horizontally aligned towards the end
    label_name = gtk_label_new ("Name:");
    gtk_widget_set_halign (label_name, GTK_ALIGN_END);
    gtk_widget_set_size_request (label_name, 60, 40);
    gtk_grid_attach (GTK_GRID (grid), label_name, 0, 1, 1, 1);

    // text entry with a placeholder text
    w->input_entry = gtk_entry_new();
    gtk_grid_attach (GTK_GRID (grid), w->input_entry, 1, 1, 1, 1);
    gtk_widget_set_size_request (w->input_entry, 60, 30);
    gtk_entry_set_placeholder_text (GTK_ENTRY (w->input_entry), "e.g., Muster");
    // connect a signal when ENTER is hit -> invoke ok_clicked() callback
    // and pass a generic pointer to the struct containing references to some widgets
    g_signal_connect (w->input_entry, "activate", G_CALLBACK (ok_clicked), (gpointer) w);

    // create a headerbar
    headerbar = gtk_header_bar_new();
    gtk_widget_show (headerbar);
    gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), "GNOME Greeter");
    gtk_header_bar_set_subtitle (GTK_HEADER_BAR (headerbar), "A simple demo application");
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerbar), TRUE);
    gtk_window_set_titlebar (GTK_WINDOW (window), headerbar);

    // put a red clear button to the left side of the header bar
    clr_button = gtk_button_new_with_mnemonic ("_Clear");
    context = gtk_widget_get_style_context (clr_button);
    gtk_style_context_add_class (context, "text-button");
    gtk_style_context_add_class (context, "destructive-action");
    gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), clr_button);
    // connect a signal when the button is clicked -> invoke clr_clicked() callback
    // and pass a generic pointer to the struct containing references to some widgets
    g_signal_connect (clr_button, "clicked", G_CALLBACK (clr_clicked), (gpointer) w);

    // put a blue okay button to the left side of the header bar
    ok_button = gtk_button_new_with_mnemonic ("_Okay");
    context = gtk_widget_get_style_context (ok_button);
    gtk_style_context_add_class (context, "text-button");
    gtk_style_context_add_class (context, "suggested-action");
    gtk_header_bar_pack_end (GTK_HEADER_BAR (headerbar), ok_button);
    // connect a signal when the button is clicked -> invoke cok_clicked() callback
    // and pass a generic pointer to the struct containing references to some widgets
    g_signal_connect (ok_button, "clicked", G_CALLBACK (ok_clicked), (gpointer) w);

    // add a fancy background image
    provider = GTK_STYLE_PROVIDER (gtk_css_provider_new());
    gtk_css_provider_load_from_resource (GTK_CSS_PROVIDER (provider), "/greeter/style/greeter.css");
    apply_css (window, provider);

    gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    // we need some memory for the widgets struct
    struct widgets *w = g_malloc (sizeof (struct widgets));

    // create a threaded application
    app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), (gpointer) w);
    // run the application -> emits an "activate" signal
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    // free the memory for the widgets struct
    g_free (w);
    w = NULL;

    return status;
}
