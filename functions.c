//Copyright Lebada Daria-Cristiana 313CAa 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "functions.h"

//functie care returneaza daca imaginea este de tip ascii sau binar
int image_type(char magic_word[LMAX])
{
	char type_ascii[LMAX][LMAX] = {"P1", "P2", "P3"};
	char type_binary[LMAX][LMAX] = {"P4", "P5", "P6"};
	//Notam 1 = ascii, 2 = binar, 0 = eroare
	int t = 0;
	for (int i = 0; i < LMAX; i++)
		if (strcmp(type_ascii[i], magic_word) == 0)
			t = 1;
		else if (strcmp(type_binary[i], magic_word) == 0)
			t = 2;

	return t;
}

//functie care returneaza daca poza este alb-negru, grey sau color
int image_color(char magic_word[LMAX])
{
	//Notam alb-negru = 0, gri = 1, color = 2
	char black_white[LMAX][LMAX] = {"P1", "P4"};
	char grey[LMAX][LMAX] = {"P2", "P5"};
	char color[LMAX][LMAX] = {"P3", "P6"};

	for (int i = 0; i < LMAX; i++) {
		if (strcmp(black_white[i], magic_word) == 0)
			return 0;
		if (strcmp(grey[i], magic_word) == 0)
			return 1;
		if (strcmp(color[i], magic_word) == 0)
			return 2;
	}
	return -1;
}

//functie care verifica existenta comentariilor
void ignore_comm(FILE *file)
{
	char ch;
	fscanf(file, "%c", &ch);
	//verificam daca primul caracter e '#'
	if (ch == '#') {
		//citim toata linia
		while (ch != '\n')
			fscanf(file, "%c", &ch);
	} else {
		//nu e comentariu, deci revenim la
		//pozitia initiala
		fseek(file, -1, SEEK_CUR);
	}
}

//functie de alocare a unei matrice(imagine)
image **alloc_matrix(int width, int height)
{
	//alocarea vectorului de pointeri
	image **a = (image **)malloc(height * sizeof(image *));
	if (!a) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}
	//alocarea fiecarei linii
	for (int i = 0; i < height; i++) {
		a[i] = (image *)malloc(width * sizeof(image));
		//in cazul in care malloc() esueaza
		if (!a[i]) {
			fprintf(stderr, "malloc() failed\n");
			//eliberarea memoriei liniilor anterior alocate
			for (int j = 0; j < i; j++)
				free(a[j]);
			free(a);
			return NULL;
		}
	}
	return a;
}

//EXIT - functie prin care se elibereaza memoria alocata
void free_matrix(image **a, int width, int height)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			free(a[i][j].pixel);
		free(a[i]);
	}
	free(a);
}

//functie care returneaza numarul de pixeli ai unei imagini
int pixels_number(int color)
{
	if (color == 2)
		return 3;
	return 1;
}

//functie prin care se creeaza o copie a matricei/ pozei curente
image **copy_matrix(image **a, int width, int height, int color)
{
	//retinem numarul de pixeli ai imaginii
	int num_pixels = pixels_number(color);
	//alocam memorie intr-o matrice auxiliara
	image **new_matrix = alloc_matrix(width, height);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			new_matrix[i][j].pixel = (double *)malloc(num_pixels
									 * sizeof(double));
			for (int k = 0; k < num_pixels; k++)
				//retinem valorile in noua matrice
				new_matrix[i][j].pixel[k] = a[i][j].pixel[k];
		}

	return new_matrix;
}

//functie care citeste pozele de tipul P1, P2, P3
image **read_matrix_ascii(FILE *file, int width, int height, int color)
{
	//retinem numarul de pixeli ai pozei
	int num_pixels = pixels_number(color);
	//alocam memorie matricei
	image **a = alloc_matrix(width, height);
	unsigned char aux;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			a[i][j].pixel = (double *)malloc(num_pixels
							* sizeof(double));

			for (int k = 0; k < num_pixels; k++) {
				//citim din fisierul tip text valorile
				fscanf(file, "%hhu", &aux);
				a[i][j].pixel[k] = (double)aux;
			}
		}
	}
	return a;
}

