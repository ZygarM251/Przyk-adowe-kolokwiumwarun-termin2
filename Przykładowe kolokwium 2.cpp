
#include <iostream>
#include <cstring>
#include <ctime>


#define _CRT_SECURE_NO_WARNINGS

using namespace std;

class Klient {
public:
	char nazwa[256];

	Klient(const char* m_nazwa) {
		setNazwa(m_nazwa);
	}

	const char* getNazwa() const { return nazwa; }

	void setNazwa(const char* m_nazwa) {
		strncpy_s(this->nazwa, m_nazwa, 256);
	}
	
};

class KontoBankowe {
private:
	char numerKonta[27] = {};
	double stanKonta;
	bool aktywne;
	Klient klient;
	static int liczbaAktywnychKont;

public:
	void setNumerKonta(const char* m_numerKonta) {
		if (strlen(m_numerKonta) != 26) {
			throw invalid_argument("Numer konta musi mieć 26 cyfr");
		}
		strncpy_s(this->numerKonta, m_numerKonta, 26);
	}

	const char* getNumerKonta() const { return numerKonta; }

	void setStanKonta(double m_stanKonta) {
		if (m_stanKonta < 0) {
			throw invalid_argument("Stan konta nie może być ujemny");
		}
		this->stanKonta = m_stanKonta;
	}

	double getStanKonta() const { return stanKonta; }

	void setAktywne(bool m_aktywne) {
		this->aktywne = m_aktywne;
	}

	bool isAktywne() const { return aktywne; }

	void setWlasciciel(const Klient& m_klient) {
		this->klient = m_klient;
	}

	Klient getWlasciciel() const { return klient; }

	static int getLiczbeAktywnychKont() { return liczbaAktywnychKont; }

	virtual double prognozaStanuKontaZaRok() const { return stanKonta; }

	KontoBankowe(const char* m_numerKonta, double m_stanKonta, bool m_aktywne, const Klient& m_klient)
		: klient(m_klient) {
		setNumerKonta(m_numerKonta);
		setStanKonta(m_stanKonta);
		setAktywne(m_aktywne);
		if (aktywne) {
			liczbaAktywnychKont++;
		}
	}

	~KontoBankowe() {
		if (aktywne) {
			liczbaAktywnychKont--;
		}
	}

	operator double() { return stanKonta; }

	KontoBankowe& operator+=(double kwota) {
		if (aktywne) {
			stanKonta += kwota;
		}
		return *this;
	}

	friend ostream& operator<<(ostream& os, KontoBankowe& konto) {
		os << konto.numerKonta << " ";
		if (konto.aktywne) {
			os << konto.klient.getNazwa() << " : " << konto.stanKonta;
		}
		else {
			os << "NIEAKTYWNE";
		}
		return os;
	}

	KontoBankowe(const Klient& m_klient) :klient(m_klient)
	{

		for (int i = 0; i < 26; i++) 
		{
			numerKonta[i] = (rand() % 10)+48; 
		}
		numerKonta[26] = '\0';
		double m_stanKonta = 0.0;

		setStanKonta(m_stanKonta);
		setAktywne(false);
	}


};

int KontoBankowe::liczbaAktywnychKont = 0;

class KontoOszczednosciowe : public KontoBankowe {
private:
	double stopaOprocentowania;

public:
	KontoOszczednosciowe(const char* numerKonta, double stanKonta, bool aktywne, const Klient& wlasciciel, double stopaOprocentowania)
		: KontoBankowe(numerKonta, stanKonta, aktywne, wlasciciel) {
		if (stopaOprocentowania < 0) {
			cerr << "Błąd";
		}
		this->stopaOprocentowania = stopaOprocentowania;
	}

	double getStopaOprocentowania() const { return stopaOprocentowania; }
	void setStopaOprocentowania(double stopaOprocentowania) {
		if (stopaOprocentowania < 0) {
			cerr << "Błąd";
		}
		this->stopaOprocentowania = stopaOprocentowania;
	}

	double prognozaStanuKontaZaRok() const override {
		return getStanKonta() * (1 + stopaOprocentowania / 100);
	}
};

int main() {
	srand(time(0));

	Klient klient1("Jan Kowalski");
	Klient klient2("Maria Nowak");

	KontoBankowe konto1("12345678901234567890123456", 1000.0, true, klient1);
	KontoBankowe konto2("09876543210987654321098765", 500.0, false, klient2);

	KontoOszczednosciowe konto3("11111111111111111111111111", 2000.0, true, klient1, 0.05);

	cout << "Liczba aktywnych kont: " << KontoBankowe::getLiczbeAktywnychKont() << endl;

	konto1 += 500.0;
	konto2.setAktywne(true);

	cout << konto1 << endl;
	cout << konto2 << endl;
	cout << konto3 << endl;

	cout << "Prognoza stanu konta konto1 za rok: " << konto1 << endl;
	cout << "Prognoza stanu konta konto3 za rok: " << konto3 << endl;

	Klient klient3("Piotr Kowalski");
	KontoBankowe konto4(klient3);

	cout << konto4 << endl;
}
