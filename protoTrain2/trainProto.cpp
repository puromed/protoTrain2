// trainProto.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <memory> // Smart pointer enabling
#include <regex> // email verification
#include <map>


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
    bool ditempah;        // Reservation status

public:
    // Constructor
    TempatDuduk(int r, char c)
        : row(r), column(c), ditempah(false) {
    }

    // Getters
    int getRow() const { return row; }
    char getColumn() const { return column; }
    bool isDitempah() const { return ditempah; }

    // Seat reservation methods
    void tempah() { ditempah = true; }       // Reserve the seat
    void batalTempah() { ditempah = false; } // Cancel reservation
};

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
    bool tempahKerusi(int row, char column) {
        auto kerusi = getSeat(row, column);
        if (kerusi && !kerusi->isDitempah()) {
            kerusi->tempah();
            cout << "Kerusi " << row << column << " Telah berjaya untuk ditempah.\n";
            return true; // Reservation successful
        }
        cout << "Seat " << row << column << " Kerusi telah ditempah atau penempahan tidak sah.\n";
        return false; // Reservation failed
    }

    void paparkanTempatDuduk() const {
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
                char statusKerusi = tempatDuduk[i][j]->isDitempah() ? 'X' : 'O';
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

    // TODO [TAMBAH ATRIBUT BARU - 1]:
  /*
  Tambah atribut baru di sini (selepas deklarasi vector dan sebelum struct):

  const string DIREKTORI_RESIT = "resit/";  // Direktori untuk simpan resit
  vector<string> failResit;                 // Senarai nama fail resit
  */


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

    bool validatePilihanKerusi(const PilihanKerusi& pilihanKerusi, int totalPenumpang,
        shared_ptr<KeteApi> keteApi) {
        // check kalau bilangan kerusi yang dipilih sepadan 
        if (pilihanKerusi.kerusi.size() != totalPenumpang) {
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
            if (tempatDuduk->isDitempah()) {
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

    // TODO [TEMPAT TAMBAH METHOD BARU - 1]: 
    /*
    Letakkan method untuk menyimpan resit di sini (selepas constructor, sebelum daftarPengguna):

    void simpanResit(const InfoPenempahan& tempahan) {
        // 1. Buat nama fail: "resit_" + tempahan.penempahanID + ".txt"
        // 2. Buka fail untuk penulisan (ofstream)
        // 3. Tulis semua maklumat tempahan dalam format yang kemas
        // 4. Tutup fail
    }

    void paparSejarahTempahan(shared_ptr<Pengguna> pengguna) {
        // 1. Tapis infoPenempahan untuk pengguna ini sahaja
        // 2. Papar senarai tempahan
        // 3. Benar pengguna pilih tempahan untuk lihat butiran penuh
    }
    */

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

    void paparPilihanKerusi(const shared_ptr<Gerabak>& gerabak) const {
        gerabak->paparkanTempatDuduk();
    }

    // paparkan kerusi yang ditempah
    void paparPilihanKerusiDenganPilihan(const shared_ptr<Gerabak>& gerabak, const vector<pair<int, char>>& kerusiDipilih) const {
        cout << "Gerabak " << gerabak->getID() << " - Visualisasi Tempat Duduk:\n";
        cout << " ";
        for (int j = 0; j < gerabak->getNumCols(); ++j) {
            char hurufColumn = 'A' + j;
            cout << hurufColumn << "  ";
            if (j == gerabak->getNumCols() / 2 - 1) {
                cout << "  ";  // lorong antara kerusi 
            }
        }
        cout << "\n";

        for (int i = 0; i < gerabak->getNumRows(); ++i) {
            cout << setw(2) << (i + 1) << " ";
            for (int j = 0; j < gerabak->getNumCols(); ++j) {
                char currentCol = 'A' + j;
                bool isSelected = false;
                for (const auto& seat : kerusiDipilih) {
                    if (seat.first == (i + 1) && seat.second == currentCol) {
                        isSelected = true;
                        break;
                    }
                }
                char statusKerusi;
                if (isSelected) {
                    statusKerusi = 'S'; // s untuk selected
                }
                else {
                    statusKerusi = gerabak->getSeat(i + 1, currentCol)->isDitempah() ? 'X' : 'O';
                }
                cout << statusKerusi << " ";
                if (j == gerabak->getNumCols() / 2 - 1) {
                    cout << "| "; //jurang lorong
                }
            }
            cout << "\n";
        }
    }



    // **Wrapper Method for PilihanKerusi**
    string buatTempahanDenganPilihan(shared_ptr<Pengguna> pembeli, shared_ptr<Laluan> laluan, shared_ptr<KeteApi> keteApi,
        int dewasa, int budak, int tua, const string& gerabakID, const vector<pair<int, char>>& kerusiDipilih,
        const string& jenisBayaran, const vector<int>& umurPenumpang, int bulan) {

        // buat PilihanKerusi
        PilihanKerusi pilihan;
        pilihan.gerabakID = gerabakID;
        pilihan.kerusi = kerusiDipilih;

        // Proceed with booking
        return buatTempahan(pembeli, laluan, keteApi, dewasa, budak, tua, pilihan, jenisBayaran, umurPenumpang, bulan);
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
        if (!validatePilihanKerusi(pilihanKerusi, jumlahPenumpang, keteApi)) {
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
            gerabak->tempahKerusi(seat.first, seat.second);
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

        // TODO [PANGGIL SIMPAN RESIT - 1]:
       /*
       Letakkan kod ini selepas infoPenempahan.push_back(tempahan):

       // Simpan resit selepas tempahan berjaya
       simpanResit(tempahan);
       */
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

        // 5. paparkan gerabak yang ada untuk kereta api
        paparGerabakTersedia(pilihanKeteApi);

        // 6. Pengguna pilih gerabak
        bool validGerabak = false;
        while (!validGerabak) {
            cout << "Masukkan ID gerabak (atau 'B' untuk kembali): ";
            getline(cin, pilihanGerabakID);
            transform(pilihanGerabakID.begin(), pilihanGerabakID.end(), pilihanGerabakID.begin(), ::toupper);

            if (pilihanGerabakID == "B") return;

            pilihanGerabak = getGerabakbyID(pilihanKeteApi, pilihanGerabakID);
            if (pilihanGerabak) {
                validGerabak = true;
            }
            else {
                cout << "Ralat: Gerabak tidak dijumpai. Sila cuba lagi.\n";
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
            cout << "Sila masukkan umur anda: ";
            cin >> umur;
            umurPenumpang.push_back(umur);
        }

        for (int i = 0; i < jumlahBudak; ++i) {
            int umur;
            cout << "Masukkan umur kanak-kanak " << (i + 1) << ": ";
            cin >> umur;
            umurPenumpang.push_back(umur);
        }

        for (int i = 0; i < jumlahTua; i++) {
            int umur;
            cout << "Masukkan umur warga emas " << (i + 1) << ": ";
            cin >> umur;

            umurPenumpang.push_back(umur);
        }

        cin.ignore();

        bool puasHati = false;
        vector<pair<int, char>> kerusiDipilih;

        while (!puasHati) {
            // 8. tunjuk pilihan kerusi
            paparPilihanKerusi(pilihanGerabak);

            // 9. Pengguna pilih kerusi
            kerusiDipilih.clear();

            int jumlahPenumpang = jumlahDewasa + jumlahBudak + jumlahTua;
            for (int i = 0; i < jumlahPenumpang; i++) {
                int row;
                char col;
                bool validInput = false;

                while (!validInput) {
                    cout << "Masukkan baris kerusi" << (i + 1) << ": ";
                    if (!(cin >> row)) { // Kalau input bukan numeric
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Ralat: Sila masukkan nombor sahaja untuk baris.\n";
                        continue;
                    }

                    cout << "Masukkan lajur Kerusi " << (i + 1) << " (A-D): ";
                    cin >> col;
                    col = toupper(col);

                    // Validate pilihan kerusi
                    if (row < 1 || row > pilihanGerabak->getNumRows()) {
                        cout << "Ralat: Baris mesti antara 1-" << pilihanGerabak->getNumRows() << ".\n";
                    }
                    else if (col < 'A' || col > 'D') {
                        cout << "Ralat: Lajur mesti antara A-D.\n";
                    }
                    else {
                        // Check for duplicate seats
                        auto seat = make_pair(row, col);
                        if (find(kerusiDipilih.begin(), kerusiDipilih.end(), seat) != kerusiDipilih.end()) {
                            cout << "Ralat: Kerusi " << row << col << " telah dipilih. Sila pilih kerusi lain.\n";
                            validInput = false;
                        }
                        else {
                            validInput = true;
                        }
                    }

                    cin.ignore();
                }
                kerusiDipilih.emplace_back(row, col);

            }


            //10. paparkan visualisasi kerusi yang dipilih
            paparPilihanKerusiDenganPilihan(pilihanGerabak, kerusiDipilih);

            //11. Tanya kepuasan pengguna
            string respon;
            cout << "Adakah anda puas hati dengan pilihan kerusi? (y/n): ";
            getline(cin, respon);

            if (respon == "y" || respon == "Y") {
                puasHati = true;
            }
            else {
                cout << "Sila pilih semula kerusi.\n";
            }
        }

        //  Cara pembayaran
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





        // Continue with booking process...
        string bookingID = buatTempahanDenganPilihan(
            penggunaSemasa,
            pilihanLaluan,
            pilihanKeteApi,
            jumlahDewasa,
            jumlahBudak,
            jumlahTua,
            pilihanGerabakID,
            kerusiDipilih,
            jenisBayaran,
            umurPenumpang,
            bulan
        );

        if (!bookingID.empty()) {
            cout << "Tempahan berjaya dengan ID: " << bookingID << "\n";
            paparButirTempahan(bookingID);
        }
        else {
            cout << "Tempahan gagal.\n";
        }
    }


    // Display booking details
    void paparButirTempahan(const string& penempahanID) const {
        for (const auto& tempahan : infoPenempahan) {
            if (tempahan.penempahanID == penempahanID) {
                cout << "\nButiran Tempahan" << endl;
                cout << "ID: " << tempahan.penempahanID << endl;
                cout << "Pelanggan: " << tempahan.pembeli->getNama() << endl;
                cout << "Laluan: " << tempahan.laluan->getNama() << endl;
                cout << "Kereta Api: " << tempahan.keteApi->getID() << endl;
                cout << "Jumlah Tiket:" << endl;
                cout << "  Dewasa: " << tempahan.jumlahDewasa << endl;
                cout << "  Kanak-kanak: " << tempahan.jumlahBudak << endl;
                cout << "  Warga Emas: " << tempahan.jumlahTua << endl;
                cout << "Jumlah Harga: RM" << tempahan.jumlahHarga << endl;
                return;
            }
        }
        cout << "Tempahan tidak dijumpai!" << endl;
    }

    // TODO [TEMPAT TAMBAH METHOD BARU - 2]:
    /*
    Letakkan method untuk membuka resit sedia ada di sini (selepas paparButirTempahan):

    void bukaTempahan(const string& penempahanID) {
        // 1. Cari fail resit berdasarkan ID
        // 2. Buka dan baca fail
        // 3. Papar kandungan kepada pengguna
    }
    */

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

        // tunjuk jalan tersedia
        cout << "\nLaluan tersedia:\n";
        for (size_t i = 0; i < senaraiLaluan.size(); ++i) {
            cout << (i + 1) << ". " << senaraiLaluan[i]->getNama() << "\n";
        }

        int pilihanLaluan;
        cout << "Pilih laluan (1-" << senaraiLaluan.size() << "): ";
        cin >> pilihanLaluan;
        cin.ignore();

        if (pilihanLaluan > 0 && pilihanLaluan <= senaraiLaluan.size()) {
            auto keteApi = make_shared<KeteApi>(id, true);
            keteApi->setLaluan(senaraiLaluan[pilihanLaluan - 1]);
            senaraiKeteApi.push_back(keteApi);
            cout << "Kereta api berjaya ditambah!\n";
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
            cout << "Tiada kereta api. Sila tambah kereta api dahulu.\n";
            return;
        }

        cout << "\nTambah Gerabak Baru\n";

        // Show available trains
        cout << "Kereta Api Tersedia:\n";
        for (size_t i = 0; i < senaraiKeteApi.size(); ++i) {
            cout << (i + 1) << ". " << senaraiKeteApi[i]->getID() << "\n";
        }

        int pilihanKeteApi;
        cout << "Pilih kereta api (1-" << senaraiKeteApi.size() << "): ";
        cin >> pilihanKeteApi;
        cin.ignore();

        if (pilihanKeteApi < 1 || pilihanKeteApi > senaraiKeteApi.size()) {
            cout << "Pilihan tidak sah!\n";
            return;
        }

        string idGerabak;
        int baris, lajur;
        cout << "ID Gerabak: ";
        getline(cin, idGerabak);
        cout << "Bilangan Baris: ";
        cin >> baris;
        cout << "Bilangan Lajur: ";
        cin >> lajur;
        cin.ignore();

        auto gerabak = make_shared<Gerabak>(idGerabak, baris, lajur);
        senaraiKeteApi[pilihanKeteApi - 1]->tambahGerabak(gerabak);
        cout << "Gerabak berjaya ditambah!\n";
    }

    // Admin Menu
    void paparMenu() {
        while (true) {
            cout << "\nMenu Admin\n";
            cout << "1. Tambah Laluan\n";
            cout << "2. Tambah Kereta Api\n";
            cout << "3. Tambah Gerabak\n";  // New option
            cout << "4. Urus Diskaun\n";
            cout << "5. Log Keluar\n";
            cout << "Pilihan: ";

            int pilihan;
            cin >> pilihan;
            cin.ignore();

            switch (pilihan) {
            case 1: tambahLaluan(); break;
            case 2: tambahKeteApi(); break;
            case 3: tambahGerabak(); break;
            case 4: urusDiskaun(); break;
            case 5: return;
            default: cout << "Pilihan tidak sah!\n";
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

    // Add trains
    auto keteApi1 = make_shared<KeteApi>("KA1", true);
    keteApi1->setLaluan(adminLaluan[0]);  // Link to L1
    auto keteApi2 = make_shared<KeteApi>("KA2", true);
    keteApi2->setLaluan(adminLaluan[1]);  // Link to L2

    adminKeteApi.push_back(keteApi1);
    adminKeteApi.push_back(keteApi2);

    // Add carriages
    auto gerabak1 = make_shared<Gerabak>("G1", 10, 4);
    auto gerabak2 = make_shared<Gerabak>("G2", 8, 4);
    keteApi1->tambahGerabak(gerabak1);
    keteApi2->tambahGerabak(gerabak2);

    // Sync with booking system
    penempahan->setLaluan(adminLaluan);
    penempahan->setKeteApi(adminKeteApi);
    penempahan->setPengurusanDiskaun(pengDiskaun);

    // --- User Registration/Login ---

    bool running = true;
    while (running) {
        cout << "\nSistem Tempahan Tiket Keretapi\n";
        cout << "1. Daftar Pengguna\n";
        cout << "2. Log Masuk \n";
        cout << "3. Log masuk Admin\n";
        cout << "4. Keluar\n";

        // TODO [TAMBAH MENU BARU - 1]:
       /*
       Tambah pilihan menu baru sebelum "Keluar":
       cout << "4. Lihat Sejarah Tempahan\n";

       5. boleh jadi keluar
       */

        cout << "Pilihan: ";

        string input;
        getline(cin, input);

        // validate input sebagai nombor
        bool isValid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        if (!isValid || input.empty()) {
            cout << "Ralat: Sila masukkan nombor sahaja (1-4).\n";
            continue;
        }

        int pilihan = stoi(input);

        if (pilihan < 1 || pilihan > 4) {
            cout << "Ralat: Sila pilih nombor antara 1-4.\n";
            continue;
        }

        switch (pilihan) {
        case 1: {
            // register pengguna
            string nama, email, password;
            int umur;
            cout << "Nama: ";
            getline(cin, nama);
            cout << "Emel: ";
            getline(cin, email);
            cout << "Kata Laluan: ";
            getline(cin, password);
            cout << "Umur: ";
            cin >> umur;
            cin.ignore();

            if (penempahan->daftarPengguna(nama, email, password, umur)) {
                cout << "Pendaftaran berjaya! Sila log masuk.\n";
            }
            break;
        }
        case 2: {
            string email, password;
            cout << "Emel: ";
            getline(cin, email);
            cout << "Kata Laluan: ";
            getline(cin, password);
            auto penggunaSemasa = penempahan->loginUser(email, password);
            if (penggunaSemasa) {
                penempahan->prosesTempahan(penggunaSemasa);
            }
            break;
        }
        case 3: {
            // admin login
            string username, password;
            cout << "Username Admin: ";
            getline(cin, username);
            cout << "Kata Laluan: ";
            getline(cin, password);

            if (admin.login(username, password)) {
                cout << "Log masuk admin berjaya!\n";
                admin.paparMenu();
                // Connect admin data to booking system
                penempahan->setLaluan(admin.getLaluan());
                penempahan->setKeteApi(admin.getKeteApi());
            }
            else {
                cout << "Log masuk admin gagal!\n";
            }
            break;
        }
        case 4:
            running = false;
            break;
        default:
            cout << "Pilihan tidak sah!\n";
        }

        // TODO [TAMBAH KES MENU BARU - 1]:
       /*
       Tambah case baru dalam switch statement:
       case 4: {
           string email, password;
           cout << "Emel: ";
           getline(cin, email);
           cout << "Kata Laluan: ";
           getline(cin, password);

           auto pengguna = penempahan->loginUser(email, password);
           if (pengguna) {
               penempahan->paparSejarahTempahan(pengguna);
           }
           break;
       }
       */
    }




    return 0;
}