// trainProto.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <memory> // Smart pointer enabling
#include <regex> // email verification
#include <map>
#include <fstream>
#include <cstdio>



using namespace std;

// Forward declarations
class PengurusanDiskaun;
class Laluan; // Already implemented
class KeteApi; // Already implemented

// Class TempatDuduk
class TempatDuduk {
private:
    int row;              // Row number (1-9)
    char column;          // Column letter (e.g., 'A'-'D')
    map<int, bool> ditempahPerBulan;        // Reservation status

public:
    // Constructor
    TempatDuduk(int r, char c)
        : row(r), column(c) {
        
    }

    int getRow() const;
    char getColumn() const;

    bool isDitempah(int bulan) const {
        // Tengok bulan valid atau tak
        if (bulan < 1 || bulan > 12) {
            cout << "Ralat: Bulan tidak sah (" << bulan << ").\n";
            return true;
        }

        // cari bulan dalam map
        auto it = ditempahPerBulan.find(bulan);
        return it != ditempahPerBulan.end() && it->second;
    }


    // Seat reservation methods
    void tempah(int bulan) {
        ditempahPerBulan[bulan] = true;
    }     // Reserve the seat
    void batalTempah(int bulan) {
        ditempahPerBulan[bulan] = false;
    } // Cancel reservation


    


};


int TempatDuduk::getRow() const {
    return row;
}
char TempatDuduk::getColumn() const {
    return column;
}

// Class Gerabak
class Gerabak {
private:
    string gerabakID;
    int numRows; // Number of rows
    int numCols; // Number of columns
    vector<vector<shared_ptr<TempatDuduk>>> tempatDuduk; // 2D vector for seats

public:
    Gerabak(string id, int rows = 9, int cols = 4)
        : gerabakID(id), numRows(rows), numCols(cols) {
        // Initialize tempat duduk
        for (int i = 1; i <= numRows; ++i) {
            vector<shared_ptr<TempatDuduk>> rowSeats;
            for (int j = 0; j < numCols; ++j) {
                char columnLetter = 'A' + j; // Column labels A to D
                rowSeats.push_back(make_shared<TempatDuduk>(i, columnLetter));
            }
            tempatDuduk.push_back(rowSeats);
        }
    }

    string getID() const {
        return gerabakID; // Get gerabak ID
    }

    int getNumRows() const { return numRows; }
    int getNumCols() const { return numCols; }

    shared_ptr<TempatDuduk> getSeat(int row, char column) const {
        int colIndex = toupper(column) - 'A';
        if (row < 1 || row > numRows || colIndex < 0 || colIndex >= numCols) {
            return nullptr; // Invalid position
        }
        return tempatDuduk[row - 1][colIndex];
    }

    // tempah kerusi
    bool tempahKerusi(int row, char column, int bulan) {
        auto kerusi = getSeat(row, column);
        if (kerusi && !kerusi->isDitempah(bulan)) {
            kerusi->tempah(bulan);
            cout << "Kerusi " << row << column << " Telah berjaya untuk ditempah.\n";
            return true; // Reservation successful
        }
        cout << "Seat " << row << column << " Kerusi telah ditempah atau penempahan tidak sah.\n";
        return false; // Reservation failed
    }

    void paparkanTempatDuduk(int bulan) const {
        cout << "Gerabak " << gerabakID << " - Aturan Tempat duduk:\n";
        cout << "  ";
        for (int j = 0; j < numCols; ++j) {
            char columnLetter = 'A' + j;
            cout << columnLetter << " ";
            if (j == numCols / 2 - 1) {
                cout << "  ";  // Aisle gap between seats
            }
        }
        cout << "\n";

        for (int i = 0; i < numRows; ++i) {
            cout << setw(2) << (i + 1) << " ";
            for (int j = 0; j < numCols; ++j) {
                char statusKerusi = tempatDuduk[i][j]->isDitempah(bulan) ? 'X' : 'O';
                cout << statusKerusi << " ";
                if (j == numCols / 2 - 1) {
                    cout << "| "; // Left aisle
                }
            }
            cout << "\n";
        }
    }
};

// Class Laluan
class Laluan {
private:
    string namaLaluan;	   // Name/ID of the route/station
    string lokasiMula;    // Starting station
    string lokasiAkhir;   // Ending station
    vector<string> jadual; // List of available times
    double dasarHarga;     // Base price for the route
    bool tersediaKe;	   // Availability status
    map<int, bool> bulanOperasi;

public:
    // Constructor to create a route
    Laluan(const string& nama, const string& mula, const string& akhir, const vector<string> masa, double harga, bool ketersediaan)
        : namaLaluan(nama), lokasiMula(mula), lokasiAkhir(akhir), jadual(masa), dasarHarga(harga), tersediaKe(ketersediaan) {
        if (harga < 0) {
            dasarHarga = 0.0;
            cout << "Harga yang dimasukkan tidak sah. Sistem meletakkan nilai kepada 0\n"; // Ensure price is valid
        }
        // Initialize setiap bulan sebagai tersedia
        for (int i = 1; i <= 12; i++) {
            bulanOperasi[i] = true;
        }
    }

    // Method to get route name/ID
    string getNama() const {
        return namaLaluan;
    }

    // Getters for starting location
    string getLokasiMula() const {
        return lokasiMula;
    }

    // Getters for ending location
    string getLokasiAkhir() const {
        return lokasiAkhir;
    }

    // Setter for starting location
    void setLokasiMula(const string& mula) {
        lokasiMula = mula;
    }

    // Setter for ending location
    void setLokasiAkhir(const string& akhir) {
        lokasiAkhir = akhir;
    }

    // Get schedule
    vector<string> getJadual() const {
        return jadual;
    }

    // Get base price
    double getHarga() const {
        return dasarHarga;
    }

    // Set route status
    void setStatus(bool ketersediaan) {
        tersediaKe = ketersediaan;
    }

    // Add a new time to the schedule
    void tambahMasa(const string& masa) {
        jadual.push_back(masa);
    }

    // Remove a time from the schedule
    bool hapusMasa(const string& masa) {
        auto it = find(jadual.begin(), jadual.end(), masa);
        if (it != jadual.end()) {
            jadual.erase(it);
            return true; // Successfully removed
        }
        return false; // Time not found
    }

    // Check availability for a specific time
    bool cekKetersediaanMasa(const string& masa) const {
        return tersediaKe && (find(jadual.begin(), jadual.end(), masa) != jadual.end());
    }

    // Get availability status
    bool getStatus() const {
        return tersediaKe;
    }

    // method untuk operasi bulan
    bool isAvailableInBulan(int bulan) const {
        return tersediaKe && bulanOperasi.at(bulan);
    }

    void setBulanOperasi(int bulan, bool available) {
        if (bulan >= 1 && bulan <= 12) {
            bulanOperasi[bulan] = available;
        }
    }

    // Constants for availability status
    const string STATUS_TERSEDIA = "Tersedia";
    const string STATUS_TIDAK_TERSEDIA = "Tidak Tersedia";

    // Display route details
    void paparButiranLaluan() const {
        cout << "Laluan: " << namaLaluan << "\n";
        cout << "Dari: " << lokasiMula << "\n";
        cout << "Ke: " << lokasiAkhir << "\n";
        cout << "Ketersediaan: " << (tersediaKe ? STATUS_TERSEDIA : STATUS_TIDAK_TERSEDIA) << "\n";
        cout << "Harga Dasar: RM" << dasarHarga << "\n";
        cout << "Jadual: ";
        for (const string& masa : jadual) {
            cout << masa << " ";
        }
        cout << "\n";
    }
};

// Class Pengguna
class Pengguna {
private:
    string nama;
    string email;
    string password;
    int umur;
    bool adminKe;

public:
    // Constructor
    Pengguna(string penggunaNama, string penggunaEmail, string penggunaPassword) {
        nama = penggunaNama;
        email = penggunaEmail;
        password = penggunaPassword;
        adminKe = false; // Indicates a regular user
    }

    // Method to set age
    bool setAge(int penggunaUmur) {
        if (penggunaUmur <= 0) {
            return false;
        }
        umur = penggunaUmur;
        return true;
    }

    // Getters
    string getNama() const { return nama; }
    string getEmail() const { return email; }
    int getAge() const { return umur; }
    bool isAdminUser() const { return adminKe; }

    // Password verification
    bool verifyPassword(string masukPassword) {
        return password == masukPassword;
    }
};

// Class PengurusanDiskaun
class PengurusanDiskaun {
private:
    struct DiskaunMusim {
        double kadar;
        int bulanMula;
        int bulanAkhir;
        bool aktif;
    };

    map<string, double> diskaunKategori;
    map<string, DiskaunMusim> diskaunMusim;
    map<string, double> diskaunBayaran;

public:
    void tambahDiskaun(const string& nama, double kadar) {
        diskaunKategori[nama] = kadar;
    }

    void tambahDiskaunMusim(const string& nama, double kadar, int bulanMula, int bulanAkhir) {
        DiskaunMusim diskaun;
        diskaun.kadar = kadar;
        diskaun.bulanMula = bulanMula;
        diskaun.bulanAkhir = bulanAkhir;
        diskaun.aktif = true;
        diskaunMusim[nama] = diskaun;
    }

    // jenis pembayaran diskaun
    void tambahDiskaunBayaran(const string& jenisBayaran, double kadar) {
        diskaunBayaran[jenisBayaran] = kadar;
    }

    double getDiskaunBayaran(const string& jenisBayaran) const {
        auto it = diskaunBayaran.find(jenisBayaran);
        return (it != diskaunBayaran.end()) ? it->second : 0.0;
    }

