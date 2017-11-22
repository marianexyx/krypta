#include <SFML/Window.hpp>

sf::String introduction =
"Algorytm pozwala zidentyfikowac pochodzenie mieszkanca \n"
"krypty na podstawie specjalnie opracowanego zestawu \n"
"10ciu pytan. \n"
"\n"
"Wyniki powinny byc pozytywne w 95 %. \n"
"\n"
"Niektore odpowiedzi moga byc dostosowane tylko dla \n"
"mezczyzn lub kobiet.";

sf::String start_quiz = "> Wprowadz swoj adres email, aby rozpoczac test.";
sf::String start_bad_email = "> BLAD: Zly adres email. Wprowadz poprawny adres.";

sf::String sfStrZgodaNaMail = 
"Podajac emaila wyrazasz zgode na przesylanie przez Stowarzyszenie Organizatorow Symulacji i Gier\n"
"Terenowych \"OldTown\" informacji handlowych droga elektroniczna, zgodnie z ustawa z dnia 18 lipca\n"
"2002 r. o swiadczeniu uslug droga elektroniczna (Dz. U. z 2002 r. Nr 144, poz. 1204 z pozn. zm.).";


int odp[10][6][26] =
{ { { 3, 8, 1, 3, 8, 10, 10, 3, 1, 2, 0, 2, 5, 6, 10, 2, 1, 6, 4, 1, 0, 3, 8, 8, 7, 0 }, //1
{ 7, 10, 3, 3, 6, 9, 10, 9, 3, 7, 1, 6, 4, 4, 8, 7, 9, 6, 5, 2, 0, 8, 5, 6, 9, 0 },
{ 9, 1, 0, 2, 3, 1, 0, 1, 8, 3, 9, 4, 10, 5, 1, 5, 0, 10, 4, 6, 10, 1, 2, 3, 4, 10 },
{ 2, 2, 6, 6, 3, 7, 8, 3, 2, 2, 4, 1, 6, 5, 4, 2, 3, 5, 8, 3, 9, 8, 5, 9, 5, 4 },
{ 6, 5, 7, 8, 3, 0, 0, 4, 7, 3, 8, 1, 4, 2, 0, 3, 1, 5, 6, 6, 6, 1, 3, 8, 2, 10 },
{ 2, 1, 3, 2, 2, 0, 0, 2, 2, 2, 1, 1, 1, 0, 1, 2, 2, 2, 1, 3, 2, 10, 3, 1, 0, 4 } },
{ { 10, 3, 10, 9, 6, 1, 0, 8, 7, 3, 7, 2, 8, 4, 10, 5, 7, 10, 9, 9, 9, 1, 2, 0, 1, 9 }, //2
{ 1, 5, 1, 4, 6, 9, 6, 7, 2, 1, 5, 2, 0, 1, 7, 2, 7, 0, 7, 7, 7, 9, 3, 9, 8, 0 },
{ 3, 1, 9, 6, 2, 0, 1, 5, 5, 1, 7, 1, 1, 3, 1, 4, 1, 0, 4, 4, 2, 6, 3, 6, 1, 2 },
{ 0, 9, 2, 3, 7, 10, 10, 1, 1, 5, 3, 7, 1, 2, 9, 1, 7, 0, 6, 6, 9, 0, 4, 4, 8, 9 },
{ 7, 1, 5, 10, 5, 0, 1, 4, 4, 1, 7, 5, 9, 5, 3, 4, 2, 2, 4, 4, 5, 5, 2, 1, 4, 2 },
{ 2, 2, 1, 1, 2, 1, 0, 1, 4, 2, 3, 1, 1, 2, 3, 2, 1, 2, 4, 5, 1, 3, 10, 2, 2, 0 } }	,
{ { 7, 1, 5, 10, 5, 0, 1, 3, 4, 1, 7, 5, 9, 5, 3, 5, 2, 2, 4, 4, 4, 2, 2, 2, 4, 5 }, //3
{ 4, 9, 5, 0, 4, 10, 5, 3, 3, 6, 3, 3, 2, 7, 7, 5, 7, 6, 4, 4, 7, 7, 8, 8, 4, 2 },
{ 5, 3, 5, 0, 5, 0, 0, 2, 6, 5, 5, 4, 0, 7, 6, 2, 5, 5, 10, 9, 8, 6, 3, 2, 3, 4 },
{ 3, 7, 4, 5, 8, 8, 7, 4, 5, 7, 0, 2, 2, 4, 5, 3, 8, 4, 4, 5, 7, 4, 7, 9, 8, 0 },
{ 5, 4, 7, 4, 8, 5, 10, 7, 7, 8, 1, 9, 5, 6, 6, 2, 5, 5, 9, 10, 5, 2, 2, 4, 10, 2 },
{ 0, 2, 0, 2, 4, 7, 1, 10, 3, 8, 4, 3, 3, 3, 2, 2, 8, 2, 6, 3, 4, 8, 4, 7, 8, 0 } },
{ { 5, 2, 5, 3, 9, 7, 4, 2, 5, 0, 4, 5, 5, 2, 3, 2, 4, 4, 5, 5, 10, 1, 2, 5, 4, 7 }, //4
{ 6, 3, 6, 4, 3, 3, 3, 2, 10, 4, 5, 5, 5, 3, 5, 1, 6, 4, 8, 3, 7, 1, 3, 5, 2, 2 },
{ 8, 5, 8, 5, 7, 3, 3, 7, 4, 10, 2, 4, 5, 6, 5, 3, 6, 6, 2, 9, 0, 7, 5, 4, 8, 0 },
{ 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 10, 7, 3, 3, 2, 4, 2, 3, 6, 2, 0, 3, 4, 5, 7, 2 },
{ 0, 2, 0, 3, 2, 10, 7, 4, 0, 4, 0, 0, 3, 0, 1, 3, 0, 2, 0, 0, 0, 5, 1, 5, 8, 0 },
{ 3, 2, 1, 3, 2, 2, 4, 1, 1, 0, 0, 1, 2, 2, 6, 1, 2, 0, 0, 0, 0, 3, 1, 2, 2, 10 } },
{ { 2, 2, 2, 2, 2, 7, 7, 3, 1, 9, 0, 2, 2, 10, 2, 0, 2, 2, 2, 1, 4, 1, 2, 5, 5, 0 }, //5
{ 2, 2, 2, 2, 2, 3, 3, 2, 1, 10, 1, 2, 2, 6, 2, 7, 2, 2, 2, 2, 0, 6, 1, 3, 2, 2 },
{ 6, 5, 6, 6, 5, 1, 1, 7, 10, 2, 2, 4, 4, 4, 3, 6, 6, 8, 7, 2, 7, 2, 5, 4, 3, 3 },
{ 6, 2, 7, 7, 5, 2, 2, 4, 6, 2, 0, 5, 6, 6, 4, 6, 8, 9, 10, 0, 10, 3, 4, 8, 4, 2 },
{ 6, 2, 7, 6, 5, 2, 2, 4, 2, 2, 0, 3, 5, 5, 4, 8, 7, 8, 0, 10, 2, 5, 2, 3, 4, 8 },
{ 6, 2, 2, 1, 1, 0, 4, 1, 4, 0, 6, 2, 1, 2, 4, 10, 2, 2, 0, 0, 2, 1, 6, 0, 6, 4 } },
{ { 3, 3, 3, 5, 3, 3, 3, 3, 4, 3, 6, 2, 4, 4, 3, 2, 10, 3, 2, 6, 3, 6, 6, 3, 2, 8 }, //6
{ 4, 4, 4, 1, 2, 1, 1, 6, 3, 3, 1, 2, 2, 3, 3, 3, 3, 3, 1, 10, 0, 5, 4, 2, 0, 4 },
{ 2, 2, 2, 3, 2, 7, 10, 5, 2, 2, 5, 2, 2, 2, 2, 6, 8, 2, 2, 3, 2, 9, 4, 4, 10, 0 },
{ 5, 4, 5, 3, 5, 2, 1, 1, 6, 5, 2, 6, 5, 5, 5, 4, 4, 5, 7, 3, 5, 2, 2, 2, 0, 3 },
{ 4, 4, 4, 5, 4, 6, 6, 3, 5, 5, 6, 3, 5, 5, 4, 2, 2, 5, 3, 6, 4, 1, 5, 6, 6, 1 },
{ 2, 1, 2, 3, 1, 2, 0, 2, 2, 0, 0, 1, 2, 1, 2, 3, 2, 2, 1, 1, 2, 3, 9, 0, 0, 0 } },
{ { 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 0, 1, 3, 10, 1, 1, 1, 1, 1, 1, 2, 2, 6, 4, 1, 1 }, //7
{ 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 10, 1, 1, 1, 1, 4, 1, 1, 1, 1, 3, 1, 4, 2, 1, 2 },
{ 2, 1, 1, 1, 1, 1, 1, 3, 1, 2, 6, 3, 1, 2, 1, 5, 1, 1, 1, 1, 10, 3, 5, 3, 1, 4 },
{ 3, 3, 2, 3, 3, 3, 2, 6, 2, 2, 2, 2, 2, 1, 6, 2, 1, 1, 10, 10, 1, 6, 3, 1, 1, 2 },
{ 0, 0, 0, 0, 0, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 8, 4, 0, 10, 0 },
{ 2, 1, 3, 0, 0, 0, 1, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 9, 0, 1, 1, 0 } },
{ { 2, 2, 3, 3, 2, 1, 1, 7, 1, 2, 5, 1, 1, 1, 2, 4, 1, 1, 1, 1, 10, 7, 7, 3, 2, 3 }, //8
{ 5, 5, 5, 4, 5, 2, 2, 3, 4, 5, 1, 3, 3, 3, 2, 7, 1, 1, 1, 10, 0, 6, 1, 4, 2, 3 },
{ 1, 1, 1, 8, 2, 2, 2, 2, 1, 7, 8, 9, 2, 2, 8, 2, 2, 2, 3, 3, 3, 3, 6, 6, 2, 3 },
{ 2, 2, 2, 2, 2, 2, 2, 6, 2, 2, 2, 2, 2, 10, 2, 4, 2, 2, 2, 2, 2, 4, 5, 2, 2, 4 },
{ 3, 2, 1, 1, 2, 1, 2, 1, 3, 1, 2, 1, 10, 2, 2, 3, 1, 2, 1, 1, 1, 0, 4, 5, 1, 0 },
{ 3, 2, 2, 3, 10, 2, 4, 8, 4, 0, 4, 0, 1, 2, 6, 10, 0, 0, 0, 1, 2, 0, 2, 0, 6, 1 } },
{ { 4, 2, 2, 3, 3, 2, 3, 4, 1, 2, 2, 2, 3, 3, 3, 6, 8, 10, 3, 2, 3, 0, 7, 3, 3, 3 }, //9
{ 4, 4, 3, 4, 3, 2, 1, 1, 4, 3, 3, 2, 3, 3, 2, 1, 2, 3, 2, 10, 3, 1, 4, 3, 3, 1 },
{ 3, 3, 3, 3, 3, 3, 3, 3, 2, 0, 4, 4, 3, 3, 2, 2, 4, 4, 3, 5, 10, 4, 0, 3, 2, 3 },
{ 2, 2, 2, 6, 3, 9, 9, 2, 5, 9, 4, 6, 3, 2, 5, 3, 4, 4, 3, 7, 4, 3, 3, 3, 3, 7 },
{ 5, 8, 5, 5, 10, 6, 5, 8, 5, 5, 5, 5, 5, 5, 4, 2, 5, 5, 5, 5, 0, 5, 4, 5, 0, 2 },
{ 2, 4, 1, 3, 1, 0, 4, 0, 0, 0, 1, 2, 2, 3, 2, 0, 1, 2, 0, 2, 0, 1, 3, 0, 0, 10 } },
{ { 2, 2, 4, 10, 3, 3, 1, 2, 4, 5, 8, 2, 9, 4, 3, 3, 5, 5, 3, 4, 4, 2, 5, 4, 2, 5 }, //10
{ 4, 2, 4, 3, 4, 1, 1, 3, 10, 5, 7, 4, 4, 1, 4, 2, 5, 9, 8, 2, 2, 3, 4, 4, 2, 4 },
{ 4, 2, 4, 2, 3, 4, 1, 7, 2, 2, 0, 10, 2, 2, 6, 5, 2, 2, 7, 1, 1, 1, 0, 4, 2, 2 },
{ 5, 2, 4, 2, 3, 5, 1, 2, 4, 2, 2, 7, 2, 2, 2, 4, 10, 3, 9, 2, 4, 4, 0, 4, 2, 3 },
{ 5, 10, 4, 2, 3, 5, 1, 5, 4, 3, 1, 8, 2, 7, 8, 1, 7, 3, 2, 9, 4, 5, 5, 4, 2, 8 },
{ 0, 2, 0, 2, 1, 0, 2, 10, 2, 4, 3, 4, 0, 4, 0, 1, 1, 5, 1, 1, 2, 1, 3, 3, 2, 4 } } };

