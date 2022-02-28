Chirac Andrei
Rosu Stefania

---------Laborator 4----------

bug1:  Pentru a satisface race condition-ul am apelat metoda start() in loc de run(),
astfel incat thread-urile sa nu se execute secvential.

bug2: Am observat ca nu avem un deadlock deoarece sychronized este reentrant.

bug3:  a si b faceau referinta la acelasi obiect. Am modificat una din ele astfel incat
programul sa functioneze corect.

bug4: Programul nu se oprea deoarce variabila keeprunning nu se actualiza. Fiind citita
de multe ori, a trebuit sa o declaram volatile pentru a i se se schimba valoarea.

bug5: Blocurile de cod se blocau gresit, din cauza dependentelor dintre lock-uri si 
astfel programul nu se termina.

bug6: A trebuit sa sincronizam metoda de getInstance astfel incat sa fim siguri ca un
singur thread creeaza o instanta a obiectului, iar celelalte nu.

doubleVectorElements: Am creat clasa MyThread a carei metoda run() dubleaza valoarea
elementelor din vector. In main, am impartit vectorul pe thread-uri cu ajutorul lui start si end.

floyd-warshall: Am paralelizat al doilea for cu ajutorul lui start si end, ce fac impartirea
pe thread-uri. Am observat ca este nevoie de o bariera pentru ca programul sa functioneze corect.