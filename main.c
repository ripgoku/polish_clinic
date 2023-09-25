#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/// <summary>
/// Struktura przedstawiajaca dane pacjenta.
/// 
/// Kazdy z pacjentow ma unikalne id - zadeklarowane jako int. Pozostale pola rowniez zadeklarowane jako liczby calkowite to: waga, wzrost.
/// Pola zadeklarowane jako ciag znakow: imie, nazwisko, pesel, data urodzenia, adres, email, telefon, oddzial nfz.
/// Struktura posiada wskazniki "previous" oraz "next". Pierwszy wskazuje poprzedni element listy, a drugi na nastepny. 
/// Nastepnie nastepuje deklaracja zmiennej struturalnej Pacjent_dane, ktora posluzy jako lista pacjentow.
/// </summary>
typedef struct Pacjent {
	int id;
	char imie[50];
	char nazwisko[50];
	char pesel[12];
	char data_urodzenia[11];
	char adres[100];
	char email[50];
	char telefon[12];
	float waga;
	float wzrost;
	char oddzial_nfz[50];
	struct Pacjent* previous;
	struct Pacjent* next;
} Pacjent_dane;

/// <summary>
/// Struktura przedstawiajaca dane lekarza.
/// 
/// Kazdy z lekarzy ma unikalne id - zadeklarowane jako int. Pola zadeklarowane jako ciag znakow: imie, nazwisko, pesel, numer PWZ, tytul zawodowy, 
/// specjalizacja, email, telefon, godziny pracy (tablica dwuwymierowa, 7 ciagow znakow).
/// Struktura posiada wskazniki "previous" oraz "next". Pierwszy wskazuje poprzedni element listy, a drugi na nastepny. 
/// Nastepnie nastepuje deklaracja zmiennej struturalnej Lekarz_dane, ktora posluzy jako lista lekarzy.
/// </summary>
typedef struct Lekarz {
	int id;
	char imie[50];
	char nazwisko[50];
	char pesel[12];
	char numer_pwz[8];
	char tytul_zawodowy[20];
	char specjalizacja[50];
	char email[50];
	char telefon[12];
	char godziny_pracy[7][20];
	struct Lekarz* previous;
	struct Lekarz* next;
} Lekarz_dane;

/// <summary>
/// Struktura przedstawiajaca dane wizyty.
/// 
/// Pola zadeklarowane jako int: id_pacjenta, id_lekarza, czas_trwania. Pola zadeklarowane jako ciag znakow: data, godzina, status.
/// Struktura posiada wskazniki "previous" oraz "next". Pierwszy wskazuje poprzedni element listy, a drugi na nastepny. 
/// Nastepnie nastepuje deklaracja zmiennej struturalnej Wizyta_dane, ktora posluzy jako lista wizyt.
/// </summary>
typedef struct Wizyta {
	int id_pacjenta;
	int id_lekarza;
	char data[11];
	char godzina[6];
	int czas_trwania;
	char status[20];
	struct Wizyta* previous;
	struct Wizyta* next;
} Wizyta_dane;

enum Sortowanie { DESC, ASC };
enum PacjentPole { pID, pIMIE, pNAZWISKO, pPESEL, DATA_URODZENIA, ADRES, pEMAIL, pTELEFON, WAGA, WZROST, ODDZIAL_NFZ, pIMIE_I_NAZWISKO };
enum LekarzPole { lID, lIMIE, lNAZWISKO, lPESEL, NUMER_PWZ, TYTUL_ZAWODOWY, SPECJALIZACJA, lEMAIL, lTELEFON, GODZINY_PRACY, lIMIE_I_NAZWISKO };
enum WizytaPole { ID_PACJENTA, ID_LEKARZA, DATA, GODZINA, CZAS_TRWANIA, STATUS };

/// <summary>
/// Fukncja wyswietlajaca logo.
/// 
/// Funkcja pobiera grafike stworzona ze znakow ASCII, z pliku logo.txt, a nastpnie ja wyswietla.
/// </summary>
void wyswietl_logo() {
	FILE* logo;
	char logo_letter;
	logo = fopen("logo.txt", "r");
	if (logo == NULL)
	{
		printf("#####/Blad otwarcia pliku 'logo'/#####");
		exit(0);
	}

	while (!feof(logo))
	{
		fscanf(logo, "%c", &logo_letter);
		if (!feof(logo))
			printf("%c", logo_letter);
	}
	fclose(logo);
}

/// <summary>
/// Funkcja wczytujaca dane do list.
/// 
/// Funkcja operuje na 3 plikach. Pobiera z pliku, wszystkie dane potrzebne do utworzenia rekordu, a nastepnie dodaje je jako element listy.
/// Dodanie wszystkich rekordow nastepuje w petli. Postepuje analogicznie dla wszystkich plikow.
/// </summary>
void wczytaj_dane(Pacjent_dane** pacjent_head, Lekarz_dane** lekarz_head, Wizyta_dane** wizyta_head) { // wczytanie danych pacjentow
	FILE* plik_pacjenci = fopen("pacjenci.txt", "a+"); // otwarcie pliku z danymi pacjentow
	if (plik_pacjenci == NULL) {
		printf("Blad otwierania pliku 'pacjenci.txt'.\n");
		return;
	}

	FILE* plik_lekarze = fopen("lekarze.txt", "a+"); // otwarcie pliku z danymi lekarzy
	if (plik_lekarze == NULL) {
		printf("Blad otwierania pliku 'lekarze.txt'.\n");
		return;
	}

	FILE* plik_wizyty = fopen("wizyty.txt", "a+"); // otwarcie pliku z danymi wizyt
	if (plik_wizyty == NULL) {
		printf("Blad otwierania pliku 'wizyty.txt'.\n");
		return;
	}

	char bufor[255];
	while (fgets(bufor, sizeof(bufor), plik_pacjenci) != NULL) {
		if (*pacjent_head == NULL)
		{
			*pacjent_head = (Pacjent_dane*)malloc(sizeof(Pacjent_dane));
			sscanf(bufor, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%f;%[^\n]",
				&(*pacjent_head)->id, (*pacjent_head)->imie, (*pacjent_head)->nazwisko, (*pacjent_head)->pesel,
				(*pacjent_head)->data_urodzenia, (*pacjent_head)->adres, (*pacjent_head)->email, (*pacjent_head)->telefon,
				&(*pacjent_head)->waga, &(*pacjent_head)->wzrost, (*pacjent_head)->oddzial_nfz);
			(*pacjent_head)->previous = NULL;
			(*pacjent_head)->next = NULL;
		}
		else
		{
			Pacjent_dane* pacjent_current = *pacjent_head;
			while (pacjent_current->next != NULL) {
				pacjent_current = pacjent_current->next;
			}
			pacjent_current->next = (Pacjent_dane*)malloc(sizeof(Pacjent_dane));
			sscanf(bufor, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%f;%f;%[^\n]",
				&pacjent_current->next->id, pacjent_current->next->imie, pacjent_current->next->nazwisko, pacjent_current->next->pesel,
				pacjent_current->next->data_urodzenia, pacjent_current->next->adres, pacjent_current->next->email, pacjent_current->next->telefon,
				&pacjent_current->next->waga, &pacjent_current->next->wzrost, pacjent_current->next->oddzial_nfz);
			pacjent_current->next->previous = pacjent_current;
			pacjent_current->next->next = NULL;
		}
	}

	while (fgets(bufor, sizeof(bufor), plik_lekarze) != NULL) {
		if (*lekarz_head == NULL)
		{
			*lekarz_head = (Lekarz_dane*)malloc(sizeof(Lekarz_dane));
			sscanf(bufor, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
				&(*lekarz_head)->id, (*lekarz_head)->imie, (*lekarz_head)->nazwisko, (*lekarz_head)->pesel,
				(*lekarz_head)->numer_pwz, (*lekarz_head)->tytul_zawodowy, (*lekarz_head)->specjalizacja, (*lekarz_head)->email,
				(*lekarz_head)->telefon, (*lekarz_head)->godziny_pracy[0],
				(*lekarz_head)->godziny_pracy[1], (*lekarz_head)->godziny_pracy[2],
				(*lekarz_head)->godziny_pracy[3], (*lekarz_head)->godziny_pracy[4],
				(*lekarz_head)->godziny_pracy[5], (*lekarz_head)->godziny_pracy[6]);

			(*lekarz_head)->previous = NULL;
			(*lekarz_head)->next = NULL;
		}
		else {
			Lekarz_dane* lekarz_current = *lekarz_head;
			while (lekarz_current->next != NULL) {
				lekarz_current = lekarz_current->next;
			}
			lekarz_current->next = (Lekarz_dane*)malloc(sizeof(Lekarz_dane));
			sscanf(bufor, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
				&lekarz_current->next->id, lekarz_current->next->imie, lekarz_current->next->nazwisko, lekarz_current->next->pesel,
				lekarz_current->next->numer_pwz, lekarz_current->next->tytul_zawodowy, lekarz_current->next->specjalizacja, lekarz_current->next->email,
				lekarz_current->next->telefon, lekarz_current->next->godziny_pracy[0], lekarz_current->next->godziny_pracy[1], lekarz_current->next->godziny_pracy[2],
				lekarz_current->next->godziny_pracy[3], lekarz_current->next->godziny_pracy[4], lekarz_current->next->godziny_pracy[5], lekarz_current->next->godziny_pracy[6]);
			lekarz_current->next->previous = lekarz_current;
			lekarz_current->next->next = NULL;
		}
	}

	while (fgets(bufor, sizeof(bufor), plik_wizyty) != NULL) {
		if (*wizyta_head == NULL)
		{
			*wizyta_head = (Wizyta_dane*)malloc(sizeof(Wizyta_dane));
			sscanf(bufor, "%d;%d;%[^;];%[^;];%d;%[^\n]",
				&(*wizyta_head)->id_pacjenta, &(*wizyta_head)->id_lekarza, (*wizyta_head)->data, (*wizyta_head)->godzina,
				&(*wizyta_head)->czas_trwania, (*wizyta_head)->status);
			(*wizyta_head)->previous = NULL;
			(*wizyta_head)->next = NULL;
		}
		else
		{
			Wizyta_dane* wizyta_current = *wizyta_head;
			while (wizyta_current->next != NULL) {
				wizyta_current = wizyta_current->next;
			}
			wizyta_current->next = (Wizyta_dane*)malloc(sizeof(Wizyta_dane));
			sscanf(bufor, "%d;%d;%[^;];%[^;];%d;%[^\n]",
				&wizyta_current->next->id_pacjenta, &wizyta_current->next->id_lekarza, wizyta_current->next->data, wizyta_current->next->godzina,
				&wizyta_current->next->czas_trwania, wizyta_current->next->status);
			wizyta_current->next->previous = wizyta_current;
			wizyta_current->next->next = NULL;
		}
	}

	fclose(plik_pacjenci); // zamkniecie pliku z danymi pacjentow
	fclose(plik_lekarze); // zamkniecie pliku z danymi lekarzy
	fclose(plik_wizyty); // zamkniecie pliku z danymi wizyt
}

/// <summary>
/// Funkcja wyswietlajaca liste pacjentow.
/// 
/// Funkcja pobiera dane z listy i wyswietla je w tabeli.
/// </summary>
void wyswietl_liste_pacjentow(Pacjent_dane** pacjent_head)
{
	Pacjent_dane* pacjent_current = *pacjent_head;
	printf("\n _______________________________________________________________________________________________________________ ");
	printf("\n|\t\t\t\t\t\t   LISTA PACJENTOW\t\t\t\t\t        |");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	while (pacjent_current != NULL)
	{
		printf("\n|---------------------------------------------------------------------------------------------------------------|");
		printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
			pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
			pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
		printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
		printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
		printf("\n|---------------------------------------------------------------------------------------------------------------|");
		pacjent_current = pacjent_current->next;
	}
	printf("\nAby wrocic wcisnij ENTER...");

	// petla do czekania na wcisniecie ENTER
	getchar();
	while (getchar() != '\n');
}

