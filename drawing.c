// Functino to init the drawing
void InitDrawing(GenBrush* that);

// Main drawing function
void Draw(GenBrush* that);

// Function to free the memory used by the drawing
void FreeDrawing(void);

// Declare the global instance of the drawing
Drawing drawing;

// Init drawing function
void InitDrawing(GenBrush* that) {

#if BUILDMODE == 0

  if (that == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'that' is null");
    PBErrCatch(AppErr);

  }

#endif

  // Get the surface of the GenBrush
  drawing.surf = GBSurf(that);

  // Create the objects used to draw the genealogy
  drawing.color = GBColorWhite;
  drawing.color._rgba[GBPixelRed] = 0;
  drawing.color._rgba[GBPixelGreen] = 0;
  drawing.color._rgba[GBPixelBlue] = 0;
  drawing.ink = GBInkSolidCreate(&(drawing.color));
  drawing.tool = GBToolPlotterCreate();
  drawing.eye = GBEyeOrthoCreate(GBEyeOrthoViewFront);
  drawing.hand = GBHandCreateStatic(GBHandTypeDefault);
  drawing.radiusCircle = 5.0;
  drawing.mode = 0;
  drawing.nbGrid = 1;
  drawing.nbPolar = 1;

  // Display the commands
  printf("m: switch mode between grid and polar\n");
  printf("a: increase the number of points\n");
  printf("z: decrease the number of points\n");

  // Create the layer for the drawing
  drawing.layer =
    GBSurfaceAddLayer(
      drawing.surf,
      GBDim(that));
  GBLayerSetStackPos(
    drawing.layer,
    GBLayerStackPosFg);
  GBLayerSetBlendMode(
    drawing.layer,
    GBLayerBlendModeOver);

  // Create a GSet to keep track of the shapoids
  drawing.shapoids = GSetCreateStatic();

}

// Function to free the memory used by the drawing
void FreeDrawing(void) {

  while (GSetNbElem(&(drawing.shapoids)) > 0) {

    free(GSetPop(&(drawing.shapoids)));

  }

  GBInkSolidFree(&(drawing.ink));
  GBToolPlotterFree(&(drawing.tool));
  GBEyeOrthoFree(&(drawing.eye));

}

// Main drawing function
void DrawGrid(GenBrush* that) {

#if BUILDMODE == 0

  if (that == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'that' is null");
    PBErrCatch(AppErr);

  }

#endif

  // Reset all the pixels to the background color
  GBFlush(that);

  // Remove all the shapoids
  while (GSetNbElem(&(drawing.shapoids)) > 0) {

    free(GSetPop(&(drawing.shapoids)));

  }

  // Remove all the pods in the GenBrush
  while (GSetNbElem(GBPods(that)) > 0) {

    free(GSetPop(GBPods(that)));

  }

  // Add the shapoids according to the Fibonacci lattice
  unsigned long nbPoints = 0;
  float* lattice =
    GetFibonacciGridLattice(
      drawing.nbGrid,
      &nbPoints);
  for (
    unsigned int iPoint = 0;
    iPoint < nbPoints;
    ++iPoint) {

    Spheroid* circle = SpheroidCreate(2);
    GSetPush(
      &(drawing.shapoids),
      circle);
    VecFloat2D w = VecFloatCreateStatic2D();
    VecSet(
      &w,
      0,
      drawing.radiusCircle);
    VecSet(
      &w,
      1,
      drawing.radiusCircle);
    ShapoidScale(
      circle,
      (VecFloat*)&w);
    VecSet(
      &w,
      0,
      lattice[iPoint * 2L] * SIZE_DRAWABLE);
    VecSet(
      &w,
      1,
      lattice[iPoint * 2L + 1L] * SIZE_DRAWABLE);
    ShapoidTranslate(
      circle,
      (VecFloat*)&w);
    GBObjPod* pod =
      GBAddShapoid(
        that,
        circle,
        &(drawing.eye),
        &(drawing.hand),
        drawing.tool,
        drawing.ink,
        drawing.layer);
    (void)pod;

  }

  free(lattice);

}

void DrawPolar(GenBrush* that) {

#if BUILDMODE == 0

  if (that == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'that' is null");
    PBErrCatch(AppErr);

  }

#endif

  // Reset all the pixels to the background color
  GBFlush(that);

  // Remove all the shapoids
  while (GSetNbElem(&(drawing.shapoids)) > 0) {

    free(GSetPop(&(drawing.shapoids)));

  }

  // Remove all the pods in the GenBrush
  while (GSetNbElem(GBPods(that)) > 0) {

    free(GSetPop(GBPods(that)));

  }

  // Add the shapoids according to the Fibonacci lattice
  unsigned long nbPoints = 0;
  float* lattice =
    GetFibonacciPolarLattice(
      drawing.nbPolar,
      &nbPoints);
  for (
    unsigned int iPoint = 0;
    iPoint < nbPoints;
    ++iPoint) {

    Spheroid* circle = SpheroidCreate(2);
    GSetPush(
      &(drawing.shapoids),
      circle);
    VecFloat2D w = VecFloatCreateStatic2D();
    VecSet(
      &w,
      0,
      drawing.radiusCircle);
    VecSet(
      &w,
      1,
      drawing.radiusCircle);
    ShapoidScale(
      circle,
      (VecFloat*)&w);
    VecSet(
      &w,
      0,
      1.0);
    VecSet(
      &w,
      1,
      0.0);
    VecRot(
      &w,
      lattice[iPoint * 2L + 1L]);
    VecScale(
      &w,
      lattice[iPoint * 2L] * 0.49 * SIZE_DRAWABLE);
    VecFloat2D center = VecFloatCreateStatic2D();
    VecSet(
      &center,
      0,
      0.5 * SIZE_DRAWABLE);
    VecSet(
      &center,
      1,
      0.5 * SIZE_DRAWABLE);
    VecOp(
      &w,
      1.0,
      &center,
      1.0);

    ShapoidTranslate(
      circle,
      (VecFloat*)&w);
    GBObjPod* pod =
      GBAddShapoid(
        that,
        circle,
        &(drawing.eye),
        &(drawing.hand),
        drawing.tool,
        drawing.ink,
        drawing.layer);
    (void)pod;

  }

  free(lattice);

}

void Draw(GenBrush* that) {

#if BUILDMODE == 0

  if (that == NULL) {

    AppErr->_type = PBErrTypeNullPointer;
    sprintf(
      AppErr->_msg,
      "'that' is null");
    PBErrCatch(AppErr);

  }

#endif

  if (drawing.mode == 0) {

    DrawGrid(that);

  } else {

    DrawPolar(that);

  }

}
