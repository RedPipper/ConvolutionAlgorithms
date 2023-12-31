## Varianta 1
Logica implementarii poate fi separata in doua moduri:
1. Master:
  a. Master-ul trimite la toate workerele prin Bcast matricea de kernel.
  b. Ulterior, masterul citeste cate i_n/numprocLinii din fisierul de input si le trimite la workerul corespondent
  c. Dupa trimitere, masterul asteapta de la oricare worker rezultatele si le scrie intr-un buffer
  d. Dupa ce a primit numproc mesaje, masterul scrie rezultatele din buffer in fisier si termina procesarea
2. Worker:
  a. Workerul primeste matricea kernel de la master
  b. Workerul primeste matricea de input de la master
  c. Daca id > 1, Trimite prima linie la workerul cu id-1 si asteapta sa primeasca o linie tot de la el
  d. Daca id < numprocs-1, Trimite ultima linie la workerul cu id+1 si asteapta sa primeasca o linie tot de la el
  e. Calculeaza rezultatele si le trimite inapoi la master


## Rezultate lab3, varianta 1
| Measurement type  | Matrix size                | Nr of procs    | Time in microseconds       |
| -- | ---------------- | ---- | ------- |
| T2 |  n=1000, m=1000  |  5   |  25642  |
| T2 |  n=1000, m=1000  |  9   |  18036  |
| T2 |  n=1000, m=1000  |  21  |  11454  |
| T1 |  n=1000, m=1000  |  5   |  210102 |
| T1 |  n=1000, m=1000  |  9   |  387285 |
| T1 |  n=1000, m=1000  |  21  |  949496 |

Pot observa ca, in cazul in care masuram print metoda T2, timpii efectivi de calcul scad pe masura ce adaugam procese noi.
In schimb, daca includem si scrierea/citirea in fisier, timpul creste pe masura ce adaugam procese noi. Acest lucru este de asteptat deoarece 
sunt mai multi workeri care trebuie sa primeasca-trimita date.

## Rezultate lab2
| Marime test       |  Nr Threaduri  |  timp rulare c++  |  timp rulare java |
| ----------------- | -------------- | ----------------- | ----------------- |
| n=1000, m=1000    |  1             |  23405            |  37603            |
| n=1000, m=1000    |  2             |  12179            |  40098            |
| n=1000, m=1000    |  4             |  8767             |  47452            |
| n=1000, m=1000    |  8             |  10494            |  60611            |
| n=1000, m=1000    |  16            |  9930             |  64894            |

Putem observa ca performantele lab2 sunt in general mai bune fata de performantele lab3 (masurate cu T2). Exceptia este rularea secventiala la lab2
cu un numar de 5 procese.
