#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct image {
  // The pixel data here is stored in row major order
  unsigned char *data;

  // Width and height of the image
  int sx;
  int sy;

} Image;

Image *newImage(int sx, int sy) {
  Image *im;

  im = (Image *)calloc(1, sizeof(Image));
  if (im != NULL) {
    im->sx = sx;
    im->sy = sy;
    im->data = (unsigned char *)calloc(im->sx * im->sy, sizeof(int));
    if (im->data != NULL) {
      memset(im->data, 255, sx * sy);
      return im;
    }
  }
  printf("Error: Unable to allocate memory for new image\n");
  return (NULL);
}

Image *copyImage(Image *src) {
  Image *im;

  im = (Image *)calloc(1, sizeof(Image));
  if (im != NULL) {
    im->sx = src->sx;
    im->sy = src->sy;
    im->data = (unsigned char *)calloc(im->sx * im->sy, sizeof(int));
    if (im->data != NULL) {
      memcpy(im->data, src->data, im->sx * im->sy * sizeof(unsigned char));
      return im;
    }
  }
  printf("Error: Unable to allocate memory for new image\n");
  return (NULL);
}

void deleteImage(Image *im) {
  free(im->data);
  free(im);
  return;
}

Image *readPGMimage(const char *filename) {
  FILE *f;
  Image *im;
  char line[1024], *tmp;
  int sizx, sizy;

  im = (Image *)calloc(1, sizeof(Image));
  if (im != NULL) {
    im->data = NULL;
    f = fopen(filename, "rb+");
    if (f == NULL) {
      printf(
          "Error: Unable to open file %s for reading, please check name and "
          "path\n",
          filename);
      free(im);
      return (NULL);
    }
    tmp = fgets(&line[0], 1000, f);
    if (strcmp(&line[0], "P5\n") != 0) {
      printf(
          "Error: Wrong file format, not a .pgm file or header end-of-line "
          "characters missing\n");
      free(im);
      fclose(f);
      return (NULL);
    }
    // Skip over comments
    tmp = fgets(&line[0], 511, f);
    while (line[0] == '#') tmp = fgets(&line[0], 511, f);
    sscanf(&line[0], "%d %d\n", &sizx, &sizy);  // Read file size
    im->sx = sizx;
    im->sy = sizy;

    tmp = fgets(&line[0], 9, f);  // Read the remaining header line
    im->data = (unsigned char *)calloc(sizx * sizy, sizeof(unsigned char));
    if (tmp == NULL) {
      printf("Error: Out of memory allocating space for image\n");
      free(im);
      fclose(f);
      return (NULL);
    }

    // Read the data
    fread(im->data, sizx * sizy * sizeof(unsigned char), 1, f);
    fclose(f);
    return (im);
  }

  printf("Error: Unable to allocate memory for image structure\n");
  return (NULL);
}

/* Outputs an image from the Image struct to a .pgm file */
void imageOutput(Image *im, const char *filename) {
  FILE *f;

  if (im != NULL)
    if (im->data != NULL) {
      f = fopen(filename, "wb+");
      if (f == NULL) {
        printf("Error: Unable to open file %s for output! No image written\n",
               filename);
        return;
      }
      fprintf(f, "P5\n");
      fprintf(f, "# Output from Quadtrees.c\n");
      fprintf(f, "%d %d\n", im->sx, im->sy);
      fprintf(f, "255\n");
      fwrite(im->data, im->sx * im->sy * sizeof(unsigned char), 1, f);
      fclose(f);
      return;
    }
  printf("Error: imageOutput(): Specified image is empty. Nothing output\n");
}