//functie care citeste pozele de tipul P4, P5, P6,
image **read_matrix_binary(FILE *file, int width, int height, int color)
{
	//retinem numarul de pixeli ai pozei
	int num_pixels = pixels_number(color);
	//alocam memorie matricei
	image **a = alloc_matrix(width, height);
	unsigned char aux;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			a[i][j].pixel = (double *)malloc(num_pixels
							* sizeof(double));

			for (int k = 0; k < num_pixels; k++) {
				//citim din fisierul tip binar valorile
				fread(&aux, sizeof(unsigned char), 1, file);
				a[i][j].pixel[k] = (double)aux;
			}
		}
	}
	return a;
}

//SELECT ALL - functie prin care selectam imaginea completa
void select_all(int *x1, int *y1, int *x2, int *y2, int width, int height)
{
	*x1 = 0;
	*y1 = 0;
	*x2 = width;
	*y2 = height;
}

//LOAD - functie prin care se adauga o matrice/ imagine in memorie
void load_image(char filename[FMAX], image ***a, int *width, int *height,
				int *x1, int *y1, int *x2, int *y2, int *type, int *color,
				int *maxval, int *err)
{
	char magic_word[LMAX];

	//deschidem fisierul
	FILE *img = fopen(filename, "rt");
	if (!img) {
		fprintf(stdout, "Failed to load %s\n", filename);
		*err = 0;
		return;
	}

	//citim cuvantul magic
	ignore_comm(img);
	fscanf(img, "%s", magic_word);

	//1 = ascii, 2 = binar, 0 = eroare
	*type = image_type(magic_word);
	//alb-negru = 0, gri = 1, color = 2
	*color = image_color(magic_word);

	//verificam daca fisierul a fost deschis cu succes
	if (*type == 0) {
		fprintf(stdout, "Invalid file type\n");
		return;
	}

	ignore_comm(img);
	//citim inaltimea si latimea pozei (numar de linii, respectiv coloane)
	fscanf(img, "%d%d", width, height);
	ignore_comm(img);

	//daca avem o imagine greyscale sau color, citim si valoarea maxima
	if (strcmp(magic_word, "P2") == 0 || strcmp(magic_word, "P3") == 0 ||
	    strcmp(magic_word, "P5") == 0 || strcmp(magic_word, "P6") == 0) {
		fscanf(img, "%d", maxval);
		ignore_comm(img);
	}

	if (*type == 1) {
		//citim matricea din fisier de tip text
		*a = read_matrix_ascii(img, *width, *height, *color);
		fprintf(stdout, "Loaded %s\n", filename);
		//selectam coordonatele
		select_all(x1, y1, x2, y2, *width, *height);
		//am incarcat o poza
		*err = 1;
		fclose(img);
	} else if (*type == 2) {
		//retinem pozitia pana unde am citit din fisier
		int ascii_end = ftell(img);
		//inchidem fisierul de tip text
		fclose(img);
		//deschidem fisierul binar si verificam deschiderea acestuia
		FILE *img = fopen(filename, "rb");
		if (!img) {
			fprintf(stdout, "Failed to load %s\n", filename);
			*err = 0;
			return;
		}
		//continuam citirea datelor unde am oprit citirea ascii
		fseek(img, ascii_end + 1, SEEK_SET);
		*a = read_matrix_binary(img, *width, *height, *color);
		fprintf(stdout, "Loaded %s\n", filename);
		//selectam coordonatele
		select_all(x1, y1, x2, y2, *width, *height);
		//am incarcat o poza
		*err = 1;
		fclose(img);
	} else {
		fprintf(stdout, "Failed to load %s\n", filename);
		*err = 0;
	}
}

