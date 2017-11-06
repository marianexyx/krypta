#include <SFML/Window.hpp>

sf::String introduction = "Algorytm pozwala zidentyfikowac pochodzenie mieszkanca \nkrypty na podstawie specjalnie opracowanego zestawu \n10ciu pytan. \n\nWyniki powinny byc pozytywne w 95 %. \n\nNiektore odpowiedzi moga byc dostosowane tylko dla \nmezczyzn lub kobiet.";

sf::String start_quiz = "> Rozpocznij test.";

int odp[10][5][21] =
{ { { 3, 8, 1, 3, 8, 10, 10, 1, 2, 0, 2, 5, 6, 10, 1, 6, 4, 1, 0, 9, 7 }, //1
{ 7, 10, 3, 3, 6, 9, 10, 3, 7, 1, 6, 4, 4, 8, 9, 6, 5, 2, 0, 7, 9 },
{ 9, 1, 0, 2, 3, 1, 0, 8, 3, 9, 4, 10, 5, 1, 0, 10, 4, 6, 10, 4, 4 },
{ 2, 2, 6, 6, 3, 7, 8, 2, 2, 4, 1, 6, 5, 4, 3, 5, 8, 3, 9, 10, 5 },
{ 6, 5, 7, 8, 3, 0, 0, 7, 3, 8, 1, 4, 2, 0, 1, 5, 6, 6, 6, 9, 2 } },
{ { 10, 3, 10, 9, 6, 1, 0, 7, 3, 7, 2, 8, 4, 10, 7, 10, 9, 9, 9, 0, 1 }, //2
{ 1, 5, 1, 4, 6, 9, 6, 2, 1, 5, 2, 0, 1, 7, 7, 0, 7, 7, 7, 10, 8 },
{ 3, 1, 9, 6, 2, 0, 1, 5, 1, 7, 1, 1, 3, 1, 1, 0, 4, 4, 2, 7, 1 },
{ 0, 9, 2, 3, 7, 10, 10, 1, 5, 3, 7, 1, 2, 9, 7, 0, 6, 6, 9, 5, 8 },
{ 7, 1, 5, 10, 5, 0, 1, 4, 1, 7, 5, 9, 5, 3, 2, 2, 4, 4, 5, 2, 4 } },
{ { 7, 1, 5, 10, 5, 0, 1, 4, 1, 7, 5, 9, 5, 3, 2, 2, 4, 4, 5, 2, 4 }, //3
{ 4, 9, 5, 0, 4, 10, 5, 3, 6, 3, 3, 2, 7, 7, 7, 6, 4, 4, 8, 9, 4 },
{ 5, 3, 5, 0, 5, 0, 0, 6, 5, 5, 4, 0, 7, 6, 5, 5, 10, 9, 9, 3, 3 },
{ 3, 7, 4, 5, 8, 8, 7, 5, 7, 0, 2, 2, 4, 5, 8, 4, 4, 5, 8, 10, 8 },
{ 5, 4, 7, 4, 8, 5, 10, 7, 8, 1, 9, 5, 6, 6, 5, 5, 9, 10, 6, 5, 10 } },
{ { 5, 2, 5, 3, 9, 7, 4, 5, 0, 4, 5, 5, 2, 4, 4, 4, 5, 5, 10, 6, 4 }, //4
{ 6, 3, 6, 4, 3, 3, 3, 10, 4, 5, 5, 5, 3, 6, 6, 4, 8, 3, 7, 6, 2 },
{ 8, 5, 8, 5, 7, 3, 3, 4, 10, 2, 4, 5, 6, 6, 6, 6, 2, 9, 0, 5, 8 },
{ 5, 5, 5, 5, 5, 5, 5, 6, 6, 10, 7, 3, 3, 3, 2, 3, 6, 2, 0, 6, 7 },
{ 0, 2, 0, 3, 2, 10, 7, 0, 4, 0, 0, 3, 0, 1, 0, 2, 0, 0, 0, 6, 8 } },
{ { 2, 2, 2, 2, 2, 7, 7, 1, 9, 0, 2, 2, 10, 2, 2, 2, 2, 1, 4, 5, 5 }, //5
{ 2, 2, 2, 2, 2, 3, 3, 1, 10, 1, 2, 2, 6, 2, 2, 2, 2, 2, 0, 3, 2 },
{ 6, 5, 6, 6, 5, 1, 1, 10, 2, 2, 4, 4, 4, 3, 6, 8, 7, 2, 7, 4, 3 },
{ 6, 2, 7, 7, 5, 2, 2, 6, 2, 0, 5, 6, 6, 5, 8, 9, 10, 0, 10, 8, 4 },
{ 6, 2, 7, 6, 5, 2, 2, 2, 2, 0, 3, 5, 5, 5, 7, 8, 0, 10, 2, 3, 4 } },
{ { 3, 3, 3, 5, 3, 3, 3, 4, 3, 6, 2, 4, 4, 3, 10, 3, 2, 6, 3, 3, 2 }, //6
{ 4, 4, 4, 1, 2, 1, 1, 3, 3, 1, 2, 2, 3, 3, 3, 3, 1, 10, 0, 2, 0 },
{ 2, 2, 2, 3, 2, 7, 10, 2, 2, 5, 2, 2, 2, 2, 8, 2, 2, 3, 2, 4, 10 },
{ 5, 4, 5, 3, 5, 2, 1, 6, 5, 2, 6, 5, 5, 6, 4, 5, 7, 3, 5, 2, 0 },
{ 4, 4, 4, 5, 4, 6, 6, 5, 5, 6, 3, 5, 5, 5, 2, 5, 3, 6, 4, 6, 6 } },
{ { 1, 1, 1, 2, 1, 2, 2, 1, 1, 0, 1, 3, 10, 1, 1, 1, 1, 1, 2, 4, 1 }, //7
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1 },
{ 2, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 1, 2, 1, 1, 1, 1, 1, 10, 3, 1 },
{ 3, 3, 2, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 7, 1, 1, 10, 10, 1, 1, 1 },
{ 0, 0, 0, 0, 0, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 } },
{ { 2, 2, 3, 3, 2, 1, 1, 1, 2, 5, 1, 1, 1, 2, 1, 1, 1, 1, 10, 3, 2 }, //8
{ 5, 5, 5, 4, 5, 2, 2, 4, 5, 1, 3, 3, 3, 2, 1, 1, 1, 10, 0, 4, 2 },
{ 1, 1, 1, 8, 2, 2, 2, 1, 7, 8, 9, 2, 2, 9, 2, 2, 3, 3, 3, 6, 2 },
{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 10, 2, 2, 2, 2, 2, 2, 2, 2 },
{ 3, 2, 1, 1, 2, 1, 2, 3, 1, 2, 1, 10, 2, 2, 1, 2, 1, 1, 1, 5, 1 } },
{ { 4, 2, 2, 3, 3, 2, 3, 1, 2, 2, 2, 3, 3, 4, 8, 10, 3, 2, 3, 3, 3 }, //9
{ 4, 4, 3, 4, 3, 2, 1, 4, 3, 3, 2, 3, 3, 3, 2, 3, 2, 10, 3, 3, 3 },
{ 3, 3, 3, 3, 3, 3, 3, 2, 0, 4, 4, 3, 3, 3, 4, 4, 3, 5, 10, 3, 2 },
{ 2, 2, 2, 6, 3, 9, 9, 5, 9, 4, 6, 3, 2, 6, 4, 4, 3, 7, 4, 3, 3 },
{ 5, 8, 5, 5, 10, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 0 } },
{ { 2, 2, 4, 10, 3, 3, 1, 4, 5, 8, 2, 9, 4, 4, 5, 5, 3, 4, 4, 5, 2 }, //10
{ 4, 2, 4, 3, 4, 1, 1, 10, 5, 7, 4, 4, 1, 5, 5, 9, 8, 2, 2, 5, 2 },
{ 4, 2, 4, 2, 3, 4, 1, 2, 2, 0, 10, 2, 2, 7, 2, 2, 7, 1, 1, 5, 2 },
{ 5, 2, 4, 2, 3, 5, 1, 4, 2, 2, 7, 2, 2, 3, 10, 3, 9, 2, 4, 5, 2 },
{ 5, 10, 4, 2, 3, 5, 1, 4, 3, 1, 8, 2, 7, 9, 7, 3, 2, 9, 4, 5, 2 } } };

