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


int One_line_option(int Line, String msg[], Text full_msg[])
{
	if (msg[Line].getSize() > 0)
	{
		full_msg[Line].setString(full_msg[Line].getString() + msg[Line][0]);
		msg[Line] = msg[Line].toAnsiString().substr(1);
	}
	else 
		return ++licznik_opcji;
}

void Char_by_char(int actual_options, int number_of_options, String msg[])
{
	licznik_opcji = 0;
	for (int i = 0; i <= number_of_options - 1; i++)
		msg[i] = pyt[actual_options][i];
}

void Clean_options(int number_of_options, String msg[], Text full_msg[])
{
	for (int i = 0; i <= number_of_options - 1; i++)
	{
		msg[i] = "";
		full_msg[i].setString(msg[i]);
	}
}

void Text_css(int number_of_options, Text full_msg[], int rozmiar_czcionki, Color kolor_czcionki, int actual_options, int WYSOKOSC, int SZEROKOSC)
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
}

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