//functie de interschimbare a doua variabile int
void swap_coord(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

//functie prin care verificam daca coordonatele citite sunt valide
//se returneaza 1 pentru valori valide, 0 in sens contrar
int check_coord(int x1, int y1, int x2, int y2, int width, int height)
{
	if (x1 < 0 || x2 > width || y1 < 0 || y2 > height)
		return 0;
	if (x1 == x2 || y1 == y2)
		return 0;
	return 1;
}

//SELECT - functie prin care selectam anumiti pixeli din imagine
void select_img(char *instr, int *x1, int *y1, int *x2, int *y2,
				int width, int height)
{
	int x11, x22, y11, y22;

	//verificam pentru fiecare coordonata daca exista si daca este numar
	if (!instr || (instr[0] >= 'a' && instr[0] <= 'z')) {
		fprintf(stdout, "Invalid command\n");
		return;
	}
	x11 = atoi(instr);
	//citim urmatoarea instructiune
	instr = strtok(NULL, " ");

	if (!instr || (instr[0] >= 'a' && instr[0] <= 'z')) {
		fprintf(stdout, "Invalid command\n");
		return;
	}
	y11 = atoi(instr);
	instr = strtok(NULL, " ");

	if (!instr || (instr[0] >= 'a' && instr[0] <= 'z')) {
		fprintf(stdout, "Invalid command\n");
		return;
	}
	x22 = atoi(instr);
	instr = strtok(NULL, " ");

	if (!instr || (instr[0] >= 'a' && instr[0] <= 'z')) {
		fprintf(stdout, "Invalid command\n");
		return;
	}
	y22 = atoi(instr);

	//verificam daca sunt citite in ordine crescatoare
	//si le inversam in sens contrar
	if (x11 > x22)
		swap_coord(&x11, &x22);

	if (y11 > y22)
		swap_coord(&y11, &y22);

	if (check_coord(x11, y11, x22, y22, width, height)) {
		//coordonatele sunt valide
		*x1 = x11;
		*y1 = y11;
		*x2 = x22;
		*y2 = y22;
		fprintf(stdout, "Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
	} else {
		fprintf(stdout, "Invalid set of coordinates\n");
	}
}

//CROP - functie prin care se decupeaza o sectiune dintr-o imagine
image **crop_matrix(image **a, int *x1, int *y1, int *x2, int *y2,
					int *width, int *height, int color)
{
	//a fost selectata toata poza anterior
	if (*x2 - *x1 == *width && *y2 - *y1 == *height) {
		fprintf(stdout, "Image cropped\n");
		return a;
	}

	//alocam noile dimensiuni
	int num_pixels = pixels_number(color);
	int new_width = *x2 - *x1;
	int new_height = *y2 - *y1;
	//alocam memorie intr-o matrice auxiliara
	image **new_matrix = alloc_matrix(new_width, new_height);

	for (int i = 0; i < new_height; i++)
		for (int j = 0; j < new_width; j++) {
			new_matrix[i][j].pixel = (double *)malloc
			(num_pixels * sizeof(double));

			for (int k = 0; k < num_pixels; k++)
				//copiem noile valori
				new_matrix[i][j].pixel[k] = a[*y1 + i][*x1 + j].pixel[k];
		}
	//eliberam memoria utilizata anterior
	free_matrix(a, *width, *height);
	//modificam coordonatele
	*width = new_width;
	*height = new_height;
	select_all(x1, y1, x2, y2, *width, *height);

	fprintf(stdout, "Image cropped\n");

	return new_matrix;
}

//functie prin care se scrie in fisier tipul de imagine
void find_magic_word(FILE *file, int ascii, int color)
{
	if (ascii == 1) {	//ascii
		if (color == 0) {	//alb-negru
			fprintf(file, "P1\n");
		} else if (color == 1) {	//gri
			fprintf(file, "P2\n");
		} else {	//color
			fprintf(file, "P3\n");
		}
	} else {	//binar
		if (color == 0) {	//alb-negru
			fprintf(file, "P4\n");
		} else if (color == 1) {	//gri
			fprintf(file, "P5\n");
		} else {	//color
			fprintf(file, "P6\n");
		}
	}
}

//functie prin care se copiaza o imagine intr-un fisier de tip text
void save_matrix_ascii(FILE *file, image **a, int width, int height, int color)
{
	int num_pixels = pixels_number(color);
	unsigned char pixel;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < num_pixels; k++) {
				//aproximam valoarea
				pixel = (unsigned char)(round(a[i][j].pixel[k]));
				fprintf(file, "%hhu ", pixel);
			}
		}
		fprintf(file, "\n");
	}
}

