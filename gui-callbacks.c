// Callback for the timer
gboolean CbTimer(gpointer data) {

  // Unused parameter
  (void)data;

  // Refresh all the widgets
  GUIRefreshWidgets();

  // Return TRUE to keep the timer alive
  // It will be killed by GUIFree
  return TRUE;

}

// Callback function for the 'clicked' event on the quit button
gboolean CbBtnQuitClicked(
  GtkButton* btn,
    gpointer user_data) {

  // Unused argument
  (void)btn;
  (void)user_data;

  // Quit the application
  GUIQuit();

  // Return true to stop the callback chain
  return TRUE;

}

// Callback function for the 'delete-event' event on the GTK application
// window
gboolean CbAppWindowDeleteEvent(
          GtkWidget* widget,
  GdkEventConfigure* event,
            gpointer user_data) {

  // Unused arguments
  (void)widget;
  (void)event;
  (void)user_data;

  // Quit the application
  GUIQuit();

  // Return false to continue the callback chain
  return FALSE;

}

// Callback function for the 'activate' event on the GTK application
void CbGtkAppActivate(
  GtkApplication* gtkApp,
         gpointer user_data) {

  // Unused arguments
  (void)gtkApp;
  (void)user_data;

  // Create a GTK builder with the GUI definition file
  GtkBuilder* gtkBuilder =
    gtk_builder_new_from_file("main.glade");

  // Set the GTK application in the GTK builder
  gtk_builder_set_application(
    gtkBuilder,
    app.gtkApp);

  // Init the windows
  GUIInitWindows(gtkBuilder);

  // Init the drawables
  GUIInitDrawables(gtkBuilder);

  // Init the drawing
  InitDrawing(appDraws.gbWidgetDrawable);

  // Init the callbacks
  GUIInitCallbacks(gtkBuilder);

  // Start the timer
  // 25 update per second -> 40ms
  unsigned int timerIntervalMs = 40;
  app.timerId = g_timeout_add (
    timerIntervalMs,
    CbTimer,
    NULL);

  // Free memory used by the GTK builder
  g_object_unref(G_OBJECT(gtkBuilder));

  // Refresh the drawable
  GUIRefreshWidgets();

  // Display the main window and all its components
  gtk_widget_show_all(appWins.main);

  // Run the application at the GTK level
  gtk_main();

}

gboolean CbKeyPress(
    GtkWidget* widget,
  GdkEventKey* event,
      gpointer user_data) {

  (void)widget;
  (void)user_data;

  printf(
    "key pressed: %c\n",
    event->keyval);
  if (event->state & GDK_SHIFT_MASK) {

    printf("shift pressed\n");

  }

  if (event->keyval == 'm') {

    drawing.mode = 1 - drawing.mode;
    Draw(appDraws.gbWidgetDrawable);

  }

  if (event->keyval == 'a') {

    if (drawing.mode == 0) {

      ++(drawing.nbGrid);

    } else {

      ++(drawing.nbPolar);

    }

    Draw(appDraws.gbWidgetDrawable);

  }

  if (event->keyval == 'z') {

    if (drawing.mode == 0 && drawing.nbGrid > 1) {

      --(drawing.nbGrid);

    } else if (drawing.nbPolar > 1) {

      --(drawing.nbPolar);

    }

    Draw(appDraws.gbWidgetDrawable);

  }

  return FALSE;

}
