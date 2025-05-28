Limbajul de programare utilizat: C
Biblioteca utilizată: RayLib

Reguli joc
Utilizatorul alege dificultatea dorită, limba din care va face parte cuvântul pe care îl va avea de ghicit și domeniul din care face parte cuvântul.
Utilizatorului i se va oferi la dispoziție un timp pe care trebuie să îl respecte pentru a câștiga punctaj pentru nivelul la care se află. 
În timpul respectiv, el trebuie să introducă diverse litere pentru a ajunge la cuvântul “ascuns”.
Dacă utilizatorul a ghicit cuvântul, nivelul este considerat “câștigat”, dacă nu, este considerat pierdut.

Cum funcționează
Dacă timpul a expirat, iar utilizatorul nu a apucat să ghicească tot cuvântul, nivelul se consideră pierdut. Odată pierdut, utilizatorul va putea trece la nivelul următor cu punctajul adunat pe nivelul/nivelurile anterior/anterioare.
Cuvintele se află în fișiere .txt diferite, în funcție de limbă și domeniu
Dificultate: cele 3 opțiuni de dificultate nu depind de lungimea cuvântului în acest joc, ci de timpul pus la dispoziție pentru ghicirea lui. Astfel:
EASY are la dispoziție un timer de 60 secunde
MEDIUM are la dispoziție un timer de 40 secunde
HARD are la dispoziție un timer de 25 secunde

       -Calculul scorului: Scorul va fi calculat în funcție de dificultatea aleasă la începutul jocului, urmând regula:
		      - EASY: 0.1 p / literă	
      -MEDIUM: 0.25 p / literă
      - HARD: 0.5 p / literă 	
Astfel, alegând nivelul HARD putem obține un scor mai mare rapid, însă avem un timp mult mai limitat decât la celălalte 2 nivele.

Componentele jocului (Ferestre)
Fereastra cu titlul jocului și mesajul “Enter SPACE to start the game”
O fereastră cu opțiunile pentru dificultate: EASY (ușor) , MEDIUM (mediu/normal) și HARD (dificil/greu)
O fereastră cu opțiunile pentru limbă: ENGLEZĂ sau ROMÂNĂ
Fereastra principală pentru joc, care conține ilustrația Hangman, informații despre nivelul curent, scorul obținut, calculat până în momentul respectiv, timer-ul care se scurge în timp real și numărul de încercări greșite pentru a atrage atenția asupra utilizatorului. Tot în fereastra principală găsim și spațiul unde vor apărea literele ghicite pentru a vedea mai bine cuvântul (NOTĂ: Pot exista și cuvinte compuse, despărțite prin cratimă sau spațiu. În aceste cazuri sunt afișate pe ecran).
Următoarea fereastră face tranziția spre noul nivel. În cadrul acesteia găsim mesajul “Level X complete!”, punctajul obținut în cadrul nivelului X, punctajul total (toate punctajele obținute anterior + punctajul nou acumulate), instructiunea “Enter SPACE to start the next level”. 
Notă: În cazul în care a fost ghicit cuvântul, va fi menționat mai jos acest fapt. La fel și în cazul în care nu a fost ghicit.
      - Fereastra de final, care conține mesajul “GAME OVER!”, alături de scorul final obținut în cadrul jocului și instrucțiunile “Enter SPACE to start again” și “Enter ESC to exit the game”, pentru ca utilizatorul să aleagă dacă dorește să oprească definitiv jocul sau dacă dorește să mai încerce încă o dată. 
      
Milestone-urile respectate
✅Varianta inițială, de bază - cea din terminal
✅Alegerea domeniului: mâncare, băutură, școală
✅Afișarea încercărilor rămase după fiecare literă greșită
✅Calcularea progresivă a unui scor / punctaj
✅Alegerea limbii din care să facă parte cuvântul: engleză sau română
✅Salvarea scorului pentru nivelele următoare
✅Alegerea nivelului de dificultate
✅Timer pentru ghicirea cuvântului
✅Adăugarea nivelelor
✅Interfață mai atractivă folosind biblioteci
✅Să poată fi ghicite și cuvintele compuse