//functie prin care se copiaza o imagine intr-un fisier de tip binar
void save_matrix_binary(FILE *file, image **a, int width, int height, int color)
{
	int num_pixels = pixels_number(color);
	unsigned char pixel;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < num_pixels; k++) {
				//aproximam valoarea
				pixel = (unsigned char)(round(a[i][j].pixel[k]));
				fwrite(&pixel, sizeof(unsigned char), 1, file);
			}
		}
	}
}

//SAVE - functie prin care se salveaza in memorie o imagine
void save_matrix(char *instr, image **a, int width, int height,
				 int color, int maxval, int err)
{
	//aflam ce trebuie facut
	int ascii = 0;
	char filename[FMAX];
	instr = strtok(NULL, " ");
	//gasim numele noului fisier
	strcpy(filename, instr);
	instr = strtok(NULL, " ");
	if (instr)
		ascii = 1;

	//verificam daca este deja o poza incarcata in memorie
	if (a && err == 1) {
		//deschidem un fisier si verificam daca se deschid corect
		FILE *file = fopen(filename, "wt");
		if (!file)	{
			fprintf(stderr, "Cannot open file\n");
			exit(-1);
		}
		//scriem in fisier cuvantul magic
		find_magic_word(file, ascii, color);
		//scriem in fisier restul datelor
		fprintf(file, "%d %d\n", width, height);
		if (color != 0)
			fprintf(file, "%d\n", maxval);

		if (ascii == 1) {
			//scrierea in fisier text
			save_matrix_ascii(file, a, width, height, color);
			//inchidem fisierul
			fclose(file);
			fprintf(stdout, "Saved %s\n", filename);

		} else {
			//scrierea in fisier binar
			//inchidem fisierul text
			fclose(file);
			//deschidem iar fisierul, de data aceasta in binar
			file = fopen(filename, "ab");
			if (!file) {
				fprintf(stderr, "Cannot open file\n");
				exit(-1);
			}
			//scriem datele in fisier
			save_matrix_binary(file, a, width, height, color);
			//inchidem fisierul
			fclose(file);
			fprintf(stdout, "Saved %s\n", filename);
		}
	} else {
		fprintf(stdout, "No image loaded\n");
	}
}

//functie de rotire la 90 de grade in sensul acelor de ceasornic
//a unei matrice complete
image **rotate_matrix(image **a, int *width, int *height, int color)
{
	int num_pixels = pixels_number(color);
	//alocam memorie unei matrice auxiliare
	image **new_matrix = alloc_matrix(*height, *width);

	for (int i = 0; i < *width; i++)
		for (int j = 0; j < *height; j++) {
			new_matrix[i][j].pixel = (double *)malloc
			(num_pixels * sizeof(double));
			for (int k = 0; k < num_pixels; k++)
				//copiem noile valori
				new_matrix[i][j].pixel[k] = a[*height - 1 - j][i].pixel[k];
		}
	//eliberam memoria alocata anterior in matricea de poze
	free_matrix(a, *width, *height);
	//interschimbam inaltimea si latimea imaginii
	swap_coord(width, height);

	return new_matrix;
}

