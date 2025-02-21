# Symulator-ruchu-obiektow-po-planszy
Celem projektu jest stworzenie symulatora fizycznego, który modeluje ruch cząsteczek w przestrzeni 2D.   
Cząsteczki o różnych kształtach (np. koła, kwadraty, trójkąty) poruszają się na planszy,   
reagując na "podmuchy wiatru" oraz odbijając się od ścianek ramki, która stanowi granicę planszy.   
Projekt jest oparty na wielowątkowości oraz szablonach klas, co zapewnia wysoką elastyczność i wydajność.  

Założenia:    
Cząsteczki: Różne kształty cząsteczek (np. koło, kwadrat, trójkąt) reprezentowane są przez różne klasy dziedziczące po wspólnej klasie bazowej. 
Cząsteczki mają różne właściwości (masa, kształt, prędkość, siły działające na nie).  
Siły: Cząsteczki reagują na siły, takie jak "podmuchy wiatru" w wybranych punktach planszy.  
Wielowątkowość: Każda cząsteczka jest przetwarzana w osobnym wątku, co umożliwia równoczesne obliczenia pozycji i sił.  
Odbicia: Cząsteczki odbijają się od granic planszy zgodnie z zasadą odbicia, zmieniając kierunek prędkości.  
Interfejs Graficzny: Symulacja jest wizualizowana w GUI stworzonym w Qt.  

Technologie:  
Język Programowania: C++  

Biblioteki:  
Qt: Do GUI i rysowania cząsteczek.  
C++ Standard Library: Wielowątkowość (std::thread, std::mutex), szablony klas, algorytmy.  
Środowisko Programistyczne: Qt Creator, Visual Studio.  
System Kontroli Wersji: Git/GitHub.  
