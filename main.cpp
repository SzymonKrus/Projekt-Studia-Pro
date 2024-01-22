#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;


struct Student {
    string imie;
    string nazwisko;
    vector<double> oceny;
};


double obliczSrednia(const vector<double>& oceny) {
    if (oceny.empty()) {
        return 0.0;
    }

    double suma = 0.0;
    for (double ocena : oceny) {
        suma += ocena;
    }

    return suma / oceny.size();
}


double obliczSredniaWszystkich(const vector<Student>& studenci) {
    if (studenci.empty()) {
        return 0.0;
    }

    double sumaWszystkich = 0.0;
    for (const auto& student : studenci) {
        sumaWszystkich += obliczSrednia(student.oceny);
    }

    return sumaWszystkich / studenci.size();
}


int liczbaStudentowPowyzejSredniej(const vector<Student>& studenci, double sredniaWszystkich) {
    int iloscStudentow = 0;

    for (const auto& student : studenci) {
        if (obliczSrednia(student.oceny) > sredniaWszystkich) {
            iloscStudentow++;
        }
    }

    return iloscStudentow;
}


void wyswietlDaneStudentow(const vector<Student>& studenci) {
    cout << "Dane studentow:" << endl;
    for (const auto& student : studenci) {
        cout << "Imie: " << student.imie << ", Nazwisko: " << student.nazwisko << ", Srednia: " << obliczSrednia(student.oceny) << endl;
    }
}


void zapiszNajwyzszaSredniaDoPliku(const vector<Student>& studenci, const string& nazwaPliku) {
    double maxSrednia = 0.0;
    const Student* studentZNajwyzszaSrednia = nullptr;

    for (const auto& student : studenci) {
        double srednia = obliczSrednia(student.oceny);
        if (srednia > maxSrednia) {
            maxSrednia = srednia;
            studentZNajwyzszaSrednia = &student;
        }
    }

    if (studentZNajwyzszaSrednia != nullptr) {
        ofstream plik(nazwaPliku);
        if (plik.is_open()) {
            plik << "Imie: " << studentZNajwyzszaSrednia->imie << ", Nazwisko: " << studentZNajwyzszaSrednia->nazwisko
                 << ", Srednia: " << obliczSrednia(studentZNajwyzszaSrednia->oceny) << endl;
            plik.close();
            cout << "Dane studenta o najwyzszej sredniej zostaly zapisane do pliku." << endl;
        } else {
            cerr << "Nie udalo sie otworzyc pliku do zapisu." << endl;
        }
    }
}


void wyszukajDaneStudenta(const vector<Student>& studenci, const string& imie, const string& nazwisko) {
    string imieLowerCase = imie;
    string nazwiskoLowerCase = nazwisko;

    // Zamień imię i nazwisko na małe litery
    transform(imieLowerCase.begin(), imieLowerCase.end(), imieLowerCase.begin(), ::tolower);
    transform(nazwiskoLowerCase.begin(), nazwiskoLowerCase.end(), nazwiskoLowerCase.begin(), ::tolower);

    for (const auto& student : studenci) {
        string studentImieLowerCase = student.imie;
        string studentNazwiskoLowerCase = student.nazwisko;

        // Zamień imię i nazwisko studenta na małe litery
        transform(studentImieLowerCase.begin(), studentImieLowerCase.end(), studentImieLowerCase.begin(), ::tolower);
        transform(studentNazwiskoLowerCase.begin(), studentNazwiskoLowerCase.end(), studentNazwiskoLowerCase.begin(), ::tolower);

        if (studentImieLowerCase == imieLowerCase && studentNazwiskoLowerCase == nazwiskoLowerCase) {
            cout << "Dane studenta " << imie << " " << nazwisko << ":" << endl;
            cout << "Imie: " << student.imie << ", Nazwisko: " << student.nazwisko << ", Srednia: " << obliczSrednia(student.oceny) << endl;
            return;
        }
    }

    cout << "Nie znaleziono studenta o podanym imieniu i nazwisku." << endl;
}



int main() {
    const string nazwaPliku = "dane.txt";

    vector<Student> studenci;

    ifstream plik(nazwaPliku);

    if (!plik.is_open()) {
        cerr << "Nie udalo sie otworzyc pliku: " << nazwaPliku << endl;
        return 1;
    }

    string linia;
    while (getline(plik, linia)) {
        istringstream strumien(linia);
        Student student;
        strumien >> student.imie >> student.nazwisko;

        double ocena;
        while (strumien >> ocena) {
            student.oceny.push_back(ocena);
        }

        studenci.push_back(student);
    }

    plik.close();

    for (size_t i = 0; i < studenci.size(); ++i) {
        double srednia = obliczSrednia(studenci[i].oceny);
        cout << "Srednia ocen studenta " << studenci[i].imie << " " << studenci[i].nazwisko << ": " << srednia << endl;
    }

    double sredniaWszystkich = obliczSredniaWszystkich(studenci);

    cout << "Srednia ocen wszystkich studentow: " << sredniaWszystkich << endl;

    int iloscStudentowPowyzejSredniej = liczbaStudentowPowyzejSredniej(studenci, sredniaWszystkich);
    cout << "Liczba studentow posiadajacych srednia ocen powyzej sredniej wszystkich studentow: "
              << iloscStudentowPowyzejSredniej << endl;

    wyswietlDaneStudentow(studenci);

    zapiszNajwyzszaSredniaDoPliku(studenci, "najwyzsza_srednia.txt");

    string imieDoWyszukania, nazwiskoDoWyszukania;
    cout << "Podaj imie studenta do wyszukania: ";
    cin >> imieDoWyszukania;
    cout << "Podaj nazwisko studenta do wyszukania: ";
    cin >> nazwiskoDoWyszukania;

    wyszukajDaneStudenta(studenci, imieDoWyszukania, nazwiskoDoWyszukania);

    return 0;
}