/// <summary>
/// Funkcja zamieniajaca pozycje dwoch pacjentow.
/// 
/// Funkcja zamienia dane pomiedzy dwoma rekordami pacjentow. Wskazniki na pacjentow przekazywane sa jako argumenty funkcji.
/// </summary>
void zamiana_pacjentow(Pacjent_dane* current, Pacjent_dane* nextPacjent) {
	// Zamiana wartoœci pól miêdzy pacjentami
	int temp_id = current->id;
	current->id = nextPacjent->id;
	nextPacjent->id = temp_id;

	char temp_imie[50];
	strcpy(temp_imie, current->imie);
	strcpy(current->imie, nextPacjent->imie);
	strcpy(nextPacjent->imie, temp_imie);

	char temp_nazwisko[50];
	strcpy(temp_nazwisko, current->nazwisko);
	strcpy(current->nazwisko, nextPacjent->nazwisko);
	strcpy(nextPacjent->nazwisko, temp_nazwisko);

	char temp_pesel[12];
	strcpy(temp_pesel, current->pesel);
	strcpy(current->pesel, nextPacjent->pesel);
	strcpy(nextPacjent->pesel, temp_pesel);

	char temp_data_urodzenia[11];
	strcpy(temp_data_urodzenia, current->data_urodzenia);
	strcpy(current->data_urodzenia, nextPacjent->data_urodzenia);
	strcpy(nextPacjent->data_urodzenia, temp_data_urodzenia);

	char temp_adres[100];
	strcpy(temp_adres, current->adres);
	strcpy(current->adres, nextPacjent->adres);
	strcpy(nextPacjent->adres, temp_adres);

	char temp_email[50];
	strcpy(temp_email, current->email);
	strcpy(current->email, nextPacjent->email);
	strcpy(nextPacjent->email, temp_email);

	char temp_telefon[12];
	strcpy(temp_telefon, current->telefon);
	strcpy(current->telefon, nextPacjent->telefon);
	strcpy(nextPacjent->telefon, temp_telefon);

	float temp_waga = current->waga;
	current->waga = nextPacjent->waga;
	nextPacjent->waga = temp_waga;

	float temp_wzrost = current->wzrost;
	current->wzrost = nextPacjent->wzrost;
	nextPacjent->wzrost = temp_wzrost;

	char temp_oddzial_nfz[50];
	strcpy(temp_oddzial_nfz, current->oddzial_nfz);
	strcpy(current->oddzial_nfz, nextPacjent->oddzial_nfz);
	strcpy(nextPacjent->oddzial_nfz, temp_oddzial_nfz);
}

