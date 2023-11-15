| Marime test       |  Nr Threaduri  |  timp rulare c++  |  timp rulare java |
| ----------------- | -------------- | ----------------- | ----------------- |
| n=10, m=10        |  1             |  131              |  1004             |
| n=10, m=10        |  2             |  187              |  1509             |
| n=1000, m=1000    |  1             |  23405            |  37603            |
| n=1000, m=1000    |  2             |  12179            |  40098            |
| n=1000, m=1000    |  4             |  8767             |  47452            |
| n=1000, m=1000    |  8             |  10494            |  60611            |
| n=1000, m=1000    |  16            |  9930             |  64894            |
| n=10000, m=10000  |  1             |  2405773          |  2827871          |
| n=10000, m=10000  |  2             |  1336916          |  1698590          |
| n=10000, m=10000  |  4             |  1054470          |  1178586          |
| n=10000, m=10000  |  8             |  751617           |  956824           |
| n=10000, m=10000  |  16            |  743525           |  920239           |

Complexitatea spatiu a algoritmului este Theta(3*n) pentru fiecare thread, intrucat fiecare retine un numar de 3 array-uri drept buffer.
La inceput, inainte de bariera, threadul copiaza linia de deasupra intervalului alocat, prima linie din interval si linia de sub intervalul alocat.
Inainte de a incepe sa calculeze o linie, threadul copiaza in buffer1 buffer2, si in buffer2 linia curenta, pana la final, si face calculele in baza:
buffer1, buffer2 si liniei urmatoare daca nu se afla pe ultima linie din interval, sau linia endbuffer copiata la inceput.

Observ ca algoritmii ating performante optime la nr de threaduri diferite pentru fiecare caz:
1. Pentru n=m=1000, performantele sunt cele mai bune cand algoritmul este rulat cu 4 threaduri
2. Pentru n=m=10000, performantele cresc pe masura ce adaugam threaduri.

Legat de implementari, observ ca varianta java este in general mai putin performanta. 
Insa dependenta performantei fata de marimea datelor de input este mult mai drastica, de multe ori algoritmul secvential fiind mai performant decat cele cu mai multe threaduri. 
Singurul caz in care beneficiile folosirii de threaduri multiple sunt vizibile este n=m=10000, unde performantele algoritmului scade pe masura ce adaugam mai multe threaduri.
