#pragma once

#include <SFML/Window.hpp>
#include <string>
#include <iostream>
#include <regex>
#include <fstream>

using namespace sf;

int licznik_opcji = 0;

/*void Draw_intro(String wstep, String zacznij_test, Text txt_wstep, Text txt_zacznij_test)
{
if (wstep.getSize() > 0)
{
txt_wstep.setString(txt_wstep.getString() + wstep[0]);
wstep = wstep.toAnsiString().substr(1);
}
if (zacznij_test.getSize() > 0 && wstep.getSize() <= 0)
{
txt_zacznij_test.setString(txt_zacznij_test.getString() + zacznij_test[0]);
zacznij_test = wstep.toAnsiString().substr(1);
}
}*/


/*void One_line_option(int Line, String msg[], Text full_msg[])
{
	if (msg[Line].getSize() > 0)
	{
		full_msg[Line].setString(full_msg[Line].getString() + msg[Line][0]);
		msg[Line] = msg[Line].toAnsiString().substr(1);
	}
	else ++licznik_opcji;
}*/

//TODO: jak ja ogarnąłem te wszystkie stare funkcje? operują one tylko na zmianach danych lokalnych

/*void Char_by_char(int actual_options, int number_of_options, String msg[])
{
	licznik_opcji = 0;
	for (int i = 0; i <= number_of_options - 1; i++)
		msg[i] = pyt[actual_options][i]; 

	/ *
	ta funkcja zawsze i tylko występuje w bloku różnych funkcji:
	Text_css(ilosc_opcji, sfTxtOpcje, font_size, zielony_fallout, aktualne_opcje, HEIGHT, WIDTH);
	for (int i = 0; i < ilosc_opcji; i++) sfTxtOpcje[i].setFont(font);
	Char_by_char(aktualne_opcje, ilosc_opcji, sfStrOpcje);
	aktualne_opcje += 1;
	* /
}*/

/*void Clean_options(int number_of_options, String msg[], Text full_msg[])
{
	for (int i = 0; i <= number_of_options - 1; i++)
	{
		msg[i] = "";
		full_msg[i].setString(msg[i]);
	}
	/ *
	to zawsze występuje razem:
	for (int i = 0; i <= 20; i++) suma_pkt[i] += odp[aktualne_opcje][5][i];
	Clean_options(ilosc_opcji, sfStrOpcje, sfTxtOpcje);
	* /
}*/

/*void Text_css(int number_of_options, Text full_msg[], int rozmiar_czcionki, Color kolor_czcionki, int actual_options, int WYSOKOSC, int SZEROKOSC)
{
	full_msg[0].setCharacterSize(rozmiar_czcionki);
	full_msg[0].setFillColor(kolor_czcionki);

	//pierwsza opcja jest rysowana zawsze bez pytania 
	full_msg[0].setPosition(SZEROKOSC / 14, 0.25*WYSOKOSC);

	//kolejne 5. jeżeli pierwsza ma enter, to każda kolejna jest przesunięta o enter
	const int ENTER = 0.038f*WYSOKOSC;
	int suma_enterow = 0;
	{
		for (int i = 1; i < number_of_options; i++)
		{
			full_msg[i].setCharacterSize(rozmiar_czcionki);
			full_msg[i].setFillColor(kolor_czcionki);

			suma_enterow += macierz_betonu[actual_options][i - 1];
			full_msg[i].setPosition(SZEROKOSC / 14, 0.25*WYSOKOSC + i*0.06*WYSOKOSC + ENTER*suma_enterow);
		}
	}
}*/

bool isEmailOk(const std::string& email)
{
	//potrafi zawiesić program przy wpisaniu ciągu tych samych znaków, np. "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"
	//std::regex emailRegex("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?"); 

	//nie przechodzą gmaile z kropką, np: kupa.jasiu@gmail.com
	//std::regex emailRegex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	//std::regex emailRegex("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?", std::regex::ECMAScript);

	std::regex emailRegex("(?:(?:[^<>()\\[\\].,;:\\s@\"]+(?:\\.[^<>()\\[\\].,;:\\s@\"]+)*)|\".+\")@(?:(?:[^<>()‌​\\[\\].,;:\\s@\"]+\\.)+[^<>()\\[\\].,;:\\s@\"]{2,})");
	return regex_match(email, emailRegex);
}

void writeEmailToFile(std::string email)
{
	std::ofstream lista_maili;
	lista_maili.open("lista_maili.txt", std::ios::app);
	lista_maili << email << "\n";
	lista_maili.close();
}

bool isTextContainsCursor(sf::Text text, sf::Sprite cursor)
{
	if ((text.getGlobalBounds().left <= cursor.getPosition().x && (text.getGlobalBounds().left + text.getGlobalBounds().width) >= cursor.getPosition().x) &&
		(text.getGlobalBounds().top <= cursor.getPosition().y && (text.getGlobalBounds().top + text.getGlobalBounds().height) >= cursor.getPosition().y))
		return true;
	else 
		return false; 
}
 
bool isTextContainsCursor(sf::Sprite sprite, sf::Sprite cursor)
{
	if ((sprite.getGlobalBounds().left <= cursor.getPosition().x && (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width) >= cursor.getPosition().x) &&
		(sprite.getGlobalBounds().top <= cursor.getPosition().y && (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height) >= cursor.getPosition().y))
		return true;
	else
		return false;
}