/// <summary>
/// Funkcja obslugujaca sortowanie pacjentow.
/// 
/// Funkcja na podstawie wybranego pola sortowania, zamienia dane pacjentow.
/// </summary>
void pacjent_sortuj(Pacjent_dane** pacjent_head, enum Sortowanie rosnaco, enum PacjentPole pole) {
	if (*pacjent_head == NULL || (*pacjent_head)->next == NULL) {
		return;
	}

	Pacjent_dane* current = *pacjent_head;
	Pacjent_dane* nextPacjent;

	while (current->next != NULL && pole == pID) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (nextPacjent->id > current->id && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (nextPacjent->id < current->id && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == pIMIE) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->imie, current->imie) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->imie, nextPacjent->imie) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == pNAZWISKO) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->nazwisko, current->nazwisko) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->nazwisko, nextPacjent->nazwisko) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == pPESEL) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->pesel, current->pesel) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->pesel, nextPacjent->pesel) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == DATA_URODZENIA) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->data_urodzenia, current->data_urodzenia) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->data_urodzenia, nextPacjent->data_urodzenia) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == ADRES) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->adres, current->adres) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->adres, nextPacjent->adres) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == pEMAIL) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->email, current->email) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->email, nextPacjent->email) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == pTELEFON) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->telefon, current->telefon) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->telefon, nextPacjent->telefon) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == WAGA) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (nextPacjent->waga > current->waga && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (nextPacjent->waga < current->waga && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == WZROST) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (nextPacjent->wzrost > current->wzrost && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (nextPacjent->wzrost < current->wzrost && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == ODDZIAL_NFZ) {
		nextPacjent = current->next;
		while (nextPacjent != NULL) {
			if (strcmp(nextPacjent->oddzial_nfz, current->oddzial_nfz) > 0 && rosnaco == 0)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			else if (strcmp(current->oddzial_nfz, nextPacjent->oddzial_nfz) > 0 && rosnaco == 1)
			{
				zamiana_pacjentow(current, nextPacjent);
			}
			nextPacjent = nextPacjent->next;
		}
		current = current->next;
	}
}

/// <summary>
/// Funkcja dodajaca pacjenta.
/// 
/// Funkcja na podstawie wpisywanych danych dodaje pacjenta do listy, a nastepnie do pliku.
/// </summary>
void dodaj_pacjenta(Pacjent_dane** pacjent_head) {
	FILE* plik_pacjenci = fopen("pacjenci.txt", "a+"); // otwarcie pliku z danymi pacjentow
	if (plik_pacjenci == NULL) {
		printf("Blad otwierania pliku 'pacjenci.txt'.\n");
		return;
	}

	Pacjent_dane* nowy_pacjent = (Pacjent_dane*)malloc(sizeof(Pacjent_dane));
	if (nowy_pacjent == NULL) {
		printf("Blad alokacji pamieci.\n");
		return;
	}

	printf("--------------------------------------------------------------------\n");
	printf("Podaj imie pacjenta: ");
	scanf("%s", &nowy_pacjent->imie);
	printf("Podaj nazwisko pacjenta: ");
	scanf("%s", &nowy_pacjent->nazwisko);
	printf("Podaj PESEL pacjenta: ");
	while (true) {
		scanf("%s", &nowy_pacjent->pesel);
		if (strlen(nowy_pacjent->pesel) != 11)
		{
			printf("Niewlasciwa ilosc znakow. Podaj PESEL pacjenta:  ");
			continue;
		}
		else
		{
			bool powtorz = false;
			for (int i = 0; i < strlen(nowy_pacjent->pesel); i++)
			{
				if (!isdigit(nowy_pacjent->pesel[i]))
				{
					printf("Numer PESEL musi skladac sie z cyfr. Podaj PESEL pacjenta:  ");
					powtorz = true;
					break;
				}
			}
			if (powtorz)
				continue;

			break;
		}
	}
	printf("Podaj date urodzenia pacjenta (RRRR-MM-DD): ");
	scanf("%s", &nowy_pacjent->data_urodzenia);
	printf("Podaj adres zamieszkania pacjenta: ");
	fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
	fgets(nowy_pacjent->adres, sizeof(nowy_pacjent->adres), stdin);
	nowy_pacjent->adres[strcspn(nowy_pacjent->adres, "\n")] = 0;	// usuwanie znaku nowej linii
	printf("Podaj e-mail pacjenta: ");
	scanf("%s", &nowy_pacjent->email);
	printf("Podaj numer telefonu pacjenta: ");
	scanf("%s", &nowy_pacjent->telefon);
	printf("Podaj wage pacjenta: ");
	scanf("%f", &nowy_pacjent->waga);
	printf("Podaj wzrost pacjenta: ");
	scanf("%f", &nowy_pacjent->wzrost);
	printf("Podaj oddzial NFZ pacjenta: ");
	scanf("%s", &nowy_pacjent->oddzial_nfz);

	if (*pacjent_head == NULL)
	{
		*pacjent_head = (Pacjent_dane*)malloc(sizeof(Pacjent_dane));
		*pacjent_head = nowy_pacjent;
		(*pacjent_head)->previous = NULL;
		(*pacjent_head)->next = NULL;
	}
	else
	{
		Pacjent_dane* current = *pacjent_head;
		while (current->next != NULL) {
			current = current->next;
		}
		int tempid = current->id;
		nowy_pacjent->id = tempid + 1;
		current->next = (Pacjent_dane*)malloc(sizeof(Pacjent_dane));
		current->next = nowy_pacjent;
		current->next->previous = current;
		current->next->next = NULL;
	}

	// zapisz dane pacjenta do pliku
	fprintf(plik_pacjenci, "%d;%s;%s;%s;%s;%s;%s;%s;%.2f;%.2f;%s\n", nowy_pacjent->id, nowy_pacjent->imie,
		nowy_pacjent->nazwisko, nowy_pacjent->pesel, nowy_pacjent->data_urodzenia, nowy_pacjent->adres,
		nowy_pacjent->email, nowy_pacjent->telefon, nowy_pacjent->waga, nowy_pacjent->wzrost, nowy_pacjent->oddzial_nfz);

	fclose(plik_pacjenci); // zamkniecie pliku z danymi pacjentow
}

/// <summary>
/// Funkcja usuwajaca pacjenta.
/// 
/// Funkcja usuwa pacjenta z listy i pliku, na podstawie id przkazanego w argumancie funkcji.
/// </summary>
void usun_pacjenta(Pacjent_dane** pacjent_head, Pacjent_dane** pacjent_current, int id) {
	puts("Czy na pewno chcesz usunac pacjenta? (t/n)");
	getchar();
	char c = getchar();
	if (c == 'n') {
		return;
	}

	Pacjent_dane* current = *pacjent_current;

	while (current != NULL) {
		if (current->id == id) {
			if (current->previous != NULL)
				current->previous->next = current->next;
			else
				*pacjent_current = current->next;

			if (current->next != NULL)
				current->next->previous = current->previous;

			free(current);

			FILE* plik_pacjenci = fopen("pacjenci.txt", "w"); // otwarcie pliku z danymi pacjentow
			if (plik_pacjenci == NULL) {
				return;
			}

			Pacjent_dane* current = *pacjent_head;
			for (int i = 1; current != NULL; i++) {
				current->id = i;
				fprintf(plik_pacjenci, "%d;%s;%s;%s;%s;%s;%s;%s;%.2f;%.2f;%s\n", current->id, current->imie,
					current->nazwisko, current->pesel, current->data_urodzenia, current->adres,
					current->email, current->telefon, current->waga, current->wzrost, current->oddzial_nfz);
				current = current->next;
			}

			fclose(plik_pacjenci); // zamkniecie pliku z danymi pacjentow

			return;
		}

		current = current->next;
	}
}

/// <summary>
/// Funkcja edytujaca pacjenta.
/// 
/// Funkcja pobiera od uzytkownika pole do edycji oraz nowe dane, a nastepnie modyfikuje rekord w liscie i pliku.
/// </summary>
void edytuj_pacjenta(Pacjent_dane** pacjent_head, Pacjent_dane** pacjent_current, int id) {
	Pacjent_dane* current = *pacjent_current;
	int wybor;
	while (current != NULL) {
		if (current->id == id) {
			do {
				printf("\nWybierz pole do edycji:\n");
				printf("1. Imie\n");
				printf("2. Nazwisko\n");
				printf("3. Pesel\n");
				printf("4. Data urodzenia\n");
				printf("5. Adres\n");
				printf("6. E-mail\n");
				printf("7. Telefon\n");
				printf("8. Waga\n");
				printf("9. Wzrost\n");
				printf("10. Oddzial NFZ\n");
				printf("11. Powrot\n");
				scanf("%d", &wybor);
				switch (wybor) {
				case 1:
					printf("Podaj nowe imie pacjenta: ");
					scanf("%s", &current->imie);
					break;
				case 2:
					printf("Podaj nowe nazwisko pacjenta: ");
					scanf("%s", &current->nazwisko);
					break;
				case 3:
					printf("Podaj nowy pesel pacjenta: ");
					while (true) {
						scanf("%s", &current->pesel);
						if (strlen(current->pesel) != 11)
						{
							printf("Niewlasciwa ilosc znakow. Podaj PESEL pacjenta:  ");
							continue;
						}
						else
						{
							bool powtorz = false;
							for (int i = 0; i < strlen(current->pesel); i++)
							{
								if (!isdigit(current->pesel[i]))
								{
									printf("Numer PESEL musi skladac sie z cyfr. Podaj PESEL pacjenta:  ");
									powtorz = true;
									break;
								}
							}
							if (powtorz)
								continue;

							break;
						}
					}
					break;
				case 4:
					printf("Podaj nowa date urodzenia pacjenta: ");
					scanf("%s", &current->data_urodzenia);
					break;
				case 5:
					printf("Podaj nowy adres pacjenta: ");
					scanf("%s", &current->adres);
					break;
				case 6:
					printf("Podaj nowy e-mail pacjenta: ");
					scanf("%s", &current->email);
					break;
				case 7:
					printf("Podaj nowy numer telefonu pacjenta: ");
					scanf("%s", &current->telefon);
					break;
				case 8:
					printf("Podaj nowa wage pacjenta: ");
					scanf("%f", &current->waga);
					break;
				case 9:
					printf("Podaj nowy wzrost pacjenta: ");
					scanf("%f", &current->wzrost);
					break;
				case 10:
					printf("Podaj nowy oddzial NFZ pacjenta: ");
					scanf("%s", &current->oddzial_nfz);
				}
			} while (wybor != 11);

			FILE* plik_pacjenci = fopen("pacjenci.txt", "w"); // otwarcie pliku z danymi pacjentow
			if (plik_pacjenci == NULL) {
				return;
			}

			current = *pacjent_head;
			while (current != NULL) {
				fprintf(plik_pacjenci, "%d;%s;%s;%s;%s;%s;%s;%s;%.2f;%.2f;%s\n", current->id, current->imie,
					current->nazwisko, current->pesel, current->data_urodzenia, current->adres,
					current->email, current->telefon, current->waga, current->wzrost, current->oddzial_nfz);
				current = current->next;
			}

			fclose(plik_pacjenci); // zamkniecie pliku z danymi pacjentow

		}
	}
}

/// <summary>
/// Funkcja znajdujaca szukanych pacjentow w liscie.
/// 
/// Funkcja na podstawie argumentow przekazancyh do listy znajduje pasujacych pacjentow oraz ich wyswietla. Nastepnie jest mozliwosc wybrania pacjenta za pomoca wpisania jego id.
/// Po wybraniu pacjenta, jest on wyswietlany, a nastepnie jest mozliwosc jego edycji lub usuniecia. 
/// </summary>
void wyszukaj_pacjenta(Pacjent_dane** pacjent_head, enum PacjentPole pole, char* wartosc, bool tylko_pokaz) {
	if (*pacjent_head == NULL) {
		printf("Lista jest pusta.\n");
		return;
	}

	Pacjent_dane* pacjent_current = *pacjent_head;
	bool znaleziono = 0;

	printf("\n _______________________________________________________________________________________________________________ ");
	printf("\n|\t\t\t\t\t\t   ZNALEZIENI PACJENCI\t\t\t\t\t        |");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");

	while (pacjent_current != NULL) {
		switch (pole) {
		case pID:
			if (pacjent_current->id == atoi(wartosc))
				znaleziono = 1;
			break;
		case pIMIE:
			if (strcmp(pacjent_current->imie, wartosc) == 0)
				znaleziono = 1;
			break;
		case pNAZWISKO:
			if (strcmp(pacjent_current->nazwisko, wartosc) == 0)
				znaleziono = 1;
			break;
		case pPESEL:
			if (strcmp(pacjent_current->pesel, wartosc) == 0)
				znaleziono = 1;
			break;
		case DATA_URODZENIA:
			if (strcmp(pacjent_current->data_urodzenia, wartosc) == 0)
				znaleziono = 1;
			break;
		case ADRES:
			if (strcmp(pacjent_current->adres, wartosc) == 0)
				znaleziono = 1;
			break;
		case pEMAIL:
			if (strcmp(pacjent_current->email, wartosc) == 0)
				znaleziono = 1;
			break;
		case pTELEFON:
			if (strcmp(pacjent_current->telefon, wartosc) == 0)
				znaleziono = 1;
			break;
		case WAGA:
			if (pacjent_current->waga == atof(wartosc))
				znaleziono = 1;
			break;
		case WZROST:
			if (pacjent_current->wzrost == atof(wartosc))
				znaleziono = 1;
			break;
		case ODDZIAL_NFZ:
			if (strcmp(pacjent_current->oddzial_nfz, wartosc) == 0)
				znaleziono = 1;
			break;
		case pIMIE_I_NAZWISKO:
		{
			char* tempdane[2];
			char* ptr = strtok(wartosc, " ");

			for (int i = 0; ptr != NULL; i++)
			{
				tempdane[i] = ptr;
				ptr = strtok(NULL, " ");
			}

			if (strcmp(pacjent_current->nazwisko, tempdane[1]) == 0 && strcmp(pacjent_current->imie, tempdane[0]) == 0)
				znaleziono = 1;
			break;
		}
		default:
			printf("Nieprawid³owe pole.\n");
			return;
		}

		if (znaleziono) {
			printf("\n|---------------------------------------------------------------------------------------------------------------|");
			printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
				pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
				pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
			printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
			printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
			printf("\n|---------------------------------------------------------------------------------------------------------------|");

			znaleziono = 0;  // Zresetuj flagê znaleziono
		}

		pacjent_current = pacjent_current->next;
	}

	if (tylko_pokaz)
		return;

	int tempint = 0;
	do {
		printf("\nPodaj ID pacjenta (0 - aby wrocic): ");
		scanf("%d", &tempint);

		if (tempint == 0)
			return;

		pacjent_current = *pacjent_head;
		while (pacjent_current != NULL) {
			if (pacjent_current->id == tempint) {
				system("cls");
				wyswietl_logo();
				printf("\n _______________________________________________________________________________________________________________");
				printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
				printf("\n|---------------------------------------------------------------------------------------------------------------|");
				printf("\n|---------------------------------------------------------------------------------------------------------------|");
				printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
					pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
					pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
				printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
				printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
				printf("\n|---------------------------------------------------------------------------------------------------------------|");
				break;
			}
			else {
				pacjent_current = pacjent_current->next;
			}

			if (pacjent_current == NULL) {
				return;
			}
		}

		printf("\n\n1. Edytuj pacjenta");
		printf("\n2. Usun pacjenta");
		printf("\n0. Wroc");
		printf("\nWybierz dzialanie: ");

		scanf("%d", &tempint);

		switch (tempint)
		{
		case 1:
			edytuj_pacjenta(pacjent_head, &pacjent_current, pacjent_current->id);
			tempint = 0;
			break;

		case 2:
			usun_pacjenta(pacjent_head, &pacjent_current, pacjent_current->id);
			tempint = 0;
			break;

		default:
			break;
		}
	} while (tempint != 0);
}

/// <summary>
/// Funkcja wyswietlajaca liste lekarzy.
/// 
/// Funkcja pobiera dane z listy i wyswietla je w tabeli.
/// </summary>
void wyswietl_liste_lekarzy(Lekarz_dane** lekarz_head)
{
	Lekarz_dane* lekarz_current = *lekarz_head;
	printf("\n _______________________________________________________________________________________________________________ ");
	printf("\n|\t\t\t\t\t\t   LISTA LEKARZY\t\t\t\t\t        |");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	printf("\n|   ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t| TYTUL\t\t| TEL\t\t| EMAIL\t\t\t|");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	while (lekarz_current != NULL)
	{
		printf("\n|---------------------------------------------------------------------------------------------------------------|");
		printf("\n|\t| %s  \t| %s  \t| %s\t| %s\t| %s\t| %s", lekarz_current->imie, lekarz_current->nazwisko, lekarz_current->pesel,
			lekarz_current->tytul_zawodowy, lekarz_current->telefon, lekarz_current->email);
		printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", lekarz_current->id);
		printf("\n|\t|PWZ: %s\t| SPECJALIZACJA: %s", lekarz_current->numer_pwz, lekarz_current->specjalizacja);
		printf("\n|\t|-------------------------------------------------------------------------------------------------------|");
		printf("\n|\t|GODZINY PRACY:\tPON: %s", lekarz_current->godziny_pracy[0]);
		printf("\n|\t|\t\tWT:  %s", lekarz_current->godziny_pracy[1]);
		printf("\n|\t|\t\tSR:  %s", lekarz_current->godziny_pracy[2]);
		printf("\n|\t|\t\tCZW: %s", lekarz_current->godziny_pracy[3]);
		printf("\n|\t|\t\tPT:  %s", lekarz_current->godziny_pracy[4]);
		printf("\n|\t|\t\tSOB: %s", lekarz_current->godziny_pracy[5]);
		printf("\tNIEDZ: %s", lekarz_current->godziny_pracy[6]);
		printf("\n|---------------------------------------------------------------------------------------------------------------|");
		lekarz_current = lekarz_current->next;
	}
	printf("\nAby wrocic wcisnij ENTER...");

	// petla do czekania na wcisniecie ENTER
	getchar();
	while (getchar() != '\n');
}

/// <summary>
/// Funkcja zamieniajaca pozycje dwoch lekarzy.
/// 
/// Funkcja zamienia dane pomiedzy dwoma rekordami lekarzy. Wskazniki na lekarzy przekazywane sa jako argumenty funkcji.
/// </summary>
void zamiana_lekarzy(Lekarz_dane* current, Lekarz_dane* nextLekarz)
{
	int tmpid;
	char tmpimie[50];
	char tmpnazwisko[50];
	char tmppesel[12];
	char tmpnumer_pwz[8];
	char tmptytul_zawodowy[20];
	char tmpspecjalizacja[50];
	char tmpemail[50];
	char tmptelefon[12];
	char tmpgodziny_pracy[7][20];

	tmpid = current->id;
	current->id = nextLekarz->id;
	nextLekarz->id = tmpid;

	strcpy(tmpimie, current->imie);
	strcpy(current->imie, nextLekarz->imie);
	strcpy(nextLekarz->imie, tmpimie);

	strcpy(tmpnazwisko, current->nazwisko);
	strcpy(current->nazwisko, nextLekarz->nazwisko);
	strcpy(nextLekarz->nazwisko, tmpnazwisko);

	strcpy(tmppesel, current->pesel);
	strcpy(current->pesel, nextLekarz->pesel);
	strcpy(nextLekarz->pesel, tmppesel);

	strcpy(tmpnumer_pwz, current->numer_pwz);
	strcpy(current->numer_pwz, nextLekarz->numer_pwz);
	strcpy(nextLekarz->numer_pwz, tmpnumer_pwz);

	strcpy(tmptytul_zawodowy, current->tytul_zawodowy);
	strcpy(current->tytul_zawodowy, nextLekarz->tytul_zawodowy);
	strcpy(nextLekarz->tytul_zawodowy, tmptytul_zawodowy);

	strcpy(tmpspecjalizacja, current->specjalizacja);
	strcpy(current->specjalizacja, nextLekarz->specjalizacja);
	strcpy(nextLekarz->specjalizacja, tmpspecjalizacja);

	strcpy(tmptelefon, current->telefon);
	strcpy(current->telefon, nextLekarz->telefon);
	strcpy(nextLekarz->telefon, tmptelefon);

	strcpy(tmpemail, current->email);
	strcpy(current->email, nextLekarz->email);
	strcpy(nextLekarz->email, tmpemail);

	strcpy(tmpgodziny_pracy[0], current->godziny_pracy[0]);
	strcpy(current->godziny_pracy[0], nextLekarz->godziny_pracy[0]);
	strcpy(nextLekarz->godziny_pracy[0], tmpgodziny_pracy[0]);

	strcpy(tmpgodziny_pracy[1], current->godziny_pracy[1]);
	strcpy(current->godziny_pracy[1], nextLekarz->godziny_pracy[1]);
	strcpy(nextLekarz->godziny_pracy[1], tmpgodziny_pracy[1]);

	strcpy(tmpgodziny_pracy[2], current->godziny_pracy[2]);
	strcpy(current->godziny_pracy[2], nextLekarz->godziny_pracy[2]);
	strcpy(nextLekarz->godziny_pracy[2], tmpgodziny_pracy[2]);

	strcpy(tmpgodziny_pracy[3], current->godziny_pracy[3]);
	strcpy(current->godziny_pracy[3], nextLekarz->godziny_pracy[3]);
	strcpy(nextLekarz->godziny_pracy[3], tmpgodziny_pracy[3]);

	strcpy(tmpgodziny_pracy[4], current->godziny_pracy[4]);
	strcpy(current->godziny_pracy[4], nextLekarz->godziny_pracy[4]);
	strcpy(nextLekarz->godziny_pracy[4], tmpgodziny_pracy[4]);

	strcpy(tmpgodziny_pracy[5], current->godziny_pracy[5]);
	strcpy(current->godziny_pracy[5], nextLekarz->godziny_pracy[5]);
	strcpy(nextLekarz->godziny_pracy[5], tmpgodziny_pracy[5]);

	strcpy(tmpgodziny_pracy[6], current->godziny_pracy[6]);
	strcpy(current->godziny_pracy[6], nextLekarz->godziny_pracy[6]);
	strcpy(nextLekarz->godziny_pracy[6], tmpgodziny_pracy[6]);
}

/// <summary>
/// Funkcja obslugujaca sortowanie lekarzy.
/// 
/// Funkcja na podstawie wybranego pola sortowania, zamienia dane lekarzy.
/// </summary>
void lekarz_sortuj(Lekarz_dane** lekarz_head, enum Sortowanie rosnaco, enum LekarzPole pole) {
	if (*lekarz_head == NULL || (*lekarz_head)->next == NULL) {
		return;
	}

	Lekarz_dane* current = *lekarz_head;
	Lekarz_dane* nextLekarz;

	while (current->next != NULL && pole == lID) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (nextLekarz->id > current->id && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (nextLekarz->id < current->id && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == lIMIE) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->imie, current->imie) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->imie, nextLekarz->imie) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == lNAZWISKO) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->nazwisko, current->nazwisko) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->nazwisko, nextLekarz->nazwisko) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == lPESEL) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->pesel, current->pesel) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->pesel, nextLekarz->pesel) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == NUMER_PWZ) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->numer_pwz, current->numer_pwz) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->numer_pwz, nextLekarz->numer_pwz) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == TYTUL_ZAWODOWY) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->tytul_zawodowy, current->tytul_zawodowy) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->tytul_zawodowy, nextLekarz->tytul_zawodowy) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == SPECJALIZACJA) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->specjalizacja, current->specjalizacja) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->specjalizacja, nextLekarz->specjalizacja) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == lEMAIL) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->email, current->email) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->email, nextLekarz->email) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == lTELEFON) {
		nextLekarz = current->next;
		while (nextLekarz != NULL) {
			if (strcmp(nextLekarz->telefon, current->telefon) > 0 && rosnaco == 0)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			else if (strcmp(current->telefon, nextLekarz->telefon) > 0 && rosnaco == 1)
			{
				zamiana_lekarzy(current, nextLekarz);
			}
			nextLekarz = nextLekarz->next;
		}
		current = current->next;
	}
}

