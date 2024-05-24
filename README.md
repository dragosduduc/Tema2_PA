----TEMA 2----<br><br>
--Cerința 1--<br>
Se dorește realizarea grafului unui turneu cu 32 de echipe, unde există muchie de la echipa i către j dacă j a învins în meciul cu i.
În acest sens se citesc informațiile privind cele 32 de echipe din fișierul de intrare, se creează lista echipelor și se definesc două tipuri de cozi: cozi de echipe (câștigători și pierzători) și coada de meciuri.
Echipele din listă (care, după prima parcurgere, a conținut echipele în ordine inversă, deoarece acestea au fost introduse la început una câte una, motiv pentru care lista a fost inversată) se mută în coada de meciuri, câștigătorii se mută în coada de câștigători, pierzătorii se mută în coada de pierzători, câștigătorii se mută înapoi în coada de meciuri și așa mai departe, până la terminarea meciurilor. Diferențele față de tema1 - task-ul 3 sunt actualizarea muchiei de la pierzător la câștigător în graful turneului și echipele pierzătoare nu se mai șterg. Acestea rămân salvate în așa fel încât, după ce echipa câștigătoare este și ea mutată din coada de câștigători în cea de pierzători, coada de câștigători să conțină toate echipele turneului în ordinea în care acestea au fost eliminate.
În acest moment, graful turneului este finalizat, așa că se afișează în fișierul de ieșire.

--Cerința 2--<br>
Pentru calculul scorului echipelor s-a creat un câmp în structura fiecărei echipe în care sunt memorate numărul de meciuri câștigate în turneu. Se scot elementele din coada de pierzători(deQueue), se calculează scorul folosind formula (1) din îndrumar și se afișează informațiile relevante pentru fiecare echipă.
