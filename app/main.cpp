#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class persoana {
private:
    int etajCurent;
    int etajDestinatie;
    int greutate;
    bool vip;

public:
    persoana() : etajCurent(0), etajDestinatie(1), greutate(70), vip(false) {}
    
    persoana(int etajC, int etajD, int g, bool isVip) 
        : etajCurent(etajC), etajDestinatie(etajD), greutate(g), vip(isVip) {}

    int getEtajCurent() const { return etajCurent; }
    int getEtajDestinatie() const { return etajDestinatie; }
    int getGreutate() const { return greutate; }
    bool isVip() const { return vip; }

    friend std::ostream& operator<<(std::ostream& os, const persoana& p) {
        os << "persoana " << (p.vip ? "VIP" : "normala") 
           << ", plecare: etajul " << p.etajCurent 
           << ", destinatie -> etajul " << p.etajDestinatie 
           << ", greutate: " << p.greutate << " kg";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, persoana& p) {
        int vFlag = 0;
        is >> p.etajCurent >> p.etajDestinatie >> p.greutate >> vFlag;
        p.vip = (vFlag == 1);
        return is;
    }
};

class etaj {
private:
    int numarEtaj;
    std::vector<persoana> coadaAsteptare;

public:
    etaj() : numarEtaj(0) {}
    explicit etaj(int nr) : numarEtaj(nr) {}

    void adaugaPersoana(const persoana& p) {
        coadaAsteptare.push_back(p);
    }

    int calculeazaGreutateTotalaAsteptare() const {
        int suma = 0;
        for (const auto& p : coadaAsteptare) suma += p.getGreutate();
        return suma;
    }

    std::vector<persoana>& getCoadaAsteptare() { return coadaAsteptare; }
    const std::vector<persoana>& getCoadaAsteptare() const { return coadaAsteptare; }
    int getNumarEtaj() const { return numarEtaj; }

    friend std::ostream& operator<<(std::ostream& os, const etaj& e) {
        os << "etajul " << e.numarEtaj << ", persoane in asteptare: " << e.coadaAsteptare.size() 
           << ", greutate totala: " << e.calculeazaGreutateTotalaAsteptare() << " kg";
        return os;
    }
};

class lift {
private:
    int id;
    int capacitateMaxPersoane;
    int greutateMax;
    int etajCurent;
    int etajMaximServit;
    bool esteVip;
    int directie; // Mereu +1 (urca)
    std::vector<persoana> pasageriLaBord;

    int calculeazaGreutateCurenta() const {
        int suma = 0;
        for (const auto& pasager : pasageriLaBord) {
            suma += pasager.getGreutate();
        }
        return suma;
    }

public:
    lift() : id(1), capacitateMaxPersoane(5), greutateMax(400), etajCurent(0), etajMaximServit(5), esteVip(false), directie(1) {}
    
    lift(int id, int capMax, int gMax, int etajC, int etajMaxS, bool vipLift)
        : id(id), capacitateMaxPersoane(capMax), greutateMax(gMax), etajCurent(etajC), etajMaximServit(etajMaxS), esteVip(vipLift), directie(1) {}

    bool estePlin() const {
        return pasageriLaBord.size() >= (size_t)capacitateMaxPersoane || calculeazaGreutateCurenta() >= greutateMax;
    }

    bool arePasageri() const {
        return !pasageriLaBord.empty();
    }

    bool esteResponsabilPentru(const persoana& p) const {
        if (esteVip) return p.isVip();
        if (p.isVip()) return false;

        bool cursaMica = (p.getEtajDestinatie() <= 5);
        if (etajMaximServit <= 5) {
            return cursaMica;
        } else {
            return !cursaMica;
        }
    }

    bool poateDeserviSiAdauga(const persoana& p) const {
        if (!esteResponsabilPentru(p)) return false;
        if (estePlin()) return false;

        if (p.getEtajDestinatie() <= p.getEtajCurent()) return false;
        if (p.getEtajDestinatie() > etajMaximServit || p.getEtajCurent() > etajMaximServit) return false;
        
        int greutateCurenta = calculeazaGreutateCurenta();
        return (greutateCurenta + p.getGreutate() <= greutateMax);
    }

    void adaugaPasager(const persoana& p) {
        pasageriLaBord.push_back(p);
    }

