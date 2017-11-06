#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <string>
#include <iostream>
#include "krypta.h"
#include "funkcje.h"
#include <cstdio>

using namespace sf;
using namespace std;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	enum WINDOW_STATE { INTRO, QUESTIONS, RESULT };
	WINDOW_STATE stan_okna;

	const int WIDTH = VideoMode::getDesktopMode().width;
	const int HEIGHT = VideoMode::getDesktopMode().height;
	//1152, 864
	//1366, 768

	//const int WIDTH = 1366 * 0.8;
	//const int HEIGHT = 768 * 0.8;

	int background_WIDTH = 1280;
	int background_HEIGHT = 1024;

	Clock clock;
	Clock clock2;
	Clock clock3;
	Clock clock4; //pó³ minuty po podaniu krypty bêdzie przeskakiwa³ ze stanu RESULT do INTRO
	Time elapsedtime;
	Time migajacy_kwadracik_time;
	Time loading_txt_time;
	Time gotointrotime; //pó³ minuty po podaniu krypty bêdzie przeskakiwa³ ze stanu RESULT do INTRO
	Time CzasOdAktualizacji = Time::Zero; // pomocniczy do tego powy¿ej
	bool czas_minal = false; //je¿eli minie powy¿sze pó³ minuty to przestawiana jest ta flaga

	//RenderWindow window(VideoMode(WIDTH - 1, HEIGHT - 1), "Fallout 3 terminal", Style::None);
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Fallout 3 terminal", Style::Fullscreen);
	window.setMouseCursorVisible(false);
	View fixed = window.getView(); // Create a fixed view

	//cursor
	Texture mouse;
	mouse.loadFromFile("fallout_3_cursor_3.png");
	Sprite cursor(mouse);

	//kolorki
	Texture background;
	background.loadFromFile("gterminal.png");
	Sprite sprite;
	sprite.setTexture(background);
	sprite.setScale(Vector2f((double)HEIGHT / (double)background_HEIGHT, (double)WIDTH / (double)background_WIDTH));
	Color zielony_fallout(40, 247, 129);

	//czcionki
	int font_size = 0.028*WIDTH;
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
	for (int i = 0; i < linijek_wstepu + 1; i++)
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

	double fOneCharWidth = 0.02*WIDTH;
	double fMailWidth = txt_stopka.getLocalBounds().width - fOneCharWidth;

	const int ilosc_opcji = 6;
	Text opcje[ilosc_opcji];
	String str[ilosc_opcji];
	int aktualne_opcje = 0;

	bool blokada_eventow = false;
	bool reset_testu = false;
	bool licz_krypte_raz = false;

	bool blokada_updown = false;
	int mouse_focus = 0;
	int aktualny_focus = 0;

	bool LOAD_INTRO = false; // warunek do za³adowania od nowa intro

	RectangleShape podswietlenie;
	podswietlenie.setFillColor(zielony_fallout);

	RectangleShape migajacy_kwadracik;
	migajacy_kwadracik.setFillColor(zielony_fallout);
	migajacy_kwadracik.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
	migajacy_kwadracik.setSize(Vector2f(0.0165*HEIGHT, 0.02*WIDTH));

	//dŸwiêki
	SoundBuffer buffer_menu_focus;
	SoundBuffer buffer_enter;
	SoundBuffer buffer_szum;
	SoundBuffer buffer_load_string;
	SoundBuffer buffer_text_entered1;
	SoundBuffer buffer_text_entered2;
	SoundBuffer buffer_text_entered3;
	SoundBuffer buffer_text_entered4;
	SoundBuffer buffer_text_entered5;
	SoundBuffer buffer_text_entered6;
	Sound sound;
	Sound sound_background;
	Sound sound_load_string;
	Sound sound_text_entered1;
	Sound sound_text_entered2;
	Sound sound_text_entered3;
	Sound sound_text_entered4;
	Sound sound_text_entered5;
	Sound sound_text_entered6;
	bool played_once[6] = { 0, 0, 0, 0, 0, 0 };
	bool not_a_loop = false;

	sound_load_string.setBuffer(buffer_load_string);
	if (!buffer_load_string.loadFromFile("sounds/ui_hacking_charscroll.wav")) return -1;

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
		
		while (window.pollEvent(zdarzenie))
		{
			if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Multiply) 
			{
				window.close(); 
			}
			else if (opcje[1].getGlobalBounds().contains(mouse) && blokada_eventow == false && stan_okna == QUESTIONS &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Num1 && 
				blokada_eventow == false && stan_okna == QUESTIONS || aktualny_focus == 1 && blokada_eventow == false &&
				stan_okna == QUESTIONS && zdarzenie.type == sf::Event::KeyPressed && 
				zdarzenie.key.code == Keyboard::Return && !opcje[1].getGlobalBounds().contains(mouse))
			{
				mouse_focus = 0;
				aktualny_focus = 0;
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();
				for (int i = 0; i <= 20; i++) suma_pkt[i] += odp[aktualne_opcje][1][i];
				Clean_options(ilosc_opcji, str, opcje);
				Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
				for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
				Char_by_char(aktualne_opcje, ilosc_opcji, str);
				aktualne_opcje += 1;
			}
			else if (opcje[2].getGlobalBounds().contains(mouse) && blokada_eventow == false && stan_okna == QUESTIONS &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Num2 && 
				blokada_eventow == false && stan_okna == QUESTIONS || aktualny_focus == 2 && blokada_eventow == false &&
				stan_okna == QUESTIONS && zdarzenie.type == sf::Event::KeyPressed && 
				zdarzenie.key.code == Keyboard::Return && !opcje[2].getGlobalBounds().contains(mouse))
			{
				mouse_focus = 0;
				aktualny_focus = 0;
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();
				for (int i = 0; i <= 20; i++) suma_pkt[i] += odp[aktualne_opcje][2][i];
				Clean_options(ilosc_opcji, str, opcje);
				Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
				for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
				Char_by_char(aktualne_opcje, ilosc_opcji, str);
				aktualne_opcje += 1;
			}
			else if (opcje[3].getGlobalBounds().contains(mouse) && blokada_eventow == false && stan_okna == QUESTIONS &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Num3 && 
				blokada_eventow == false && stan_okna == QUESTIONS || aktualny_focus == 3 && blokada_eventow == false &&
				stan_okna == QUESTIONS && zdarzenie.type == sf::Event::KeyPressed && 
				zdarzenie.key.code == Keyboard::Return && !opcje[3].getGlobalBounds().contains(mouse))
			{
				mouse_focus = 0;
				aktualny_focus = 0;
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();
				for (int i = 0; i <= 20; i++) suma_pkt[i] += odp[aktualne_opcje][3][i];
				Clean_options(ilosc_opcji, str, opcje);
				Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
				for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
				Char_by_char(aktualne_opcje, ilosc_opcji, str);
				aktualne_opcje += 1;
			}
			else if (opcje[4].getGlobalBounds().contains(mouse) && blokada_eventow == false && stan_okna == QUESTIONS &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Num4 && 
				blokada_eventow == false && stan_okna == QUESTIONS || aktualny_focus == 4 && blokada_eventow == false &&
				stan_okna == QUESTIONS && zdarzenie.type == sf::Event::KeyPressed && 
				zdarzenie.key.code == Keyboard::Return && !opcje[4].getGlobalBounds().contains(mouse)) 
				// dalej je¿eli po podœwietleniu opcji z zjechaniu z niej wcisne enter to akceptuje. powinno byæ gdzieœ resetowanie aktualnego focusu do 0
			{
				mouse_focus = 0;
				aktualny_focus = 0;
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();
				for (int i = 0; i <= 20; i++) suma_pkt[i] += odp[aktualne_opcje][4][i];
				Clean_options(ilosc_opcji, str, opcje);
				Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
				for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
				Char_by_char(aktualne_opcje, ilosc_opcji, str);
				aktualne_opcje += 1;
			}
			else if (opcje[5].getGlobalBounds().contains(mouse) && blokada_eventow == false && stan_okna == QUESTIONS &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Num5 && 
				blokada_eventow == false && stan_okna == QUESTIONS || aktualny_focus == 5 && blokada_eventow == false && 
				stan_okna == QUESTIONS && zdarzenie.type == sf::Event::KeyPressed && 
				zdarzenie.key.code == Keyboard::Return && !opcje[5].getGlobalBounds().contains(mouse))
			{
				mouse_focus = 0;
				aktualny_focus = 0;
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();
				for (int i = 0; i <= 20; i++) suma_pkt[i] += odp[aktualne_opcje][5][i];
				Clean_options(ilosc_opcji, str, opcje);
				Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
				for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
				Char_by_char(aktualne_opcje, ilosc_opcji, str);
				aktualne_opcje += 1;
			}
			else if (stan_okna == INTRO && blokada_eventow == false &&
				((txt_start_quiz.getGlobalBounds().contains(mouse) && zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left) ||
				(zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Return)))
			{
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();

				string stdStrEmail = strStopka;
				stdStrEmail.erase(0, 2); // usuñ "> "
				stdStrEmail.erase(remove_if(stdStrEmail.begin(), stdStrEmail.end(), isspace), stdStrEmail.end()); //remove whitespaces
				if (isEmailOk(stdStrEmail))
				{
					writeEmailToFile(stdStrEmail);

					//strStopka = "> ";
					txt_start_quiz.setString("");

					stan_okna = QUESTIONS;
					Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
					for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
					Char_by_char(aktualne_opcje, ilosc_opcji, str);
					aktualne_opcje += 1;
				}
				//todo3: po przejœciu przez 1 sekwencjê testu pierdoli siê intro
				else
				{
					txt_start_quiz.setString(start_bad_email);
					strStopka = "> ";
					fMailWidth = 0;
				}
			}
			else if (sprite.getGlobalBounds().contains(mouse) && stan_okna == RESULT &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left && blokada_eventow == false ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Return && 
				blokada_eventow == false && stan_okna == RESULT) 
			{
				LOAD_INTRO = true;
			}
			else if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Up && 
				blokada_eventow == false && stan_okna == QUESTIONS && blokada_updown == false)
			{
				if (mouse_focus == 0) mouse_focus = 6;
				mouse_focus--;
				if (mouse_focus < 1) mouse_focus = 1;

			}
			else if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Down && 
				blokada_eventow == false && stan_okna == QUESTIONS && blokada_updown == false)
			{
				mouse_focus++;
				if (mouse_focus > 5) mouse_focus = 5;
			}
			else if (stan_okna == INTRO && zdarzenie.type == Event::TextEntered && zdarzenie.text.unicode != 10 && zdarzenie.text.unicode != 13) //10 i 13 to entery
			{
				if (zdarzenie.text.unicode > 32 && strStopka.getSize() < 60) //znak jest textem i text nie wychodzi za ekran
					strStopka += String(zdarzenie.text.unicode);
				else if (zdarzenie.text.unicode == 8 && strStopka.getSize() > 2 ) //backspace i wiêksze ni¿ "> " 
					strStopka.erase(strStopka.getSize() - 1, strStopka.getSize());

				txt_stopka.setString(strStopka);

				fMailWidth = txt_stopka.getLocalBounds().width - fOneCharWidth;

				int sound_dice = rand() % 6 + 1;
				switch (sound_dice)
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
		}

		if (LOAD_INTRO == true || czas_minal == true)
		{
			sound.setBuffer(buffer_enter);
			if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
			sound.play();

			stan_okna = INTRO;
			aktualne_opcje = 0;
			for (int i = 0; i <= 20; i++) suma_pkt[i] = 0;
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

			licz_krypte_raz = false; //mozna ponownie obliczyæ wynikow¹ krytpê je¿eli przyjdzie na to czas
			czas_minal = false; //mozna bêdzie ponownie odczekaæ 30 sekund po podaniu wyniku krypty w RESULT, kiedy przyjdzie na to czas
			
			LOAD_INTRO = false;
			strStopka = "> ";
			fMailWidth = 0;
		}

		//nie mozna kliknac up/down jezeli myszka ma focus na którejœ z opcji odpowiedzi
		if (opcje[1].getGlobalBounds().contains(mouse) || opcje[2].getGlobalBounds().contains(mouse) || 
			opcje[3].getGlobalBounds().contains(mouse) || opcje[4].getGlobalBounds().contains(mouse) || 
			opcje[5].getGlobalBounds().contains(mouse))
			blokada_updown = true;
		else
		{
			blokada_updown = false;
			if (mouse_focus == 0) //je¿eli ¿adna opcja up/down nie jest podœwietlona
				aktualny_focus = 0; //to nie mo¿na wcisn¹æ enter
		}
		
		elapsedtime += clock.restart();
		while (elapsedtime >= seconds(.007f)) //.007f
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
				if (start_quiz.getSize() > 0 && introduction.getSize() <= 0)
				{
					txt_start_quiz.setString(txt_start_quiz.getString() + start_quiz[0]);
					start_quiz = start_quiz.toAnsiString().substr(1);
				} break;
			case QUESTIONS:
				if (licznik_opcji <= ilosc_opcji)
				{
					do
					{
						One_line_option(licznik_opcji, str, opcje);
					} while (licznik_opcji == ilosc_opcji);
				} break;
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

		if (licznik_opcji <= ilosc_opcji && stan_okna == QUESTIONS ||
			start_quiz.getSize() > 0 && stan_okna == INTRO || 
			wyliczona_krypta.getSize() > 0 && stan_okna == RESULT) blokada_eventow = true;
		else blokada_eventow = false;

		cursor.setPosition(static_cast<Vector2f>(Mouse::getPosition(window)));

		window.clear();
		window.draw(sprite); //background

		/*RenderTexture texture_tlo_liter;
		texture_tlo_liter.create(100, 100);
		String temp = "kupa";
		Text txt_temp;
		txt_temp.setFont(font);
		txt_temp.setString(temp);
		txt_temp.setFillColor(Color::Red);
		texture_tlo_liter.draw(txt_temp, BlendAlpha);
		texture_tlo_liter.display();
		Sprite sprite_tlo_liter;
		sprite_tlo_liter.setTexture(texture_tlo_liter.getTexture());
		window.draw(sprite_tlo_liter);*/

		if (stan_okna == INTRO)
		{
			if (txt_start_quiz.getGlobalBounds().contains(mouse))
			{
				txt_start_quiz.setFillColor(Color(0, 0, 0, 255));
				podswietlenie.setSize(Vector2f(WIDTH*0.82, HEIGHT / 26));
				podswietlenie.setPosition(txt_start_quiz.getPosition().x, txt_start_quiz.getPosition().y + HEIGHT / 128);
				window.draw(podswietlenie);
				if (played_once[0] == false)
				{
					sound.setBuffer(buffer_menu_focus);
					if (!buffer_menu_focus.loadFromFile("sounds/ui_menu_focus.wav")) return -1;
					sound.play();
					played_once[0] = true;
				}
			}
			else
			{
				txt_start_quiz.setFillColor(zielony_fallout);
				played_once[0] = false;
			}
		}

		if (stan_okna == QUESTIONS)
		{
			for (int i = 1; i < ilosc_opcji; i++)
			{
				if (opcje[i].getGlobalBounds().contains(mouse) || mouse_focus == i)
				{
					aktualny_focus = i; // mówi która opcja jest podœwietlana
					//usuwa focus z up/down je¿eli z³apie focus z myszki o innym indeksie, tj. inn¹ opcjê odpowiedzi. nie bêd¹ podœwietlane 2 opcje
					if (aktualny_focus != mouse_focus) mouse_focus = 0; 
					podswietlenie.setSize(Vector2f(WIDTH*0.82, (HEIGHT / 26) + (HEIGHT / 26)*macierz_betonu[aktualne_opcje - 1][i]));
					opcje[i].setFillColor(Color(0, 0, 0, 255));
					podswietlenie.setPosition(opcje[i].getPosition().x, opcje[i].getPosition().y + HEIGHT / 128);
					window.draw(podswietlenie);
					if (played_once[i] == false)
					{
						sound.setBuffer(buffer_menu_focus);
						if (!buffer_menu_focus.loadFromFile("sounds/ui_menu_focus.wav")) return -1;
						sound.play();
						played_once[i] = true;
					}
				}
				else
				{
					opcje[i].setFillColor(zielony_fallout);
					played_once[i] = false;
				}
			}
		}

		loading_txt_time += clock3.restart(); //tak d³ugo jak text siê ³aduje odtwarzaj dŸwiêk
		while (loading_txt_time >= seconds(.75f))
		{
			loading_txt_time -= seconds(.75f);
			if (blokada_eventow == true)
			{
				sound_load_string.play();
			}
			else sound_load_string.stop();
		}

		for (int i = 0; i < linijek_wstepu; i++) //zawsze rysuj nag³ówek
			window.draw(wstep[i]);

		if (stan_okna == INTRO)
		{
			twoja_krypta.setString("");
			for (int i = 0; i < ilosc_opcji; i++) opcje[i].setString("");
			window.draw(txt_intro);
			window.draw(txt_start_quiz);
			migajacy_kwadracik.setPosition((WIDTH / 10.1) + fMailWidth, 0.886*HEIGHT);
			txt_stopka.setString(strStopka);
		}
		if (stan_okna == QUESTIONS)
		{
			reset_testu = false;
			txt_intro.setString("");
			txt_start_quiz.setString("");
			twoja_krypta.setString("");
			for (int i = 0; i < ilosc_opcji; i++)
				window.draw(opcje[i]);
			migajacy_kwadracik.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
			txt_stopka.setString("> ");
		}
		if (aktualne_opcje > 10) stan_okna = RESULT; // je¿eli pêtla przesz³a przez wszystkie pytania (10)- przejdŸ do podania wyniku
		if (stan_okna == RESULT) //je¿eli stan okna to result, tj. podawanie wyniku
		{
			txt_intro.setString(""); //wymarz inne wyœwietlanie stringi
			txt_start_quiz.setString(""); //wymarz inne wyœwietlanie stringi
			for (int i = 0; i < ilosc_opcji; i++) opcje[i].setString(""); //wymarz inne wyœwietlanie stringi
			reset_testu = false;
			if (licz_krypte_raz == false) //je¿eli jeszcze nie ma wyniku krypty
			{
				clock4.restart();
				for (int i = 0; i <= 20; i++) //przeleæ przez wszystkie iteracje macierzy
				{
					if (suma_pkt[i] > wskaznik_krypty) //znajduj zawsze wiêkszy wynik
					{
						wskaznik_krypty = suma_pkt[i]; // i docelowo najwiêkszy bêdzie wynikiem. to taka oszukañcza lamusowa funkcja MAX()
						wyliczona_krypta = krypty[i]; //wynik- wyliczona krypta
						//wyliczona_krypta = krypty[20]; // sprawdzanie wybranej krypty
					}
				}
				licz_krypte_raz = true; //nie zamulaj procesora obliczaj¹c ci¹gle wynikow¹ kryptê
			}
			window.draw(twoja_krypta); //podaj jaka krypta wyszla

			txt_stopka.setString("> Nacisnij ENTER lub LPM by zresetowac."); //text na dole ekranu po podaniu krypty
			migajacy_kwadracik.setPosition(WIDTH - WIDTH / 2.9, 0.886*HEIGHT); //pozycja migaj¹cego kwadracika za textem

			gotointrotime = clock4.getElapsedTime(); 
			if (gotointrotime >= seconds(60.0f)) //po jakimœ czasie po podaniu krypty bêdzie przeskakiwa³ ze stanu RESULT do INTRO
			{
				czas_minal = true;
				clock4.restart();
			}
		}
		
		window.draw(txt_stopka); // rysuj stopkê: > + to co napisze cz³owiek

		migajacy_kwadracik_time = clock2.getElapsedTime(); //migaj¹cy kwadracik 
		if (migajacy_kwadracik_time < seconds(.5f))	migajacy_kwadracik.setFillColor(Color(0, 0, 0, 0));
		if (migajacy_kwadracik_time >= seconds(.5f)) migajacy_kwadracik.setFillColor(Color(zielony_fallout));
		if (migajacy_kwadracik_time >= seconds(1.0f)) clock2.restart();

		window.draw(migajacy_kwadracik); 

		/*for (int i = 0; i <= 20; i++) //wyswietla pomocniczo wartosci suma_pkt[]
		{
		String jakis_string;
		Text jakis_text;
		jakis_string = to_string(suma_pkt[i]);
		jakis_text.setString(jakis_string);
		jakis_text.setFont(font);
		jakis_text.setPosition(0, i * 30);
		window.draw(jakis_text);
		}

		String jakis_string0; //wyœwietla pomocniczo czas do przejœcia z RESULT do INTRO
		Text jakis_text0;
		jakis_string0 = to_string(gotointrotime.asSeconds());
		jakis_text0.setString(jakis_string0);
		jakis_text0.setFont(font);
		window.draw(jakis_text0);

		String jakis_string1; //wyœwietla pomocniczo warunek do przejœcia z RESULT do INTRO
		Text jakis_text1;
		jakis_string1 = "czas_minal = " + to_string(czas_minal);
		jakis_text1.setPosition(0, 1 * 30);
		jakis_text1.setString(jakis_string1);
		jakis_text1.setFont(font);
		window.draw(jakis_text1);*/
		
		
		window.setView(fixed);
		window.draw(cursor);
	
		window.display();
	}
	return 0;
}