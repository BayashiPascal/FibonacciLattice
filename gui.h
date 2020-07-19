#include "pberr.h"
#include "pbjson.h"
#include "pbmath.h"
#include "genbrush.h"
#include <gtk/gtk.h>

#define SIZE_DRAWABLE 400

#define appWins app.windows
#define appMainWin app.windows.main
#define appConf app.config
#define appInp app.inputs
#define appDraws app.drawables

typedef struct Drawing {

  GBSurface* surf;
  GBPixel color;
  GBInkSolid* ink;
  GBToolPlotter* tool;
  GBEyeOrtho* eye;
  GBHand hand;
  GBLayer* layer;
  GSet shapoids;
  float radiusCircle;
  int mode;
  int nbGrid;
  int nbPolar;

} Drawing;

typedef struct GUIWindows {

  // Main window of the application
  GtkWidget* main;

} GUIWindows;

typedef struct GUIConfig {

  // Path to the gui definition file
  char* gladeFilePath;

} GUIConfig;

typedef struct GUIInputs {

} GUIInputs;

typedef struct GUIDrawables {

  // GenBrush to draw on the widget
  GenBrush* gbWidgetDrawable;

} GUIDrawables;

typedef struct GUI {

  // Runtime configuration
  GUIConfig config;

  // Drawable widgets
  GUIDrawables drawables;

  // Windows
  GUIWindows windows;

  // Timer
  unsigned int timerId;

  // GTK application and its G version
  GtkApplication* gtkApp;
  GApplication* gApp;

} GUI;

// Declare the global instance of the application
extern GUI app;

// Function to init the windows
void GUIInitWindows(GtkBuilder* const gtkBuilder);

// Free memory used by the runtime configuration
void GUIFreeConf(void);

// Free memory used by the application
void GUIFree(void);

// Function called before the application quit
void GUIQuit(void);

// Callback function for the 'clicked' event on the quit button
gboolean CbBtnQuitClicked(
  GtkButton* btn,
    gpointer user_data);

// Callback function for the 'delete-event' event on the GTK application
// window
gboolean CbAppWindowDeleteEvent(
          GtkWidget* widget,
  GdkEventConfigure* event,
            gpointer user_data);

// Function to init the callbacks
void GUIInitCallbacks(GtkBuilder* const gtkBuilder);

// Callback function for the 'activate' event on the GTK application
void CbGtkAppActivate(
  GtkApplication* gtkApp,
         gpointer user_data);

// Parse the command line arguments
// Return true if the arguments were valid, false else
bool GUIParseArg(
     int argc,
  char** argv);

// Function to init the configuration
void GUIInitConf(
     int argc,
  char** argv);

// Function to init the drawables
void GUIInitDrawables(GtkBuilder* const gtkBuilder);

// Helper function to add a GBWidget into a GtkBox
void GUIPackGBWidget(
   GenBrush* gbWidget,
  GtkWidget* gtkBox);

// Free memory used by the drawables
void GUIFreeDrawables(void);

// Create an instance of the application
GUI GUICreate(
     int argc,
  char** argv);

// Main function of the application
int GUIMain(void);