//functie de rotire la 90 de grade in sensul acelor de ceasornic
//a unei zone dintr-o matrice
void rotate_selection(image ***a, int x1, int y1, int y2, int color)
{
	int height_sel = y2 - y1;
	int num_pixels = pixels_number(color);

	//lucrez doar in jumatate de matrice si interschimb cate 4 "colturi" o data
	for (int i = 0; i < height_sel / 2; i++) {
		for (int j = i; j < height_sel - i - 1; j++) {
			for (int k = 0; k < num_pixels; k++) {
				int aux = (*a)[y1 + i][x1 + j].pixel[k];

				(*a)[y1 + i][x1 + j].pixel[k] =
				(*a)[y1 + height_sel - 1 - j][x1 + i].pixel[k];

				(*a)[y1 + height_sel - 1 - j][x1 + i].pixel[k] =
				(*a)[y1 + height_sel - 1 - i][x1 + height_sel - 1 - j].pixel[k];

				(*a)[y1 + height_sel - 1 - i]
				[x1 + height_sel - 1 - j].pixel[k] =
				(*a)[y1 + j][x1 + height_sel - 1 - i].pixel[k];

				(*a)[y1 + j][x1 + height_sel - 1 - i].pixel[k] = aux;
			}
		}
	}
}

//ROTATE - functie prin care rotim o imagine/ o zona
//dintr-o imagine la un anumit unghi
void rotate_img(image ***a, int *width, int *height, int *x1,
				int *y1, int *x2, int *y2, int color, int angle)
{
	//verificam tipul de rotire dorita
	if (*x1 == 0 && *y1 == 0 && *x2 == *width && *y2 == *height) {
		//rotire completa
		//vom repeta rotirea la 90 de grade de cate ori este
		//necesar pentru fiecare caz
		//ungiurile complementare vor necesita acelasi numar de rotiri
		//la 90 de grade
		if (angle == 90 || angle == -270) {
			*a = rotate_matrix(*a, width, height, color);
			select_all(x1, y1, x2, y2, *width, *height);
			fprintf(stdout, "Rotated %d\n", angle);

		} else if (angle == 180 || angle == -180) {
			for (int i = 1; i <= 2; i++) {
				*a = rotate_matrix(*a, width, height, color);
				select_all(x1, y1, x2, y2, *width, *height);
			}
			fprintf(stdout, "Rotated %d\n", angle);

		} else if (angle == 270 || angle == -90) {
			for (int i = 1; i <= 3; i++) {
				*a = rotate_matrix(*a, width, height, color);
				select_all(x1, y1, x2, y2, *width, *height);
			}
			fprintf(stdout, "Rotated %d\n", angle);

		} else if (angle == 360 || angle == -360 || angle == 0) {
			fprintf(stdout, "Rotated %d\n", angle);
		} else {
			fprintf(stdout, "Unsupported rotation angle\n");
		}

	} else {
		//rotirea unei zone selectate
		if (*x2 - *x1 == *y2 - *y1) {
			if (angle == 90 || angle == -270) {
				rotate_selection(a, *x1, *y1, *y2, color);
				fprintf(stdout, "Rotated %d\n", angle);
			} else if (angle == 180 || angle == -180) {
				for (int i = 1; i <= 2; i++)
					rotate_selection(a, *x1, *y1, *y2, color);
				fprintf(stdout, "Rotated %d\n", angle);
			} else if (angle == 270 || angle == -90) {
				for (int i = 1; i <= 3; i++)
					rotate_selection(a, *x1, *y1, *y2, color);
				fprintf(stdout, "Rotated %d\n", angle);

			} else if (angle == 360 || angle == -360 || angle == 0) {
				fprintf(stdout, "Rotated %d\n", angle);
			} else {
				fprintf(stdout, "Unsupported rotation angle\n");
			}

		} else {
			fprintf(stdout, "The selection must be square\n");
		}
	}
}

//verificam daca noile valori obtinute se incadreaza
//in intervalul [0, valoare_maxima]
double new_color(double new, int maxval)
{
	if (new > maxval)
		new = (double)maxval;
	if (new < 0)
		new = 0;

	return new;
}

