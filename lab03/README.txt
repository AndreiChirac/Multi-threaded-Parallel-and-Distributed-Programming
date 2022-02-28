Chirac Andrei
Rosu Adriana-Stefania

------Laborator 3 APD---------

ex1. Am paralelizat algoritmul Bubble Sort, sortand simultan cu ajutorul mai multor 
thread-uri mai intai elementele de pe pozitii pare si mai apoi cele de pe pozitii impare.
Am folosit doua bariere pentru a asigura corectitudinea programului: prima bariera 
este folosita pentru a se astepta toate thread-urile care au sortat elementele
de pe pozitii pare, iar a doua pentru cele impare.

ex2. Am paralelizat algoritmul de Shear Sort. Astfel, am sortat in paralel liniile:
cele cu indice par in ordine crescatoare, iar cele cu indice impar descrescator
(la start si end am luat in considerare latura matricei, si nu numarul de elemente 
al acesteia deoarece se sorteaza doar linii si coloane de lungime l). Am asteptat toate
thread-urile cu ajutorul unei bariere pentru a ne asigura ca toate liniile au fost sortate
inainte de incepere sortarii coloanelor. Am sortat coloanele, folosind mai apoi o noua 
bariera pentru corectitudinea rezultatului.

ex3. Am paralelizat algoritmul de Merge Sort. Am calculat start si end in functie de numarul
de piese ce trebuie merge-uite, si apeland mai apoi functia de merge. 
Am folosit o bariera pentru a astepta merge-uirea , iar mai apoi, pe un singur thread,
s-au sortat elementele.

ex4. Am paralelizat algoritmul de Binary Search. Cat timp nu s-a gasit elementul 
(keep_running = 1), calculam dimennsiunea intervalului, verificand-o, fiecare thread
avand un start si un end calculate pe baza capetelor intervalului (data->right 
si data->left). Pe fiecare thread se verifica daca numarul cautat este in interiorul
intervalului corespunzator lui sau daca a acesta a fost gasit (este egal cu unul 
din capete), acutalizandu-se vectorul found cu INSIDE, OUTSIDE, sau pozitia numarului
cautat. Dupa folosirea unei bariere pentru a astepta raspunsul tuturor thread-urilor,
un singur thread va determina noul interval.
