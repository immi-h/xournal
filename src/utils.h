/****************
 * this file mostly contains structs that are would otherwise cause circular dependencies.
 * This is mostly a workaround for the - lets call it - odd structure of the files in the project.
 *
 * ***************/
#ifndef UTILS
#define UTILS

typedef struct BBox {
  double left, right, top, bottom;
} BBox;

#endif // UTILS