sf::String pyt[10][6] =
{ { "Jaka jest pierwsza rzecz jaka pamietasz?", //1
"> Wielki potwor zywiacy sie rowiesnikami.",
"> Niepokoj przebywania w krypcie.",
"> W sumie nie pamietam nic ciekawego wartego \n zapamietania.",
"> Dziecinstwo moje bylo pelne niezrozumialych snow.",
"> Uczucie ciepla i bliskosci rowiesnikow." },
{ "Czy w twojej krypcie dzialo sie cos niezwyklego?", //2
"> A co moglo sie dziac w krypcie niezwyklego ?",
"> Zycie tam bylo straszne! Cudem zyje.",
"> Najdziwniejsze miejsce jakie mozna sobie wyobrazic.",
"> Niezwykle bylo zycie w tej krypcie. Chcialoby sie \n tam wrocic.",
"> Tak." },
{ "Ktory z ponizszych opisow najbardziej do Ciebie \n pasuje?", //3
"> Sympatyczny grubas.",
"> Chuderlawy typ z problemami z cera.",
"> Najlepszy lachon na pustkowiach.",
"> Narkoman.",
"> Samiec alfa ociekajacy testosteronem." },
{ "Kiedy znajdziesz sie w zatloczonym miejscu, jak \n reagujesz?", //4
"> Nie przeszkadza mi obecnosc innych.",
"> Dobrze jest! Dawac mi tu kompana do gry w ruletke \n albo black jacka.",
"> Zabierzcie ode mnie ten zmutowany plebs!",
"> Nie lubie takich miejsc - zawsze nie starcza dla \n mnie zabawek.",
"> A moge ich zjesc?" },
{ "Twoje wymarzone miejsce na urlop to?",	//5
"> Jakas przytulna jaskinia, moze byc ewentualnie nora \n lub bunkier.",
"> Samotnia na wastelandzie z dala od ludzi, ghuli i \n innego paskudztwa.",
"> Kasyno.",
"> Burdel.",
"> Jakis porzadny postapokaliptyczny salon pieknosci." },
{ "Kiedy podczas podrozy po wastelandzie zniszczeniu \n ulegnie czesc twojego ekwipunku, jak zareagujesz?", //6
"> Tylko nie tooo!!! Niech mnie ktos rzuci ghulom na \n pozacie!",
"> Zaczynam sie rozgladac za jakims fagasem, ktory sie \n tym zajmie, by zdobyc moje wzgledy.",
"> Rozwale cala reszte ghhrrrr...",
"> Troche tasmy, patykow i bedzie dzialac jak ta lala!",
"> A ta srubka idzie gdzie?." },
{ "Zakladasz najbardziej odjazdowa kapele na wastelandzie, \n jej nazwa to:", //7
"> Zmutowany kot.",
"> Bunkrowe przedszkole.",
"> Kukielka.",
"> Ja i moj harem.",
"> Hrrrfkbbkwwwww!!!!" },
{ "Najstraszniejszy koszmar jaki potrafisz sobie \n wyobrazic to:", //8
"> Jestem otoczony/a przez mordercze kukielki, ktore \n probuja mnie zabic.",
"> Stado kobiet wrzeszczacych na mnie.",
"> Jestem goniony przez stado grubych dzieciakow na \n panterach, a amunicja wlasnie sie skonczyla.",
"> Zmuszaja mnie do patrzenia na wielka zarowke, ktora \n roztapia mi oczy.",
"> Tone w morzu kaszy, do tego zle przyprawionej!" },
{ "Twoj aktualny najwiekszy problem to:", //9
"> Niedobor rozrywek.",
"> Boje sie, ze zostane stara panna.",
"> Nie mam z kim porozmawiac.",
"> Zjadlbym cos.",
"> Boje sie, ze strace przyjaciela." },
{ "Znajdujesz na wastelandzie sklep wypelniony po brzegi \n rarytasami, niestety slyszysz nadchodzace mutanty i \n czasu starczy Ci na zabranie.", //10
"> Butelka nuka-coli.",
"> Karty do gry.",
"> Granat.",
"> Zestaw majsterkowicza.",
"> Sprzet medyczny/stimpaki." } };

