/* Inspiration: https://www.youtube.com/watch?v=AJIyRE2vZ_0&t=6213s
 * 
 * Took some inspiration (COPY :)) from **Tsoding** video.
 * Trying ppm images, just for fun.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void write_ppm(const char *file_path, size_t *pixels, size_t width, size_t hight) {
  FILE *file = fopen(file_path, "w");
  if (!file) {
    fprintf(stderr, "ERROR: Failed to open file \"%s\"\n", file_path);
  }
  fprintf(stdout, "Writing %s...\n", file_path);

  fprintf(file, "P6\n");
  fprintf(file, "%" PRIu64 " %" PRIu64 "\n255\n", width, hight);

  for (size_t h = 0; h < hight; ++h) {
    for (size_t w = 0; w < width; ++w) {
      size_t pixel = pixels[h * width + w];
      fputc((pixel >> 8 * 2) & 0xff, file); // red
      fputc((pixel >> 8 * 1) & 0xff, file); // green
      fputc((pixel >> 8 * 0) & 0xff, file); // blue
    }
  }

  fclose(file);
}

void fillwith(size_t *pixels, size_t width, size_t hight, size_t color) {
  for (size_t h = 0; h < hight; ++h) {
    for (size_t w = 0; w < width; ++w) {
      pixels[h * width + w] = color;
    }
  }
}

void checker_pattern(size_t *pixels, size_t width, size_t hight,
                     size_t tile_size, size_t foreground, size_t background) {
  for (size_t y = 0; y < hight; ++y) {
    for (size_t x = 0; x < width; ++x) {
      if ((x / tile_size + y / tile_size) % 2 == 0)
        pixels[y * width + x] = foreground;
      else
        pixels[y * width + x] = background;
    }
  }
}

void fill_circle(size_t *pixels, size_t width, size_t hight, size_t radius, size_t foreground, size_t background) {
  int cx = width;
  int cy = hight;
  int r = radius * 2;
  for (size_t y = 0; y < hight; ++y) {
    for (size_t x = 0; x < width; ++x) {
      int dx = (x * 2 + 1) - cx;
      int dy = (y * 2 + 1) - cy;
      if (dx * dx + dy * dy <= r * r)
        pixels[y * width + x] = foreground;
      else
        pixels[y * width + x] = background;
    }
  }
}

// color code
// 0xRRGGBB
int main(void) {
  size_t WIDTH = 1024;
  size_t HIGHT = 1024;
  size_t foreground = 0x66CC8F;
  size_t background = 0x993370;
  size_t *pixels = (size_t *)malloc(WIDTH * HIGHT * sizeof(size_t));

  fillwith(pixels, WIDTH, HIGHT, 0xff00ff);
  write_ppm("solid.ppm", pixels, WIDTH, HIGHT);

  fillwith(pixels, WIDTH, HIGHT, 0xff00ff);
  checker_pattern(pixels, WIDTH, HIGHT, 32, foreground, background);
  write_ppm("checker_pattern.ppm", pixels, WIDTH, HIGHT);

  fillwith(pixels, WIDTH, HIGHT, 0xff00ff);
  fill_circle(pixels, WIDTH, HIGHT, WIDTH / 2, foreground, background);
  write_ppm("fill_circle.ppm", pixels, WIDTH, HIGHT);

  free(pixels);
  return 0;
}
