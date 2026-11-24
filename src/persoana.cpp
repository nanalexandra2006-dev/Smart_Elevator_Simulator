#include "persoana.h"

Persoana::Persoana() : etajCurent(0), etajDestinatie(0), greutate(70) {}

Persoana::Persoana(int etajCurent, int etajDestinatie, int greutate) {
    std::cout << "Persoana constructor parametrizabil" << std::endl;
    this->etajCurent = etajCurent;
    this->etajDestinatie = etajDestinatie;
    this->greutate = greutate;
}

Persoana::Persoana(const Persoana& p) {
    std::cout << "Persoana copy constructor" << std::endl;
    this->etajCurent = p.etajCurent;
    this->etajDestinatie = p.etajDestinatie;
    this->greutate = p.greutate;
}

Persoana& Persoana::operator=(const Persoana& p) {
    std::cout << "Persoana assignment operator" << std::endl;
    if (this != &p) {
        this->etajCurent = p.etajCurent;
        this->etajDestinatie = p.etajDestinatie;
        this->greutate = p.greutate;
    }
    return *this;
}

Persoana::~Persoana() = default;

std::ostream& operator<<(std::ostream& os, const Persoana& p) {
    os << "Persoana [Etaj curent: " << p.etajCurent 
       << " -> Destinatie: " << p.etajDestinatie 
       << " (" << p.greutate << " kg)]";
    return os;
}

std::istream& operator>>(std::istream& is, Persoana& p) {
    std::cout << "Etaj curent: ";
    is >> p.etajCurent;
    std::cout << "Etaj destinatie: ";
    is >> p.etajDestinatie;
    std::cout << "Greutate: ";
    is >> p.greutate;
    return is;
}

void Persoana::setEtajCurent(int etajCurent) { this->etajCurent = etajCurent; }
void Persoana::setEtajDestinatie(int etajDestinatie) { this->etajDestinatie = etajDestinatie; }
void Persoana::setGreutate(int greutate) { this->greutate = greutate; }

int Persoana::getEtajCurent() const { return etajCurent; }
int Persoana::getEtajDestinatie() const { return etajDestinatie; }
int Persoana::getGreutate() const { return greutate; }

void Persoana::print() const {
    std::cout << "Persoana pleaca de la etajul " << etajCurent 
              << " spre etajul " << etajDestinatie 
              << " (" << greutate << " kg)" << std::endl;
}