# Symulator-ruchu-obiektow-po-planszy
Celem projektu jest stworzenie symulatora fizycznego, który modeluje ruch cząsteczek w przestrzeni 2D.       
Cząsteczki o różnych kształtach ( koło, kwadrat, prostokat, trójkąt, trapez) poruszają się na planszy,       
reagując na "podmuchy wiatru", które są stworzone jako symboliczne punkty na ekranie.   
Granicę stanowi przerywane obramowanie planszy, które symbolizuje przepaść.     
Po wyjściu za granicę obiekty są usuwane.   
Obiekty wpływają na siebie nawzajem(występują kolizje, które mogą spowodować zmianę trajektori ruchu obiektu).  
Projekt jest oparty na wielowątkowości oraz szablonach klas, co zapewnia wysoką elastyczność i wydajność.   
            


Założenia:          
Cząsteczki: Różne kształty cząsteczek (np. koło, kwadrat, trójkąt) reprezentowane są przez różne klasy, 
które stworzonę są jako uniwersalny template.   
Wyróżniamy 2 templaty; jeden, który jest dla figur ogólnych, drug dla figur składających się z łączenia punktów.    
Cząsteczki mają różne właściwości (masa, kształt, tarcie). Każda figura inaczej "naciska" na podłoże.   
Siły: Cząsteczki reagują na siły, takie jak "podmuchy wiatru" w wybranych punktach planszy.     
Wielowątkowość: Każda cząsteczka jest tworzona w osobnym wątku. 
Odbicia: Cząsteczki odbijają się od siebie w przypadku kontaktu.    
Interfejs Graficzny: Symulacja jest wizualizowana w GUI stworzonym w Qt.    
    
Technologie:    
Język Programowania: C++    
    
Biblioteki:         
Qt: Do GUI i rysowania cząsteczek.      
C++ Standard Library: Wielowątkowość (std::thread, std::mutex), szablony klas, algorytmy.   
Środowisko Programistyczne: Qt Creator, Visual Studio Code.     
System Kontroli Wersji: Git/GitHub.     