/// <summary>
/// Funkcja dodajaca lekarza.
/// 
/// Funkcja na podstawie wpisywanych danych dodaje lekarza do listy, a nastepnie do pliku.
/// </summary>
void dodaj_lekarza(Lekarz_dane** lekarz_head) {
	FILE* plik_lekarze = fopen("lekarze.txt", "a+"); // otwarcie pliku z danymi lekarzy
	if (plik_lekarze == NULL) {
		printf("Blad otwierania pliku 'lekarze.txt'.\n");
		return;
	}

	Lekarz_dane* nowy_lekarz = (Lekarz_dane*)malloc(sizeof(Lekarz_dane));
	if (nowy_lekarz == NULL) {
		printf("Blad alokacji pamieci.\n");
		return;
	}
	printf("Podaj imie lekarza: ");
	scanf("%s", &nowy_lekarz->imie);
	printf("Podaj nazwisko lekarza: ");
	scanf("%s", &nowy_lekarz->nazwisko);
	printf("Podaj PESEL lekarza: ");
	while (true) {
		scanf("%s", &nowy_lekarz->pesel);
		if (strlen(nowy_lekarz->pesel) != 11)
		{
			printf("Niewlasciwa ilosc znakow. Podaj PESEL lekarza:  ");
			continue;
		}
		else
		{
			bool powtorz = false;
			for (int i = 0; i < strlen(nowy_lekarz->pesel); i++)
			{
				if (!isdigit(nowy_lekarz->pesel[i]))
				{
					printf("Numer PESEL musi skladac sie z cyfr. Podaj PESEL lekarza:  ");
					powtorz = true;
					break;
				}
			}
			if (powtorz)
				continue;

			break;
		}
	}
	printf("Podaj numer PWZ lekarza: ");
	scanf("%s", &nowy_lekarz->numer_pwz);
	printf("Podaj tytul zawodowy lekarza: ");
	fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
	fgets(nowy_lekarz->tytul_zawodowy, sizeof(nowy_lekarz->tytul_zawodowy), stdin);
	nowy_lekarz->tytul_zawodowy[strcspn(nowy_lekarz->tytul_zawodowy, "\n")] = 0;	// usuwanie znaku nowej linii
	printf("Podaj specjalizacje lekarza: ");
	fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
	fgets(nowy_lekarz->specjalizacja, sizeof(nowy_lekarz->specjalizacja), stdin);
	nowy_lekarz->specjalizacja[strcspn(nowy_lekarz->specjalizacja, "\n")] = 0;	// usuwanie znaku nowej linii
	printf("Podaj e-mail lekarza: ");
	scanf("%s", &nowy_lekarz->email);
	printf("Podaj numer telefonu lekarza: ");
	scanf("%s", &nowy_lekarz->telefon);
	printf("Podaj godziny pracy lekarza: ");
	for (int i = 0; i < 7; i++) {
		printf("\t\n%d dzien tygodnia: ", i + 1);
		scanf("%s", &nowy_lekarz->godziny_pracy[i]);
	}

	if (*lekarz_head == NULL) {
		*lekarz_head = (Lekarz_dane*)malloc(sizeof(Lekarz_dane));
		*lekarz_head = nowy_lekarz;
		(*lekarz_head)->previous = NULL;
		(*lekarz_head)->next = NULL;
	}
	else {
		Lekarz_dane* current = *lekarz_head;
		while (current->next != NULL) {
			current = current->next;
		}
		int tempid = current->id;
		nowy_lekarz->id = tempid + 1;
		current->next = (Lekarz_dane*)malloc(sizeof(Lekarz_dane));
		current->next = nowy_lekarz;
		current->next->previous = current;
		current->next->next = NULL;
	}

	// zapisz dane lekarza do pliku
	fprintf(plik_lekarze, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s", nowy_lekarz->id, nowy_lekarz->imie,
		nowy_lekarz->nazwisko, nowy_lekarz->pesel, nowy_lekarz->numer_pwz, nowy_lekarz->tytul_zawodowy,
		nowy_lekarz->specjalizacja, nowy_lekarz->email, nowy_lekarz->telefon, nowy_lekarz->godziny_pracy[0]
		, nowy_lekarz->godziny_pracy[1], nowy_lekarz->godziny_pracy[2], nowy_lekarz->godziny_pracy[3]
		, nowy_lekarz->godziny_pracy[4], nowy_lekarz->godziny_pracy[5], nowy_lekarz->godziny_pracy[6]);

	fclose(plik_lekarze); // zamkniecie pliku z danymi lekarzy
}

/// <summary>
/// Funkcja usuwajaca lekarza.
/// 
/// Funkcja usuwa lekarza z listy i pliku, na podstawie id przekazanego w argumencie funkcji.
/// </summary>
void usun_lekarza(Lekarz_dane** lekarz_head, Lekarz_dane** lekarz_current, int id) {
	puts("Czy na pewno chcesz usunac lekarza? (t/n)");
	getchar();
	char c = getchar();
	if (c == 'n') {
		return;
	}

	Lekarz_dane* current = *lekarz_current;

	while (current != NULL) {
		if (current->id == id) {
			if (current->previous != NULL)
				current->previous->next = current->next;
			else
				*lekarz_current = current->next;

			if (current->next != NULL)
				current->next->previous = current->previous;

			free(current);

			FILE* plik_lekarze = fopen("lekarze.txt", "w"); // otwarcie pliku z danymi pacjentow
			if (plik_lekarze == NULL) {
				return;
			}

			Lekarz_dane* current = *lekarz_head;
			for (int i = 1; current != NULL; i++) {
				current->id = i;
				fprintf(plik_lekarze, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", current->id, current->imie,
					current->nazwisko, current->pesel, current->numer_pwz, current->tytul_zawodowy,
					current->specjalizacja, current->email, current->telefon, current->godziny_pracy[0]
					, current->godziny_pracy[1], current->godziny_pracy[2], current->godziny_pracy[3]
					, current->godziny_pracy[4], current->godziny_pracy[5], current->godziny_pracy[6]);
				current = current->next;
			}

			fclose(plik_lekarze); // zamkniecie pliku z danymi pacjentow

			return;
		}

		current = current->next;
	}
}

/// <summary>
/// Funkcja edytujaca lekarza.
/// 
/// Funkcja pobiera od uzytkownika pole do edycji oraz nowe dane, a nastepnie modyfikuje rekord w liscie i pliku.
/// </summary>
void edytuj_lekarza(Lekarz_dane** lekarz_head, Lekarz_dane** lekarz_current, int id) {
	Lekarz_dane* current = *lekarz_current;
	int wybor;
	while (current != NULL) {
		if (current->id == id) {
			do {
				printf("\nWybierz pole do edycji:\n");
				puts("1. Imie");
				puts("2. Nazwisko");
				puts("3. Pesel");
				puts("4. Numer PWZ");
				puts("5. Tytul zawodowy");
				puts("6. Specjalizacja");
				puts("7. Email");
				puts("8. Telefon");
				puts("9. Godziny pracy");
				puts("0. Powrot");
				scanf("%d", &wybor);
				switch (wybor) {
				case 1:
					printf("Podaj nowe imie: ");
					scanf("%s", &current->imie);
					break;
				case 2:
					printf("Podaj nowe nazwisko: ");
					scanf("%s", &current->nazwisko);
					break;
				case 3:
					printf("Podaj nowy pesel: ");
					while (true) {
						scanf("%s", &current->pesel);
						if (strlen(current->pesel) != 11)
						{
							printf("Niewlasciwa ilosc znakow. Podaj PESEL lekarza:  ");
							continue;
						}
						else
						{
							bool powtorz = false;
							for (int i = 0; i < strlen(current->pesel); i++)
							{
								if (!isdigit(current->pesel[i]))
								{
									printf("Numer PESEL musi skladac sie z cyfr. Podaj PESEL lekarza:  ");
									powtorz = true;
									break;
								}
							}
							if (powtorz)
								continue;

							break;
						}
					}
					break;
				case 4:
					printf("Podaj nowy numer PWZ: ");
					scanf("%s", &current->numer_pwz);
					break;
				case 5:
					printf("Podaj nowy tytul zawodowy: ");
					scanf("%s", &current->tytul_zawodowy);
					break;
				case 6:
					printf("Podaj nowa specjalizacje: ");
					scanf("%s", &current->specjalizacja);
					break;
				case 7:
					printf("Podaj nowy email: ");
					scanf("%s", &current->email);
					break;
				case 8:
					printf("Podaj nowy telefon: ");
					scanf("%s", &current->telefon);
					break;
				case 9:
					printf("Podaj nowe godziny pracy: ");
					for (int i = 0; i < 7; i++) {
						printf("\t\n%d dzien tygodnia: ", i + 1);
						scanf("%s", &current->godziny_pracy[i]);
					}
					break;
				}
			} while (wybor != 0);

			FILE* plik_lekarze = fopen("lekarze.txt", "w"); // otwarcie pliku z danymi pacjentow
			if (plik_lekarze == NULL) {
				return;
			}

			current = *lekarz_head;
			while (current != NULL) {
				fprintf(plik_lekarze, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", current->id, current->imie,
					current->nazwisko, current->pesel, current->numer_pwz, current->tytul_zawodowy,
					current->specjalizacja, current->email, current->telefon, current->godziny_pracy[0]
					, current->godziny_pracy[1], current->godziny_pracy[2], current->godziny_pracy[3]
					, current->godziny_pracy[4], current->godziny_pracy[5], current->godziny_pracy[6]);
				current = current->next;
			}

			fclose(plik_lekarze); // zamkniecie pliku z danymi pacjentow

		}
	}
}

