//Copyright Lebada Daria-Cristiana 313CAa 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(void)
{
	//declarari
	int height, width, type, err = 0;
	int color, maxval, x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	char command[CMAX], filename[FMAX];
	image **a = NULL;

	//citim prima comanda
	fgets(command, CMAX, stdin);
	command[strlen(command) - 1] = 0;

	while (strcmp(command, "EXIT") !=  0) {
		//aflam actiunea ceruta
		char *action = strtok(command, " ");

		if (strcmp(action, "LOAD") == 0) {
			//verificam daca exista vreo imagine deja incarcata
			if (a && err == 1)
				//eliberam memoria pentru a incarca a noua imagine
				free_matrix(a, width, height);
			//aflam numele fisierului
			action = strtok(NULL, " ");
			strcpy(filename, action);

			load_image(filename, &a, &width, &height, &x1, &y1, &x2,
					   &y2, &type, &color, &maxval, &err);
		} else if (strcmp(action, "SELECT") == 0) {
			if (a && err == 1) {
				action = strtok(NULL, " ");
				if (strcmp(action, "ALL") == 0) {
					select_all(&x1, &y1, &x2, &y2, width, height);
					fprintf(stdout, "Selected ALL\n");
				} else {
					select_img(action, &x1, &y1, &x2, &y2, width, height);
				}
			} else {
				fprintf(stdout, "No image loaded\n");
			}
		} else if (strcmp(action, "SAVE") == 0) {
			save_matrix(action, a, width, height, color, maxval, err);
		} else if (strcmp(action, "CROP") == 0) {
			if (a && err == 1)
				a = crop_matrix(a, &x1, &y1, &x2, &y2, &width, &height, color);
			else
				fprintf(stdout, "No image loaded\n");
		} else if (strcmp(command, "ROTATE") == 0) {
			if (a && err == 1) {
				action = strtok(NULL, " ");
				int angle = atoi(action);
				rotate_img(&a, &width, &height, &x1, &y1, &x2, &y2,
						   color, angle);
			} else {
				fprintf(stdout, "No image loaded\n");
			}
		} else if (strcmp(command, "APPLY") == 0) {
			if (a && err == 1) {
				apply_img(action, &a, x1, y1, x2, y2, width, height,
						  maxval, color);
			} else {
				fprintf(stdout, "No image loaded\n");
			}
		} else {
			fprintf(stdout, "Invalid command\n");
		}
		//citim o noua comanda
		fgets(command, CMAX, stdin);
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = 0;
	}
	//s-a apelat comanda EXIT
	if (a && err == 1)
		free_matrix(a, width, height);
	else
		printf("No image loaded\n");

	return 0;
}