sf::String pyt[10][7] =
{ { "Jaka jest pierwsza rzecz jaka pamietasz?", //1
"> Wielki potwor zywiacy sie rowiesnikami.",
"> Niepokoj przebywania w krypcie.",
"> W sumie nie pamietam nic ciekawego wartego \n  zapamietania.",
"> Dziecinstwo moje bylo pelne niezrozumialych snow.",
"> Uczucie ciepla i bliskosci rowiesnikow.",
"> Muzyka." },
{ "Czy w twojej krypcie dzialo sie cos niezwyklego?", //2
"> A co moglo sie dziac w krypcie niezwyklego ?",
"> Zycie tam bylo straszne! Cudem zyje.",
"> Najdziwniejsze miejsce jakie mozna sobie wyobrazic.",
"> Niezwykle bylo zycie w tej krypcie. Chcialoby sie \n  tam wrocic.",
"> Tak.",
"> Gobliny! Smoki! Czary!" },
{ "Ktory z ponizszych opisow najbardziej do Ciebie \npasuje?", //3
"> Sympatyczny grubas.",
"> Chuderlawy typ z problemami z cera.",
"> Najlepszy lachon na pustkowiach.",
"> Narkoman.",
"> Samiec alfa ociekajacy testosteronem.",
"> Agresywny neandertalczyk." },
{ "Kiedy znajdziesz sie w zatloczonym miejscu, jak \nreagujesz?", //4
"> Nie przeszkadza mi obecnosc innych.",
"> Dobrze jest! Dawac mi tu kompana do gry w ruletke \n  albo black jacka.",
"> Zabierzcie ode mnie ten zmutowany plebs!",
"> Nie lubie takich miejsc - zawsze nie starcza dla \n  mnie zabawek.",
"> A moge ich zjesc?",
"> Nie bywam w takich miejscach." },
{ "Twoje wymarzone miejsce na urlop to?",	//5
"> Jakas przytulna jaskinia, moze byc ewentualnie nora \n  lub bunkier.",
"> Samotnia na wastelandzie z dala od ludzi, ghuli i \n  innego paskudztwa.",
"> Kasyno.",
"> Burdel.",
"> Jakis porzadny postapokaliptyczny salon pieknosci.",
"> Gdziekolwiek, byle wsrod swoich." },
{ "Kiedy podczas podrozy po wastelandzie zniszczeniu \nulegnie czesc twojego ekwipunku, jak zareagujesz?", //6
"> Tylko nie tooo!!! Niech mnie ktos rzuci ghulom na \n  pozacie!",
"> Zaczynam sie rozgladac za jakims fagasem, ktory sie \n  tym zajmie, by zdobyc moje wzgledy.",
"> Rozwale cala reszte ghhrrrr...",
"> Troche tasmy, patykow i bedzie dzialac jak ta lala!",
"> A ta srubka idzie gdzie?.",
"> Magiczny proszek pomoze na wszystko." },
{ "Zakladasz najbardziej odjazdowa kapele na wastelandzie, \njej nazwa to:", //7
"> Zmutowany kot.",
"> Bunkrowe przedszkole.",
"> Kukielka.",
"> Ja i moj harem.",
"> Hrrrfkbbkwwwww!!!!",
"> La amour de la Theatre." },
{ "Najstraszniejszy koszmar jaki potrafisz sobie \nwyobrazic to:", //8
"> Jestem otoczony/a przez mordercze kukielki, ktore \n  probuja mnie zabic.",
"> Stado kobiet wrzeszczacych na mnie.",
"> Jestem goniony przez stado grubych dzieciakow na \n  panterach, a amunicja wlasnie sie skonczyla.",
"> Zmuszaja mnie do patrzenia na wielka zarowke, ktora \n  roztapia mi oczy.",
"> Tone w morzu kaszy, do tego zle przyprawionej!",
"> Nie jestem juz czescia mojej ekipy." },
{ "Twoj aktualny najwiekszy problem to:", //9
"> Niedobor rozrywek.",
"> Boje sie, ze zostane stara panna.",
"> Nie mam z kim porozmawiac.",
"> Zjadlbym cos.",
"> Boje sie, ze strace przyjaciela.",
"> Nie mam problemow." },
{ "Znajdujesz na wastelandzie sklep wypelniony po brzegi \nrarytasami, niestety slyszysz nadchodzace mutanty i \nczasu starczy Ci na zabranie.", //10
"> Butelka nuka-coli.",
"> Karty do gry.",
"> Granat.",
"> Zestaw majsterkowicza.",
"> Sprzet medyczny/stimpaki.",
"> Jakis gadzet w moim ulubionym kolorze." } };