/* //chyba istneije już funkcja na to
RenderTexture texture_tlo_liter;
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

/*
String sfStrPozycjaKursora;
Text sfTxtPozycjaKursora;
sfStrPozycjaKursora = "kursor: " + std::to_string((int)sprCursor.getPosition().x) + "," + std::to_string((int)sprCursor.getPosition().y);
sfTxtPozycjaKursora.setString(sfStrPozycjaKursora);
sfTxtPozycjaKursora.setFont(font);
sfTxtPozycjaKursora.setPosition(5, 650);
window.draw(sfTxtPozycjaKursora);

String sfStrPozycjaMyszki;
Text sfTxtPozycjaMyszki;
sfStrPozycjaMyszki = "myszka: " + std::to_string((int)mouse.x) + "," + std::to_string((int)mouse.y);
sfTxtPozycjaMyszki.setString(sfStrPozycjaMyszki);
sfTxtPozycjaMyszki.setFont(font);
sfTxtPozycjaMyszki.setPosition(5, 680);
window.draw(sfTxtPozycjaMyszki);

String sfStrPozycjaKlikalnegoNapisu;
Text sfTxtrPozycjaKlikalnegoNapisu;
sfStrPozycjaKlikalnegoNapisu = "napis: " + std::to_string((int)txt_start_quiz.getGlobalBounds().left) + "," + std::to_string((int)txt_start_quiz.getGlobalBounds().top) + "," +
std::to_string((int)txt_start_quiz.getGlobalBounds().width) + "," + std::to_string((int)txt_start_quiz.getGlobalBounds().height);
sfTxtrPozycjaKlikalnegoNapisu.setString(sfStrPozycjaKlikalnegoNapisu);
sfTxtrPozycjaKlikalnegoNapisu.setFont(font);
sfTxtrPozycjaKlikalnegoNapisu.setPosition(5, 710);
window.draw(sfTxtrPozycjaKlikalnegoNapisu);
*/

/*String sfStrBlokadaUpDown;
Text sfTxtBlokadaUpDown;
String sfStrBlokadaUpDownTF;
if (blokada_updown == true) sfStrBlokadaUpDownTF = "true";
else sfStrBlokadaUpDownTF = "false";
sfStrBlokadaUpDown = "blokada up/down: " + sfStrBlokadaUpDownTF;
sfTxtBlokadaUpDown.setString(sfStrBlokadaUpDown);
sfTxtBlokadaUpDown.setFont(font);
sfTxtBlokadaUpDown.setPosition(5, 650);
window.draw(sfTxtBlokadaUpDown);

String sfStrBlokadaEventow;
Text sfTxtBlokadaEventow;
String sfStrBlokadaEventowTF;
if (blokada_eventow == true) sfStrBlokadaEventowTF = "true";
else sfStrBlokadaEventowTF = "false";
sfStrBlokadaEventow = "blokada eventow: " + sfStrBlokadaEventowTF;
sfTxtBlokadaEventow.setString(sfStrBlokadaEventow);
sfTxtBlokadaEventow.setFont(font);
sfTxtBlokadaEventow.setPosition(5, 680);
window.draw(sfTxtBlokadaEventow);

String sfStrMouseFocus;
Text sfTxtMouseFocus;
String sfStrMouseFocusTF;
if (blokada_eventow == true) sfStrMouseFocusTF = "true";
else sfStrMouseFocusTF = "false";
sfStrMouseFocus = "mouse focus: " + std::to_string(mouse_focus);
sfTxtMouseFocus.setString(sfStrMouseFocus);
sfTxtMouseFocus.setFont(font);
sfTxtMouseFocus.setPosition(5, 710);
window.draw(sfTxtMouseFocus);

String sfStrAktualnyFocus = "aktualny focus: " + std::to_string(aktualny_focus);
Text sfTxtAktualnyFokus;
sfTxtAktualnyFokus.setString(sfStrAktualnyFocus);
sfTxtAktualnyFokus.setFont(font);
sfTxtAktualnyFokus.setPosition(5, 740);
window.draw(sfTxtAktualnyFokus);*/

/*String sfStrsStanOkna = "NONE";
switch (stan_okna)
{
case INTRO: sfStrsStanOkna = "INTRO"; break;
case QUESTIONS: sfStrsStanOkna = "QUESTIONS"; break;
case RESULT: sfStrsStanOkna = "RESULT"; break;
default: sfStrsStanOkna = "UNKNOWN"; break;
}
sfStrsStanOkna = "stan okna: " + sfStrsStanOkna;
Text sfTxtStanOkna;
sfTxtStanOkna.setString(sfStrsStanOkna);
sfTxtStanOkna.setFont(font);
sfTxtStanOkna.setPosition(5, 740);
window.draw(sfTxtStanOkna);*/

/*//wyswietla pomocniczo wartosci suma_pkt[]
for (int i = 0; i <= 25; i++)
{
	String jakis_string;
	Text jakis_text;
	jakis_string = to_string(suma_pkt[i]);
	jakis_text.setString(jakis_string);
	jakis_text.setFont(font);
	jakis_text.setPosition(0, i * 30);
	window.draw(jakis_text);
}


//sfStrTesty = "suma_pkt[i]: " + std::to_string(suma_pkt[i]); //wrzucic pod wyliczona_krypta
String sfStrTesty = "-";
Text sfTxtTesty;
String sfStrTestyPokaz = "testy: " + sfStrTesty;
sfTxtTesty.setString(sfStrTestyPokaz);
sfTxtTesty.setFont(font);
sfTxtTesty.setPosition(5, 790);
window.draw(sfTxtTesty);*/