    int debarcaPasageriLaEtaj(int etajCurentCorespunzator) {
        int debarcati = 0;
        for (auto it = pasageriLaBord.begin(); it != pasageriLaBord.end(); ) {
            if (it->getEtajDestinatie() == etajCurentCorespunzator) {
                std::cout << "   -> pasager " << (it->isVip() ? "VIP" : "normal") 
                          << " a ajuns la destinatie (etajul " << etajCurentCorespunzator 
                          << ") si a coborat din liftul " << id << "\n";
                it = pasageriLaBord.erase(it);
                debarcati++;
            } else {
                ++it;
            }
        }
        return debarcati;
    }

    void actualizeazaDeplasare(bool areCereriDeasupra) {
        if ((!pasageriLaBord.empty() || areCereriDeasupra) && etajCurent < etajMaximServit) {
            etajCurent++;
        }
    }

    int getEtajCurent() const { return etajCurent; }
    int getId() const { return id; }
    int getEtajMaximServit() const { return etajMaximServit; }
    bool getEsteVip() const { return esteVip; }
    int getDirectie() const { return directie; }

    friend std::ostream& operator<<(std::ostream& os, const lift& l) {
        os << "lift " << l.id << (l.esteVip ? " (VIP)" : " (standard)") 
           << ", etaj curent: " << l.etajCurent 
           << ", sens: urca"
           << ", la bord: " << l.pasageriLaBord.size();
        return os;
    }
};

struct configLift {
    int etajMax;
    int greutateMax;
    int persMax;
    bool esteVip;
};

class cladire {
private:
    std::string denumire;
    int totalEtaje;
    int numarLifturi;
    etaj* etaje;
    lift* lifturi;