int dodatkowePodstwietlanieOdpowiedzi[10][7] =
{ { 1,0,0,1,0,0,0 }, //1
{ 1,0,0,0,1,0,0 }, //2
{ 2,0,0,0,0,0,0 }, //3
{ 2,0,1,0,1,0,0 }, //4
{ 1,1,1,0,0,0,0 }, //5 
{ 2,1,1,0,0,0,0 }, //6
{ 2,0,0,0,0,0,0 }, //7
{ 2,1,0,1,1,0,0 }, //8
{ 1,0,0,0,0,0,0 }, //9
{ 3,0,0,0,0,0,0 }, //10
};

int suma_pkt[26] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int wskaznik_krypty = 0;
sf::String wyliczona_krypta;

sf::String krypty[26] =
{ ///////////////////////////////W krypcie sprawdzono jak mieszkancy poradza sobie bez W krypcie sprawdzono jak mieszkancy poradza sobi| Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i  \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n
	"Nalezales/as do krypty 0. \n\n Krypta nie byla elementem Eksperymentu Spolecznego. \n Zostala zaprojektowania do nadzorowania innych \n krypt. Funkcje kontrolna wszystkich systemow Krypty\n pelnil komputer polaczony z kilkoma ludzkimi mozgami,\n zwany jako Kalkulator. W przypadku wybuchu wojny\n nuklearnej Kalkulator, wykorzystujac zasoby\n intelektualne i techniczne zgromadzone w krypcie 0 mial\n poprowadzic ludzi do nowego swiata, pomagajac im ponownie\n zasiedlic zniszczony kontynent.", //1
	"Nalezales/as do krypty 7. \n\n W krypcie sprawdzono jak mieszkancy poradza sobie bez \n medykamentow i pomocy medycznej.", //2
	"Nalezales/as do krypty 8. \n\n Krypta ktora ma sie otworzyc po 10 latach, by mieszkancy\n zaludnili powierzchnie. Krypta posiada rozwinieta\n technologie medyczna. Krypta otrzymala dodatkowe\n hydroprocesory przeznaczone dla krypty 13, zamiast\n zapasowego G.E.C.K'a.", //3
	"Nalezales/as do krypty 10. \n\n Krypta ktorej celem jest przetestowanie systemu barowego\n EAT'O'MATIC. Jedynym mozliwym napojem jest \n Nuka-Cola. Krypta nie zostala wyposazona w sale\n gimnastyczne. Przewiduje sie stopniowy wzrost masy\n wszystkich mieszkancow.", //4
	"Nalezales/as do krypty 11. \n\n Krypta zaprojektowania do badan nad ludzka natura. \n Sprawdzane jest w jakim stopniu ludzie beda w stanie \n sie poswiecic dla siebie. Co roku jedna osoba\n miala za zadanie poswiecic swe zycie dla przyszlosci\n krypty i jej mieszkancow. W innym wypadku wszyscy\n mieszkancy mieli zostac zabici przez komputer glowny\n (grozba zabicia wszystkich mieszkancow w razie nie\n wykonywania polecen byla fikcja). W krypcie ukryta byla\n Komora Ofiarna. Ofiare przed smiercia informowano,\n ze jest doskonalym przykladem dla ludzkosci i ze\n dzieki niej nikt nie zostanie zabity.", //5
	"Nalezales/as do krypty 12. \n\n Krypta ktorej wrota mialy nigdy sie nie zamknac, by\n wystawic mieszkancow na promieniowanie. Przewiduje sie\n mutacje u mieszkacow.", //6
	"Nalezales/as do krypty 17. \n\n Krypta zostala najechana przez supermutanty.", //7
	"Nalezales/as do krypty 19. \n\n Krypta w ktorej ludzie przynaleza do jednego z dwoch\n obozow(czerwonego albo blekitnego) wrogo do siebie\n nastawionych.",
	"Nalezales/as do krypty 21. \n\n Krypta przepelniona tylko i wylacznie rozrywkami z\n dziedziny hazardu. Krypta nie ma nadzorcy, wszyscy jej\n mieszkancy sa sobie rowni. Mieszkancy wszystkie\n swoje problemy powinni zalatwiac poprzez hazard;\n droga przypadku. Przewiduje sie rozrost anarchii w\n krypcie.", //8
	"Nalezales/as do krypty 27. \n\n Krypta przepelniona. Zostalo w niej umieszczone dwa \n razy wiecej ludzi, niz przewidywalne maksimum, tj.\n 2000 zamiast 1000.", //9
	"Nalezales/as do krypty 29. \n\n Krypta w ktorej umieszczono dzieci w wieku maksymalnie\n 15 lat. Ewentualni rodzice wpuszczeni do krypty\n posiadali zaawansowane choroby o skutkach letalnych.\n Zamiast ludzkiego Nadzorcy schronowi przewodzi\n superkomputer ZAX, ktory jest zaprogramowany tak, by\n wychowywac dzieci przy pomocy robotow pomocnikow.\n Komputer bedzie wychowywal je w prymitywnej kulturze, a\n po osiagnieciu dojrzalosci wypusci z krypty, aby mogly\n zyc w kontrolowanym srodowisku.", //10
	"Nalezales/as do krypty 34. \n\n Krypta posiadajaca duza ilosc broni i amunicji dostepnej\n dla mieszkancow.", //11
	"Nalezales/as do krypty 36. \n\n Krypta w ktorej wytlaczarka zywnosci w Krypcie zostala\n zaprojektowana do produkcji tylko cienkiej, wodnistej\n papki. Braki w pozywieniu moga byc przyczyna\n podatnosci mieszkancow na choroby, oslabienia fizyczne\n i psychiczne. Moze to doprowadzic ich do wymarcia.", //12
	"Nalezales/as do krypty 42. \n\n Krypta w ktorej zarowki mialy moc maksymalnie 40W. Celem\n eksperymentu jest sprawdzenie wplywu efektow\n slabego oswietlenia na przyczyne depresji lub innych\n problemow psychologicznych mieszkancow Krypt.", //13
	"Nalezales/as do krypty 43. \n\n Krypta w ktorej zamknieto 20 mezczyzn, 10 kobiet i \n jedna pantere.", //14
	"Nalezales/as do krypty 50. \n\n Uczestnicy krypty zostali podzieleni na obozy, ktore\n mialy sie zwalczac politycznie.",
	"Nalezales/as do krypty 53. \n\n Jako czesc Eksperymentu Krypt, wiekszosc wyposazenia\n Krypty zostalo zaprojektowane, aby psulo sie co kilka\n miesiecy. Naprawy wyposazenia mialy maksymalnie\n stresowac mieszkancow.", //15
	"Nalezales/as do krypty 55. \n\n Jako czesc Eksperymentu Krypt, wszystkie tasmy z\n nagraniami rozrywkowymi Krypty zostaly usuniete. Socjolodzy\n przewidywali, ze ta Krypta przetrwa dluzej niz Krypta 56\n w ktorej wszystkie tasmy z nagraniami rozrywkowymi\n Krypty zostaly usuniete, oprocz tych, ktore zawieraly\n bardzo kiepskich aktorow komediowych.", //16
	"Nalezales/as do krypty 68. \n\n W krypcie zostala umieszczona jedna kobieta i 999 \n mezczyzn.", //17
	"Nalezales/as do krypty 69. \n\n W krypcie zostal umieszczony jeden mezczyzna i 999 \n kobiet.", //18
	"Nalezales/as do krypty 77. \n\n W krypcie zostal umieszczony jeden czlowiek i duza \n ilosc maskotek.", //19
	"Nalezales/as do krypty 92. \n\n Krypta zamieszkana przez slawnych muzynkow. Poddawani\n oni byli dzialaniu fal SBS, które wywolywaly w nich\n agresje.",
	"Nalezales/as do krypty 95. \n\n Jako uczestnikow krypty wybrano samych narkomanow.\n Uczestnicy eksperymentu mieli zostac wyleczeni z nalogu,\n by po 5 latach dac im dostep do duzej ilosci narkotykow, by\n sprawdzic ich reakcje.",
	"Nalezales/as do krypty 106. \n\n Krypta w ktorej w ramach eksperymentu, dziesiec dni\n po zamknieciu krypty do klimatyzacji wpuszczono duze\n ilosci silnych srodkow psychoaktywnych. W krypcie\n nie ma wewnetrznego panelu otwierania glownego wlazu\n przez co krypte mozna otworzyc tylko z zewnatrz.", //20
	"Nalezales/as do krypty 108. \n\n Jako czesc Eksperymentu Krypt mialo byc zbadanie konfliktu\n o wladze w Krypcie. Nadzorca w momencie zamkniecia Krypty\n 108 byl chory na raka i mial umrzec okolo 40 miesiecy\n po zakmniecia krypty. Laboratorium klonowania sprawialo,\n ze w krypcie co jakis czas pojawialy sie identyczne klony,\n a sam eksperyment z klonami mial na celu pokazac czy\n zdolaja sie one zorganizowac i czy zapanuja nad krypta.", //21
	"Nalezales/as do krypty 112. \n\n Kazdy z 85 mieszkacow krypty tkwi w symulacji komputerowej\n utopijnego spokojnego miasteczka."

};

//krypta 0 niespójna z uniwersum. dobrze by by³o usunšæ
//krypt 7 nie ma nigdzie infomacji? skad ja jš wzia³em?
//krypta 17- nie ma ¿adnych oficjalnych ciekawych danych o niej- usunac