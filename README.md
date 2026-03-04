#### PCLP1

# Assignment 3 – Image Editor

## Context
This project implements a **command-line image editor in C** for processing images in the **NetPBM format** (PPM and PGM).

## Implementation
To solve this assignment, a **dynamically allocated matrix (`a`)** is used to store the information of each loaded image (PGM or PPM).

The matrix stores elements of type **`image`**, a structure defined to represent pixel data depending on the image format:

- **Grayscale images** – store intensity values for black-and-white or grayscale images
- **Color images** – store combinations of **red, green, and blue (RGB)** values

The program reads each line from **stdin** and processes it to determine the requested commands.  
Execution continues until the **`EXIT`** command is received.

---

## Commands

### `LOAD` – Load an Image into Memory
If an image is already loaded, its memory is released before loading the new one.

The program opens the specified file (in text format). If the file opens successfully, the data is read sequentially:

1. The **magic word** (e.g., `P1`, `P2`, `P3`, etc.) is read to determine:
   - the image type (**ASCII or binary**)
   - the image format (**black-and-white, grayscale, or color**)
2. The pixel values are then read according to the specified format (ASCII or binary).

---

### `SELECT` – Restrict the Effect of Subsequent Commands to a Pixel Region
For the command **`SELECT ALL`**, the entire image is selected using the coordinates:  **`(0, 0, height, width)`**

When selecting specific coordinates, the program first checks if:

- the coordinates are **valid**
- they are provided in **ascending order**

If the input is correct, the region is selected.

---

### `ROTATE` – Rotate the Current Selection
The program first checks the selected coordinates to determine whether:

- the **entire image** is being rotated, or
- only a **square selection**.

For rotating the entire image, an **auxiliary matrix** is used to store the **transpose of the matrix rotated by 90 degrees**.

For rotating a square selection, the rotation is performed **directly in matrix `a`**, by swapping **four elements at a time** (one from each side of the square region) in **clockwise order**.

The image is rotated as many times as required:

- **90° or -270°** → one right rotation
- **±180°** → two right rotations

---

### `CROP` – Reduce the Image to the Selected Region
The values from the selected area are copied into an **auxiliary matrix**.  
The memory of the original matrix is then released, and the new matrix becomes the current image.

---

### `APPLY` – Apply a Filter to the Current Selection
Depending on the selected filter (`edge`, `sharpen`, `blur`, `gaussian blur`), a **specific convolution kernel** is applied to each pixel:

- **Red**
- **Green**
- **Blue**

---

### `SAVE` – Save the Current Image
The current image is saved to the specified file in the selected format:

- **ASCII**
- **Binary**

---

### `EXIT` – Terminate the Program
All allocated memory is released (if an image is currently loaded), and the program execution stops.
