// WindowsProject1.cpp : definiuje punkt wejścia dla aplikacji
//

#pragma once

#include "stdafx.h"
#include "WindowsProject1.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <string>
#include <iostream>
#include "krypta.h"
#include "funkcje.h"
#include <cstdio>

#define MAX_LOADSTRING 100

using namespace sf;
using namespace std;

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

// Przekaż dalej deklaracje funkcji dołączonych w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // tu się zaczyna kod typu "body"
	enum WINDOW_STATE { INTRO, QUESTIONS, RESULT };
	WINDOW_STATE stan_okna;

	const int WIDTH = VideoMode::getDesktopMode().width;
	const int HEIGHT = VideoMode::getDesktopMode().height;
	/* */
	int background_WIDTH = 1280;
	int background_HEIGHT = 1024; //*/
	/* * /
	int background_WIDTH = 1152;
	int background_HEIGHT = 854; //*/

	Clock clockLoadOneChar;
	Clock clockBlinkingSquare;
	Clock clockLoadingTextSound;
	Clock clockShowingResult; //po pewnym czasie po podaniu krypty będzie przeskakiwał ze stanu RESULT do INTRO
	Time elapsedtime;
	Time migajacy_kwadracik_time;
	Time loading_txt_time;
	Time gotointrotime; //po pewnym czasie po podaniu krypty będzie przeskakiwał ze stanu RESULT do INTRO
	Time CzasOdAktualizacji = Time::Zero; // pomocniczy do tego powyżej
	bool czas_minal = false; //jeżeli minie powyższy czas to przestawiana jest ta flaga

	//RenderWindow window(VideoMode(WIDTH - 1, HEIGHT - 1), "Fallout 3 terminal", Style::Resize);
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Fallout 3 terminal", Style::Fullscreen); //Style::Fullscreen);
	window.setMouseCursorVisible(false);
	View fixed = window.getView(); // Create a fixed view

	//cursor
	Texture mouse;
	mouse.loadFromFile("fallout_3_cursor_3.png");
	Sprite sprCursor(mouse);

	//kolorki
	Texture background;
	background.loadFromFile("gterminal.png");
	Sprite sprite;
	sprite.setTexture(background);
	sprite.setScale(Vector2f((double)HEIGHT / (double)background_HEIGHT, (double)WIDTH / (double)background_WIDTH));
	Color zielony_fallout(40, 247, 129);

	//czcionki
	const int font_size = 0.028*WIDTH;
	Font font;
	font.loadFromFile("Fixedsys500c.ttf");

	const int linijek_wstepu = 6;
	Text wstep[linijek_wstepu];
	String naglowek[] =
	{ "ROBCO INDUSTRIES UNIFIED OPERATING SYSTEM",
		"COPYRIGHT 2075-2077 ROBCO INDUSTRIES",
		"-Server 48-",
		" ",
		"Z jakiej krypty pochodzisz (v.3)",
		"_____________________________________" };
	for (int i = 0; i <= linijek_wstepu-1; i++)
	{
		wstep[i].setFont(font);
		wstep[i].setCharacterSize(font_size);
		wstep[i].setFillColor(zielony_fallout);
		wstep[i].setString(naglowek[i]);
		if (i >= 0 && i < 4)
			wstep[i].setPosition(WIDTH / 2 - wstep[i].getGlobalBounds().width / 2, 0.015*HEIGHT + i*0.04*HEIGHT);
		if (i == 4) wstep[i].setPosition(WIDTH / 14, 0.015*HEIGHT + i*0.04*HEIGHT);
		if (i == 5) wstep[i].setPosition(WIDTH / 14, i*0.038*HEIGHT);
	}

	stan_okna = INTRO;

	Text txt_intro;
	Text txt_start_quiz;
	txt_intro.setFont(font);
	txt_intro.setCharacterSize(font_size);
	txt_intro.setFillColor(zielony_fallout);
	txt_intro.setPosition(WIDTH / 14, 0.25*HEIGHT);

	txt_start_quiz.setFont(font);
	txt_start_quiz.setCharacterSize(font_size);
	txt_start_quiz.setFillColor(zielony_fallout);
	txt_start_quiz.setPosition(WIDTH / 14, 0.25*HEIGHT + 0.34*HEIGHT);

	Text twoja_krypta;
	twoja_krypta.setFont(font);
	twoja_krypta.setCharacterSize(font_size);
	twoja_krypta.setFillColor(zielony_fallout);
	twoja_krypta.setPosition(WIDTH / 14, 0.25*HEIGHT);

	String strStopka = "> ";
	Text txt_stopka;
	txt_stopka.setString(strStopka);
	txt_stopka.setFont(font);
	txt_stopka.setCharacterSize(font_size);
	txt_stopka.setFillColor(zielony_fallout);
	txt_stopka.setPosition(WIDTH / 14, 0.875*HEIGHT);

	Text sfTxtZgodaNaMail;
	sfTxtZgodaNaMail.setFont(font);
	sfTxtZgodaNaMail.setCharacterSize((int)(font_size/1.7));
	sfTxtZgodaNaMail.setFillColor(zielony_fallout);
	sfTxtZgodaNaMail.setPosition(WIDTH / 14, 0.75*HEIGHT);

	double fOneCharWidth = 0.02*WIDTH;
	double fMailWidth = txt_stopka.getLocalBounds().width - fOneCharWidth;

	const int ilosc_opcji = 7; //sfTxtOpcje + pytanie
	Text sfTxtOpcje[ilosc_opcji];
	String sfStrOpcje[ilosc_opcji];
	String sfStringLongestQuestion = //wersja release w vs17 ma problem ze wstepna alokacja pamieci dla sfStringow...
		"------------------------------------------------------------------------------------"
		"------------------------------------------------------------------------------------";
	sfStrOpcje[0] = sfStringLongestQuestion; //...dlatego podaje mu stringa dluzszego niz jakikolwiek uzywany...
	//... - zostanie on na wstepie wyczyszczonyf
	int aktualne_opcje = 0;
	short sChoosenOption = 0;

	bool blokada_eventow = false;
	bool reset_testu = false;
	bool licz_krypte_raz = false;

	bool blokada_updown = false;
	int mouse_focus = 0;
	int aktualny_focus = 0;

	bool LOAD_INTRO = false; // warunek do załadowania od nowa intro

	RectangleShape podswietlenie;
	podswietlenie.setFillColor(zielony_fallout);

	RectangleShape migajacy_kwadracik;
	migajacy_kwadracik.setFillColor(zielony_fallout);
	migajacy_kwadracik.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
	migajacy_kwadracik.setSize(Vector2f(0.0165*HEIGHT, 0.02*WIDTH));

	//dźwięki
	SoundBuffer buffer_menu_focus;
	SoundBuffer buffer_enter;
	SoundBuffer buffer_szum;
	SoundBuffer buffer_load_string;
	SoundBuffer buffer_bad_email;
	SoundBuffer buffer_text_entered1;
	SoundBuffer buffer_text_entered2;
	SoundBuffer buffer_text_entered3;
	SoundBuffer buffer_text_entered4;
	SoundBuffer buffer_text_entered5;
	SoundBuffer buffer_text_entered6;
	Sound sound_enter;
	Sound sound_focus;
	Sound sound_background;
	Sound sound_load_string;
	Sound sound_bad_email;
	Sound sound_text_entered1;
	Sound sound_text_entered2;
	Sound sound_text_entered3;
	Sound sound_text_entered4;
	Sound sound_text_entered5;
	Sound sound_text_entered6;
	bool played_once[7] = { 0, 0, 0, 0, 0, 0, 0 };
	bool not_a_loop = false;

	sound_enter.setBuffer(buffer_enter);
	if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;

	sound_focus.setBuffer(buffer_menu_focus);
	if (!buffer_menu_focus.loadFromFile("sounds/ui_menu_focus.wav")) return -1;

	sound_load_string.setBuffer(buffer_load_string);
	if (!buffer_load_string.loadFromFile("sounds/ui_hacking_charscroll.wav")) return -1;

	sound_bad_email.setBuffer(buffer_bad_email);
	if (!buffer_bad_email.loadFromFile("sounds/ui_hacking_passbad.wav")) return -1;

	sound_background.setBuffer(buffer_szum);
	if (!buffer_szum.loadFromFile("sounds/obj_computerterminal_01_lp.wav")) return -1;
	sound_background.setLoop(true);
	sound_background.play();

	sound_text_entered1.setBuffer(buffer_text_entered1);
	if (!buffer_text_entered1.loadFromFile("sounds/ui_hacking_charsingle_01.wav")) return -1;
	sound_text_entered2.setBuffer(buffer_text_entered2);
	if (!buffer_text_entered2.loadFromFile("sounds/ui_hacking_charsingle_02.wav")) return -1;
	sound_text_entered3.setBuffer(buffer_text_entered3);
	if (!buffer_text_entered3.loadFromFile("sounds/ui_hacking_charsingle_03.wav")) return -1;
	sound_text_entered4.setBuffer(buffer_text_entered4);
	if (!buffer_text_entered4.loadFromFile("sounds/ui_hacking_charsingle_04.wav")) return -1;
	sound_text_entered5.setBuffer(buffer_text_entered5);
	if (!buffer_text_entered5.loadFromFile("sounds/ui_hacking_charsingle_05.wav")) return -1;
	sound_text_entered6.setBuffer(buffer_text_entered6);
	if (!buffer_text_entered6.loadFromFile("sounds/ui_hacking_charsingle_06.wav")) return -1;

	while (window.isOpen())
	{
		Event zdarzenie;
		Vector2f mouse(Mouse::getPosition(window));
		sprCursor.setPosition(static_cast<Vector2f>(Mouse::getPosition(window)));

		while (window.pollEvent(zdarzenie))
		{
			if (zdarzenie.type == Event::KeyPressed && (zdarzenie.key.code == Keyboard::Multiply || zdarzenie.key.code == Keyboard::Num0))
			{
				sound_enter.stop();
				sound_background.stop();
				sound_load_string.stop();
				sound_bad_email.stop();
				window.close();
			}
			else if (stan_okna == INTRO && !blokada_eventow && zdarzenie.type == Event::TextEntered && zdarzenie.text.unicode != 10 && zdarzenie.text.unicode != 13) //10 i 13 to entery
			{
				if (zdarzenie.text.unicode > 32 && strStopka.getSize() < 60) //znak jest textem i text nie wychodzi za ekran
					strStopka += String(zdarzenie.text.unicode);
				else if (zdarzenie.text.unicode == 8 && strStopka.getSize() > 2) //backspace i większe niż "> " 
					strStopka.erase(strStopka.getSize() - 1, strStopka.getSize());

				txt_stopka.setString(strStopka);

				fMailWidth = txt_stopka.getLocalBounds().width - fOneCharWidth;

				int nSoundDice = rand() % 6 + 1;
				switch (nSoundDice)
				{
				case 1: sound_text_entered1.play(); break;
				case 2: sound_text_entered2.play(); break;
				case 3: sound_text_entered3.play(); break;
				case 4: sound_text_entered4.play(); break;
				case 5: sound_text_entered5.play(); break;
				case 6: sound_text_entered6.play(); break;
				default: return -1;
				}
			}
			else if (stan_okna == INTRO && !blokada_eventow && 
				(((isTextContainsCursor(txt_start_quiz, sprCursor) && zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left) ||
				(zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Return))))
			{
				string stdStrEmail = strStopka;
				stdStrEmail.erase(0, 2); // usuń "> "
				stdStrEmail.erase(remove_if(stdStrEmail.begin(), stdStrEmail.end(), isspace), stdStrEmail.end()); //remove whitespaces
				if (isEmailOk(stdStrEmail))
				{
					sound_enter.play();

					writeEmailToFile(stdStrEmail);

					stan_okna = QUESTIONS;
					sChoosenOption = 0;

					//Text_css(ilosc_opcji, sfTxtOpcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
					//Text_css
					sfTxtOpcje[0].setCharacterSize(font_size);
					sfTxtOpcje[0].setFillColor(zielony_fallout);
					sfTxtOpcje[0].setPosition(WIDTH / 14, 0.25*HEIGHT); //pierwsza opcja jest rysowana zawsze bez pytania 
					const int ENTER = 0.038f*HEIGHT; //kolejne 5. jeżeli pierwsza ma enter, to każda kolejna jest przesunięta o enter
					int suma_enterow = 0;
					{
						for (int i = 1; i <= ilosc_opcji; ++i)
						{
							sfTxtOpcje[i].setCharacterSize(font_size);
							sfTxtOpcje[i].setFillColor(zielony_fallout);
							suma_enterow += dodatkowePodstwietlanieOdpowiedzi[aktualne_opcje][i - 1];
							sfTxtOpcje[i].setPosition(WIDTH / 14, 0.25*HEIGHT + i*0.06*HEIGHT + ENTER*suma_enterow);
						}
					}

					for (int i = 0; i <= ilosc_opcji-1; ++i) 
						sfTxtOpcje[i].setFont(font);
					//Char_by_char(aktualne_opcje, ilosc_opcji, sfStrOpcje);
					//Char_by_char
					licznik_opcji = 0;

					//przypisz pierwsze pytanie i odpowiedzi
					for (int i = 0; i <= ilosc_opcji-1; ++i) 
						sfStrOpcje[i] = pyt[aktualne_opcje][i];

					aktualne_opcje += 1;
				}
				else
				{
					sound_bad_email.play();

					txt_start_quiz.setString(start_bad_email);
					strStopka = "> ";
					fMailWidth = 0;
				}
			} 
			else if (stan_okna == QUESTIONS && !blokada_eventow )
			{
				if (zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left)
				{
					for (int i = 1; i <= ilosc_opcji-1; i++)
					{
						if (isTextContainsCursor(sfTxtOpcje[i], sprCursor))
						{
							sChoosenOption = i;
							break;
						}
					}
				}
				else if (zdarzenie.type == Event::KeyPressed)
				{
					if (zdarzenie.key.code == Keyboard::Up && !isTextContainsCursor(sfTxtOpcje[1], sprCursor) &&
						!isTextContainsCursor(sfTxtOpcje[2], sprCursor) && !isTextContainsCursor(sfTxtOpcje[3], sprCursor) &&
						!isTextContainsCursor(sfTxtOpcje[4], sprCursor) && !isTextContainsCursor(sfTxtOpcje[5], sprCursor) &&
						!isTextContainsCursor(sfTxtOpcje[6], sprCursor))
					{
						if (mouse_focus == 0) mouse_focus = 7;
						mouse_focus--;
						if (mouse_focus < 1) mouse_focus = 1;
					}
					else if (zdarzenie.key.code == Keyboard::Down && !isTextContainsCursor(sfTxtOpcje[1], sprCursor) &&
						!isTextContainsCursor(sfTxtOpcje[2], sprCursor) && !isTextContainsCursor(sfTxtOpcje[3], sprCursor) &&
						!isTextContainsCursor(sfTxtOpcje[4], sprCursor) && !isTextContainsCursor(sfTxtOpcje[5], sprCursor) &&
						!isTextContainsCursor(sfTxtOpcje[6], sprCursor))
					{
						mouse_focus++;
						if (mouse_focus >= ilosc_opcji) mouse_focus = ilosc_opcji-1;
					}
					else
					{
						switch (zdarzenie.key.code)
						{
						case Keyboard::Return:
							for (int i = 1; i <= ilosc_opcji - 1; i++)
							{
								if (aktualny_focus == i && !isTextContainsCursor(sfTxtOpcje[i], sprCursor))
									sChoosenOption = i;
							}
							break;
						case Keyboard::Num1: sChoosenOption = 1; break;
						case Keyboard::Num2: sChoosenOption = 2; break;
						case Keyboard::Num3: sChoosenOption = 3; break;
						case Keyboard::Num4: sChoosenOption = 4; break;
						case Keyboard::Num5: sChoosenOption = 5; break;
						case Keyboard::Num6: sChoosenOption = 6; break;
						default: sChoosenOption = 0; break;
						}
					}
				}

				if (sChoosenOption != 0)
				{
					mouse_focus = 0;
					aktualny_focus = 0;

					sound_enter.play();
	
					for (int i = 0; i <= 25; ++i)
						suma_pkt[i] += odp[aktualne_opcje-1][sChoosenOption-1][i];
					//Clean_options
					for (int i = 0; i <= ilosc_opcji - 1; ++i)
					{
						sfStrOpcje[i] = "";
						sfTxtOpcje[i].setString(sfStrOpcje[i]);
					}

					//Text_css
					sfTxtOpcje[0].setCharacterSize(font_size);
					sfTxtOpcje[0].setFillColor(zielony_fallout);
					sfTxtOpcje[0].setPosition(WIDTH / 14, 0.25*HEIGHT); //pierwsza opcja jest rysowana zawsze bez pytania 
					const int ENTER = 0.038f*HEIGHT; //kolejne. jeżeli pierwsza ma enter, to każda kolejna jest przesunięta o enter
					int suma_enterow = 0;
					{
						for (int i = 1; i <= ilosc_opcji-1; ++i)
						{
							sfTxtOpcje[i].setCharacterSize(font_size);
							sfTxtOpcje[i].setFillColor(zielony_fallout);
							suma_enterow += dodatkowePodstwietlanieOdpowiedzi[aktualne_opcje][i - 1];
							sfTxtOpcje[i].setPosition(WIDTH / 14, 0.25*HEIGHT + i*0.06*HEIGHT + ENTER*suma_enterow);
						}
					}

					for (int i = 0; i < ilosc_opcji-1; ++i)
						sfTxtOpcje[i].setFont(font);

					//Char_by_char
					licznik_opcji = 0;
					for (int i = 0; i <= ilosc_opcji - 1; ++i)
					{
						sfTxtOpcje[i].setFont(font); 
						sfStrOpcje[i].clear();
						sfStrOpcje[i] = pyt[aktualne_opcje][i];
					}

					aktualne_opcje += 1;

					sChoosenOption = 0;
				}
			}
			else if (stan_okna == RESULT && !blokada_eventow &&
				((isTextContainsCursor(sprite, sprCursor) && zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left) ||
				(zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Return)))
			{
				LOAD_INTRO = true;
			}
		}

		//stan okna = intro
		if (LOAD_INTRO == true || czas_minal == true)
		{
			sound_enter.play();

			stan_okna = INTRO;
			aktualne_opcje = 0;
			for (int i = 0; i <= 25; i++) 
				suma_pkt[i] = 0;
			wskaznik_krypty = 0;
			reset_testu = true;

			txt_intro.setFont(font);
			txt_intro.setCharacterSize(font_size);
			txt_intro.setFillColor(zielony_fallout);
			txt_intro.setPosition(WIDTH / 14, 0.25*HEIGHT);

			txt_start_quiz.setFont(font);
			txt_start_quiz.setCharacterSize(font_size);
			txt_start_quiz.setFillColor(zielony_fallout);
			txt_start_quiz.setPosition(WIDTH / 14, 0.25*HEIGHT + 0.34*HEIGHT);

			introduction =
				"Algorytm pozwala zidentyfikowac pochodzenie mieszkanca \n"
				"krypty na podstawie specjalnie opracowanego zestawu \n"
				"10ciu pytan. \n"
				"\n"
				"Wyniki powinny byc pozytywne w 95 %. \n"
				"\n"
				"Niektore odpowiedzi moga byc dostosowane tylko dla \n"
				"mezczyzn lub kobiet.";
			start_quiz = "> Wprowadz swoj adres email, aby rozpoczac test.";
			sfStrZgodaNaMail =	
				"Podajac emaila wyrazasz zgode na przesylanie przez Stowarzyszenie Organizatorow Symulacji i Gier\n"
				"Terenowych \"OldTown\" informacji handlowych droga elektroniczna, zgodnie z ustawa z dnia 18 lipca\n"
				"2002 r. o swiadczeniu uslug droga elektroniczna (Dz. U. z 2002 r. Nr 144, poz. 1204 z pozn. zm.).";

			licz_krypte_raz = false; //mozna ponownie obliczyć wynikową krytpę jeżeli przyjdzie na to czas
			czas_minal = false; //mozna będzie ponownie odczekać 30 sekund po podaniu wyniku krypty w RESULT, kiedy przyjdzie na to czas

			LOAD_INTRO = false;
			strStopka = "> ";
			fMailWidth = 0;
		}

		//nie mozna kliknac up/down jezeli myszka ma focus na którejś z opcji odpowiedzi
		if (isTextContainsCursor(sfTxtOpcje[1], sprCursor) || isTextContainsCursor(sfTxtOpcje[2], sprCursor) ||
			isTextContainsCursor(sfTxtOpcje[3], sprCursor) || isTextContainsCursor(sfTxtOpcje[4], sprCursor) ||
			isTextContainsCursor(sfTxtOpcje[5], sprCursor) || isTextContainsCursor(sfTxtOpcje[6], sprCursor))
		{
			blokada_updown = true;
		}
		else
		{
			blokada_updown = false;
			if (mouse_focus == 0) //jeżeli żadna opcja up/down nie jest podświetlona
				aktualny_focus = 0; //to nie można wcisnąć enter
		}

		//wczytywanie textowej zawartości stron
		elapsedtime += clockLoadOneChar.restart();
		while (elapsedtime >= seconds(.007f)) 
		{
			elapsedtime -= seconds(.007f);
			switch (stan_okna)
			{
			case INTRO:
				if (introduction.getSize() > 0)
				{
					txt_intro.setString(txt_intro.getString() + introduction[0]);
					introduction = introduction.toAnsiString().substr(1);
				}
				else if (introduction.getSize() <= 0 && start_quiz.getSize() > 0)
				{
					txt_start_quiz.setString(txt_start_quiz.getString() + start_quiz[0]);
					start_quiz = start_quiz.toAnsiString().substr(1);
				} 
				else if (introduction.getSize() <= 0 && start_quiz.getSize() <= 0 && sfStrZgodaNaMail.getSize() > 0)
				{
					sfTxtZgodaNaMail.setString(sfTxtZgodaNaMail.getString() + sfStrZgodaNaMail[0]);
					sfStrZgodaNaMail = sfStrZgodaNaMail.toAnsiString().substr(1);
				}
				break;
			case QUESTIONS:
				if (ilosc_opcji-1 >= licznik_opcji )
				{
					do
					{
						if (sfStrOpcje[licznik_opcji].getSize() > 0)
						{
							sfTxtOpcje[licznik_opcji].setString(sfTxtOpcje[licznik_opcji].getString() + sfStrOpcje[licznik_opcji].toAnsiString().substr(0,1)); 
							sfStrOpcje[licznik_opcji] = sfStrOpcje[licznik_opcji].toAnsiString().substr(1);
						}
						else ++licznik_opcji;
					} 
					while (licznik_opcji-1 >= ilosc_opcji);
				} 
				break;
			case RESULT:
				if (wyliczona_krypta.getSize() > 0)
				{
					blokada_eventow = true;
					twoja_krypta.setString(twoja_krypta.getString() + wyliczona_krypta[0]);
					wyliczona_krypta = wyliczona_krypta.toAnsiString().substr(1);
				}
				else blokada_eventow = false;
				break;
			}
		}

		//blokuj eventy jeżeli text się wczytuje
		if (sfStrZgodaNaMail.getSize() > 0 && stan_okna == INTRO ||
			ilosc_opcji-1 >= licznik_opcji  && stan_okna == QUESTIONS ||
			wyliczona_krypta.getSize() > 0 && stan_okna == RESULT)
		{
			blokada_eventow = true;
		}
		else blokada_eventow = false;

		window.clear();
		window.draw(sprite); //background			 

		//podswietlenia opcji
		if (stan_okna == INTRO) 
		{
			if (isTextContainsCursor(txt_start_quiz, sprCursor))
			{
				txt_start_quiz.setFillColor(Color(0, 0, 0, 255));
				podswietlenie.setSize(Vector2f(WIDTH*0.82, HEIGHT / 26));
				podswietlenie.setPosition(txt_start_quiz.getPosition().x, txt_start_quiz.getPosition().y + HEIGHT / 128);
				window.draw(podswietlenie);
				if (played_once[0] == false)
				{
					sound_focus.play();
					played_once[0] = true;
				}
			}
			else
			{
				txt_start_quiz.setFillColor(zielony_fallout);
				played_once[0] = false;
			}
		}
		else if (stan_okna == QUESTIONS)
		{
			for (int i = 1; i <= ilosc_opcji-1; ++i)
			{
				if (isTextContainsCursor(sfTxtOpcje[i], sprCursor) || mouse_focus == i)
				{
					aktualny_focus = i; // mówi która opcja jest podświetlana
										//usuwa focus z up/down jeżeli złapie focus z myszki o innym indeksie, tj. inną opcję odpowiedzi. nie będą podświetlane 2 sfTxtOpcje
					if (aktualny_focus != mouse_focus) mouse_focus = 0;
					podswietlenie.setSize(Vector2f(WIDTH*0.82, (HEIGHT / 26) + (HEIGHT / 26)*dodatkowePodstwietlanieOdpowiedzi[aktualne_opcje - 1][i]));
					sfTxtOpcje[i].setFillColor(Color(0, 0, 0, 255));
					podswietlenie.setPosition(sfTxtOpcje[i].getPosition().x, sfTxtOpcje[i].getPosition().y + HEIGHT / 128);
					window.draw(podswietlenie);
					if (played_once[i] == false)
					{
						sound_focus.play();
						played_once[i] = true;
					}
				}
				else
				{ 
					sfTxtOpcje[i].setFillColor(zielony_fallout);
					played_once[i] = false;
				}
			}
		}

		//graj dźwięk wgrywania textu, jeżeli text do wgrania jest tj. mamy blokadę eventów
		loading_txt_time += clockLoadingTextSound.restart();
		while (loading_txt_time >= seconds(.75f))
		{
			loading_txt_time -= seconds(.75f);
			if (blokada_eventow == true)
			{
				sound_load_string.play();
			}
			else sound_load_string.stop();
		}

		for (int i = 0; i < linijek_wstepu; i++) //zawsze rysuj nagłówek
			window.draw(wstep[i]);

		//rysuj treść stron 
		if (stan_okna == INTRO)
		{
			twoja_krypta.setString("");
			for (int i = 0; i <= ilosc_opcji-1; i++) 
				sfTxtOpcje[i].setString("");
			window.draw(txt_intro);
			window.draw(txt_start_quiz);
			window.draw(sfTxtZgodaNaMail);
			migajacy_kwadracik.setPosition((WIDTH / 10.1) + fMailWidth, 0.886*HEIGHT);
			txt_stopka.setString(strStopka);
		}
		else if (stan_okna == QUESTIONS) 
		{
			reset_testu = false;
			txt_intro.setString("");
			txt_start_quiz.setString("");
			twoja_krypta.setString("");
			for (int i = 0; i < ilosc_opcji; ++i)
				window.draw(sfTxtOpcje[i]);
			migajacy_kwadracik.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
			txt_stopka.setString("> ");

			if (aktualne_opcje > 10) 
				stan_okna = RESULT;
		}
		else if (stan_okna == RESULT) 
		{
			txt_intro.setString(""); 
			txt_start_quiz.setString(""); 
			sfTxtZgodaNaMail.setString("");
			for (int i = 0; i < ilosc_opcji; i++) 
				sfTxtOpcje[i].setString(""); 
			reset_testu = false;
			if (licz_krypte_raz == false) //jeżeli jeszcze nie ma wyniku krypty
			{
				clockShowingResult.restart();
				for (int i = 0; i <= 25; ++i) //przeleź przez wszystkie iteracje macierzy
				{
					if (suma_pkt[i] > wskaznik_krypty) //znajduj zawsze większy wynik
					{
						wskaznik_krypty = suma_pkt[i]; // i docelowo największy będzie wynikiem. to taka oszukańcza lamusowa funkcja MAX()
						wyliczona_krypta = krypty[i]; //wynik- wyliczona krypta
						//sfStrTesty = "suma_pkt[i]: " + std::to_string(suma_pkt[i]);
					}
				}
				licz_krypte_raz = true; //nie zamulaj procesora obliczając ciągle wynikową kryptę
			}
			window.draw(twoja_krypta); //podaj jaka krypta wyszla

			txt_stopka.setString("> Nacisnij ENTER lub LPM by zresetowac."); //text na dole ekranu po podaniu krypty
			migajacy_kwadracik.setPosition(WIDTH - WIDTH / 2.9, 0.886*HEIGHT); //pozycja migającego kwadracika za textem

			gotointrotime = clockShowingResult.getElapsedTime();
			if (gotointrotime >= seconds(60.0f)) //po jakimś czasie po podaniu krypty będzie przeskakiwać ze stanu RESULT do INTRO
			{
				czas_minal = true;
				clockShowingResult.restart();
			}
		}

		window.draw(txt_stopka); //zawsze rysuj stopkę: > + to co napisze gracz

		//niech kwadracik miga
		migajacy_kwadracik_time = clockBlinkingSquare.getElapsedTime(); 
		if (migajacy_kwadracik_time < seconds(.5f))	migajacy_kwadracik.setFillColor(Color(0, 0, 0, 0));
		if (migajacy_kwadracik_time >= seconds(.5f)) migajacy_kwadracik.setFillColor(Color(zielony_fallout));
		if (migajacy_kwadracik_time >= seconds(1.0f)) clockBlinkingSquare.restart();
		window.draw(migajacy_kwadracik);

		window.setView(fixed);
		window.draw(sprCursor);

		window.display();
	}
	return 0; //koniec pętli- wyłącz apkę

	///////////////koniec mojego kodu//////////////////////




    // Zainicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjacje aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Główna pętla wiadomości:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  CEL: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   CEL: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  CEL: Przetwarza wiadomości dla okna głównego.
//
//  WM_COMMAND — przetwarzaj menu aplikacji
//  WM_PAINT — pomaluj okno główne
//  WM_DESTROY — opublikuj komunikat o rezygnacji i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: tutaj dodaj kod rysowania używający elementu hdc...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//Procedura obsługi wiadomości dla okna informacji o programie.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