    bool arePersoaneDeasupra(const lift& lift) const {
        for (int e = lift.getEtajCurent() + 1; e < totalEtaje; ++e) {
            for (const auto& p : etaje[e].getCoadaAsteptare()) {
                if (lift.esteResponsabilPentru(p)) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    cladire(const std::string& denumire, int totalEtaje, int numarLifturi, const std::vector<configLift>& configuratii)
        : denumire(denumire), totalEtaje(totalEtaje), numarLifturi(numarLifturi) {
        
        etaje = new etaj[totalEtaje];
        for (int i = 0; i < totalEtaje; ++i) {
            etaje[i] = etaj(i);
        }

        lifturi = new lift[numarLifturi];
        for (int i = 0; i < numarLifturi; ++i) {
            int eMax = (i < (int)configuratii.size()) ? configuratii[i].etajMax : totalEtaje - 1;
            int gMax = (i < (int)configuratii.size()) ? configuratii[i].greutateMax : 400;
            int pMax = (i < (int)configuratii.size()) ? configuratii[i].persMax : 5;
            bool vFlag = (i < (int)configuratii.size()) ? configuratii[i].esteVip : false;
            
            lifturi[i] = lift(i + 1, pMax, gMax, 0, eMax, vFlag);
        }
    }

    cladire(const cladire& other)
        : denumire(other.denumire), totalEtaje(other.totalEtaje), numarLifturi(other.numarLifturi) {
        etaje = new etaj[totalEtaje];
        for (int i = 0; i < totalEtaje; ++i) etaje[i] = other.etaje[i];
        
        lifturi = new lift[numarLifturi];
        for (int i = 0; i < numarLifturi; ++i) lifturi[i] = other.lifturi[i];
    }

    cladire& operator=(const cladire& other) {
        if (this == &other) return *this;
        delete[] etaje;
        delete[] lifturi;

        denumire = other.denumire;
        totalEtaje = other.totalEtaje;
        numarLifturi = other.numarLifturi;

        etaje = new etaj[totalEtaje];
        for (int i = 0; i < totalEtaje; ++i) etaje[i] = other.etaje[i];

        lifturi = new lift[numarLifturi];
        for (int i = 0; i < numarLifturi; ++i) lifturi[i] = other.lifturi[i];

        return *this;
    }

    ~cladire() {
        delete[] etaje;
        delete[] lifturi;
    }

    void adaugaPersoanaLaEtaj(int indexEtaj, const persoana& p) {
        if (indexEtaj >= 0 && indexEtaj < totalEtaje) {
            etaje[indexEtaj].adaugaPersoana(p);
        }
    }

    bool esteGata() const {
        for (int i = 0; i < totalEtaje; ++i) {
            if (!etaje[i].getCoadaAsteptare().empty()) return false;
        }
        for (int i = 0; i < numarLifturi; ++i) {
            if (lifturi[i].arePasageri()) return false;
        }
        return true;
    }

    void pasSimulare() {
        for (int i = 0; i < numarLifturi; ++i) {
            int etajCurentLift = lifturi[i].getEtajCurent();
            
            std::cout << "liftul " << lifturi[i].getId() << (lifturi[i].getEsteVip() ? " VIP" : " STANDARD") 
                      << " - etaj -> " << etajCurentLift 
                      << " - Sens -> urca\n";
            
            lifturi[i].debarcaPasageriLaEtaj(etajCurentLift);
        }

        for (int i = 0; i < numarLifturi; ++i) {
            int etajCurentLift = lifturi[i].getEtajCurent();
            auto& coada = etaje[etajCurentLift].getCoadaAsteptare();
            
            for (auto it = coada.begin(); it != coada.end(); ) {
                persoana p = *it;
                bool imbarcat = false;

                if (p.isVip()) {
                    if (numarLifturi >= 3 && lifturi[2].getEtajCurent() == etajCurentLift && lifturi[2].poateDeserviSiAdauga(p)) {
                        lifturi[2].adaugaPasager(p);
                        std::cout << " * persoana VIP (destinatie etajul " << p.getEtajDestinatie() 
                                  << ", " << p.getGreutate() << " kg) a urcat in liftul 3 [VIP]\n";
                        imbarcat = true;
                    }
                } else {
                    bool cursaMica = (p.getEtajDestinatie() <= 5);

                    if (cursaMica) {
                        if (numarLifturi >= 2 && lifturi[1].getEtajCurent() == etajCurentLift && lifturi[1].poateDeserviSiAdauga(p)) {
                            lifturi[1].adaugaPasager(p);
                            std::cout << " * persoana normala (destinatie etajul " << p.getEtajDestinatie() 
                                      << ", " << p.getGreutate() << " kg) a urcat in liftul 2\n";
                            imbarcat = true;
                        }
                    } else {
                        if (lifturi[0].getEtajCurent() == etajCurentLift && lifturi[0].poateDeserviSiAdauga(p)) {
                            lifturi[0].adaugaPasager(p);
                            std::cout << " * persoana normala (destinatie etajul " << p.getEtajDestinatie() 
                                      << ", " << p.getGreutate() << " kg) a urcat in liftul 1\n";
                            imbarcat = true;
                        }
                    }
                }

                if (imbarcat) {
                    it = coada.erase(it);
                } else {
                    ++it;
                }
            }
        }

        for (int i = 0; i < numarLifturi; ++i) {
            bool cereriDeasupra = arePersoaneDeasupra(lifturi[i]);
            lifturi[i].actualizeazaDeplasare(cereriDeasupra);
        }
        std::cout << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const cladire& c) {
        os << c.denumire << ", etaje: " << c.totalEtaje << ", lifturi: " << c.numarLifturi << "\n";
        os << "itinerariu lifturi:\n";
        for (int i = 0; i < c.numarLifturi; ++i) {
            os << "  - " << c.lifturi[i] << "\n";
        }
        os << "stare curenta:\n";
        for (int i = 0; i < c.totalEtaje; ++i) {
            os << "  - " << c.etaje[i] << "\n";
        }
        return os;
    }
};

int main() {
    std::ifstream fin("tastatura.txt");
    if (fin.is_open()) {
        std::cin.rdbuf(fin.rdbuf());
    }

    int nrEtaje, nrLifturi;
    if (!(std::cin >> nrEtaje >> nrLifturi)) {
        nrEtaje = 10;
        nrLifturi = 3;
    }

    std::vector<configLift> configuratii;
    for (int i = 0; i < nrLifturi; ++i) {
        configLift cfg;
        int vipFlag = 0;
        std::cin >> cfg.etajMax >> cfg.greutateMax >> cfg.persMax >> vipFlag;
        cfg.esteVip = (vipFlag == 1);
        configuratii.push_back(cfg);
    }

    cladire cladire("PB Tower", nrEtaje, nrLifturi, configuratii);

    int nrPersoane = 0;
    if (std::cin >> nrPersoane) {
        for (int i = 0; i < nrPersoane; ++i) {
            persoana p;
            std::cin >> p;
            cladire.adaugaPersoanaLaEtaj(p.getEtajCurent(), p);
        }
    }

    std::cout << cladire << "\n";

    while (!cladire.esteGata()) {
        cladire.pasSimulare();
    }

    return 0;
}