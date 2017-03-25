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
	Time inserttime;
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
	sprite.setScale(Vector2f((float)HEIGHT / (float)background_HEIGHT, (float)WIDTH / (float)background_WIDTH));
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
	"Z jakiej krypty pochodzisz (beta v.2)",
	"_____________________________________" };
	for (int i = 0; i < linijek_wstepu + 1; i++)
	{
		wstep[i].setFont(font);
		wstep[i].setCharacterSize(font_size);
		wstep[i].setColor(zielony_fallout);
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
	txt_intro.setColor(zielony_fallout);
	//txt_intro.setString(introduction);
	txt_intro.setPosition(WIDTH / 14, 0.25*HEIGHT);

	txt_start_quiz.setFont(font);
	txt_start_quiz.setCharacterSize(font_size);
	txt_start_quiz.setColor(zielony_fallout);
	//txt_start_quiz.setString(start_quiz);
	txt_start_quiz.setPosition(WIDTH / 14, 0.25*HEIGHT + 0.34*HEIGHT);

	Text twoja_krypta;
	twoja_krypta.setFont(font);
	twoja_krypta.setCharacterSize(font_size);
	twoja_krypta.setColor(zielony_fallout);
	twoja_krypta.setPosition(WIDTH / 14, 0.25*HEIGHT);

	Text txt_stopka;
	txt_stopka.setString(">");
	txt_stopka.setFont(font);
	txt_stopka.setCharacterSize(font_size);
	txt_stopka.setColor(zielony_fallout);
	txt_stopka.setPosition(WIDTH / 14, 0.875*HEIGHT);

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

	RectangleShape insert;
	insert.setFillColor(zielony_fallout);
	insert.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
	insert.setSize(Vector2f(0.0165*HEIGHT, 0.02*WIDTH));

	//dŸwiêki
	SoundBuffer buffer_menu_focus;
	SoundBuffer buffer_enter;
	SoundBuffer buffer_szum;
	SoundBuffer buffer_load_string;
	Sound sound;
	Sound sound_background;
	Sound sound_load_string;
	bool played_once[6] = { 0, 0, 0, 0, 0, 0 };
	bool not_a_loop = false;

	sound_load_string.setBuffer(buffer_load_string);
	if (!buffer_load_string.loadFromFile("sounds/ui_hacking_charscroll.wav")) return -1;

	sound_background.setBuffer(buffer_szum);
	if (!buffer_szum.loadFromFile("sounds/obj_computerterminal_01_lp.wav")) return -1;
	sound_background.setLoop(true);
	sound_background.play();

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
				zdarzenie.key.code == Keyboard::Return && !opcje[4].getGlobalBounds().contains(mouse)) // dalej je¿eli po podœwietleniu opcji z zjechaniu z niej wcisne enter to akceptuje. powinno byæ gdzieœ resetowanie aktualnego focusu do 0
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
			else if (txt_start_quiz.getGlobalBounds().contains(mouse) && stan_okna == INTRO &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left && blokada_eventow == false ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Return && 
				blokada_eventow == false && stan_okna == INTRO)
			{
				sound.setBuffer(buffer_enter);
				if (!buffer_enter.loadFromFile("sounds/ui_hacking_charenter_02.wav")) return -1;
				sound.play();
				stan_okna = QUESTIONS;
				Text_css(ilosc_opcji, opcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
				for (int i = 0; i < ilosc_opcji; i++) opcje[i].setFont(font);
				Char_by_char(aktualne_opcje, ilosc_opcji, str);
				aktualne_opcje += 1;
			}
			else if (sprite.getGlobalBounds().contains(mouse) && stan_okna == RESULT &&
				zdarzenie.type == Event::MouseButtonPressed && zdarzenie.key.code == Mouse::Left && blokada_eventow == false ||
				zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == Keyboard::Return && 
				blokada_eventow == false && stan_okna == RESULT) //je¿eli minê³o 30 sekund od podania wyniku
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
			txt_intro.setColor(zielony_fallout);
			//txt_intro.setString(introduction);
			txt_intro.setPosition(WIDTH / 14, 0.25*HEIGHT);

			txt_start_quiz.setFont(font);
			txt_start_quiz.setCharacterSize(font_size);
			txt_start_quiz.setColor(zielony_fallout);
			//txt_start_quiz.setString(start_quiz);
			txt_start_quiz.setPosition(WIDTH / 14, 0.25*HEIGHT + 0.34*HEIGHT);

			introduction = "Algorytm pozwala zidentyfikowac pochodzenie mieszkanca \nkrypty na podstawie specjalnie opracowanego zestawu \n10ciu pytan. \n\nWyniki powinny byc pozytywne w 95 %. \n\nNiektore odpowiedzi moga byc dostosowane tylko dla \nmezczyzn lub kobiet.";
			start_quiz = "> Rozpocznij test.";

			licz_krypte_raz = false; //mozna ponownie obliczyæ wynikow¹ krytpê je¿eli przyjdzie na to czas
			czas_minal = false; //mozna bêdzie ponownie odczekaæ 30 sekund po podaniu wyniku krypty w RESULT, kiedy przyjdzie na to czas
			
			LOAD_INTRO = false;
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
		txt_temp.setColor(Color::Red);
		texture_tlo_liter.draw(txt_temp, BlendAlpha);
		texture_tlo_liter.display();
		Sprite sprite_tlo_liter;
		sprite_tlo_liter.setTexture(texture_tlo_liter.getTexture());
		window.draw(sprite_tlo_liter);*/

		if (stan_okna == INTRO)
		{
			if (txt_start_quiz.getGlobalBounds().contains(mouse))
			{
				txt_start_quiz.setColor(Color(0, 0, 0, 255));
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
				txt_start_quiz.setColor(zielony_fallout);
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
					if (aktualny_focus != mouse_focus) mouse_focus = 0; //usuwa focus z up/down je¿eli z³apie focus z myszki o innym indeksie, tj. inn¹ opcjê odpowiedzi. nie bêd¹ podœwietlane 2 opcje
					podswietlenie.setSize(Vector2f(WIDTH*0.82, (HEIGHT / 26) + (HEIGHT / 26)*macierz_betonu[aktualne_opcje - 1][i]));
					opcje[i].setColor(Color(0, 0, 0, 255));
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
					opcje[i].setColor(zielony_fallout);
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
			twoja_krypta.setString(" ");
			for (int i = 0; i < ilosc_opcji; i++) opcje[i].setString(" ");
			window.draw(txt_intro);
			window.draw(txt_start_quiz);
			insert.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
			txt_stopka.setString(">");
		}
		if (stan_okna == QUESTIONS)
		{
			reset_testu = false;
			txt_intro.setString(" ");
			txt_start_quiz.setString(" ");
			twoja_krypta.setString(" ");
			for (int i = 0; i < ilosc_opcji; i++)
				window.draw(opcje[i]);
			insert.setPosition(WIDTH / 10.1, 0.886*HEIGHT);
			txt_stopka.setString(">");
		}
		if (aktualne_opcje > 10) stan_okna = RESULT; // je¿eli pêtla przesz³a przez wszystkie pytania (10)- przejdŸ do podania wyniku
		if (stan_okna == RESULT) //je¿eli stan okna to result, tj. podawanie wyniku
		{
			txt_intro.setString(" "); //wymarz inne wyœwietlanie stringi
			txt_start_quiz.setString(" "); //wymarz inne wyœwietlanie stringi
			for (int i = 0; i < ilosc_opcji; i++) opcje[i].setString(" "); //wymarz inne wyœwietlanie stringi
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
			insert.setPosition(WIDTH - WIDTH / 2.9, 0.886*HEIGHT); //pozycja migaj¹cego kwadracika (insertu) za textem

			gotointrotime = clock4.getElapsedTime(); //pó³ minuty po podaniu krypty bêdzie przeskakiwa³ ze stanu RESULT do INTRO
			if (gotointrotime >= seconds(50.0f))
			{
				czas_minal = true;
				clock4.restart();
			}
		}
		
		window.draw(txt_stopka); // rysuj ">"

		inserttime = clock2.getElapsedTime(); //migaj¹cy kwadracik (insert)
		if (inserttime < seconds(.5f))	insert.setFillColor(Color(0, 0, 0, 0));
		if (inserttime >= seconds(.5f)) insert.setFillColor(Color(zielony_fallout));
		if (inserttime >= seconds(1.0f)) clock2.restart();

		window.draw(insert); //rysuj migaj¹cy kwadracik (insert)

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


/* auto-entery
for (int i = 0; i <= 9; i++)
{
for (int j = 0; j <= 5; j++)
{
int max_msg_lenght = WIDTH / 28.5;
if (pyt[i][j].getSize() > max_msg_lenght)
{
int breaking_place;
bool space_finded = false;
while (space_finded == false)
{
String msg_check = pyt[i][j];
msg_check = msg_check.toAnsiString().substr(max_msg_lenght, 1);
if (msg_check == " ")
{
breaking_place = max_msg_lenght;
space_finded = true;
}
else max_msg_lenght--;
// brakuje: if (nie ma spacji) przetnij text na pó³ bezczelnie;
}
string msg_part1;
string msg_part2;
msg_part1 = pyt[i][j].toAnsiString().substr(0, breaking_place);
msg_part2 = pyt[i][j].toAnsiString().substr(breaking_place);
pyt[i][j] = msg_part1 + "\n" + msg_part2;
}
}
}
///////////////////////////////////////////////////////////////////////
if (msg_part2 > max_msg_lenght)
wykonaj funkcjê jeszcze raz

msg_part2 = nowy msg_part2 + msg_part_3

ca³a pêtla taka by siê przyda³a*/