int macierz_betonu[10][6] = 
{ {1,0,0,1,0,0}, //1
{1,0,0,0,1,0}, //2
{2,0,0,0,0,0}, //3
{2,0,1,0,1,0}, //4
{1,1,1,0,0,0}, //5 
{2,1,1,0,0,0}, //6
{2,0,0,0,0,0}, //7
{2,1,0,1,1,0}, //8
{1,0,0,0,0,0}, //9
{3,0,0,0,0,0}, //10
};

int suma_pkt[21] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int wskaznik_krypty = 0;
sf::String wyliczona_krypta;

sf::String krypty[21] =
{ 
"Nalezales/as do krypty 0. \n\n Krypta nie byla elementem Eksperymentu Spo³ecznego. Zostala zaprojektowania do nadzorowania innych \n krypt. Funkcjê kontroln¹ wszystkich systemów Krypty pe³ni³ komputer po³¹czony z kilkoma ludzkimi mózgami, zwany jako Kalkulator. W przypadku wybuchu wojny nuklearnej Kalkulator, wykorzystuj¹c zasoby intelektualne i techniczne zgromadzone w Krypcie 0 mia³ poprowadziæ ludzi do nowego œwiata, pomagaj¹c im ponownie zasiedliæ zniszczony kontynent. \n\nLokalizacja: Swiatynia wojny, Gora Cheyenne, Colorado.", //1
"Nalezales/as do krypty 7. \n\n W krypcie sprawdzono jak mieszkancy poradza sobie bez \n medykamentow i pomocy medycznej.", //2
"Nalezales/as do krypty 8. \n\n Krypta posiadajaca nadmiar G.E.C.K'ow", //3
"Nalezales/as do krypty 10. \n\n Krypta w ktorej jedynym mozliwym napojem jest \n Nuka-Cola. Przewiduje sie nagly wzrost masy wszystkich \n mieszkancow.", //4
"Nalezales/as do krypty 11. \n\n Krypta zaprojektowania do badan nad ludzka natura. \n Sprawdzane jest w jakim stopniu ludzie beda w stanie \n sie poswiecic dla siebie.", //5
"Nalezales/as do krypty 12. \n\n Krypta miala nigdy sie nie zamknac. Przewiduje sie \n mutacje u mieszkacow.", //6
"Nalezales/as do krypty 17. \n\n Krypta zostala najechana przez supermutanty.", //7
"Nalezales/as do krypty 21. \n\n Krypta przepelniona tylko i wylacznie rozrywkami z \n dziedziny hazardu.", //8
"Nalezales/as do krypty 27. \n\n Krypta przepelniona. Zostalo w niej umieszczone dwa \n razy wiecej ludzi, niz przewidywalne maksimum.", //9
"Nalezales/as do krypty 29. \n\n Krypta w ktorej umieszczono same dzieci.", //10
"Nalezales/as do krypty 34. \n\n Krypta posiadajaca duza ilosc broni i amunicji.", //11
"Nalezales/as do krypty 36. \n\n W krypcie jedynym mozliwym pozywieniem do \n wyprodukowania byla kasza.", //12
"Nalezales/as do krypty 42. \n\n Krypta w ktorej zarowki mialy moc maksymalnie 40W.", //13
"Nalezales/as do krypty 43. \n\n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n jedna pantere.", //14
"Nalezales/as do krypty 53. \n\n Krypta w ktorej wszystkie urzadzenia mialy sie \n bezustannie psuc. Powinny one wywowalac ogromny stres.", //15
"Nalezales/as do krypty 55. \n\n Krypta pozbawiona rozrywek dla ludzi zamknietych.", //16
"Nalezales/as do krypty 68. \n\n W krypcie zostala umieszczona jedna kobieta i 999 \n mezczyzn.", //17
"Nalezales/as do krypty 69. \n\n W krypcie zostal umieszczony jeden mezczyzna i 999 \n kobiet.", //18
"Nalezales/as do krypty 77. \n\n W krypcie zostal umieszczony jeden czlowiek i duza \n ilosc maskotek.", //19
"Nalezales/as do krypty 106. \n\n Krypta w ktorej ludzie poddawani sa caly czas srodkom \n psychoaktywnym", //20
"Nalezales/as do krypty 108. \n\n Krypta w ktorej umieszczono zwyklych ludzi, klona \n agresywnego osobnika (Gary'ego) i dano mozliwosc \n klonowania Gary'ego" //22
};

//krypta 0 niespójna. dobrze by by³o usun¹æ