//functie prin care se aplica efectul EDGE pe o imagine/ zona
image **apply_edge(image **a, int x1, int y1, int x2, int y2,
				   int width, int height, int maxval, int color)
{
	image **new_matrix = copy_matrix(a, width, height, color);
	double new_red, new_green, new_blue;
	//definim matricea kernel pentru aplicarea filtrului
	double kernel[KSIZE][KSIZE] = {
		{-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}
	};
	//verificam sa putem selecta mereu 3x3 pixeli
	if (x1 == 0)
		x1++;

	if (y1 == 0)
		y1++;

	if (x2 == width)
		x2--;

	if (y2 == height)
		y2--;

	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			new_red = 0;
			new_green = 0;
			new_blue = 0;
			for (int k = i - 1; k <= i + 1; k++) {
				for (int p = j - 1; p <= j + 1; p++) {
					//valoarea noua a pixelilor pentru culoarea rosie
					new_red += a[k][p].pixel[0] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea verde
					new_green += a[k][p].pixel[1] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea albastra
					new_blue += a[k][p].pixel[2] *
					kernel[k - i + 1][p - j + 1];
				}
			}
			//alocam noile valori
			new_matrix[i][j].pixel[0] = new_color(new_red, maxval);
			new_matrix[i][j].pixel[1] = new_color(new_green, maxval);
			new_matrix[i][j].pixel[2] = new_color(new_blue, maxval);
		}
	}
	//eliberam vechea memorie
	free_matrix(a, width, height);

	return new_matrix;
}

//functie prin care se aplica efectul SHARPEN pe o imagine/ zona
image **apply_sharpen(image **a, int x1, int y1, int x2, int y2,
					  int width, int height, int maxval, int color)
{
	image **new_matrix = copy_matrix(a, width, height, color);
	double new_red, new_green, new_blue;
	//definim matricea kernel pentru aplicarea filtrului
	double kernel[KSIZE][KSIZE] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};
	//verificam sa putem selecta mereu 3x3 pixeli
	if (x1 == 0)
		x1++;

	if (y1 == 0)
		y1++;

	if (x2 == width)
		x2--;

	if (y2 == height)
		y2--;

	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			new_red = 0;
			new_green = 0;
			new_blue = 0;
			for (int k = i - 1; k <= i + 1; k++) {
				for (int p = j - 1; p <= j + 1; p++) {
					//valoarea noua a pixelilor pentru culoarea rosie
					new_red += a[k][p].pixel[0] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea verde
					new_green += a[k][p].pixel[1] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea albastra
					new_blue += a[k][p].pixel[2] *
					kernel[k - i + 1][p - j + 1];
				}
			}
			//alocam noile valori
			new_matrix[i][j].pixel[0] = new_color(new_red, maxval);
			new_matrix[i][j].pixel[1] = new_color(new_green, maxval);
			new_matrix[i][j].pixel[2] = new_color(new_blue, maxval);
		}
	}
	//eliberam vechea memorie
	free_matrix(a, width, height);

	return new_matrix;
}

//functie prin care se aplica efectul BLUR pe o imagine/ zona
image **apply_blur(image **a, int x1, int y1, int x2, int y2,
				   int width, int height, int maxval, int color)
{
	image **new_matrix = copy_matrix(a, width, height, color);
	double new_red, new_green, new_blue;
	//definim matricea kernel pentru aplicarea filtrului
	double kernel[KSIZE][KSIZE] = {
		{1. / 9, 1. / 9, 1. / 9},
		{1. / 9, 1. / 9, 1. / 9},
		{1. / 9, 1. / 9, 1. / 9}
	};
	//verificam sa putem selecta mereu 3x3 pixeli
	if (x1 == 0)
		x1++;

	if (y1 == 0)
		y1++;

	if (x2 == width)
		x2--;

	if (y2 == height)
		y2--;

	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			new_red = 0;
			new_green = 0;
			new_blue = 0;
			for (int k = i - 1; k <= i + 1; k++) {
				for (int p = j - 1; p <= j + 1; p++) {
					//valoarea noua a pixelilor pentru culoarea rosie
					new_red += a[k][p].pixel[0] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea verde
					new_green += a[k][p].pixel[1] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea albastra
					new_blue += a[k][p].pixel[2] *
					kernel[k - i + 1][p - j + 1];
				}
			}
			//alocam noile valori
			new_matrix[i][j].pixel[0] = new_color(new_red, maxval);
			new_matrix[i][j].pixel[1] = new_color(new_green, maxval);
			new_matrix[i][j].pixel[2] = new_color(new_blue, maxval);
		}
	}
	//eliberam vechea memorie
	free_matrix(a, width, height);

	return new_matrix;
}