    double kiraHarga(double hargaAsal, const string& jenisBayaran, int bulan, const vector<int>& umurPenumpang) {
        double jumlahDiskaun = 0.0;
        double hargaAkhir;

        // Tengok kategori diskaun untuk umur
        for (const auto& umur : umurPenumpang) {
            for (const auto& diskaun : diskaunKategori) {
                cout << "- Diskaun " << diskaun.first << ": " << (diskaun.second * 100) << "%\n";
                if (diskaun.first == "Pelajar" && umur >= 7 && umur <= 25) {
                    jumlahDiskaun += diskaun.second;
                    break;
                }
                else if (diskaun.first == "Warga Emas" && umur >= 60) {
                    jumlahDiskaun += diskaun.second;
                    break;
                }
                else if (diskaun.first == "Kanak-kanak" && umur < 7) {
                    jumlahDiskaun += diskaun.second;
                    break;
                }
                else if (diskaun.first != "Pelajar") {
                    jumlahDiskaun += diskaun.second;
                    break; // apply sekali saja untuk seorang
                }
            }
        }

        // Tengok diskaun untuk musim
        for (const auto& diskaun : diskaunMusim) {
            if (diskaun.second.aktif &&
                bulan >= diskaun.second.bulanMula &&
                bulan <= diskaun.second.bulanAkhir) {
                cout << "- Diskaun Musim " << diskaun.first << ": " << (diskaun.second.kadar * 100) << "%\n";
            }
        }

        // tengok diskaun untuk jenis pembayaran
        auto paymentDiscount = getDiskaunBayaran(jenisBayaran);
        if (paymentDiscount > 0) {
            jumlahDiskaun += paymentDiscount;
            cout << "\nDiskaun yang dikenakan:\n";
            cout << "- Diskaun " << jenisBayaran << ": " << (paymentDiscount * 100) << "%\n";
        }

        // apply total diskaun
        hargaAkhir = hargaAsal * (1.0 - jumlahDiskaun);
        return (hargaAkhir < 0) ? 0.0 : hargaAkhir;
    }

    void paparSenaraiDiskaun() const {
        cout << "\nSenarai Diskaun:\n";
        for (const auto& diskaun : diskaunKategori) {
            cout << "Nama: " << diskaun.first
                << " | Kadar: " << (diskaun.second * 100) << "%\n";
        }
        for (const auto& diskaun : diskaunMusim) {
            cout << "Nama: " << diskaun.first
                << " | Kadar: " << (diskaun.second.kadar * 100) << "%"
                << " | Musim: " << diskaun.second.bulanMula << "-" << diskaun.second.bulanAkhir
                << " | Status: " << (diskaun.second.aktif ? "Aktif" : "Tidak Aktif") << "\n";
        }
    }
};

// Class KeteApi
class KeteApi {
private:
    string trenID;
    shared_ptr<Laluan> pilihanLaluan; // Pointer to reference Laluan
    bool aktifKe;
    vector<shared_ptr<Gerabak>> gerabakLebih; // Smart pointer to Gerabak

public:
    KeteApi(string id, bool status)
        : trenID(id), aktifKe(status), pilihanLaluan(nullptr) {
    }

    string getID() const {
        return trenID;
    }

    bool getStatus() const {
        return aktifKe;
    }

    // Setter for active status
    void setStatus(bool status) {
        aktifKe = status;
    }

    void setLaluan(const shared_ptr<Laluan>& laluan) {
        pilihanLaluan = laluan;
    }

    shared_ptr<Laluan> getLaluan() const {
        return pilihanLaluan;
    }

    void tambahGerabak(shared_ptr<Gerabak> gerabak) {
        for (const auto& gerabakSediaAda : gerabakLebih) {
            if (gerabakSediaAda->getID() == gerabak->getID()) {
                cout << "Gerabak dengan ID " << gerabak->getID() << " sudah wujud di dalam sistem\n";
                return;
            }
        }
        gerabakLebih.push_back(gerabak);
    }

    // Remove gerabak
    bool hapusGerabak(const string& gerabakID) {
        auto it = find_if(gerabakLebih.begin(), gerabakLebih.end(),
            [&gerabakID](const shared_ptr<Gerabak>& gerabak) {
                return gerabak->getID() == gerabakID;
            });
        if (it != gerabakLebih.end()) {
            gerabakLebih.erase(it);
            return true; // Successfully removed
        }
        return false; // Gerabak not found
    }

    vector<shared_ptr<Gerabak>> getGerabak() const {
        return gerabakLebih;
    }

    void paparanButiranKeteApi() const {
        cout << "Tren ID: " << trenID << "\n";
        cout << "Status: " << (aktifKe ? "Aktif" : "Tidak Aktif") << "\n";
        cout << "Pilihan Laluan: "
            << (pilihanLaluan ? pilihanLaluan->getNama() : "Tiada") << "\n";
        cout << "Gerabak: ";
        if (gerabakLebih.empty()) {
            cout << "None";
        }
        else {
            for (const shared_ptr<Gerabak>& gerabak : gerabakLebih) {
                cout << gerabak->getID() << " ";
            }
            cout << "\n";
        }
    }
};

// Class Penempahan
class Penempahan {
private:
    const string DIREKTORI_RESIT = "resit/";  // Add this constant at class level
    vector<string> failResit;

    // dapatkan masa untuk resit
    string getCurrentDate() const {
        auto now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);

        struct tm tm;
        localtime_s(&tm, &time);

        ostringstream oss;
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }


    // System components
    shared_ptr<PengurusanDiskaun> pengurusanDiskaun;
    vector<shared_ptr<Pengguna>> senaraiPengguna;
    vector<shared_ptr<Laluan>> senaraiLaluan;
    vector<shared_ptr<KeteApi>> senaraiKeteApi;

    // kaedah bayaran
    const vector<string> KAEDAH_BAYARAN = {
        "Kad Kredit",
        "Kad Debit",
        "Online Banking",
        "E-Wallet"
    };

    // Penempahan structure
    struct InfoPenempahan {
        string penempahanID;
        shared_ptr<Pengguna> pembeli;
        shared_ptr<Laluan> laluan;
        shared_ptr<KeteApi> keteApi;
        vector<shared_ptr<TempatDuduk>> tempatDuduk;
        int jumlahDewasa;
        int jumlahBudak;
        int jumlahTua;
        double jumlahHarga;
        string tarikhTempahan; // tarikh book (YYYY-MM-DD)
        string masaTempahan; // masa book ( HH:MM:SS)
        string receiptFilePath; // Directory path
        string jenisBayaran;
        vector<string> kerusiTempahan;

        // TODO: TAMBAH field baru untuk receipt
        /*
        boleh tambah ni:
        string tarikhTempahan; // booking date
        string receiptFilePath; // Path to receipt file
        */
    };
    vector<InfoPenempahan> infoPenempahan;

    // pilihan kerusi
    struct PilihanKerusi {
        string gerabakID;
        vector<pair<int, char >> kerusi;


    };

    // TODO: Tambah attribute untuk receipt dan history
    /*
    Cadangan: Tambah attribute:
    string receiptDirectory; // Directory untuk store receipt files
    vector<string> receiptFiles; // Track all generated receipt files
    */



    // fungsi pembantu untuk email validasi
    bool emailValidKe(const string& email) {
        const regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return regex_match(email, pattern);
    }

    bool validatePilihanKerusi(const PilihanKerusi& pilihanKerusi, int jumlahPenumpang,
        shared_ptr<KeteApi> keteApi, int bulan) {
        // check kalau bilangan kerusi yang dipilih sepadan 
        if (pilihanKerusi.kerusi.size() != jumlahPenumpang) {
            cout << "Ralat: Bilangan kerusi yang dipilih tidak sama dengan bilangan penumpang. \n";
            return false;
        }

        // cari gerabak
        auto gerabakList = keteApi->getGerabak();
        auto gerabak = find_if(gerabakList.begin(), gerabakList.end(), [&](const shared_ptr<Gerabak>& g) {
            return g->getID() == pilihanKerusi.gerabakID;
            });

        // check kalau gerabak wujud 
        if (gerabak == gerabakList.end()) {
            cout << "Ralat: Gerabak yang dipilih tidak dijumpai.\n";
            return false;
        }

        for (const auto& seat : pilihanKerusi.kerusi) {
            int row = seat.first;
            char col = seat.second;

            //dapatkan kerusi dari gerabak
            auto tempatDuduk = (*gerabak)->getSeat(row, col);

            // check kalau kerusi wujud dan ada
            if (!tempatDuduk) {
                cout << "Ralat: kerusi " << row << col << "tidak sah.\n";
                return false;
            }
            if (tempatDuduk->isDitempah(bulan)) {
                cout << "Ralat: kerusi " << row << col << "telah ditempah.\n";
                return false;
            }
        }

        // semua validasi selesai
        return true;

    }

