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

void ok_clicked (GtkWidget *widget, gpointer data)
{
    gchar *buffer1, *buffer2;
    struct widgets *w = (struct widgets*) data;

    buffer1 = (gchar*) gtk_entry_get_text (GTK_ENTRY (w->input_entry));
    buffer2 = g_malloc (sizeof (gchar) * (strlen (buffer1) + 7));
    sprintf (buffer2, "Hello %s!", buffer1);
    gtk_label_set_text (GTK_LABEL (w->label_output), buffer2);
    g_free (buffer2);
}

void clr_clicked (GtkWidget *widget, gpointer data)
{
    struct widgets *w = (struct widgets*) data;

    gtk_entry_set_text (GTK_ENTRY (w->input_entry), "");
    gtk_entry_set_placeholder_text (GTK_ENTRY (w->input_entry), "e.g., Muster");
    gtk_label_set_text (GTK_LABEL (w->label_output), "Hello ?");
}

void destroy_clicked (GtkWidget *widget, gpointer data) {
    struct widgets *w = (struct widgets*) data;

    g_free(w);
    gtk_main_quit();
}

static void apply_css (GtkWidget *widget, GtkStyleProvider *provider)
{
    gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
    if (GTK_IS_CONTAINER (widget)) {
        gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
    }
}

int main (int argc, char **argv)
{
    GtkWidget *window;
    GtkBuilder *builder;
    GtkStyleProvider *provider;

    // we need some memory for the widgets struct
    struct widgets *w = g_malloc (sizeof (struct widgets));

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "res/greeter.ui", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_builder_connect_signals(builder, (gpointer)w);

    w->input_entry = GTK_WIDGET(gtk_builder_get_object(builder, "input_entry"));
    w->label_output = GTK_WIDGET(gtk_builder_get_object(builder, "label_output"));

    provider = GTK_STYLE_PROVIDER (gtk_css_provider_new());
    gtk_css_provider_load_from_resource (GTK_CSS_PROVIDER (provider), "/greeter/style/greeter.css");
    apply_css (window, provider);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}