/// <summary>
/// Funkcja znajdujaca szukanych lekarzy w liscie.
/// 
/// Funkcja na podstawie argumentow przekazancyh do listy znajduje pasujacych lekarzy oraz ich wyswietla. Nastepnie jest mozliwosc wybrania lekrza za pomoca wpisania jego id.
/// Po wybraniu lekrza, jest on wyswietlany, a nastepnie jest mozliwosc jego edycji lub usuniecia. 
/// </summary>
void wyszukaj_lekarza(Lekarz_dane** lekarz_head, enum LekarzPole pole, char* wartosc) {
	if (*lekarz_head == NULL) {
		printf("Lista jest pusta.\n");
		return;
	}

	Lekarz_dane* lekarz_current = *lekarz_head;
	bool znaleziono = 0;

	printf("\n _______________________________________________________________________________________________________________ ");
	printf("\n|\t\t\t\t\t\t   ZNALEZIENI LEKARZE\t\t\t\t\t        |");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	printf("\n|   ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t| TYTUL\t\t| TEL\t\t| EMAIL\t\t\t|");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");

	while (lekarz_current != NULL) {
		switch (pole) {
		case lID:
			if (lekarz_current->id == atoi(wartosc))
				znaleziono = 1;
			break;
		case lIMIE:
			if (strcmp(lekarz_current->imie, wartosc) == 0)
				znaleziono = 1;
			break;
		case lNAZWISKO:
			if (strcmp(lekarz_current->nazwisko, wartosc) == 0)
				znaleziono = 1;
			break;
		case lPESEL:
			if (strcmp(lekarz_current->pesel, wartosc) == 0)
				znaleziono = 1;
			break;
		case NUMER_PWZ:
			if (strcmp(lekarz_current->numer_pwz, wartosc) == 0)
				znaleziono = 1;
			break;
		case TYTUL_ZAWODOWY:
			if (strcmp(lekarz_current->tytul_zawodowy, wartosc) == 0)
				znaleziono = 1;
			break;
		case SPECJALIZACJA:
			if (strcmp(lekarz_current->specjalizacja, wartosc) == 0)
				znaleziono = 1;
			break;
		case lEMAIL:
			if (strcmp(lekarz_current->email, wartosc) == 0)
				znaleziono = 1;
			break;
		case lTELEFON:
			if (strcmp(lekarz_current->telefon, wartosc) == 0)
				znaleziono = 1;
			break;
		case lIMIE_I_NAZWISKO:
		{
			char* tempdane[2];
			char* ptr = strtok(wartosc, " ");

			for (int i = 0; ptr != NULL; i++)
			{
				tempdane[i] = ptr;
				ptr = strtok(NULL, " ");
			}

			if (strcmp(lekarz_current->nazwisko, tempdane[1]) == 0 && strcmp(lekarz_current->imie, tempdane[0]) == 0)
				znaleziono = 1;
			break;
		}
		default:
			printf("Nieprawid³owe pole.\n");
			return;
		}


		if (znaleziono) {
			printf("\n|---------------------------------------------------------------------------------------------------------------|");
			printf("\n|\t| %s  \t| %s  \t| %s\t| %s\t| %s\t| %s", lekarz_current->imie, lekarz_current->nazwisko, lekarz_current->pesel,
				lekarz_current->tytul_zawodowy, lekarz_current->telefon, lekarz_current->email);
			printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", lekarz_current->id);
			printf("\n|\t|PWZ: %s\t| SPECJALIZACJA: %s", lekarz_current->numer_pwz, lekarz_current->specjalizacja);
			printf("\n|\t|-------------------------------------------------------------------------------------------------------|");
			printf("\n|\t|GODZINY PRACY:\tPON: %s", lekarz_current->godziny_pracy[0]);
			printf("\n|\t|\t\tWT:  %s", lekarz_current->godziny_pracy[1]);
			printf("\n|\t|\t\tSR:  %s", lekarz_current->godziny_pracy[2]);
			printf("\n|\t|\t\tCZW: %s", lekarz_current->godziny_pracy[3]);
			printf("\n|\t|\t\tPT:  %s", lekarz_current->godziny_pracy[4]);
			printf("\n|\t|\t\tSOB: %s", lekarz_current->godziny_pracy[5]);
			printf("\tNIEDZ: %s", lekarz_current->godziny_pracy[6]);
			printf("\n|---------------------------------------------------------------------------------------------------------------|");

			znaleziono = 0;  // Zresetuj flagê znaleziono
		}

		lekarz_current = lekarz_current->next;
	}

	int tempint = 0;
	do {
		printf("\nPodaj ID lekarza (0 - aby wrocic): ");
		scanf("%d", &tempint);

		if (tempint == 0)
			return;

		lekarz_current = *lekarz_head;
		while (lekarz_current != NULL) {
			if (lekarz_current->id == tempint) {
				system("cls");
				wyswietl_logo();
				printf("\n _______________________________________________________________________________________________________________ ");
				printf("\n|   ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t| TYTUL\t\t| TEL\t\t| EMAIL\t\t\t|");
				printf("\n|---------------------------------------------------------------------------------------------------------------|");
				printf("\n|---------------------------------------------------------------------------------------------------------------|");
				printf("\n|\t| %s  \t| %s  \t| %s\t| %s\t| %s\t| %s", lekarz_current->imie, lekarz_current->nazwisko, lekarz_current->pesel,
					lekarz_current->tytul_zawodowy, lekarz_current->telefon, lekarz_current->email);
				printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", lekarz_current->id);
				printf("\n|\t|PWZ: %s\t| SPECJALIZACJA: %s", lekarz_current->numer_pwz, lekarz_current->specjalizacja);
				printf("\n|\t|-------------------------------------------------------------------------------------------------------|");
				printf("\n|\t|GODZINY PRACY:\tPON: %s", lekarz_current->godziny_pracy[0]);
				printf("\n|\t|\t\tWT:  %s", lekarz_current->godziny_pracy[1]);
				printf("\n|\t|\t\tSR:  %s", lekarz_current->godziny_pracy[2]);
				printf("\n|\t|\t\tCZW: %s", lekarz_current->godziny_pracy[3]);
				printf("\n|\t|\t\tPT:  %s", lekarz_current->godziny_pracy[4]);
				printf("\n|\t|\t\tSOB: %s", lekarz_current->godziny_pracy[5]);
				printf("\tNIEDZ: %s", lekarz_current->godziny_pracy[6]);
				printf("\n|---------------------------------------------------------------------------------------------------------------|");
				break;
			}
			else {
				lekarz_current = lekarz_current->next;
			}

			if (lekarz_current == NULL) {
				return;
			}
		}

		printf("\n\n1. Edytuj lekarza");
		printf("\n2. Usun lekarza");
		printf("\n0. Wroc");
		printf("\nWybierz dzialanie: ");

		scanf("%d", &tempint);

		switch (tempint)
		{
		case 1:
			edytuj_lekarza(lekarz_head, &lekarz_current, lekarz_current->id);
			tempint = 0;
			break;

		case 2:
			usun_lekarza(lekarz_head, &lekarz_current, lekarz_current->id);
			tempint = 0;
			break;
		default:
			break;
		}
	} while (tempint != 0);
}

/// <summary>
/// Funkcja wyswietlajaca spis wizyt.
/// 
/// Funkcja pobiera dane z listy i wyswietla je w tabeli.
/// </summary>
void wyswietl_wizyty(Wizyta_dane** wizyta_head)
{
	Wizyta_dane* current = *wizyta_head;
	printf("\n _______________________________________________________________ ");
	printf("\n|\t\t\t     WIZYTY\t\t\t\t|");
	printf("\n|---------------------------------------------------------------|");
	printf("\n|   ID   |  ID   |     DATA \t| GODZ\t| CZAS \t|    STATUS \t|");
	printf("\n|PACJENTA|LEKARZA|\t\t|\t|TRWANIA|\t\t|");
	printf("\n|---------------------------------------------------------------|");
	while (current != NULL)
	{
		printf("\n|---------------------------------------------------------------|");
		printf("\n|  %d\t |  %d\t |  %s\t| %s\t| %dmin\t| %s\t|", current->id_pacjenta, current->id_lekarza, current->data,
			current->godzina, current->czas_trwania, current->status);
		printf("\n|---------------------------------------------------------------|");
		current = current->next;
	}
	printf("\nAby wrocic wcisnij ENTER...");

	// petla do czekania na wcisniecie ENTER
	getchar();
	while (getchar() != '\n');
}

/// <summary>
/// Funkcja zamieniajaca pozycje dwoch wizyt.
/// 
/// Funkcja zamienia dane pomiedzy dwoma rekordami wizyt. Wskazniki na wizyty przekazywane sa jako argumenty funkcji.
/// </summary>
void zamiana_wizyt(Wizyta_dane* current, Wizyta_dane* nextWizyta)
{
	int tmpid_pacjenta;
	int tmpid_lekarza;
	char tmpdata[11];
	char tmpgodzina[6];
	int tmpczas_trwania;
	char tmpstatus[20];

	tmpid_pacjenta = current->id_pacjenta;
	current->id_pacjenta = nextWizyta->id_pacjenta;
	nextWizyta->id_pacjenta = tmpid_pacjenta;

	tmpid_lekarza = current->id_lekarza;
	current->id_lekarza = nextWizyta->id_lekarza;
	nextWizyta->id_lekarza = tmpid_lekarza;

	tmpczas_trwania = current->czas_trwania;
	current->czas_trwania = nextWizyta->czas_trwania;
	nextWizyta->czas_trwania = tmpczas_trwania;

	strcpy(tmpdata, current->data);
	strcpy(current->data, nextWizyta->data);
	strcpy(nextWizyta->data, tmpdata);

	strcpy(tmpgodzina, current->godzina);
	strcpy(current->godzina, nextWizyta->godzina);
	strcpy(nextWizyta->godzina, tmpgodzina);

	strncpy(tmpstatus, current->status, sizeof(tmpstatus));
	strncpy(current->status, nextWizyta->status, sizeof(current->status));
	strncpy(nextWizyta->status, tmpstatus, sizeof(nextWizyta->status));
}