public:
    Penempahan(shared_ptr<PengurusanDiskaun> pengDiskaun) {
        this->pengurusanDiskaun = pengDiskaun;


    }

    // buang tempahan
    bool batalTempahan(const string& penempahanID) {
        // 1. Cari booking untuk dibuang
        auto it = find_if(infoPenempahan.begin(), infoPenempahan.end(),
            [&](const InfoPenempahan& tempahan) {
                return tempahan.penempahanID == penempahanID;
            });
        if (it == infoPenempahan.end()) {
            cout << "Ralat: Tempahan tidak dijumpai.\n";
            return false;

        }

        // 2. Extract booking month from the date
        string tarikhTempahan = it->tarikhTempahan;
        int bulan = stoi(tarikhTempahan.substr(5, 2)); 

        // 3. Free up the booked seats for this month
        for (const auto& kerusi : it->kerusiTempahan) {
            // Get row and column from seat string (e.g., "1A" -> row=1, col='A)
            int row = stoi(kerusi.substr(0, kerusi.length() - 1));
            char col = kerusi.back();

            // dapatkan gerabak dan kerusi
            auto gerabakList = it->keteApi->getGerabak();
            for (const auto& gerabak : gerabakList) {
                auto seat = gerabak->getSeat(row, col);
                if (seat) {
                    seat->batalTempah(bulan);
                }
            }
        }

        //4. Buang receipt file
        string namaFail = DIREKTORI_RESIT + "resit_" + penempahanID + ".txt";
        if (remove(namaFail.c_str()) != 0) {
            cout << "Amaran: Gagal memadamkan fail resit.\n";
        }

        //5. Buang booking daripada vector
        infoPenempahan.erase(it);
        cout << "Tempahan berjaya dibatalkan.\n";
        return true;
    }



    // Function untuk register user baru

    bool daftarPengguna(const string& nama, const string& email, const string& password, int umur) {
        // guna library regex untuk validasi email
        if (!emailValidKe(email)) {
            cout << "Ralat: Format emel tidak sah.\n";
            return false;
        }

        // Check kalau email telah didaftarkan
        for (const auto& user : senaraiPengguna) {
            if (user->getEmail() == email) {
                cout << "Ralat: Emel telah didaftarkan. \n";
                return false;
            }
        }

        // daftar pengguna baru dan masukkan dalam list
        auto newUser = make_shared<Pengguna>(nama, email, password);
        if (!newUser->setAge(umur)) {
            cout << "Ralat: Umur tidak sah. \n";
            return false;
        }
        senaraiPengguna.push_back(newUser);

        cout << "Pendaftaran berjaya!\n";
        return true;
    }

    // function untuk log in kan pengguna sedia ada
    shared_ptr<Pengguna> loginUser(const string& email, const string& password) {
        for (const auto& user : senaraiPengguna) {
            if (user->getEmail() == email && user->verifyPassword(password)) {
                cout << "Log masuk berjaya!\n";
                return user;
            }
        }
        cout << "Ralat: Emel atau kata laluan tidak sah. \n";
        return nullptr;
    }

    // tunjuk route dan masa
    void paparLaluanTersedia(int bulan) const {
        cout << "\nLaluan yang tersedia: \n";
        cout << setw(5) << left << "ID"
            << setw(20) << left << "Dari"
            << setw(20) << left << "Ke"
            << setw(15) << left << "harga (RM)"
            << "Jadual\n";
        cout <<
            "---------------------------------------------------------------------\n";

        for (const auto& laluan : senaraiLaluan) {
            // Add month check alongside existing status check
            if (laluan->getStatus() && laluan->isAvailableInBulan(bulan)) {
                cout << setw(5) << left << laluan->getNama()
                    << setw(20) << left << laluan->getLokasiMula()
                    << setw(20) << left << laluan->getLokasiAkhir()
                    << setw(15) << left << fixed << setprecision(2) << laluan->getHarga();

                for (const string& masa : laluan->getJadual()) {
                    cout << masa << " ";
                }
                cout << "\n";
            }
        }
    }

    // function untuk dapatkan laluan melalui id
    shared_ptr<Laluan> getLaluanByID(const string& laluanID) const {
        for (const auto& laluan : senaraiLaluan) {
            if (laluan->getNama() == laluanID) {
                return laluan;
            }
        }
        return nullptr;

    }

    // Pilihan kereta api dan gerabak
    void paparKeteApiTersedia(const shared_ptr<Laluan>& laluan) const {
        cout << "\nKereta Api yang tersedia untuk laluan " << laluan->getNama() << ":\n";
        cout << setw(10) << left << "ID KeteApi"
            << "Status\n";
        cout << "------------------------\n";

        for (const auto& keteApi : senaraiKeteApi) {
            if (keteApi->getLaluan() == laluan && keteApi->getStatus()) {
                cout << setw(10) << left << keteApi->getID()
                    << (keteApi->getStatus() ? "Tersedia" : "Tidak Tersedia") << "\n";
            }
        }
        cout << "\n";
    }

    // Function untuk dapatkan kereta api menggunakan ID
    shared_ptr<KeteApi> getKeteApiByID(const string& keteApiID) const {
        for (const auto& keteApi : senaraiKeteApi) {
            if (keteApi->getID() == keteApiID) {
                return keteApi;
            }
        }
        return nullptr;
    }

    void paparGerabakTersedia(const shared_ptr<KeteApi>& keteApi) const {
        cout << "\nGerabak yang tersedia untuk kete api " << keteApi->getID() << "\n";

        cout << setw(15) << left << "ID Gerabak"
            << "Status\n";
        cout << "---------------------------\n";

        auto gerabakList = keteApi->getGerabak();
        for (const auto& gerabak : gerabakList) {
            cout << setw(15) << left << gerabak->getID() << "Tersedia" << "\n";
        }
        cout << "\n";
    }

    // Function untuk dapatkan gerabak melalui ID  dalam spesifik kereta api
    shared_ptr<Gerabak> getGerabakbyID(const shared_ptr<KeteApi>& keteApi, const string& gerabakID) const {
        auto gerabakList = keteApi->getGerabak();
        for (const auto& gerabak : gerabakList) {
            if (gerabak->getID() == gerabakID) {
                return gerabak;
            }
        }
        return nullptr;
    }

    void paparPilihanKerusi(const shared_ptr<Gerabak>& gerabak, int bulan) const {
        cout << "\nGerabak " << gerabak->getID() << " - Visualisasi Tempat Duduk:\n";
        cout << "Petunjuk: O = Kosong, X = Ditempah\n\n";

        // Print column headers (A, B, C, etc.)
        cout << "   ";
        for (int j = 0; j < gerabak->getNumCols(); ++j) {
            cout << char('A' + j) << " ";
            if (j == gerabak->getNumCols() / 2 - 1) {
                cout << "| "; // lorong
            }
        }
        cout << "\n";

        // Print rows with seat status
        for (int i = 0; i < gerabak->getNumRows(); ++i) {
            cout << setw(2) << (i + 1) << " ";
            for (int j = 0; j < gerabak->getNumCols(); ++j) {
                auto seat = gerabak->getSeat(i + 1, 'A' + j);
                char statusKerusi = seat->isDitempah(bulan) ? 'X' : 'O';
                cout << statusKerusi << " ";
                if (j == gerabak->getNumCols() / 2 - 1) {
                    cout << "| "; // lorong
                }
            }
            cout << "\n";
        }
        cout << "\n";

    }

    // paparkan kerusi yang ditempah
    void paparPilihanKerusiDenganPilihan(const shared_ptr<Gerabak>& gerabak,
        const vector<pair<int, char>>& kerusiDipilih,
        int bulan) const {
        cout << "Gerabak " << gerabak->getID() << " - Visualisasi Tempat Duduk:\n";
        cout << "Petunjuk: O = Kosong, X = Ditempah, S = Dipilih\n\n";

        // Print column headers (A, B, C, etc.)
        cout << "   ";
        for (int j = 0; j < gerabak->getNumCols(); ++j) {
            cout << char('A' + j) << " ";
            if (j == gerabak->getNumCols() / 2 - 1) {
                cout << "| "; // lorong
            }
        }
        cout << "\n";

        // Print rows with seat status
        for (int i = 0; i < gerabak->getNumRows(); ++i) {
            cout << setw(2) << (i + 1) << " ";
            for (int j = 0; j < gerabak->getNumCols(); ++j) {
                char currentCol = 'A' + j;
                bool isSelected = false;

                // Check if seat is selected
                for (const auto& seat : kerusiDipilih) {
                    if (seat.first == (i + 1) && seat.second == currentCol) {
                        isSelected = true;
                        break;
                    }
                }

                char statusKerusi;
                if (isSelected) {
                    statusKerusi = 'S';  // S untuk selected
                }
                else {
                    auto seat = gerabak->getSeat(i + 1, currentCol);
                    statusKerusi = seat->isDitempah(bulan) ? 'X' : 'O';  // Check for specific month
                }

                cout << statusKerusi << " ";
                if (j == gerabak->getNumCols() / 2 - 1) {
                    cout << "| "; // jurang lorong
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }



    // **Wrapper Method for PilihanKerusi**
    string buatTempahanDenganPilihan(shared_ptr<Pengguna> pembeli, shared_ptr<Laluan> laluan,
        shared_ptr<KeteApi> keteApi, int dewasa, int budak, int tua, const string& gerabakID,
        const vector<pair<int, char>>& kerusiDipilih, const string& jenisBayaran,
        const vector<int>& umurPenumpang, int bulan) {

        // Create PilihanKerusi
        PilihanKerusi pilihan;
        pilihan.gerabakID = gerabakID;
        pilihan.kerusi = kerusiDipilih;

        // Calculate total passengers
        int jumlahPenumpang = dewasa + budak + tua;
        vector<string> pilihanKerusi;
        for (const auto& seat : kerusiDipilih) {
            string kerusi = to_string(seat.first) + seat.second;
            pilihanKerusi.push_back(kerusi);
        }

        // Check existing bookings for the specific month
        for (const auto& tempahan : infoPenempahan) {
            if (tempahan.keteApi->getID() == keteApi->getID()) {
                int bulanTempahan = stoi(tempahan.tarikhTempahan.substr(5, 2));

                if (bulanTempahan == bulan) {
                    for (const auto& kerusiLama : tempahan.kerusiTempahan) {
                        for (const auto& kerusiBaru : pilihanKerusi) {
                            if (kerusiLama == kerusiBaru) {
                                cout << "Ralat: Kerusi " << kerusiBaru
                                    << " telah ditempah untuk bulan " << bulan << endl;
                                return "";
                            }
                        }
                    }
                }
            }
        }

        // Create InfoPenempahan object
        InfoPenempahan tempahan;
        tempahan.penempahanID = generateID();
        tempahan.pembeli = pembeli;
        tempahan.laluan = laluan;
        tempahan.keteApi = keteApi;
        tempahan.jumlahDewasa = dewasa;
        tempahan.jumlahBudak = budak;
        tempahan.jumlahTua = tua;
        tempahan.kerusiTempahan = pilihanKerusi;
        tempahan.jenisBayaran = jenisBayaran;

        // Get and book seats with month parameter
        auto gerabakList = keteApi->getGerabak();
        auto gerabak = find_if(gerabakList.begin(), gerabakList.end(),
            [&](const shared_ptr<Gerabak>& g) {
                return g->getID() == gerabakID;
            });

        if (gerabak != gerabakList.end()) {
            for (const auto& seat : kerusiDipilih) {
                auto tempatDuduk = (*gerabak)->getSeat(seat.first, seat.second);
                if (tempatDuduk) {
                    // Book seat for specific month
                    tempatDuduk->tempah(bulan);
                    tempahan.tempatDuduk.push_back(tempatDuduk);
                }
            }
        }

       
        // Set correct booking date with selected month
        auto now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        struct tm tm;
        localtime_s(&tm, &time);
        tm.tm_mon = bulan - 1;

        // Format date and time separately
        ostringstream ossDate, ossTime;
        ossDate << put_time(&tm, "%Y-%m-%d");
        ossTime << put_time(&tm, "%H:%M:%S");

        // Assign to tarikhTempahan and masaTempahan
        tempahan.tarikhTempahan = ossDate.str();
        tempahan.masaTempahan = ossTime.str();
       
       

        // Calculate price
        double hargaAsal = laluan->getHarga() * (dewasa + budak + tua);
        tempahan.jumlahHarga = pengurusanDiskaun->kiraHarga(hargaAsal, jenisBayaran, bulan, umurPenumpang);

        // Store booking and save receipt
        infoPenempahan.push_back(tempahan);
        simpanResit(tempahan);

        return tempahan.penempahanID;
    }

    // Process booking
    string buatTempahan(shared_ptr<Pengguna> pembeli, shared_ptr<Laluan> laluan, shared_ptr<KeteApi> keteApi,
        int dewasa, int budak, int tua, const PilihanKerusi& pilihanKerusi, const string& jenisBayaran, const vector<int>& umurPenumpang, int bulan) {
        if (!pembeli || !laluan || !keteApi) {
            return ""; // Invalid booking
        }





        // jumlah Penumpang
        int jumlahPenumpang = dewasa + budak + tua;

        //validasi pilihan kerusi
        if (!validatePilihanKerusi(pilihanKerusi, jumlahPenumpang, keteApi, bulan)) {
            return "";
        }

        //dapatkan gerabak dan kerusi yang ditempah
        auto gerabakList = keteApi->getGerabak();
        auto gerabakIter = find_if(gerabakList.begin(), gerabakList.end(),
            [&](const shared_ptr<Gerabak>& g) {
                return g->getID() == pilihanKerusi.gerabakID;
            });

        // dapatkan shared_ptr ke gerabak
        shared_ptr<Gerabak> gerabak = *gerabakIter;
        vector<shared_ptr<TempatDuduk>> tempahBerjaya;

        //Book kan semua seat  gunakan shared_ptr
        for (const auto& seat : pilihanKerusi.kerusi) {
            auto tempatDuduk = gerabak->getSeat(seat.first, seat.second);
            gerabak->tempahKerusi(seat.first, seat.second, bulan);
            tempahBerjaya.push_back(tempatDuduk);
        }

        //Kira harga dengan harga diskaun
        double hargaAsal = laluan->getHarga() * jumlahPenumpang;
        double hargaAkhir = pengurusanDiskaun->kiraHarga(hargaAsal, jenisBayaran, bulan, umurPenumpang);

        // informasi tempahan
        InfoPenempahan tempahan;
        tempahan.penempahanID = generateID();
        tempahan.pembeli = pembeli;
        tempahan.laluan = laluan;
        tempahan.keteApi = keteApi;
        tempahan.jumlahDewasa = dewasa;
        tempahan.jumlahBudak = budak;
        tempahan.jumlahTua = tua;
        tempahan.tempatDuduk = tempahBerjaya;
        tempahan.jumlahHarga = hargaAkhir;

        infoPenempahan.push_back(tempahan);

      
        return tempahan.penempahanID;
    }

    const vector<string>& getKaedahBayaran() const {
        return KAEDAH_BAYARAN;
    }

private:
    string generateID() {
        static int counter = 1000;
        return "BKG" + to_string(++counter);
    }

public:
    // System management
    void tambahPengguna(shared_ptr<Pengguna> pembeli) {
        senaraiPengguna.push_back(pembeli);
    }

    void tambahLaluan(shared_ptr<Laluan> laluan) {
        senaraiLaluan.push_back(laluan);
    }

    void tambahKeteApi(shared_ptr<KeteApi> keteApi) {
        senaraiKeteApi.push_back(keteApi);
    }

    void prosesTempahan(shared_ptr<Pengguna> penggunaSemasa) {
        // Declare selection variables at function scope
        shared_ptr<Laluan> pilihanLaluan = nullptr;
        shared_ptr<KeteApi> pilihanKeteApi = nullptr;
        shared_ptr<Gerabak> pilihanGerabak = nullptr;
        string pilihanLaluanId, pilihanKeteApiID, pilihanGerabakID;



        // pilih bulan perjalanan
        int bulan;
        cout << "Masukkan bulan perjalanan (1-12): ";
        while (!(cin >> bulan) || bulan < 1 || bulan > 12) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ralat: Bulan mesti antara 1-12. Sila cuba lagi: ";
        }
        cin.ignore();


        // 1. Tunjuk jalan tersedia
        paparLaluanTersedia(bulan);

        //2. Pengguna pilih laluan
        string pilihanLaluanID;
        bool validLaluan = false;
        while (!validLaluan) {
            cout << "Masukkan ID laluan yang dipilih (atau 'B' untuk kembali): ";
            getline(cin, pilihanLaluanID);
            transform(pilihanLaluanID.begin(), pilihanLaluanID.end(), pilihanLaluanID.begin(), ::toupper);

            if (pilihanLaluanID == "B") return; // Return to main menu

            pilihanLaluan = getLaluanByID(pilihanLaluanID);
            if (pilihanLaluan) {
                validLaluan = true;
            }
            else {
                cout << "Ralat: Laluan tidak dijumpai. Sila cuba lagi.\n";
            }
            cout << "[DEBUG] Selected route: "
                << (pilihanLaluan ? pilihanLaluan->getNama() : "nullptr")
                << endl;


        }



        //3. Tunjuk kereta api yang ada
        paparKeteApiTersedia(pilihanLaluan);

        //4. Pengguna pilih kereta api
        bool validKeteApi = false;
        while (!validKeteApi) {
            cout << "Masukkan ID kereta Api (atau 'B' untuk kembali): ";
            getline(cin, pilihanKeteApiID);
            transform(pilihanKeteApiID.begin(), pilihanKeteApiID.end(), pilihanKeteApiID.begin(), ::toupper);

            if (pilihanKeteApiID == "B") return;

            pilihanKeteApi = getKeteApiByID(pilihanKeteApiID);
            if (pilihanKeteApi && pilihanKeteApi->getLaluan() == pilihanLaluan) {
                validKeteApi = true;
            }
            else {
                cout << "Ralat: Kereta api tidak dijumpai atau tidak sesuai dengan laluan.\n";
            }
        }


        // 7. Dapatkan jumlah penumpang
        int jumlahDewasa, jumlahBudak, jumlahTua;
        cout << "Masukkan bilangan penumpang dewasa: ";
        cin >> jumlahDewasa;
        cout << "Masukkan bilangan penumpang kanak-kanak: ";
        cin >> jumlahBudak;
        cout << "Masukkan bilangan penumpang warga emas: ";
        cin >> jumlahTua;
        cin.ignore();

        // 7.5 dapatkan umur penumpang
        vector<int> umurPenumpang;
        for (int i = 0; i < jumlahDewasa; ++i) {
            int umur;
            do {
                cout << "Masukkan umur Dewasa " << (i + 1) << " (antara 18-59): ";
                cin >> umur;

                if (umur < 18 || umur > 59) {
                    cout << "Ralat: Umur mesti antara 18 dan 59. Sila cuba lagi.\n";
                }
            } while (umur < 18 || umur > 59);

            umurPenumpang.push_back(umur);
        }

        for (int i = 0; i < jumlahBudak; ++i) {
            int umur;
            do {
                cout << "Masukkan umur Kanak-kanak " << (i + 1) << " (antara 6-17): ";
                cin >> umur;

                if (umur < 6 || umur > 17) {
                    cout << "Ralat: Umur mesti antara 6 dan 17. Sila cuba lagi.\n";
                }
            } while (umur < 1 || umur > 17);

            umurPenumpang.push_back(umur);
        }

        for (int i = 0; i < jumlahTua; i++) {
            int umur;
            do {
                cout << "Masukkan umur Warga Emas " << (i + 1) << " (60 dan ke atas): ";
                cin >> umur;

                if (umur < 60 || umur > 120) {
                    cout << "Ralat: Umur mesti 60 dan ke atas. Sila cuba lagi.\n";
                }
            } while (umur < 60 || umur > 120);

            umurPenumpang.push_back(umur);
        }

        cin.ignore();

        // --- START OF REVAMPED SEAT SELECTION ---
    

    // Get available carriages for the chosen train
        vector<shared_ptr<Gerabak>> availableCarriages = pilihanKeteApi->getGerabak();

        // Filter out carriages with no available seats for the selected month
        availableCarriages.erase(remove_if(availableCarriages.begin(), availableCarriages.end(),
            [&](const shared_ptr<Gerabak>& g) {
                for (int r = 1; r <= g->getNumRows(); ++r) {
                    for (char c = 'A'; c < 'A' + g->getNumCols(); ++c) {
                        if (!g->getSeat(r, c)->isDitempah(bulan)) {
                            return false; // Carriage has at least one available seat
                        }
                    }
                }
                return true; // No available seats found
            }), availableCarriages.end());

        // Check if any seats are available on the train
        if (availableCarriages.empty()) {
            cout << "Maaf, tiada tempat duduk tersedia di dalam kereta api ini.\n";
            return; // No seats available on the entire train
        }

        // Initialize carriage index and selected seats vector
        int currentCarriageIndex = 0;
        vector<pair<int, char>> kerusiDipilih;

        // Seat selection loop
        int jumlahPenumpang = jumlahDewasa + jumlahBudak + jumlahTua; // Calculate once
        while (kerusiDipilih.size() < jumlahPenumpang) {
            shared_ptr<Gerabak> currentCarriage = availableCarriages[currentCarriageIndex];

            // Display current carriage's seat layout
            paparPilihanKerusi(currentCarriage, bulan);

            // Get user input for seat selection or navigation
            string input;
            cout << "Pilih tempat duduk (contoh: 1A), 'N' untuk seterusnya, atau 'B' untuk kembali: ";
            getline(cin, input);

            if (input == "N" || input == "n") {
                // Move to the next carriage (if available)
                if (currentCarriageIndex < availableCarriages.size() - 1) {
                    currentCarriageIndex++;
                }
                else {
                    cout << "Tiada lagi gerabak seterusnya.\n";
                }
            }
            else if (input == "B" || input == "b") {
                // Move to the previous carriage (if available)
                if (currentCarriageIndex > 0) {
                    currentCarriageIndex--;
                }
                else {
                    cout << "Tiada lagi gerabak sebelumnya.\n";
                }
            }
            else {
                // Attempt to parse seat selection
                try {
                    if (input.length() >= 2) {
                        int row = stoi(input.substr(0, input.length() - 1));
                        char col = toupper(input.back());

                        // Validate row and column
                        if (row >= 1 && row <= currentCarriage->getNumRows() &&
                            col >= 'A' && col < 'A' + currentCarriage->getNumCols()) {

                            auto seat = currentCarriage->getSeat(row, col);
                            if (seat && !seat->isDitempah(bulan)) {
                                // Seat is available
                                kerusiDipilih.emplace_back(row, col);
                                seat->tempah(bulan); // Reserve the seat temporarily
                                cout << "Kerusi " << row << col << " berjaya dipilih.\n";
                            }
                            else {
                                cout << "Kerusi " << row << col << " telah ditempah atau tidak wujud.\n";
                            }
                        }
                        else {
                            cout << "Input tidak sah. Sila pastikan format yang betul (contoh: 1A).\n";
                        }
                    }
                    else {
                        cout << "Input tidak sah. Sila pastikan format yang betul (contoh: 1A).\n";
                    }
                }
                catch (const invalid_argument&) {
                    cout << "Input tidak sah. Sila masukkan format yang betul (contoh: 1A).\n";
                }
                catch (const out_of_range&) {
                    cout << "Input tidak sah. Nombor baris di luar julat.\n";
                }
            }
        }

        // --- END OF REVAMPED SEAT SELECTION ---

       
       

        // If all seats are selected, proceed to booking
        if (kerusiDipilih.size() == jumlahPenumpang) {
            // Payment method selection
            cout << "\nKaedah pembayaran tersedia:\n";
            for (size_t i = 0; i < KAEDAH_BAYARAN.size(); ++i) {
                cout << (i + 1) << ". " << KAEDAH_BAYARAN[i] << "\n";
            }

            int pilihanBayaran;
            string jenisBayaran;
            cout << "Pilih kaedah pembayaran (1-" << KAEDAH_BAYARAN.size() << "): ";
            while (!(cin >> pilihanBayaran) || pilihanBayaran < 1 || pilihanBayaran > KAEDAH_BAYARAN.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Sila pilih nombor antara 1-" << KAEDAH_BAYARAN.size() << ": ";
            }
            cin.ignore();

            jenisBayaran = KAEDAH_BAYARAN[pilihanBayaran - 1];

            // Create booking (use carriage ID from the last selected carriage)
            string bookingID = buatTempahanDenganPilihan(
                penggunaSemasa,
                pilihanLaluan,
                pilihanKeteApi,
                jumlahDewasa,
                jumlahBudak,
                jumlahTua,
                availableCarriages[currentCarriageIndex]->getID(), // Carriage ID
                kerusiDipilih,
                jenisBayaran,
                umurPenumpang,
                bulan
            );

            if (!bookingID.empty()) {
                cout << "Tempahan berjaya dengan ID: " << bookingID << "\n";
                paparButirTempahan(bookingID);

                // Find the booking info to generate receipt
                for (const auto& tempahan : infoPenempahan) {
                    if (tempahan.penempahanID == bookingID) {
                        simpanResit(tempahan);
                        break;
                    }
                }
            }
            else {
                cout << "Tempahan gagal.\n";
            }
        }
    }


    // Display booking details
    void paparButirTempahan(const string& penempahanID) const {
        for (const auto& tempahan : infoPenempahan) {
            if (tempahan.penempahanID == penempahanID) {
                cout << "\nButiran Tempahan" << endl;
                cout << "ID: " << tempahan.penempahanID << endl;
                cout << "Pelanggan: " << tempahan.pembeli->getNama() << endl;
                cout << "Tarikh Tempahan: " << tempahan.tarikhTempahan << endl; // Added date
                cout << "Masa Tempahan: " << tempahan.masaTempahan << endl;     // Added time
                cout << "Laluan: " << tempahan.laluan->getNama() << endl;
                cout << "Kereta Api: " << tempahan.keteApi->getID() << endl;
                cout << "Jumlah Tiket:" << endl;
                cout << "  Dewasa: " << tempahan.jumlahDewasa << endl;
                cout << "  Kanak-kanak: " << tempahan.jumlahBudak << endl;
                cout << "  Warga Emas: " << tempahan.jumlahTua << endl;
                cout << "Jumlah Harga: RM " << fixed << setprecision(2) << tempahan.jumlahHarga << endl;
                return;
            }
        }
        cout << "Tempahan tidak dijumpai!" << endl;
    }

    void paparSejarahTempahan(shared_ptr<Pengguna> pengguna) const {
        cout << "\nSejarah Tempahan untuk " << pengguna->getNama() << ":\n";
        bool found = false;
        vector<string> availableIDs;  // Store valid booking IDs

        // Display all bookings
        for (const auto& tempahan : infoPenempahan) {
            if (tempahan.pembeli->getEmail() == pengguna->getEmail()) {
                found = true;
                availableIDs.push_back(tempahan.penempahanID);
                cout << "----------------------------------------\n";
                cout << "ID: " << tempahan.penempahanID << "\n";
                cout << "Tarikh: " << tempahan.tarikhTempahan << "\n";
                cout << "Laluan: " << tempahan.laluan->getLokasiMula()
                    << " -> " << tempahan.laluan->getLokasiAkhir() << "\n";
                cout << "Tarikh Tempahan: " << tempahan.tarikhTempahan << "\n";
                cout << "Masa Tempahan: " << tempahan.masaTempahan << "\n";
                cout << "Jumlah: RM" << fixed << setprecision(2)
                    << tempahan.jumlahHarga << "\n";
            }
        }

        if (!found) {
            cout << "Tiada rekod tempahan.\n";
            return;
        }

        // Receipt viewing interface
        while (true) {
            cout << "\nPilihan:"
                << "\n1. Buka resit tempahan"
                << "\n2. Batal tempahan"
                << "\n3. Kembali ke menu utama"
                << "\nPilihan: ";

            string pilihan;
            getline(cin, pilihan);

            if (pilihan == "1") {
                cout << "Masukkan ID Tempahan: ";
                string id;
                getline(cin, id);

                string namaFail = DIREKTORI_RESIT + "resit_" + id + ".txt";

#ifdef _WIN32
                system(("start \"\" \"" + namaFail + "\"").c_str());
#else
                system(("xdg-open \"" + namaFail + "\"").c_str());
#endif
            }
            else if (pilihan == "2") {
                cout << "Masukkan ID Tempahan untuk dibatalkan: ";
                string id;
                getline(cin, id);

                cout << "Adakah anda pasti mahu membatalkan tempahan ini? (Y/N): ";
                string confirm;
                getline(cin, confirm);

                if (confirm == "Y" || confirm == "y") {
                    const_cast<Penempahan*>(this)->batalTempahan(id);
                }
            }
            else if (pilihan == "3") {
                break;
            }
            else {
                cout << "Pilihan tidak sah!\n";
            }
        }
    }

    // Function to center text
    void centerText(ofstream& file, const string& text, int receiptWidth) {
        int padding = (receiptWidth - text.length()) / 2;
        file << string(max(0, padding), ' ') << text << "\n";
    }

    // function to center key-value pairs
    void centerKeyValue(ofstream& file, const string& key, const string& value, int receiptWidth) {
        string line = key + ": " + value;
        int padding = (receiptWidth - line.length()) / 2;
        file << string(max(0, padding), ' ') << line << "\n";
    }

    // SimpanResit
    void simpanResit(const InfoPenempahan& tempahan) {
        // Create directory if it doesn't exist
#ifdef _WIN32
        system("mkdir resit 2> nul");  // Windows
#else
        system("mkdir -p resit");      // Linux/Mac
#endif

        const int RECEIPT_WIDTH = 50;
        string namaFail = DIREKTORI_RESIT + "resit_" + tempahan.penempahanID + ".txt";
        cout << "Menyimpan ke: " << namaFail << endl;

        // Improved file opening with error checking
        ofstream file(namaFail);
        if (!file.is_open()) {
            cerr << "Ralat: Gagal membuka fail: " << namaFail << endl;
            return;
        }

        file << "\n\n";

        // Centered header
        centerText(file, "=======================", RECEIPT_WIDTH);
        centerText(file, "  RESIT TEMPAHAN", RECEIPT_WIDTH);
        centerText(file, "=======================", RECEIPT_WIDTH);
        file << "\n";

        // split tarikhTempahan into date and time
        string tarikh = tempahan.tarikhTempahan;
        string masa = tempahan.masaTempahan;

        // Centered content
        centerKeyValue(file, "ID Tempahan", tempahan.penempahanID, RECEIPT_WIDTH);
        centerKeyValue(file, "Nama Pembeli", tempahan.pembeli->getNama(), RECEIPT_WIDTH);
        centerKeyValue(file, "Email Pembeli", tempahan.pembeli->getEmail(), RECEIPT_WIDTH);
        centerKeyValue(file, "Tarikh Tempahan", tempahan.tarikhTempahan, RECEIPT_WIDTH);
        centerKeyValue(file, "Masa Tempahan", masa, RECEIPT_WIDTH);
        centerKeyValue(file, "Laluan", tempahan.laluan->getLokasiMula() + " -> " + tempahan.laluan->getLokasiAkhir(), RECEIPT_WIDTH);
        ostringstream hargaStream;
        hargaStream << std::fixed << std::setprecision(2) << tempahan.jumlahHarga;
        centerKeyValue(file, "Harga", "RM " + hargaStream.str(), RECEIPT_WIDTH);
        centerKeyValue(file, "Kaedah Bayaran", tempahan.jenisBayaran, RECEIPT_WIDTH);

        // Centered list of seats
        string kerusiList;
        for (size_t i = 0; i < tempahan.kerusiTempahan.size(); ++i) {
            kerusiList += tempahan.kerusiTempahan[i];
            if (i < tempahan.kerusiTempahan.size() - 1) {
                kerusiList += ", ";
            }
        }
        centerKeyValue(file, "Kerusi Tempahan", kerusiList, RECEIPT_WIDTH);

        // Centered footer
        file << "\n\n";
        centerText(file, "=======================", RECEIPT_WIDTH);
        centerText(file, "  Terima Kasih!", RECEIPT_WIDTH);
        centerText(file, "=======================", RECEIPT_WIDTH);

        file.close();
        cout << "Resit berjaya disimpan di: " << namaFail << endl;

        // Open file automatically after saving with error handling
#ifdef _WIN32
        if (system(("start \"\" \"" + namaFail + "\"").c_str()) != 0) {
            cerr << "Ralat: Gagal membuka resit secara automatik" << endl;
        }
#else
        if (system(("xdg-open \"" + namaFail + "\"").c_str()) != 0) {
            cerr << "Ralat: Gagal membuka resit secara automatik" << endl;
        }
#endif
    }

    // Check kalau seat available
    bool isKerusiTersedia(const string& kerusi, shared_ptr<KeteApi> keteApi, int bulan) {
        // Check all existing bookings in memory
        for (const auto& tempahan : infoPenempahan) {
            if (tempahan.keteApi->getID() == keteApi->getID()) {
                // ambik bulan dari tarikh booking
                int bulanTempahan = stoi(tempahan.tarikhTempahan.substr(5, 2));

                if (bulanTempahan == bulan) {
                    // Tengok kalau seat dah ditempah
                    for (const auto& kerusiDitempah : tempahan.kerusiTempahan) {
                        if (kerusiDitempah == kerusi) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }


    void setLaluan(const vector<shared_ptr<Laluan>>& laluan) {
        this->senaraiLaluan = laluan;
    }

    void setKeteApi(const vector<shared_ptr<KeteApi>>& keteApi) {
        this->senaraiKeteApi = keteApi;
    }

    void setPengurusanDiskaun(shared_ptr<PengurusanDiskaun> pengDiskaun) {
        this->pengurusanDiskaun = pengDiskaun;
    }
};

// fungsi admin
class Admin {
private:
    string username;
    string password;
    shared_ptr<PengurusanDiskaun> pengurusanDiskaun;
    shared_ptr<Penempahan> penempahan;
    vector<shared_ptr<Laluan>> senaraiLaluan;
    vector<shared_ptr<KeteApi>> senaraiKeteApi;

public:
    Admin(string username, string password, shared_ptr<PengurusanDiskaun> pengDiskaun, shared_ptr<Penempahan> penemp) {
        this->username = username;
        this->password = password;
        this->pengurusanDiskaun = pengDiskaun;
        this->penempahan = penemp;
    }

    bool login(string username, string password) {
        return this->username == username && this->password == password;
    }

    // getter
    vector<shared_ptr<Laluan>>& getLaluan() {
        return senaraiLaluan;
    }

    vector<shared_ptr<KeteApi>>& getKeteApi() {
        return senaraiKeteApi;
    }

    // Route Management
    void tambahLaluan() {
        string id, dari, ke;
        double harga;
        vector<string> jadual;

        cout << "\nTambah Laluan Baru\n";
        cout << "ID Laluan: ";
        getline(cin, id);
        cout << "Dari: ";
        getline(cin, dari);
        cout << "Ke: ";
        getline(cin, ke);
        cout << "Harga (RM): ";
        cin >> harga;
        cin.ignore();

        string masa;
        cout << "Masukkan masa perjalanan (Tekan Enter tanpa input untuk selesai): \n";
        while (true) {
            getline(cin, masa);
            if (masa.empty()) break;
            jadual.push_back(masa);
        }

        auto laluan = make_shared<Laluan>(id, dari, ke, jadual, harga, true);

        // tambah bulan keberadaaan
        cout << "\nTetapkan bulan-bulan yang TIDAK tersedia:\n";
        cout << "Masukkan nombor bulan (1-12, -1 untuk selesai):\n";

        while (true) {
            int bulan;
            cout << "Bulan: ";
            cin >> bulan;

            if (bulan == -1) break;

            if (bulan >= 1 && bulan <= 12) {
                laluan->setBulanOperasi(bulan, false);
                cout << "Bulan " << bulan << " ditetapkan sebagai tidak tersedia.\n";
            }
            else {
                cout << "Ralat: Bulan mesti antara 1-12.\n";
            }
        }
        cin.ignore();

        senaraiLaluan.push_back(laluan);
        cout << "Laluan berjaya ditambah!\n";
    }

    // Pengurusan kereta api
    void tambahKeteApi() {
        string id;
        cout << "\nTambah Kereta Api Baru\n";
        cout << "ID Kereta Api: ";
        getline(cin, id);

        if (senaraiLaluan.empty()) {
            cout << "Ralat: Tiada laluan tersedia. Sila tambah laluan dahulu.\n";
            return;
        }

        // Display available routes
        cout << "\nLaluan tersedia:\n";
        for (size_t i = 0; i < senaraiLaluan.size(); ++i) {
            cout << (i + 1) << ". " << senaraiLaluan[i]->getNama() << "\n";
        }

        // Route selection with validation
        while (true) {
            string input;
            cout << "Pilih laluan (1-" << senaraiLaluan.size() << ") atau 'B' untuk kembali: ";
            getline(cin, input);

            // Check for return option
            if (input == "B" || input == "b") {
                cout << "Kembali ke menu utama.\n";
                return;
            }

            try {
                int pilihanLaluan = stoi(input);
                if (pilihanLaluan >= 1 && pilihanLaluan <= static_cast<int>(senaraiLaluan.size())) {
                    auto keteApi = make_shared<KeteApi>(id, true);
                    keteApi->setLaluan(senaraiLaluan[pilihanLaluan - 1]);
                    senaraiKeteApi.push_back(keteApi);
                    cout << "Kereta api berjaya ditambah!\n";
                    return;
                }
                else {
                    cout << "Ralat: Sila pilih nombor antara 1-" << senaraiLaluan.size() << ".\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Ralat: Sila masukkan nombor sahaja.\n";
            }
            catch (const out_of_range&) {
                cout << "Ralat: Nombor terlalu besar.\n";
            }
        }
    }

    // Pengurusan diskaun
    void urusDiskaun() {
        cout << "\nUrusan Diskaun\n";
        cout << "1. Tambah Diskaun Kategori\n";
        cout << "2. Tambah Diskaun Bayaran\n";
        cout << "3. Tambah Diskaun Musim\n";

        int pilihan;
        cout << "Pilihan: ";

        while (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ralat: Input tidak sah. Sila masukkan nombor (1-3): ";

        }
        cin.ignore();

        switch (pilihan) {
        case 1: {
            string nama;
            double kadar;
            int umurMin, umurMax;

            cout << "Nama kategori: ";
            getline(cin, nama);

            cout << "Kadar diskaun (0.0-1.0): ";
            while (!(cin >> kadar) || kadar < 0.0 || kadar > 1.0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Kadar mesti antara 0.0-1.0. Sila cuba lagi: ";
            }

            cout << "Umur minimum: ";
            while (!(cin >> umurMin) || umurMin < 0 || umurMin > 120) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Umur minimum mesti antara 0-120. Sila cuba lagi: ";
            }

            cout << "Umur maximum: ";
            while (!(cin >> umurMax) || umurMax < umurMin || umurMax > 120) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Umur maximum mesti antara " << umurMin << "-120. Sila cuba lagi: ";
            }

            pengurusanDiskaun->tambahDiskaun(nama, kadar);
            cout << "Diskaun kategori berjaya ditambah!\n";
            cin.ignore();
            break;
        }
        case 2: {
            string nama;
            double kadar = 0.0;

            cout << "\nTambah Diskaun untuk Kaedah Pembayaran\n";
            cout << "Kaedah pembayaran tersedia:\n";
            auto kaedahBayaran = penempahan->getKaedahBayaran();
            for (size_t i = 0; i < kaedahBayaran.size(); ++i) {
                cout << (i + 1) << ". " << kaedahBayaran[i] << "\n";
            }

            int pilihanBayaran;
            cout << "Pilih kaedah pembayaran (1-" << kaedahBayaran.size() << "): ";
            while (!(cin >> pilihanBayaran) || pilihanBayaran < 1 || pilihanBayaran > kaedahBayaran.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Sila pilih nombor antara 1-" << kaedahBayaran.size() << ": ";
            }
            cin.ignore();

            string jenisBayaran = kaedahBayaran[pilihanBayaran - 1];

            cout << "Kadar diskaun (0.0-1.0): ";
            while (!(cin >> kadar) || kadar < 0.0 || kadar > 1.0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Kadar mesti antara 0.0-1.0. Sila cuba lagi: ";
            }
            cin.ignore();

            pengurusanDiskaun->tambahDiskaunBayaran(jenisBayaran, kadar);
            cout << "Diskaun pembayaran berjaya ditambah!\n";
            break;
        }
        case 3: {
            string nama;
            double kadar;
            int bulanMula, bulanAkhir;

            cout << "Nama kategori: ";
            getline(cin, nama);

            cout << "Kadar diskaun (0.0-1.0): ";
            while (!(cin >> kadar) || kadar < 0.0 || kadar > 1.0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Kadar mesti antara 0.0-1.0. Sila cuba lagi: ";
            }

            cout << "Bulan pertama diskaun (1-12): ";
            while (!(cin >> bulanMula) || bulanMula < 1 || bulanMula > 12) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Bulan mesti antara 1-12. Sila cuba lagi: ";
            }

            cout << "Bulan terakhir diskaun (1-12): ";
            while (!(cin >> bulanAkhir) || bulanAkhir < 1 || bulanAkhir > 12 || bulanAkhir < bulanMula) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ralat: Bulan mesti antara " << bulanMula << "-12. Sila cuba lagi: ";
            }

            pengurusanDiskaun->tambahDiskaunMusim(nama, kadar, bulanMula, bulanAkhir);
            cout << "Diskaun musim berjaya ditambah!\n";
            cin.ignore();
            break;
        }


        }
    }

    // Add to Admin class
    void tambahGerabak() {
        if (senaraiKeteApi.empty()) {
            cout << "Ralat: Tiada kereta api tersedia. Sila tambah kereta api dahulu.\n";
            return;
        }

        cout << "\nTambah Gerabak Baru\n";

        // Show available trains
        cout << "Kereta Api Tersedia:\n";
        for (size_t i = 0; i < senaraiKeteApi.size(); ++i) {
            cout << (i + 1) << ". " << senaraiKeteApi[i]->getID() << "\n";
        }

        // Train selection with validation
        int pilihanKeteApi;
        while (true) {
            string input;
            cout << "Pilih kereta api (1-" << senaraiKeteApi.size() << ") atau 'B' untuk kembali: ";
            getline(cin, input);

            if (input == "B" || input == "b") {
                cout << "Kembali ke menu utama.\n";
                return;
            }

            try {
                pilihanKeteApi = stoi(input);
                if (pilihanKeteApi >= 1 && pilihanKeteApi <= static_cast<int>(senaraiKeteApi.size())) {
                    break;
                }
                else {
                    cout << "Ralat: Sila pilih nombor antara 1-" << senaraiKeteApi.size() << ".\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Ralat: Sila masukkan nombor sahaja.\n";
            }
            catch (const out_of_range&) {
                cout << "Ralat: Nombor terlalu besar.\n";
            }
        }
        shared_ptr<KeteApi> selectedTrain = senaraiKeteApi[pilihanKeteApi - 1];

        // dapatkan nombor gerabak untuk ditambah
        int numCarriagesToAdd;
        while (true) {
            string input;
            cout << "Bilangan gerabak untuk ditambah: ";
            getline(cin, input);
            try {
                numCarriagesToAdd = stoi(input);
                if (numCarriagesToAdd > 0) {
                    break;
                }
                else {
                    cout << "Ralat: Bilangan gerabak mesti lebih daripada 0.\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Ralat: Sila masukkan nombor sahaja.\n";
            }
            catch (const out_of_range&) {
                cout << "Ralat: Nombor terlalu besar.\n";
            }
        }

        // Dapatkan base carriage id
        string baseIdGerabak;
        cout << "ID Gerabak asas (Jika mula dari satu, tidak perlu masukkan nombor, hanya huruf, contoh 'G'): ";
        getline(cin, baseIdGerabak);

        int baris, lajur;

        while (true) {
            string input;
            cout << "Bilangan Baris: ";
            getline(cin, input);
            try {
                baris = stoi(input);
                if (baris > 0 && baris <= 20) {
                    break;
                }
                else {
                    cout << "Ralat: Bilangan baris mesti antara 1-20\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Ralat: Sila masukkan nombor sahaja.\n";
            }
            catch (const out_of_range&) {
                cout << "Ralat: Nombor terlalu besar.\n";
            }
        }

        while (true) {
            string input;
            cout << "Bilangan Lajur: ";
            getline(cin, input);
            try {
                lajur = stoi(input);
                if (lajur > 0 && lajur <= 6) { // Assuming max 6 columns (A-F)
                    break;
                }
                else {
                    cout << "Ralat: Bilangan lajur mesti antara 1-6.\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Ralat: Sila masukkan nombor sahaja.\n";
            }
            catch (const out_of_range&) {
                cout << "Ralat: Nombor terlalu besar.\n";
            }
        }

        // Track gerabak berjaya ditambah atau tak
        bool anyAdded = false;

        // loop untuk tambah gerabak
        for (int i = 1; i <= numCarriagesToAdd; ++i) {
            string idGerabak = baseIdGerabak + to_string(i);
            auto gerabak = make_shared<Gerabak>(idGerabak, baris, lajur);

            // tengok kalau gerabak dengan ID sama dah wujud dalam kereta api
            bool gerabakExists = false;
            for (const auto& existingGerabak : selectedTrain->getGerabak()) {
                if (existingGerabak->getID() == idGerabak) {
                    gerabakExists = true;
                    break;
                }
            }
            if (!gerabakExists) {
                selectedTrain->tambahGerabak(gerabak);
                cout << "Gerabak " << idGerabak << " berjaya ditambah ke kereta api " << selectedTrain->getID() << ".\n";
                anyAdded = true;
            }
            else {
                cout << "Amaran: Gerabak dengan ID " << idGerabak << " sudah wujud dalam kereta api " << selectedTrain->getID() << ". Tidak dapat menambah.\n";
            }
        }

        // Display appropriate message based on whether any carriages were added
        if (anyAdded) {
            cout << "Gerabak berjaya ditambah (batch process).\n";
        }
        else {
            cout << "Tiada gerabak berjaya ditambah.\n";
        }
    }

    // Admin Menu
    void paparMenu(shared_ptr<Penempahan> penempahan) {
        while (true) {
            // Clear any previous input
            cin.clear();

            // Display menu once per loop
            cout << "\n\tMenu Admin\n";
            cout << "1. Tambah Laluan\n";
            cout << "2. Tambah Kereta Api\n";
            cout << "3. Tambah Gerabak\n";
            cout << "4. Urus Diskaun\n";
            cout << "5. Log Keluar\n";

            string input;
            int pilihan;

            cout << "Pilihan: ";
            getline(cin, input);

            // Try to convert input to integer
            try {
                pilihan = stoi(input);
                if (pilihan >= 1 && pilihan <= 5) {
                    // Valid input - process menu choice
                    switch (pilihan) {
                    case 1:
                        tambahLaluan();
                        break;
                    case 2:
                        tambahKeteApi();
                        break;
                    case 3:
                        tambahGerabak();
                        break;
                    case 4:
                        urusDiskaun();
                        break;
                    case 5:
                        cout << "Log keluar berjaya.\n";
                        return;
                    }
                }
                else {
                    cout << "Ralat: Sila masukkan nombor antara 1-5.\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Ralat: Sila masukkan nombor sahaja.\n";
            }
            catch (const out_of_range&) {
                cout << "Ralat: Nombor terlalu besar.\n";
            }
        }
    }
};

// Main function
int main() {
    auto pengDiskaun = make_shared<PengurusanDiskaun>();
    auto penempahan = make_shared<Penempahan>(pengDiskaun);
    Admin admin("admin", "admin123", pengDiskaun, penempahan);

    // Add default data directly to vectors
    vector<shared_ptr<Laluan>>& adminLaluan = admin.getLaluan();
    vector<shared_ptr<KeteApi>>& adminKeteApi = admin.getKeteApi();

    // Add routes
    adminLaluan.push_back(make_shared<Laluan>("L1", "Melaka", "Kuala Lumpur",
        vector<string>{"08:00", "12:00"}, 100.00, true));
    adminLaluan.back()->setBulanOperasi(1, false); // Tak available untuk januari
    adminLaluan.back()->setBulanOperasi(2, false); // Tak available untuk Februari

    adminLaluan.push_back(make_shared<Laluan>("L2", "Johor", "Kedah",
        vector<string>{"09:00", "13:00"}, 350.50, true));
    adminLaluan.back()->setBulanOperasi(11, false); // Tak available untuk bulan nov
    adminLaluan.back()->setBulanOperasi(12, false); // Tak available untuk bulan Dec

    adminLaluan.push_back(make_shared<Laluan>("L3", "Perlis", "Selangor",
        vector<string>{"10:00", "15:00"}, 250.50, true));
    adminLaluan.back()->setBulanOperasi(5, false); // Tak available untuk bulan may
    adminLaluan.back()->setBulanOperasi(10, false); // Tak available untuk bulan okt

    adminLaluan.push_back(make_shared<Laluan>("L4", "Pahang", "Perak",
        vector<string>{"8:00", "14:00"}, 150.00, true));
    adminLaluan.back()->setBulanOperasi(9, false); // Tak available untuk bulan sept

    adminLaluan.push_back(make_shared<Laluan>("L5", "Pulau Penang", "Negeri Sembilan",
        vector<string>{"8:00", "14:00"}, 150.00, true));
    adminLaluan.back()->setBulanOperasi(12, false); // Tak available untuk bulan dec

    adminLaluan.push_back(make_shared<Laluan>("L6", "Kelantan", "Thailand",
        vector<string>{"7:00", "12:00"}, 125.80, true));
    adminLaluan.back()->setBulanOperasi(11, false); // Tak available untuk bulan nov

    // Add trains
    auto keteApi1 = make_shared<KeteApi>("KA1", true);
    keteApi1->setLaluan(adminLaluan[0]);  // Link to L1
    auto keteApi2 = make_shared<KeteApi>("KA2", true);
    keteApi2->setLaluan(adminLaluan[1]);  // Link to L2
    auto keteApi3 = make_shared<KeteApi>("KA3", true);
    keteApi3->setLaluan(adminLaluan[2]);  // Link to L3
    auto keteApi4 = make_shared<KeteApi>("KA4", true);
    keteApi4->setLaluan(adminLaluan[3]);  // Link to L4
    auto keteApi5 = make_shared<KeteApi>("KA5", true);
    keteApi5->setLaluan(adminLaluan[4]);  // Link to L5
    auto keteApi6 = make_shared<KeteApi>("KA6", true);
    keteApi6->setLaluan(adminLaluan[5]);  // Link to L6

    adminKeteApi.push_back(keteApi1);
    adminKeteApi.push_back(keteApi2);
    adminKeteApi.push_back(keteApi3);
    adminKeteApi.push_back(keteApi4);
    adminKeteApi.push_back(keteApi5);
    adminKeteApi.push_back(keteApi6);

    // Add carriages
    auto gerabak1 = make_shared<Gerabak>("G1", 10, 4);
    auto gerabak2 = make_shared<Gerabak>("G2", 8, 4);
    auto gerabak3 = make_shared<Gerabak>("G3", 10, 4);
    auto gerabak4 = make_shared<Gerabak>("G4", 9, 4);
    auto gerabak5 = make_shared<Gerabak>("G5", 10, 4);

    //KA1
    keteApi1->tambahGerabak(gerabak1);
    keteApi1->tambahGerabak(gerabak2);
    keteApi1->tambahGerabak(gerabak3);
    keteApi1->tambahGerabak(gerabak4);
    keteApi1->tambahGerabak(gerabak5);

    //KA2
    keteApi2->tambahGerabak(gerabak1);
    keteApi2->tambahGerabak(gerabak2);
    keteApi2->tambahGerabak(gerabak3);
    keteApi2->tambahGerabak(gerabak4);
    keteApi2->tambahGerabak(gerabak5);

    //KA3
    keteApi3->tambahGerabak(gerabak1);
    keteApi3->tambahGerabak(gerabak2);
    keteApi3->tambahGerabak(gerabak3);
    keteApi3->tambahGerabak(gerabak4);
    keteApi3->tambahGerabak(gerabak5);

    //KA4
    keteApi4->tambahGerabak(gerabak1);
    keteApi4->tambahGerabak(gerabak2);
    keteApi4->tambahGerabak(gerabak3);
    keteApi4->tambahGerabak(gerabak4);
    keteApi4->tambahGerabak(gerabak5);

    //KA5
    keteApi5->tambahGerabak(gerabak1);
    keteApi5->tambahGerabak(gerabak2);
    keteApi5->tambahGerabak(gerabak3);
    keteApi5->tambahGerabak(gerabak4);
    keteApi5->tambahGerabak(gerabak5);

    //KA6
    keteApi6->tambahGerabak(gerabak1);
    keteApi6->tambahGerabak(gerabak2);
    keteApi6->tambahGerabak(gerabak3);
    keteApi6->tambahGerabak(gerabak4);
    keteApi6->tambahGerabak(gerabak5);

    // Sync with booking system
    penempahan->setLaluan(adminLaluan);
    penempahan->setKeteApi(adminKeteApi);
    penempahan->setPengurusanDiskaun(pengDiskaun);

    // --- User Registration/Login ---

    bool running = true;
    while (running) {
        cout << "Selamat Datang ke Sistem Kereta api ETS!\n";
        cout << "--------------------------------\n";
        cout << "1. Daftar Pengguna\n";
        cout << "2. Log Masuk\n";
        cout << "3. Log masuk Admin\n";
        cout << "4. Lihat Sejarah Tempahan\n";
        cout << "5. Keluar\n";
        cout << "Pilihan: ";

        string input;
        getline(cin, input);

        try {
            int pilihan = stoi(input);

            switch (pilihan) {
            case 1: {
                string nama, email, password;
                int umur;

                cout << "\nDaftar Pengguna Baru\n";
                cout << "-------------------\n";
                cout << "Nama: ";
                getline(cin, nama);
                cout << "Emel: ";
                getline(cin, email);
                cout << "Kata Laluan: ";
                getline(cin, password);
                cout << "Umur: ";
                getline(cin, input);

                try {
                    umur = stoi(input);
                    if (penempahan->daftarPengguna(nama, email, password, umur)) {
                        cout << "\nPendaftaran berjaya!\n";
                    }
                    else {
                        cout << "\nPendaftaran gagal. Sila cuba lagi.\n";
                    }
                }
                catch (...) {
                    cout << "\nRalat: Umur tidak sah.\n";
                }
                cout << "\nTekan Enter untuk teruskan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 2: {
                string email, password;
                cout << "\nLog Masuk Pengguna\n";
                cout << "----------------\n";
                cout << "Emel: ";
                getline(cin, email);
                cout << "Kata Laluan: ";
                getline(cin, password);

                auto pengguna = penempahan->loginUser(email, password);
                if (pengguna) {
                    penempahan->prosesTempahan(pengguna);
                }
                else {
                    cout << "Log masuk gagal. Sila cuba lagi.\n";
                }
                cout << "\nTekan Enter untuk teruskan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 3: {
                string username, password;
                cout << "\nLog Masuk Admin\n";
                cout << "--------------\n";
                cout << "Username: ";
                getline(cin, username);
                cout << "Kata Laluan: ";
                getline(cin, password);

                if (admin.login(username, password)) {
                    cout << "Log masuk admin berjaya!\n";
                    admin.paparMenu(penempahan);
                    // Connect admin data to booking system
                    penempahan->setLaluan(admin.getLaluan());
                    penempahan->setKeteApi(admin.getKeteApi());
                }
                else {
                    cout << "Log masuk gagal. Sila cuba lagi.\n";
                }
                cout << "\nTekan Enter untuk teruskan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 4: {
                string email, password;
                cout << "\nLog Masuk untuk Lihat Sejarah\n";
                cout << "--------------------------\n";
                cout << "Emel: ";
                getline(cin, email);
                cout << "Kata Laluan: ";
                getline(cin, password);

                auto pengguna = penempahan->loginUser(email, password);
                if (pengguna) {
                    penempahan->paparSejarahTempahan(pengguna);
                }
                else {
                    cout << "Log masuk gagal. Sila cuba lagi.\n";
                }
                cout << "\nTekan Enter untuk teruskan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 5:
                cout << "\nTerima kasih! Jumpa lagi.\n";
                running = false;
                break;
            default:
                cout << "Pilihan tidak sah!\n";
                cout << "\nTekan Enter untuk teruskan...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        catch (const invalid_argument&) {
            cout << "Ralat: Sila masukkan nombor sahaja.\n";
            cout << "\nTekan Enter untuk teruskan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (const out_of_range&) {
            cout << "Ralat: Nombor terlalu besar.\n";
            cout << "\nTekan Enter untuk teruskan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    }

    return 0;
}