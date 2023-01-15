//Copyright Lebada Daria-Cristiana 313CAa 2021-2022
#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define LMAX 3
#define CMAX 100
#define FMAX 50
#define KSIZE 3

typedef struct {
	double *pixel;
} image;

int image_type(char magic_word[LMAX]);

int image_color(char magic_word[LMAX]);

void ignore_comm(FILE *file);

image **alloc_matrix(int width, int height);

void free_matrix(image **a, int width, int height);

int pixels_number(int color);

image **copy_matrix(image **a, int width, int height, int color);

image **read_matrix_ascii(FILE *file, int width, int height, int color);

image **read_matrix_binary(FILE *file, int width, int height, int color);

void load_image(char filename[FMAX], image ***a, int *width, int *height,
				int *x1, int *y1, int *x2, int *y2, int *type, int *color,
				int *maxval, int *err);

void select_all(int *x1, int *y1, int *x2, int *y2, int width, int height);

void swap_coord(int *a, int *b);

int check_coord(int x1, int y1, int x2, int y2, int width, int height);

void select_img(char *instr, int *x1, int *y1, int *x2, int *y2,
				int width, int height);

void print_matrix(image **a, int width, int height, int color, int maxval);

image **crop_matrix(image **a, int *x1, int *y1, int *x2, int *y2,
					int *width, int *height, int color);

void find_magic_word(FILE *file, int ascii, int color);

void save_matrix_ascii(FILE *file, image **a, int width, int height, int color);

void save_matrix_binary(FILE *file, image **a, int width, int height,
						int color);

void save_matrix(char *instr, image **a, int width, int height, int color,
				 int maxval, int err);

image **rotate_matrix(image **a, int *width, int *height, int color);

void rotate_selection(image ***a, int x1, int y1, int y2, int color);

void rotate_img(image ***a, int *width, int *height, int *x1, int *y1, int *x2,
				int *y2, int color, int angle);

double new_color(double new, int maxval);

image **apply_edge(image **a, int x1, int y1, int x2, int y2, int width,
				   int height, int maxval, int color);

image **apply_sharpen(image **a, int x1, int y1, int x2, int y2, int width,
					  int height, int maxval, int color);

image **apply_blur(image **a, int x1, int y1, int x2, int y2, int width,
				   int height, int maxval, int color);

image **apply_gblur(image **a, int x1, int y1, int x2, int y2, int width,
					int height, int maxval, int color);

void apply_img(char *instr, image ***a, int x1, int y1, int x2, int y2,
			   int width, int height, int maxval, int color);

#endif