//functie prin care se aplica efectul GAUSSIAN_BLUR pe o imagine/ zona
image **apply_gblur(image **a, int x1, int y1, int x2, int y2,
					int width, int height, int maxval, int color)
{
	image **new_matrix = copy_matrix(a, width, height, color);
	double new_red, new_green, new_blue;
	//definim matricea kernel pentru aplicarea filtrului
	double kernel[KSIZE][KSIZE] = {
		{1. / 16, 2. / 16, 1. / 16},
		{2. / 16, 4. / 16, 2. / 16},
		{1. / 16, 2. / 16, 1. / 16}
	};
	//verificam sa putem selecta mereu 3x3 pixeli
	if (x1 == 0)
		x1++;

	if (y1 == 0)
		y1++;

	if (x2 == width)
		x2--;

	if (y2 == height)
		y2--;

	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			new_red = 0;
			new_green = 0;
			new_blue = 0;
			for (int k = i - 1; k <= i + 1; k++) {
				for (int p = j - 1; p <= j + 1; p++) {
					//valoarea noua a pixelilor pentru culoarea rosie
					new_red += a[k][p].pixel[0] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea verde
					new_green += a[k][p].pixel[1] *
					kernel[k - i + 1][p - j + 1];
					//valoarea noua a pixelilor pentru culoarea albastra
					new_blue += a[k][p].pixel[2] *
					kernel[k - i + 1][p - j + 1];
				}
			}
			//alocam noile valori
			new_matrix[i][j].pixel[0] = new_color(new_red, maxval);
			new_matrix[i][j].pixel[1] = new_color(new_green, maxval);
			new_matrix[i][j].pixel[2] = new_color(new_blue, maxval);
		}
	}
	//eliberam vechea memorie
	free_matrix(a, width, height);

	return new_matrix;
}

//APPLY - functie prin care se aplica un anumit efect pe o poza
void apply_img(char *instr, image ***a, int x1, int y1, int x2, int y2,
			   int width, int height, int maxval, int color)
{
	//verificam parametrul dorit
	instr = strtok(NULL, " ");
	if (!instr) {
		fprintf(stdout, "Invalid command\n");
	} else {
		if (color == 2) {
			if (strcmp(instr, "EDGE") == 0) {
				//se aplica efectul edge
				*a = apply_edge(*a, x1, y1, x2, y2, width, height,
								maxval, color);
				fprintf(stdout, "APPLY EDGE done\n");
			} else if (strcmp(instr, "SHARPEN") == 0) {
				//se aplica efectul sharpen
				*a = apply_sharpen(*a, x1, y1, x2, y2, width,
								   height, maxval, color);
				fprintf(stdout, "APPLY SHARPEN done\n");
			} else if (strcmp(instr, "BLUR") == 0) {
				//se aplica efectul blur
				*a = apply_blur(*a, x1, y1, x2, y2, width, height,
								maxval, color);
				fprintf(stdout, "APPLY BLUR done\n");
			} else if (strcmp(instr, "GAUSSIAN_BLUR") == 0) {
				//se aplica efectul gaussian blur
				*a = apply_gblur(*a, x1, y1, x2, y2, width, height,
								 maxval, color);
				fprintf(stdout, "APPLY GAUSSIAN_BLUR done\n");
			} else {
				fprintf(stdout, "APPLY parameter invalid\n");
			}
		} else {
			//imaginea nu este color
			fprintf(stdout, "Easy, Charlie Chaplin\n");
		}
	}
}
