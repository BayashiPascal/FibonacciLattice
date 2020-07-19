#include "gui.h"

// Error manager
PBErr* AppErr = &thePBErr;

// Declare the global instance of the application
GUI app;

// Include the drawing functions
#include "drawing.c"

// Function to refresh the content of all graphical widgets
void GUIRefreshWidgets(void);

// Include the callbacks
#include "gui-callbacks.c"

// Function to init the windows
void GUIInitWindows(GtkBuilder* const gtkBuilder) {

#if BUILDMODE == 0

  if (gtkBuilder == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'gtkBuilder' is null");
    PBErrCatch(AppErr);

  }

#endif

  // Get the main window
  GObject* mainWindow=
    gtk_builder_get_object(
      gtkBuilder,
      "appMainWin");
  appWins.main = GTK_WIDGET(mainWindow);

  // Allow window resizing
  gtk_window_set_resizable(
    GTK_WINDOW(appWins.main),
    TRUE);

}

// Free memory used by the runtime configuration
void GUIFreeConf(void) {

  free(appConf.gladeFilePath);

}

// Free memory used by the drawables
void GUIFreeDrawables(void) {

  GBFree(&(appDraws.gbWidgetDrawable));

}

// Function to refresh the content of all graphical widgets
void GUIRefreshWidgets(void) {

  // Update the widget
  GBUpdate(appDraws.gbWidgetDrawable);
  GBRender(appDraws.gbWidgetDrawable);

  // Give back the hand to the main loop to process pending events
  while(gtk_events_pending()) {

    gtk_main_iteration();

  }

}

// Free memory used by the application
void GUIFree(void) {

  // Stop the timer
  bool ret = g_source_remove(app.timerId);
  (void)ret;

  // Free the drawable
  GUIFreeDrawables();

  // Free memory
  GUIFreeConf();

}

// Function called before the application quit
void GUIQuit(void) {

  // Free the memory used by the drawing
  FreeDrawing();

  // Free memory used by the GUI
  GUIFree();

  // Quit the application at GTK level
  gtk_main_quit();

  // Quit the application at G level
  g_application_quit(app.gApp);

}

// Function to init the callbacks
void GUIInitCallbacks(GtkBuilder* const gtkBuilder) {

#if BUILDMODE == 0

  if (gtkBuilder == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'gtkBuilder' is null");
    PBErrCatch(AppErr);

  }

#endif

  // Set the callback on the delete-event of the main window
  g_signal_connect(
    appWins.main,
    "delete-event",
    G_CALLBACK(CbAppWindowDeleteEvent),
    NULL);

  // Set the callback on the 'clicked' event of the quit button
  GObject* obj =
    gtk_builder_get_object(
      gtkBuilder,
      "btnQuit");
  GtkWidget* btnQuit = GTK_WIDGET(obj);
  g_signal_connect(
    btnQuit,
    "clicked",
    G_CALLBACK(CbBtnQuitClicked),
    NULL);

  GObject* window =
    gtk_builder_get_object(
      gtkBuilder,
      "appMainWin");
  g_signal_connect (
    window,
    "key_press_event",
    G_CALLBACK(CbKeyPress),
    NULL);

  // Disable the other signals defined in the UI definition file
  gtk_builder_connect_signals(
    gtkBuilder,
    NULL);

}

// Function to init the configuration
void GUIInitConf(
     int argc,
  char** argv) {

#if BUILDMODE == 0

  if (argv == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'argv' is null");
    PBErrCatch(AppErr);

  }

  if (argc < 0) {

    AppErr->_type = PBErrTypeInvalidArg;
    sprintf(
      AppErr->_msg,
      "'argc' is invalid (%d>=0)",
      argc);
    PBErrCatch(AppErr);

  }

#endif

  // Init the default path for the Glade file
  appConf.gladeFilePath = strdup("./main.glade");

}

// Function to init the drawables
void GUIInitDrawables(GtkBuilder* const gtkBuilder) {

#if BUILDMODE == 0

  if (gtkBuilder == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'gtkBuilder' is null");
    PBErrCatch(AppErr);

  }

#endif

  // Create the GBWidget for the control widget
  VecShort2D dimGBWidget = VecShortCreateStatic2D();
  VecSet(
    &dimGBWidget,
    0,
    SIZE_DRAWABLE);
  VecSet(
    &dimGBWidget,
    1,
    SIZE_DRAWABLE);
  appDraws.gbWidgetDrawable = GBCreateWidget(&dimGBWidget);

  // Pack the widget in the layout
  GtkWidget* layMain = GTK_WIDGET(
    gtk_builder_get_object(
      gtkBuilder,
      "layMain"));
  GUIPackGBWidget(
    appDraws.gbWidgetDrawable,
    layMain);

}

// Helper function to add a GBWidget into a GtkBox
void GUIPackGBWidget(
   GenBrush* gbWidget,
  GtkWidget* gtkBox) {

  // Create the widget containers for the GBwidget
  GtkWidget* container =
    gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      0);

  // Get the dimension of the GbWidget
  VecShort2D* dimGBWidget = GBDim(gbWidget);

  // Fix the size of the GtkWidget container
  short w =
    VecGet(
      dimGBWidget,
      0);
  short h =
    VecGet(
      dimGBWidget,
      1);
  gtk_widget_set_size_request(
    container,
    w,
    h);
  gtk_widget_set_hexpand(
    container,
    FALSE);
  gtk_widget_set_vexpand(
    container,
    FALSE);

  // Get the GtkWidget from the GBwidgets
  GtkWidget* widget = GBGetGtkWidget(gbWidget);

  // Fix the size of the GtkWidget
  gtk_widget_set_hexpand(
    widget,
    FALSE);
  gtk_widget_set_vexpand(
    widget,
    FALSE);

  // Insert the GBwidget into its container
  gtk_box_pack_start(
    GTK_BOX(container),
    widget,
    FALSE,
    FALSE,
    0);

  // Insert the GBwidget container into the layout
  gtk_box_pack_start(
    GTK_BOX(gtkBox),
    container,
    FALSE,
    FALSE,
    0);

}

// Create an instance of the application
GUI GUICreate(
     int argc,
  char** argv) {

#if BUILDMODE == 0

  if (argv == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'argv' is null");
    PBErrCatch(AppErr);

  }

  if (argc < 0) {

    AppErr->_type = PBErrTypeInvalidArg;
    sprintf(
      AppErr->_msg,
      "'argc' is invalid (%d>=0)",
      argc);
    PBErrCatch(AppErr);

  }

#endif

  // Init the runtime configuration
  GUIInitConf(
    argc,
    argv);

  // Create a GTK application
  app.gtkApp = gtk_application_new(
    NULL,
    G_APPLICATION_FLAGS_NONE);
  app.gApp = G_APPLICATION(app.gtkApp);

  // Connect the callback function on the 'activate' event of the GTK
  // application
  g_signal_connect(
    app.gtkApp,
    "activate",
    G_CALLBACK(CbGtkAppActivate),
    NULL);

  // Return the instance of the application
  return app;

}

// Main function of the application
int GUIMain(void) {

  // Run the application at the G level
  int status =
    g_application_run(
      app.gApp,
      0,
      NULL);

  // If the application failed
  if (status != 0) {

    printf(
      "g_application_run failed (%d)",
      status);

  }

  // Unreference the GTK application
  g_object_unref(app.gtkApp);

  // Return the status code
  return status;

}
