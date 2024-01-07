#include <gtk/gtk.h>
#include <unistd.h>

static int frame = 0;
static const int MAX_FRAMES = 10;

static GtkWidget *window, *backButton, *nextButton, *curFrameLabel, *table;
static GtkWidget *menuBar, *fileMenu, *fileMi, *openMi, *saveMi;


static void startQemu() {
    pid_t pid = fork();
    if (pid == 0) {
        // This is the child process. Start QEMU.
        execlp("qemu-system-x86_64", "qemu-system-x86_64", "-hda", "", NULL);
        // If we get here, starting QEMU failed.
        perror("execlp");
        exit(1);
    } else if (pid < 0) {
        // The fork failed.
        perror("fork");
        exit(1);
    }
}

static void updateLabel() {
	char str[32];
	snprintf(str, sizeof(str), "Frame: %d", frame);
	gtk_label_set_text(GTK_LABEL(curFrameLabel), str);
}

static void changeFrame(GtkWidget *widget, gpointer data) {
	int step = GPOINTER_TO_INT(data);
	if ((frame + step >= 0) && (frame + step <= MAX_FRAMES)) {
		frame += step;
		updateLabel();
	}
}

static void fileDialog(GtkMenuItem *menuitem, gpointer user_data) {
	GtkFileChooserAction action = (GtkFileChooserAction)GPOINTER_TO_INT(user_data);
	const char* title = (action == GTK_FILE_CHOOSER_ACTION_OPEN) ? "Open File" : "Save File";
	GtkWidget *dialog = gtk_file_chooser_dialog_new(title, GTK_WINDOW(window), action, "Cancel", GTK_RESPONSE_CANCEL, title, GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		// Perform file action
		g_free(filename);
	}
	gtk_widget_destroy(dialog);
}

static void createWindow(GtkApplication* app, gpointer user_data) {
	window = gtk_application_window_new(app);

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	menuBar = gtk_menu_bar_new();
	fileMenu = gtk_menu_new();

	fileMi = gtk_menu_item_new_with_label("File");
	openMi = gtk_menu_item_new_with_label("Open");
	saveMi = gtk_menu_item_new_with_label("Save");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMi);
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileMi);

	g_signal_connect(G_OBJECT(openMi), "activate", G_CALLBACK(fileDialog), GINT_TO_POINTER(GTK_FILE_CHOOSER_ACTION_OPEN));
	g_signal_connect(G_OBJECT(saveMi), "activate", G_CALLBACK(fileDialog), GINT_TO_POINTER(GTK_FILE_CHOOSER_ACTION_SAVE));

	gtk_container_add(GTK_CONTAINER(vbox), menuBar);

	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

	backButton = gtk_button_new_with_label("◀");
	g_signal_connect(backButton, "clicked", G_CALLBACK(changeFrame), GINT_TO_POINTER(-1));

	nextButton = gtk_button_new_with_label("▶");
	g_signal_connect(nextButton, "clicked", G_CALLBACK(changeFrame), GINT_TO_POINTER(1));

	curFrameLabel = gtk_label_new("Frame: 0");

	table = gtk_grid_new(); 
	gtk_grid_attach(GTK_GRID(table), backButton, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(table), nextButton, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(table), curFrameLabel, 2, 0, 1, 1);

	gtk_container_add(GTK_CONTAINER(vbox), table);	
	gtk_widget_show_all(window);
}

void GUI_Init(int argc, char *argv[]) {
	GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(createWindow), NULL);
	g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
}