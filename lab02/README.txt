Chirac Andrei
Rosu Adriana Stefania

Ex1

In cadrul acestui exercitiu am folosit un mutex pentru a delimita o zona 
critica astfel incat instructiunea sa se realizeze corect.

Ex2

La acest exercitiu am folosit o bariera deoarece dorim ca thread-ul sa nu
treaca mai in fata daca nu l-a afisat pe 1, afisandu-l mai apoi pe 2.

Ex3

Pentru acest exercitiu am folosit abordarea prezentata in laboratorul 1.Astfel
am folosit ideea de distibuire a thread-urilor prin start = i * (double)N / P
si end = min((i + 1) * (double)N / P, N), astfel fiecare dintre thredurile
folosite vor realiza operatiile pentru un alt i fara a se produce efecte de
suprascriere.

Ex4

Am folosit aceeasi abordare ca la Ex2 doar ca de data aceasta pentru j.

Ex5

Abordare similara cu cea de la exercitiile precedente doar ca de data aceasta
am observat fenomenul de suprascriere asa ca am folosit un mutex pentru a nu
permite scrierea in matrice pentru ac i si j.

Ex6

In cadrul acestui exercitiu ne am gandit ca datorita calcului individual ale
matricelor de la M1 la M7 putem sa realizam o paralelizare bazata pe 7 
thread-uri. Ulterior am observat faptul ca si calcularea matricelor C11, C12,
C21,C22 se realizeaza pe baza matricelor precedente si astfel am folosit o 
bariera pentru a astepta toate thread-urile. Am observat si ca se realizeaza independent, astfel din cele 7 thread-uri am utilizat doar 4. 



