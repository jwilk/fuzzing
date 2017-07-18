/* This is a small test program for FreeType,
 * which can be used as a fuzzing target.
 * It was adapted from:
 * https://www.freetype.org/freetype2/docs/tutorial/example1.c
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH 140
#define HEIGHT 60

unsigned char image[HEIGHT][WIDTH];

void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
  FT_Int i, j, p, q;
  FT_Int x_max = x + bitmap->width;
  FT_Int y_max = y + bitmap->rows;
  for (i = x, p = 0; i < x_max; i++, p++) {
    for (j = y, q = 0; j < y_max; j++, q++) {
      if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
        continue;
      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

void show_image(void)
{
  int  i, j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++)
      putchar(image[i][j] == 0 ? ' ' : image[i][j] < 128 ? '+' : '*');
    putchar('\n');
  }
}

int main(int argc, char** argv)
{
  FT_Library library;
  FT_Face face;
  FT_GlyphSlot slot;
  FT_Matrix matrix;
  FT_Vector pen;
  FT_Error error;
  char* text;
  char* filename;
  double angle;
  int target_height;
  int n, num_chars;
  if (argc != 3) {
    fprintf(stderr, "usage: %s font sample-text\n", argv[0]);
    exit(1);
  }
  filename = argv[1];
  text = argv[2];
  num_chars = strlen(text);
  angle = (25.0 / 360) * 3.14159 * 2;  /* 25° */
  target_height = HEIGHT;
  error = FT_Init_FreeType(&library);
  if (error)
    exit(1);
#ifdef __AFL_HAVE_MANUAL_CONTROL
  __AFL_INIT();
#endif
  error = FT_New_Face(library, filename, 0, &face);
  if (error)
    exit(1);
  error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0);
  if (error)
    exit(1);
  slot = face->glyph;
  matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
  matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
  matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
  matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);
  pen.x = 30 * 64;
  pen.y = (target_height - 60) * 64;
  for (n = 0; n < num_chars; n++) {
    FT_Set_Transform(face, &matrix, &pen);
    error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
    if (error)
      continue;
    draw_bitmap(&slot->bitmap, slot->bitmap_left, target_height - slot->bitmap_top);
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }
  show_image();
  FT_Done_Face(face);
  return 0;
}
