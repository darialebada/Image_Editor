###### // Copyright Lebada Daria-Cristiana 313CAa 2021-2022

#### PCLP1

# Tema 3 - Editor de imagini

### Context
Implementarea unui program cu interfata text in C pentru prelucrarea imaginilor
in formatul NetPBM (PPM si PGM).

### Implementare
In rezolvarea temei am utilizat o matrice alocata dinamic (a) in care stochez
informatiile fiecarei imagini (pgm sau ppm). Matricea este de tip image - structura
definita pentru a putea retine combinatiile de pixeli pentru formatele de poze
(intensitate culori alb-negru pentru pozele alb-negru sau greyscale, respectiv
combinatie de rosu, verde si albastru pentru pozele color).
Citesc fiecare linie din stdin si le prelucrez astfel incat sa obtin toate
instructiunile necesare. Programul va rula pana cand se citeste comanda EXIT.

### Comanda LOAD - incarcarea in memorie a unei imagini:
Daca exista deja o imagine incarcata, atunci se va elibera memoria inaite de citire.
Prima data vom deschide fisierul dorit, in format text. Daca acesta se deschide cu
succes, vom salva pe rand datele din fisier. Prima data aflam cuvantul magic (de
exemplu P1) cu ajutorul caruia putem afla tipul (ascii/ binar) si culoarea imaginii
(alb-negru, greyscale, color). Apoi se vor citi valorile pixelilor in formatul in care
se gasesc (ascii/ binar).

### Comanda SELECT - restrangerea efectului comenzilor urmatoare la un interval de pixeli:
Pentru comanda SELECT ALL se vor selecta colturile imaginii (0, 0, inaltime, latime).
Pentru cand se doreste selectarea unor anumite coordonate, prima data vom verifica
daca coordonatele sunt valide si in ordine crescatoare. Daca datele sunt corecte, le
vom selecta.

### Comanda ROTATE - rotirea selectiei curente la un anumit grad:
Prima data verificam coordonatele pentru a afla tipul de rotire: completa sau a unei
selectii patrate. Vom utiliza o matrice auxiliara in cazul unei rotiri a imaginii
complete in care vom salva "transpusa" matricei rotite la 90 de grade. Pentru rotirea
unei selectii patrate, vom lucra direct in matricea a, interschimband cate 4 variabile
la fiecare pas (cate una de pe fiecare "latura" a patratului zonei) in sensul acelor de
ceasornic. 
Matricea (imaginea) va fi rotita de cate ori este necesar (de exmplu, pentru unghi de 90
sau de -270 este necesara o singura rotire spre dreapta, in timp ce pentru unghiurile de
+- 180 doua rotiri spre dreapta).

### Comanda CROP - reducerea imaginii la o anumita selectie:
Copiem valorile din zona dorita intr-o matrice auxiliara, eliberam vechea memorie din
matrice si salvam noua matrice.

### Comanda APPLY - aplicarea unui filtru asupra selectiei curente:
In functie de filtrul selectat (edge, sharpen, blur, gaussian blur) se va aplica pe
fiecare pixel (rosu, verde, albastru) kernelul specific efectului dorit.

### Comanda SAVE - salvarea imaginii curente:
Se salveaza imaginea in fisierul dorit in formatul ales: ascii sau binar.

### Comanda EXIT - inchiderea programului:
Se elibereaza memoria (doar daca exista o poza incarcata deja in memorie)
si se termina programul.
