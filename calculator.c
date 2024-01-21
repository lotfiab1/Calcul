#include <gtk/gtk.h>

GtkWidget *entry;

gchar *calculate_result(const gchar *expression);

void button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));

    if (g_strcmp0(button_label, "=") == 0) {
        const gchar *expression = gtk_entry_get_text(GTK_ENTRY(entry));
        gchar *result = calculate_result(expression);
        gtk_entry_set_text(GTK_ENTRY(entry), result);
        g_free(result);
    } else if (g_strcmp0(button_label, "C") == 0) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    } else {
        gchar *current_text = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
        gchar *new_text = g_strconcat(current_text, button_label, NULL);
        gtk_entry_set_text(GTK_ENTRY(entry), new_text);
        g_free(current_text);
        g_free(new_text);
    }
}

gchar *calculate_result(const gchar *expression) {
    gchar **tokens = g_strsplit_set(expression, " ", -1);
    if (g_strv_length(tokens) != 3) {
        g_strfreev(tokens);
        return g_strdup("Error");
    }

    double operand1 = g_ascii_strtod(tokens[0], NULL);
    double operand2 = g_ascii_strtod(tokens[2], NULL);
    gchar *operator = tokens[1];

    double result;

    if (g_strcmp0(operator, "+") == 0) {
        result = operand1 + operand2;
    } else if (g_strcmp0(operator, "-") == 0) {
        result = operand1 - operand2;
    } else if (g_strcmp0(operator, "*") == 0) {
        result = operand1 * operand2;
    } else if (g_strcmp0(operator, "/") == 0) {
        if (operand2 != 0) {
            result = operand1 / operand2;
        } else {
            g_strfreev(tokens);
            return g_strdup("Error: Division by zero");
        }
    } else {
        g_strfreev(tokens);
        return g_strdup("Error: Invalid operator");
    }

    g_strfreev(tokens);
    return g_strdup_printf("%g", result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    gchar *button_labels[] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+",
        "C"
    };

    for (int i = 0; i < 20; i++) {
        GtkWidget *button = gtk_button_new_with_label(button_labels[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
        gtk_grid_attach(GTK_GRID(grid), button, i % 4, 1 + i / 4, 1, 1);
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