/// <summary>
/// Funkcja obslugujaca sortowanie wizyt.
/// 
/// Funkcja na podstawie wybranego pola sortowania, zamienia dane wizyt.
/// </summary>
void wizyta_sortuj(Wizyta_dane** wizyta_head, enum Sortowanie rosnaco, enum WizytaPole pole) {
	if (*wizyta_head == NULL || (*wizyta_head)->next == NULL) {
		return;
	}

	Wizyta_dane* current = *wizyta_head;
	Wizyta_dane* nextWizyta;

	while (current->next != NULL && pole == ID_PACJENTA) {
		nextWizyta = current->next;
		while (nextWizyta != NULL) {
			if (nextWizyta->id_pacjenta > current->id_pacjenta && rosnaco == 0)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			else if (nextWizyta->id_pacjenta < current->id_pacjenta && rosnaco == 1)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			nextWizyta = nextWizyta->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == ID_LEKARZA) {
		nextWizyta = current->next;
		while (nextWizyta != NULL) {
			if (nextWizyta->id_lekarza > current->id_lekarza && rosnaco == 0)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			else if (nextWizyta->id_lekarza < current->id_lekarza && rosnaco == 1)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			nextWizyta = nextWizyta->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == DATA) {
		nextWizyta = current->next;
		while (nextWizyta != NULL) {
			if (strcmp(nextWizyta->data, current->data) > 0 && rosnaco == 0)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			else if (strcmp(current->data, nextWizyta->data) > 0 && rosnaco == 1)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			nextWizyta = nextWizyta->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == GODZINA) {
		nextWizyta = current->next;
		while (nextWizyta != NULL) {
			if (strcmp(nextWizyta->godzina, current->godzina) > 0 && rosnaco == 0)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			else if (strcmp(current->godzina, nextWizyta->godzina) > 0 && rosnaco == 1)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			nextWizyta = nextWizyta->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == CZAS_TRWANIA) {
		nextWizyta = current->next;
		while (nextWizyta != NULL) {
			if (nextWizyta->czas_trwania > current->czas_trwania && rosnaco == 0)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			else if (nextWizyta->czas_trwania < current->czas_trwania && rosnaco == 1)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			nextWizyta = nextWizyta->next;
		}
		current = current->next;
	}

	while (current->next != NULL && pole == STATUS) {
		nextWizyta = current->next;
		while (nextWizyta != NULL) {
			if (strcmp(nextWizyta->status, current->status) > 0 && rosnaco == 0)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			else if (strcmp(current->status, nextWizyta->status) > 0 && rosnaco == 1)
			{
				zamiana_wizyt(current, nextWizyta);
			}
			nextWizyta = nextWizyta->next;
		}
		current = current->next;
	}
}

/// <summary>
/// Funkcja dodajaca wizyte.
/// 
/// Funkcja na podstawie przekazanych argumentow najpierw znajduje oraz wyswietla wybranych lekarzy. Nastepnie by lekarza dodac do wizyty trzeba podaj jego id.
/// Po wybraniu lekarza funkcja przechodzi do wybrania pola na podstawie, ktorego wyszukamy pacjenta. Po wpisaniu wywolywana jest funkcja wyszukaj_pacjenta().
/// Jesli zostal znaleziony, aby go wybrac musimy wpisac jego id. Jesli wszystko przebieglo pomyslnie, to mamy mozliwosc wpisania pozostalych danych do wizyty.
/// Dane te zostaja przekazane do listy, a nastepnie do pliku.
/// </summary>
void dodaj_wizyte(Pacjent_dane** pacjent_head, Lekarz_dane** lekarz_head, Wizyta_dane** wizyta_head, enum LekarzPole pole, char* wartosc) {
	if (*lekarz_head == NULL) {
		printf("Lista jest pusta.\n");
		return;
	}
	Pacjent_dane* pacjent_current = *pacjent_head;
	Lekarz_dane* lekarz_current = *lekarz_head;
	bool znaleziono = 0;

	printf("\n _______________________________________________________________________________________________________________ ");
	printf("\n|\t\t\t\t\t\t   ZNALEZIENI LEKARZE\t\t\t\t\t        |");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");
	printf("\n|   ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t| TYTUL\t\t| TEL\t\t| EMAIL\t\t\t|");
	printf("\n|---------------------------------------------------------------------------------------------------------------|");

	while (lekarz_current != NULL) {
		switch (pole) {
		case lID:
			if (lekarz_current->id == atoi(wartosc))
				znaleziono = 1;
			break;
		case NUMER_PWZ:
			if (strcmp(lekarz_current->numer_pwz, wartosc) == 0)
				znaleziono = 1;
			break;
		case TYTUL_ZAWODOWY:
			if (strcmp(lekarz_current->tytul_zawodowy, wartosc) == 0)
				znaleziono = 1;
			break;
		case SPECJALIZACJA:
			if (strcmp(lekarz_current->specjalizacja, wartosc) == 0)
				znaleziono = 1;
			break;
		case lIMIE_I_NAZWISKO:
		{
			char* tempdane[2];
			char* ptr = strtok(wartosc, " ");

			for (int i = 0; ptr != NULL; i++)
			{
				tempdane[i] = ptr;
				ptr = strtok(NULL, " ");
			}

			if (strcmp(lekarz_current->nazwisko, tempdane[1]) == 0 && strcmp(lekarz_current->imie, tempdane[0]) == 0)
				znaleziono = 1;
			break;
		}
		default:
			printf("Nieprawid³owe pole.\n");
			return;
		}


		if (znaleziono) {
			printf("\n|---------------------------------------------------------------------------------------------------------------|");
			printf("\n|\t| %s  \t| %s  \t| %s\t| %s\t| %s\t| %s", lekarz_current->imie, lekarz_current->nazwisko, lekarz_current->pesel,
				lekarz_current->tytul_zawodowy, lekarz_current->telefon, lekarz_current->email);
			printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", lekarz_current->id);
			printf("\n|\t|PWZ: %s\t| SPECJALIZACJA: %s", lekarz_current->numer_pwz, lekarz_current->specjalizacja);
			printf("\n|\t|-------------------------------------------------------------------------------------------------------|");
			printf("\n|\t|GODZINY PRACY:\tPON: %s", lekarz_current->godziny_pracy[0]);
			printf("\n|\t|\t\tWT:  %s", lekarz_current->godziny_pracy[1]);
			printf("\n|\t|\t\tSR:  %s", lekarz_current->godziny_pracy[2]);
			printf("\n|\t|\t\tCZW: %s", lekarz_current->godziny_pracy[3]);
			printf("\n|\t|\t\tPT:  %s", lekarz_current->godziny_pracy[4]);
			printf("\n|\t|\t\tSOB: %s", lekarz_current->godziny_pracy[5]);
			printf("\tNIEDZ: %s", lekarz_current->godziny_pracy[6]);
			printf("\n|---------------------------------------------------------------------------------------------------------------|");

			znaleziono = 0;  // Zresetuj flagê znaleziono
		}

		lekarz_current = lekarz_current->next;
	}

	int tempint = 0;
	int w2 = 0;


	printf("\nPodaj ID lekarza (0 - aby wrocic): ");
	scanf("%d", &tempint);

	if (tempint == 0)
		return;

	lekarz_current = *lekarz_head;
	while (lekarz_current != NULL) {
		if (lekarz_current->id == tempint) {
			system("cls");
			wyswietl_logo();
			printf("\n _______________________________________________________________________________________________________________ ");
			printf("\n|   ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t| TYTUL\t\t| TEL\t\t| EMAIL\t\t\t|");
			printf("\n|---------------------------------------------------------------------------------------------------------------|");
			printf("\n|---------------------------------------------------------------------------------------------------------------|");
			printf("\n|\t| %s  \t| %s  \t| %s\t| %s\t| %s\t| %s", lekarz_current->imie, lekarz_current->nazwisko, lekarz_current->pesel,
				lekarz_current->tytul_zawodowy, lekarz_current->telefon, lekarz_current->email);
			printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", lekarz_current->id);
			printf("\n|\t|PWZ: %s\t| SPECJALIZACJA: %s", lekarz_current->numer_pwz, lekarz_current->specjalizacja);
			printf("\n|\t|-------------------------------------------------------------------------------------------------------|");
			printf("\n|\t|GODZINY PRACY:\tPON: %s", lekarz_current->godziny_pracy[0]);
			printf("\n|\t|\t\tWT:  %s", lekarz_current->godziny_pracy[1]);
			printf("\n|\t|\t\tSR:  %s", lekarz_current->godziny_pracy[2]);
			printf("\n|\t|\t\tCZW: %s", lekarz_current->godziny_pracy[3]);
			printf("\n|\t|\t\tPT:  %s", lekarz_current->godziny_pracy[4]);
			printf("\n|\t|\t\tSOB: %s", lekarz_current->godziny_pracy[5]);
			printf("\tNIEDZ: %s", lekarz_current->godziny_pracy[6]);
			printf("\n|---------------------------------------------------------------------------------------------------------------|");
			break;
		}
		else {
			lekarz_current = lekarz_current->next;
		}

		if (lekarz_current == NULL) {
			return;
		}
	}

	printf("\n\nWyszukaj pacjenta po:");
	printf("\n\t1. ID");
	printf("\n\t2. IMIE I NAZWISKO");
	printf("\n\t3. PESEL");
	printf("\n\t4. EMAIL");
	printf("\n\t5. TELEFON");
	printf("\n\0. Powrot do menu pacjentow\n\n");

	do
	{
		char tempchar[100];
		printf("\tWybierz opcje: ");
		scanf("%d", &w2);

		switch (w2) {
		case 1:
			printf("\n\t\tPodaj ID: ");
			scanf("%s", tempchar);
			wyszukaj_pacjenta(pacjent_head, pID, tempchar, true);

			printf("\nPodaj ID pacjenta (0 - aby wrocic): ");
			scanf("%d", &tempint);

			if (tempint == 0)
				return;

			while (pacjent_current != NULL) {
				if (pacjent_current->id == tempint) {
					printf("\n _______________________________________________________________________________________________________________");
					printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
						pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
						pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
					printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
					printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					break;
				}
				else {
					pacjent_current = pacjent_current->next;
				}

				if (pacjent_current == NULL) {
					return;
				}
			}

			w2 = 0;
			break;
		case 2:
			printf("\n\t\tPodaj IMIE I NAZWISKO: ");
			fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
			fgets(tempchar, sizeof(tempchar), stdin);
			tempchar[strcspn(tempchar, "\n")] = 0;	// usuwanie znaku nowej linii
			wyszukaj_pacjenta(pacjent_head, pIMIE_I_NAZWISKO, tempchar, true);

			printf("\nPodaj ID pacjenta (0 - aby wrocic): ");
			scanf("%d", &tempint);

			if (tempint == 0)
				return;

			while (pacjent_current != NULL) {
				if (pacjent_current->id == tempint) {
					printf("\n _______________________________________________________________________________________________________________");
					printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
						pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
						pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
					printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
					printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					break;
				}
				else {
					pacjent_current = pacjent_current->next;
				}

				if (pacjent_current == NULL) {
					return;
				}
			}

			w2 = 0;
			break;
		case 3:
			printf("\n\t\tPodaj PESEL: ");
			scanf("%s", tempchar);
			wyszukaj_pacjenta(pacjent_head, pPESEL, tempchar, true);

			printf("\nPodaj ID pacjenta (0 - aby wrocic): ");
			scanf("%d", &tempint);

			if (tempint == 0)
				return;

			while (pacjent_current != NULL) {
				if (pacjent_current->id == tempint) {
					printf("\n _______________________________________________________________________________________________________________");
					printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
						pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
						pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
					printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
					printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					break;
				}
				else {
					pacjent_current = pacjent_current->next;
				}

				if (pacjent_current == NULL) {
					return;
				}
			}

			w2 = 0;
			break;
		case 4:
			printf("\n\t\tPodaj EMAIL: ");
			scanf("%s", tempchar);
			wyszukaj_pacjenta(pacjent_head, pEMAIL, tempchar, true);

			printf("\nPodaj ID pacjenta (0 - aby wrocic): ");
			scanf("%d", &tempint);

			if (tempint == 0)
				return;

			while (pacjent_current != NULL) {
				if (pacjent_current->id == tempint) {
					printf("\n _______________________________________________________________________________________________________________");
					printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
						pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
						pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
					printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
					printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					break;
				}
				else {
					pacjent_current = pacjent_current->next;
				}

				if (pacjent_current == NULL) {
					return;
				}
			}

			w2 = 0;
			break;
		case 5:
			printf("\n\t\tPodaj TELEFON: ");
			scanf("%s", tempchar);
			wyszukaj_pacjenta(pacjent_head, pTELEFON, tempchar, true);

			printf("\nPodaj ID pacjenta (0 - aby wrocic): ");
			scanf("%d", &tempint);

			if (tempint == 0)
				return;

			while (pacjent_current != NULL) {
				if (pacjent_current->id == tempint) {
					printf("\n _______________________________________________________________________________________________________________");
					printf("\n| ID\t| IMIE\t\t| NAZWISKO\t| PESEL\t\t|DATA URODZENIA\t| WAGA\t|WZROST\t| TELEFON\t\t|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					printf("\n|\t| %s   \t| %s\t| %s\t| %s\t| %.2f\t| %.1f\t| %s\t\t|", pacjent_current->imie,
						pacjent_current->nazwisko, pacjent_current->pesel, pacjent_current->data_urodzenia,
						pacjent_current->waga, pacjent_current->wzrost, pacjent_current->telefon);
					printf("\n| %d\t|-------------------------------------------------------------------------------------------------------|", pacjent_current->id);
					printf("\n|\t|ODDZIAL NFZ: %s    \t| ADRES: %s    \t|%s\t|", pacjent_current->oddzial_nfz, pacjent_current->adres, pacjent_current->email);
					printf("\n|---------------------------------------------------------------------------------------------------------------|");
					break;
				}
				else {
					pacjent_current = pacjent_current->next;
				}

				if (pacjent_current == NULL) {
					return;
				}
			}

			w2 = 0;
			break;
		default:
			break;
		}
	} while (w2 != 0);


	FILE* plik_wizyty = fopen("wizyty.txt", "a+"); // otwarcie pliku z danymi wizyt
	if (plik_wizyty == NULL) {
		return;
	}

	Wizyta_dane* nowa_wizyta = (Wizyta_dane*)malloc(sizeof(Wizyta_dane));
	if (nowa_wizyta == NULL) {
		return;
	}



	nowa_wizyta->id_pacjenta = pacjent_current->id;
	nowa_wizyta->id_lekarza = lekarz_current->id;
	printf("\nPodaj date wizyty (RRRR-MM-DD): ");
	scanf("%s", &nowa_wizyta->data);
	printf("Podaj godzine wizyty (HH:MM): ");
	scanf("%s", &nowa_wizyta->godzina);
	printf("Podaj czas trwania wizyty (w minutach): ");
	scanf("%d", &nowa_wizyta->czas_trwania);
	printf("Podaj status wizyty (zaplanowana, odbyta, odwolana): ");
	scanf("%s", &nowa_wizyta->status);

	if (*wizyta_head == NULL)
	{
		*wizyta_head = (Wizyta_dane*)malloc(sizeof(Wizyta_dane));
		*wizyta_head = nowa_wizyta;
		(*wizyta_head)->previous = NULL;
		(*wizyta_head)->next = NULL;
	}
	else
	{
		Wizyta_dane* current = *wizyta_head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = (Wizyta_dane*)malloc(sizeof(Wizyta_dane));
		current->next = nowa_wizyta;
		current->next->previous = current;
		current->next->next = NULL;
	}

	// zapis do pliku
	fprintf(plik_wizyty, "%d;%d;%s;%s;%d;%s", nowa_wizyta->id_pacjenta, nowa_wizyta->id_lekarza, nowa_wizyta->data,
		nowa_wizyta->godzina, nowa_wizyta->czas_trwania, nowa_wizyta->status);

	fclose(plik_wizyty); // zamkniecie pliku z danymi wizyt
}

/// <summary>
/// Funkcja znajdujaca szukane wizyty
/// 
/// Funkcja na podstawie argumentow przekazancyh do listy znajduje pasujace wizyty oraz je wyswietla. Nastepnie jest mozliwosc wybrania wizyty za pomoca wpisania id pacjenta,
/// daty wizyty oraz godziny wizyty.
/// Po wybraniu wizyty, jest ona wyswietlana, a nastepnie jest mozliwosc jej edycji. Na koncu zmiany zapisywane sa w pliku.
/// </summary>
void wyszukaj_wizyte(Wizyta_dane** wizyta_head, enum WizytaPole pole, const char* wartosc) {
	if (*wizyta_head == NULL) {
		printf("Lista jest pusta.\n");
		return;
	}

	Wizyta_dane* wizyta_current = *wizyta_head;
	bool znaleziono = 0;

	printf("\n _______________________________________________________________ ");
	printf("\n|\t\t\tZNALEZIONE WIZYTY\t\t\t\t|");
	printf("\n|---------------------------------------------------------------|");
	printf("\n|   ID   |  ID   |     DATA \t| GODZ\t| CZAS \t|    STATUS \t|");
	printf("\n|PACJENTA|LEKARZA|\t\t|\t|TRWANIA|\t\t|");
	printf("\n|---------------------------------------------------------------|");

	while (wizyta_current != NULL) {
		switch (pole) {
		case ID_PACJENTA:
			if (wizyta_current->id_pacjenta == atoi(wartosc))
				znaleziono = 1;
			break;
		case ID_LEKARZA:
			if (wizyta_current->id_lekarza == atoi(wartosc))
				znaleziono = 1;
			break;
		case DATA:
			if (strcmp(wizyta_current->data, wartosc) == 0)
				znaleziono = 1;
			break;
		case GODZINA:
			if (strcmp(wizyta_current->godzina, wartosc) == 0)
				znaleziono = 1;
			break;
		case CZAS_TRWANIA:
			if (wizyta_current->czas_trwania == atoi(wartosc))
				znaleziono = 1;
			break;
		case STATUS:
			if (strcmp(wizyta_current->status, wartosc) == 0)
				znaleziono = 1;
			break;
		default:
			return;
		}


		if (znaleziono) {
			printf("\n|---------------------------------------------------------------|");
			printf("\n|  %d\t |  %d\t |  %s\t| %s\t| %dmin\t| %s\t|", wizyta_current->id_pacjenta, wizyta_current->id_lekarza, wizyta_current->data,
				wizyta_current->godzina, wizyta_current->czas_trwania, wizyta_current->status);
			printf("\n|---------------------------------------------------------------|");

			znaleziono = 0;  // Zresetuj flagê znaleziono
		}

		wizyta_current = wizyta_current->next;
	}

	char tempgodz[6];
	int tempint = 0;
	char tempdata[11];

	printf("\n\nPodaj ID PACJENTA (0 - aby wyjsc): ");
	scanf("%d", &tempint);

	if (tempint == 0)
		return;

	printf("\nPodaj DATE WIZYTY (RRRR-MM-DD): ");
	scanf("%s", tempdata);

	printf("\nPodaj GODZINE WIZYTY (HH:MM): ");
	scanf("%s", tempgodz);

	wizyta_current = *wizyta_head;
	while (wizyta_current != NULL) {
		if (wizyta_current->id_pacjenta == tempint && strcmp(wizyta_current->data, tempdata) == 0 && strcmp(wizyta_current->godzina, tempgodz) == 0) {
			system("cls");
			wyswietl_logo();
			printf("\n ---------------------------------------------------------------");
			printf("\n|   ID   |  ID   |     DATA \t| GODZ\t| CZAS \t|    STATUS \t|");
			printf("\n|PACJENTA|LEKARZA|\t\t|\t|TRWANIA|\t\t|");
			printf("\n|---------------------------------------------------------------|");
			printf("\n|---------------------------------------------------------------|");
			printf("\n|  %d\t |  %d\t |  %s\t| %s\t| %dmin\t| %s\t|", wizyta_current->id_pacjenta, wizyta_current->id_lekarza, wizyta_current->data,
				wizyta_current->godzina, wizyta_current->czas_trwania, wizyta_current->status);
			printf("\n|---------------------------------------------------------------|");
			break;
		}
		else {
			wizyta_current = wizyta_current->next;
		}

		if (wizyta_current == NULL) {
			return;
		}
	}

	int wybor = 0;

	do {
		printf("\nWybierz pole do edycji:\n");
		puts("1. ID Pacjenta");
		puts("2. ID Lekarza");
		puts("3. Data wizyty");
		puts("4. Godzina wizyty");
		puts("5. Czas trawania");
		puts("6. Status");
		puts("0. Wyjdz");
		scanf("%d", &wybor);
		switch (wybor) {
		case 1:
			printf("Podaj nowe ID pacjenta: ");
			scanf("%d", &wizyta_current->id_pacjenta);
			break;
		case 2:
			printf("Podaj nowe ID lekarza: ");
			scanf("%d", &wizyta_current->id_lekarza);
			break;
		case 3:
			printf("Podaj nowa date wizyty (RRRR-MM-DD): ");
			scanf("%s", &wizyta_current->data);
			break;
		case 4:
			printf("Podaj nowa godzine wizyty (HH:MM): ");
			scanf("%s", &wizyta_current->godzina);
			break;
		case 5:
			printf("Podaj nowy czas trwania wizyty (w minutach): ");
			scanf("%d", &wizyta_current->czas_trwania);
			break;
		case 6:
			printf("Podaj nowy status (zaplanowana, odbyta, odwolana): ");
			scanf("%s", &wizyta_current->status);
			break;
		default:
			break;
		}
	} while (wybor != 0);

	FILE* plik_wizyty = fopen("wizyty.txt", "w"); // otwarcie pliku z danymi pacjentow
	if (plik_wizyty == NULL) {
		return;
	}

	wizyta_current = *wizyta_head;
	while (wizyta_current != NULL) {
		fprintf(plik_wizyty, "%d;%d;%s;%s;%d;%s\n", wizyta_current->id_pacjenta, wizyta_current->id_lekarza, wizyta_current->data,
			wizyta_current->godzina, wizyta_current->czas_trwania, wizyta_current->status);
		wizyta_current = wizyta_current->next;
	}

	fclose(plik_wizyty); // zamkniecie pliku z danymi pacjentow
}

/// <summary>
/// Wyswietla menu pacjentow
/// 
/// Funkcja wyswietla logo oraz menu pacjentow. Dostepne akcje to: dodanie pacjenta, wyswietlenie listy pacjentow, wyszukanie pacjenta oraz powrot. Jesli wybrane zostalo wyswietlenie
/// listy pacjentow, do wyboru jest pole wedlug, ktorego ma byc przeprowadzone sortowanieo oraz kierunke sortowania. 
/// Jesli wyborem bylo wyszukanie pacjenta, to musimy wybrac pole wedlug, ktorego mamy wyszukac pacjenta. Funkcja konczy sie wraz z wybraniem cyfry odpowiedzialnej za powrot.
/// </summary>
void pacjenci(Pacjent_dane** pacjent_head) {
	int w, w2;
	do
	{
		system("cls");
		wyswietl_logo();
		printf("\n\n1. Dodaj pacjenta");
		printf("\n2. Wyswietl liste pacjentow");
		printf("\n3. Wyszukaj pacjenta");
		printf("\n0. Powrot do menu glownego\n\n");

		printf("Wybierz opcje: ");
		scanf("%d", &w);

		switch (w) {
		case 1:
			dodaj_pacjenta(pacjent_head);
			break;
		case 2:
			printf("\n\tSortuj wedlug:");
			printf("\n\t1. ID");
			printf("\n\t2. IMIE");
			printf("\n\t3. NAZWISKO");
			printf("\n\t4. PESEL");
			printf("\n\t5. DATA URODZENIA");
			printf("\n\t6. ADRES");
			printf("\n\t7. EMAIL");
			printf("\n\t8. TELEFON");
			printf("\n\t9. WAGA");
			printf("\n\t10. WZROST");
			printf("\n\t11. ODDZIAL NFZ");
			printf("\n\0. Powrot do menu pacjentow\n\n");

			do
			{
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tID:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, pID);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, pID);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 2:
					printf("\n\t\tIMIE:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, pIMIE);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, pIMIE);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 3:
					printf("\n\t\tNAZWISKO:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, pNAZWISKO);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, pNAZWISKO);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 4:
					printf("\n\t\tPESEL:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, pPESEL);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, pPESEL);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 5:
					printf("\n\t\tDATA URODZENIA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, DATA_URODZENIA);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, DATA_URODZENIA);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 6:
					printf("\n\t\tADRES:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, ADRES);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, ADRES);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 7:
					printf("\n\t\tEMAIL:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, pEMAIL);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, pEMAIL);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 8:
					printf("\n\t\tTELEFON:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, pTELEFON);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, pTELEFON);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 9:
					printf("\n\t\tWAGA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, WAGA);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, WAGA);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 10:
					printf("\n\t\tWZROST:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, WZROST);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, WZROST);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				case 11:
					printf("\n\t\tODDZIAL NFZ:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						pacjent_sortuj(pacjent_head, DESC, ODDZIAL_NFZ);
					else if (w2 == 1)
						pacjent_sortuj(pacjent_head, ASC, ODDZIAL_NFZ);
					wyswietl_liste_pacjentow(pacjent_head);
					pacjent_sortuj(pacjent_head, ASC, pID);
					w2 = 0;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		case 3:
			printf("\nWyszukaj po:");
			printf("\n\t1. ID");
			printf("\n\t2. IMIE");
			printf("\n\t3. NAZWISKO");
			printf("\n\t4. IMIE I NAZWISKO");
			printf("\n\t5. PESEL");
			printf("\n\t6. DATA URODZENIA");
			printf("\n\t7. ADRES");
			printf("\n\t8. EMAIL");
			printf("\n\t9. TELEFON");
			printf("\n\t10. WAGA");
			printf("\n\t11. WZROST");
			printf("\n\t12. ODDZIAL NFZ");
			printf("\n\0. Powrot do menu pacjentow\n\n");

			do
			{
				char tempchar[100];
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tPodaj ID: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, pID, tempchar, false);

					w2 = 0;
					break;
				case 2:
					printf("\n\t\tPodaj IMIE: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, pIMIE, tempchar, false);

					w2 = 0;
					break;
				case 3:
					printf("\n\t\tPodaj NAZWISKO: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, pNAZWISKO, tempchar, false);

					w2 = 0;
					break;
				case 4:
					printf("\n\t\tPodaj IMIE I NAZWISKO: ");
					fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
					fgets(tempchar, sizeof(tempchar), stdin);
					tempchar[strcspn(tempchar, "\n")] = 0;	// usuwanie znaku nowej linii
					wyszukaj_pacjenta(pacjent_head, pIMIE_I_NAZWISKO, tempchar, false);

					w2 = 0;
					break;
				case 5:
					printf("\n\t\tPodaj PESEL: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, pPESEL, tempchar, false);

					w2 = 0;
					break;
				case 6:
					printf("\n\t\tPodaj DATE URODZENIA: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, DATA_URODZENIA, tempchar, false);
					w2 = 0;
					break;
				case 7:
					printf("\n\t\tPodaj ADRES: ");
					fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
					fgets(tempchar, sizeof(tempchar), stdin);
					tempchar[strcspn(tempchar, "\n")] = 0;	// usuwanie znaku nowej linii
					wyszukaj_pacjenta(pacjent_head, ADRES, tempchar, false);
					w2 = 0;
					break;
				case 8:
					printf("\n\t\tPodaj EMAIL: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, pEMAIL, tempchar, false);
					w2 = 0;
					break;
				case 9:
					printf("\n\t\tPodaj TELEFON: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, pTELEFON, tempchar, false);
					w2 = 0;
					break;
				case 10:
					printf("\n\t\tPodaj WAGE: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, WAGA, tempchar, false);
					w2 = 0;
					break;
				case 11:
					printf("\n\t\tPodaj WZROST: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, WZROST, tempchar, false);
					w2 = 0;
					break;
				case 12:
					printf("\n\t\tPodaj ODDZIAL NFZ: ");
					scanf("%s", tempchar);
					wyszukaj_pacjenta(pacjent_head, ODDZIAL_NFZ, tempchar, false);
					w2 = 0;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		default:
			break;
		}
	} while (w != 0);
}

/// <summary>
/// Wyswietla menu lekarzy
/// 
/// Funkcja wyswietla logo oraz menu lekarzy. Dostepne akcje to: dodanie lekarza, wyswietlenie listy lekarzy, wyszukanie lekarza oraz powrot. Jesli wybrane zostalo wyswietlenie
/// listy lekarzy, do wyboru jest pole wedlug, ktorego ma byc przeprowadzone sortowanie oraz kierunek sortowania. 
/// Jesli wyborem bylo wyszukanie lekarza, to musimy wybrac pole wedlug, ktorego mamy go wyszukac. Funkcja konczy sie wraz z wybraniem cyfry odpowiedzialnej za powrot.
/// </summary>
void lekarze(Lekarz_dane** lekarz_head) {
	int w, w2;
	do
	{
		system("cls");
		wyswietl_logo();
		printf("\n\n1. Dodaj lekarza");
		printf("\n2. Wyswietl liste lekarzy");
		printf("\n3. Wyszukaj lekarza");
		printf("\n0. Powrot do menu glownego\n\n\n");

		printf("Wybierz opcje: ");
		scanf("%d", &w);

		switch (w) {
		case 1:
			dodaj_lekarza(lekarz_head);
			break;
		case 2:
			printf("\n\tSortuj wedlug:");
			printf("\n\t1. ID");
			printf("\n\t2. IMIE");
			printf("\n\t3. NAZWISKO");
			printf("\n\t4. PESEL");
			printf("\n\t5. NUMER PWZ");
			printf("\n\t6. TYTUL_ZAWODOWY");
			printf("\n\t7. SPECJALIZACJA");
			printf("\n\t8. EMAIL");
			printf("\n\t9. TELEFON");
			printf("\n\t0. Powrot do menu lekarzy\n\n");

			do
			{
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tID:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, lID);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, lID);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				case 2:
					printf("\n\t\tIMIE:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, lIMIE);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, lIMIE);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				case 3:
					printf("\n\t\tNAZWISKO:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, lNAZWISKO);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, lNAZWISKO);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				case 4:
					printf("\n\t\tPESEL:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, lPESEL);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, lPESEL);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				case 5:
					printf("\n\t\tNUMER PWZ:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, NUMER_PWZ);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, NUMER_PWZ);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				case 6:
					printf("\n\t\tTYTUL ZAWODOWY:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, TYTUL_ZAWODOWY);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, TYTUL_ZAWODOWY);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;;
					break;
				case 7:
					printf("\n\t\tSPECJALIZACJA:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, SPECJALIZACJA);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, SPECJALIZACJA);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				case 8:
					printf("\n\t\tEMAIL:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, lEMAIL);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, lEMAIL);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);;
					w2 = 0;
					break;
				case 9:
					printf("\n\t\tTELEFON:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						lekarz_sortuj(lekarz_head, DESC, lTELEFON);
					else if (w2 == 1)
						lekarz_sortuj(lekarz_head, ASC, lTELEFON);
					wyswietl_liste_lekarzy(lekarz_head);
					lekarz_sortuj(lekarz_head, ASC, lID);
					w2 = 0;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		case 3:
			printf("\nWyszukaj po:");
			printf("\n\t1. ID");
			printf("\n\t2. IMIE");
			printf("\n\t3. NAZWISKO");
			printf("\n\t4. IMIE I NAZWISKO");
			printf("\n\t5. PESEL");
			printf("\n\t6. NUMER PWZ");
			printf("\n\t7. TYTUL_ZAWODOWY");
			printf("\n\t8. SPECJALIZACJA");
			printf("\n\t9. EMAIL");
			printf("\n\t10. TELEFON");
			printf("\n\0. Powrot do menu lekarzy\n\n");

			do
			{
				char tempchar[100];
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tPodaj ID: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, lID, tempchar);

					w2 = 0;
					break;
				case 2:
					printf("\n\t\tPodaj IMIE: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, lIMIE, tempchar);

					w2 = 0;
					break;
				case 3:
					printf("\n\t\tPodaj NAZWISKO: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, lNAZWISKO, tempchar);

					w2 = 0;
					break;
				case 4:
					printf("\n\t\tPodaj IMIE I NAZWISKO: ");
					fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
					fgets(tempchar, sizeof(tempchar), stdin);
					tempchar[strcspn(tempchar, "\n")] = 0;	// usuwanie znaku nowej linii
					wyszukaj_lekarza(lekarz_head, lIMIE_I_NAZWISKO, tempchar);

					w2 = 0;
					break;
				case 5:
					printf("\n\t\tPodaj PESEL: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, lPESEL, tempchar);

					w2 = 0;
					break;
				case 6:
					printf("\n\t\tPodaj NUMER PWZ: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, NUMER_PWZ, tempchar);

					w2 = 0;
					break;
				case 7:
					printf("\n\t\tPodaj TYTUL_ZAWODOWY: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, TYTUL_ZAWODOWY, tempchar);

					w2 = 0;
					break;
				case 8:
					printf("\n\t\tPodaj SPECJALIZACJA: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, SPECJALIZACJA, tempchar);

					w2 = 0;
					break;
				case 9:
					printf("\n\t\tPodaj EMAIL: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, lEMAIL, tempchar);

					w2 = 0;
					break;
				case 10:
					printf("\n\t\tPodaj TELEFON: ");
					scanf("%s", tempchar);
					wyszukaj_lekarza(lekarz_head, lTELEFON, tempchar);

					w2 = 0;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		default:
			break;
		}
	} while (w != 0);
}

/// <summary>
/// Wyswietla menu wizyt
/// 
/// Funkcja wyswietla logo oraz menu wizyt. Dostepne akcje to: dodanie wizyty, wyswietlenie listy wizyt, wyszukanie wizyty oraz powrot. Jesli wybrane zostalo wyswietlenie
/// wizyt, do wyboru jest pole wedlug, ktorego ma byc przeprowadzone sortowanie oraz kierunek sortowania. 
/// Jesli wyborem bylo wyszukanie wizyty, to musimy wybrac pole wedlug, ktorego mamy jej wyszukac. Funkcja konczy sie wraz z wybraniem cyfry odpowiedzialnej za powrot.
/// </summary>
void wizyty(Pacjent_dane** pacjent_head, Lekarz_dane** lekarz_head, Wizyta_dane** wizyta_head)
{
	int w, w2;
	do
	{
		system("cls");
		wyswietl_logo();
		printf("\n\n1. Dodaj wizyte");
		printf("\n2. Wyswietl wszystkie wizyty");
		printf("\n3. Wyszukaj wizyte");
		printf("\n0. Powrot do menu glownego\n\n\n");

		printf("Wybierz opcje: ");
		scanf("%d", &w);

		switch (w) {
		case 1:
			printf("\nZnajdz lekarza po:");
			printf("\n\t1. ID");
			printf("\n\t2. IMIE I NAZWISKO");
			printf("\n\t3. NUMER PWZ");
			printf("\n\t4. TYTUL_ZAWODOWY");
			printf("\n\t5. SPECJALIZACJA");
			printf("\n\0. Powrot do menu lekarzy\n\n");

			do
			{
				char tempchar[100];
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tPodaj ID: ");
					scanf("%s", tempchar);
					dodaj_wizyte(pacjent_head, lekarz_head, wizyta_head, lID, tempchar);

					w2 = 0;
					break;
				case 2:
					printf("\n\t\tPodaj IMIE I NAZWISKO: ");
					fseek(stdin, 0, SEEK_END);			// czyszczenie bufora
					fgets(tempchar, sizeof(tempchar), stdin);
					tempchar[strcspn(tempchar, "\n")] = 0;	// usuwanie znaku nowej linii
					dodaj_wizyte(pacjent_head, lekarz_head, wizyta_head, lIMIE_I_NAZWISKO, tempchar);

					w2 = 0;
					break;
				case 3:
					printf("\n\t\tPodaj NUMER PWZ: ");
					scanf("%s", tempchar);
					dodaj_wizyte(pacjent_head, lekarz_head, wizyta_head, NUMER_PWZ, tempchar);

					w2 = 0;
					break;
				case 4:
					printf("\n\t\tPodaj TYTUL_ZAWODOWY: ");
					scanf("%s", tempchar);
					dodaj_wizyte(pacjent_head, lekarz_head, wizyta_head, TYTUL_ZAWODOWY, tempchar);

					w2 = 0;
					break;
				case 5:
					printf("\n\t\tPodaj SPECJALIZACJA: ");
					scanf("%s", tempchar);
					dodaj_wizyte(pacjent_head, lekarz_head, wizyta_head, SPECJALIZACJA, tempchar);

					w2 = 0;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		case 2:
			printf("\n\tSortuj wedlug:");
			printf("\n\t1. ID PACJENTA");
			printf("\n\t2. ID LEKARZA");
			printf("\n\t3. DATA WIZYTY");
			printf("\n\t4. GODZINA WIZYTY");
			printf("\n\t5. CZAS TRWANIA WIZYTY");
			printf("\n\t6. STATUS");
			printf("\n\t0. Powrot do menu lekarzy\n\n");

			do
			{
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tID PACJENTA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						wizyta_sortuj(wizyta_head, DESC, ID_PACJENTA);
					else if (w2 == 1)
						wizyta_sortuj(wizyta_head, ASC, ID_PACJENTA);
					wyswietl_wizyty(wizyta_head);
					wizyta_sortuj(wizyta_head, ASC, DATA);
					w2 = 0;
					break;
				case 2:
					printf("\n\t\tID LEKARZA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						wizyta_sortuj(wizyta_head, DESC, ID_LEKARZA);
					else if (w2 == 1)
						wizyta_sortuj(wizyta_head, ASC, ID_LEKARZA);
					wyswietl_wizyty(wizyta_head);
					wizyta_sortuj(wizyta_head, ASC, DATA);
					w2 = 0;
					break;
				case 3:
					printf("\n\t\tDATA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						wizyta_sortuj(wizyta_head, DESC, DATA);
					else if (w2 == 1)
						wizyta_sortuj(wizyta_head, ASC, DATA);
					wyswietl_wizyty(wizyta_head);
					wizyta_sortuj(wizyta_head, ASC, DATA);
					w2 = 0;
					break;
				case 4:
					printf("\n\t\tGODZINA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						wizyta_sortuj(wizyta_head, DESC, GODZINA);
					else if (w2 == 1)
						wizyta_sortuj(wizyta_head, ASC, GODZINA);
					wyswietl_wizyty(wizyta_head);
					wizyta_sortuj(wizyta_head, ASC, DATA);
					w2 = 0;
					break;
				case 5:
					printf("\n\t\tCZAS TRWANIA:");
					printf("\n\t\t1. Rosnaco");
					printf("\n\t\t2. Malejaco");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						wizyta_sortuj(wizyta_head, DESC, CZAS_TRWANIA);
					else if (w2 == 1)
						wizyta_sortuj(wizyta_head, ASC, CZAS_TRWANIA);
					wyswietl_wizyty(wizyta_head);
					wizyta_sortuj(wizyta_head, ASC, DATA);
					w2 = 0;
					break;
				case 6:
					printf("\n\t\tSTATUS:");
					printf("\n\t\t1. A->Z");
					printf("\n\t\t2. Z->A");
					printf("\n\n\t\tWybierz opcje: ");
					do
					{
						scanf("%d", &w2);
					} while (w2 != 1 && w2 != 2);
					if (w2 == 2)
						wizyta_sortuj(wizyta_head, DESC, STATUS);
					else if (w2 == 1)
						wizyta_sortuj(wizyta_head, ASC, STATUS);
					wyswietl_wizyty(wizyta_head);
					wizyta_sortuj(wizyta_head, ASC, DATA);
					w2 = 0;;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		case 3:
			printf("\nWyszukaj po:");
			printf("\n\t1. ID PACJENTA");
			printf("\n\t2. ID LEKARZA");
			printf("\n\t3. DATA WIZYTY");
			printf("\n\t4. GODZINA WIZYTY");
			printf("\n\t5. CZAS TRWANIA WIZYTY");
			printf("\n\t6. STATUS");
			printf("\n\0. Powrot do menu pacjentow\n\n");

			do
			{
				char tempchar[100];
				printf("\tWybierz opcje: ");
				scanf("%d", &w2);

				switch (w2) {
				case 1:
					printf("\n\t\tPodaj ID PACJENTA: ");
					scanf("%s", tempchar);
					wyszukaj_wizyte(wizyta_head, ID_PACJENTA, tempchar);

					w2 = 0;
					break;
				case 2:
					printf("\n\t\tPodaj ID LEKARZA: ");
					scanf("%s", tempchar);
					wyszukaj_wizyte(wizyta_head, ID_LEKARZA, tempchar);

					w2 = 0;
					break;
				case 3:
					printf("\n\t\tPodaj DATE WIZYTY (RRRR-MM-DD): ");
					scanf("%s", tempchar);
					wyszukaj_wizyte(wizyta_head, DATA, tempchar);

					w2 = 0;
					break;
				case 4:
					printf("\n\t\tPodaj GODZINE WIZYTY (HH:MM): ");
					scanf("%s", tempchar);
					wyszukaj_wizyte(wizyta_head, GODZINA, tempchar);

					w2 = 0;
					break;
				case 5:
					printf("\n\t\tPodaj CZAS TRWANIA WIZYTY (w minutach): ");
					scanf("%s", tempchar);
					wyszukaj_wizyte(wizyta_head, CZAS_TRWANIA, tempchar);

					w2 = 0;
					break;
				case 6:
					printf("\n\t\tPodaj STATUS WIZYTY: ");
					scanf("%s", tempchar);
					wyszukaj_wizyte(wizyta_head, STATUS, tempchar);
					w2 = 0;
					break;
				default:
					break;
				}
			} while (w2 != 0);
			break;
		default:
			break;
		}
	} while (w != 0);
}

/// <summary>
/// Glowna funkcja programu
/// 
/// Na poczatku deklarowane sa 3 wskazniki na poczatek kazdej z list. Nastepnie deklarowane sa zmienne oblugujace wybor akcji uzytkownika oraz obsluge bledu podczas wyboru.
/// Dalej mamy petle do...while, w ktorej mamy do wyboru wyswietlenie menu: pacjentow, lekarzy, wizyt, oraz zakonczenie dzialania programu.
/// Na koncu zwalniana jest pamiec, w ktorej przechowywane byly listy.
/// </summary>
int main() {
	Pacjent_dane* pacjent_head; // wskaznik na poczatek listy pacjentow
	pacjent_head = (Pacjent_dane*)malloc(sizeof(Pacjent_dane));
	pacjent_head = NULL;

	Lekarz_dane* lekarz_head; // wskaznik na poczatek listy lekarzy
	lekarz_head = (Lekarz_dane*)malloc(sizeof(Lekarz_dane));
	lekarz_head = NULL;

	Wizyta_dane* wizyta_head; // wskaznik na poczatek listy wizyt
	wizyta_head = (Wizyta_dane*)malloc(sizeof(Wizyta_dane));
	wizyta_head = NULL;

	int w; // zmienna do wyboru opcji w menu
	bool blad = false; // zmienna do obslugi zlego wyboru opcji w menu

	wczytaj_dane(&pacjent_head, &lekarz_head, &wizyta_head); // wczytanie danych z plikow do list

	do
	{
		system("cls"); // czyszczenie ekranu
		wyswietl_logo(); // wyswietlenie logo
		printf("\n\n1. Pacjenci");
		printf("\n2. Lekarze");
		printf("\n3. Wizyty");
		if (blad)
		{
			printf("\n0. Zamknij program\n\nNie ma takiej opcji!\n");
			blad = false;
		}
		else
			printf("\n0. Zamknij program\n\n\n");

		printf("Wybierz opcje: ");
		scanf("%d", &w);

		switch (w) {
		case 1:
			pacjenci(&pacjent_head); // wywolanie funkcji wyswietlajacej menu pacjentow
			break;
		case 2:
			lekarze(&lekarz_head); // wywolanie funkcji wyswietlajacej menu lekarzy
			break;
		case 3:
			wizyty(&pacjent_head, &lekarz_head, &wizyta_head); // wywolanie funkcji wyswietlajacej menu wizyt
			break;
		case 0:
			system("cls"); // czyszczenie ekranu
			printf("Zamykanie programu...\n");
			break;
		default:
			blad = true;
			break;
		}
	} while (w != 0);

	free(pacjent_head);
	free(lekarz_head);
	free(wizyta_head);

	return 0;
}