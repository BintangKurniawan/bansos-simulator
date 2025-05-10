#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

struct Warga
{
    string nama;
    string alamat;
    string kategori;
};

struct RT
{
    string nama;
    vector<Warga> wargaList;
    RT(string nama) : nama(nama) {}

    void tambahWarga(const Warga &warga)
    {
        wargaList.push_back(warga);
    }

    void tampilkanWarga() const
    {
        if (wargaList.empty())
        {
            cout << "  Tidak ada warga terdaftar di " << nama << endl;
            return;
        }

        cout << "  Daftar Warga di " << nama << ":" << endl;
        for (size_t i = 0; i < wargaList.size(); ++i)
        {
            cout << "  " << (i + 1) << ". Nama: " << wargaList[i].nama << "\n"
                 << "     Alamat: " << wargaList[i].alamat << "\n"
                 << "     Kategori: " << wargaList[i].kategori
                 << endl
                 << endl;
        }
    }
};

struct Wilayah
{
    string nama;
    vector<RT> rtList;

    Wilayah(const string &nama) : nama(nama) {}

    void tambahRt(const RT &rt)
    {
        rtList.push_back(rt);
    }

    void tampilkanRt() const
    {
        if (rtList.empty())
        {
            cout << "Tidak ada RT terdaftar di wilayah ini" << endl;
            return;
        }

        cout << "Daftar RT di wilayah " << nama << ":" << endl;
        for (size_t i = 0; i < rtList.size(); ++i)
        {
            cout << "  " << (i + 1) << ". " << rtList[i].nama << endl;
        }
    }

    void tampilkanSemuaData() const
    {
        cout << "Wilayah: " << nama << endl;
        for (const auto &rt : rtList)
        {
            cout << rt.nama << ":" << endl;
            rt.tampilkanWarga();
        }
    }
};

void inputDataWarga(Wilayah &wilayah)
{
    while (true)
    {
        system("cls");
        cout << "====== INPUT DATA WARGA ======" << endl;

        if (wilayah.rtList.empty())
        {
            cout << "Tidak ada RT yang tersedia. Silakan tambahkan RT terlebih dahulu." << endl;
            return;
        }

        wilayah.tampilkanRt();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";
        int pilihanRt;
        cin >> pilihanRt;
        cin.ignore();

        if (pilihanRt == 0)
            return;
        if (pilihanRt < 1 || pilihanRt > wilayah.rtList.size())
        {
            cout << "RT tidak valid!" << endl;
            system("pause");
            continue;
        }

        RT &rtTerpilih = wilayah.rtList[pilihanRt - 1];
        char tambahLagi;

        do
        {
            system("cls");
            cout << "RT " << rtTerpilih.nama << endl;
            cout << "====== INPUT DATA WARGA BARU ======" << endl;

            Warga wargaBaru;

            cout << "Nama Warga: ";
            getline(cin, wargaBaru.nama);
            if (wargaBaru.nama.empty())
            {
                cout << "Nama tidak boleh kosong!" << endl;
                system("pause");
                continue;
            }

            cout << "Alamat: ";
            getline(cin, wargaBaru.alamat);
            if (wargaBaru.alamat.empty())
            {
                cout << "Alamat tidak boleh kosong!" << endl;
                system("pause");
                continue;
            }

            while (true)
            {
                cout << "Kategori (Miskin/Lansia/Yatim): ";
                getline(cin, wargaBaru.kategori);

                string lowerKategori;
                for (char c : wargaBaru.kategori)
                {
                    lowerKategori += tolower(c);
                }

                if (lowerKategori == "miskin" || lowerKategori == "lansia" || lowerKategori == "yatim")
                {
                    if (!wargaBaru.kategori.empty())
                    {
                        wargaBaru.kategori[0] = toupper(wargaBaru.kategori[0]);
                    }
                    break;
                }
                else
                {
                    cout << "Kategori tidak valid! Pilih antara Miskin, Lansia, atau Yatim" << endl;
                }
            }

            rtTerpilih.tambahWarga(wargaBaru);
            cout << "Warga " << wargaBaru.nama << " berhasil ditambahkan!" << endl;

            cout << "Tambah warga lagi? (Y/N): ";
            cin >> tambahLagi;
            cin.ignore();
            tambahLagi = toupper(tambahLagi);
        } while (tambahLagi == 'Y');

        break;
    }
}

void editDataWarga(Wilayah &wilayah)
{
    while (true)
    {
        system("cls");
        cout << "====== EDIT DATA WARGA ======" << endl;

        if (wilayah.rtList.empty())
        {
            cout << "Tidak ada RT yang tersedia. Silakan tambahkan RT terlebih dahulu." << endl;
            return;
        }

        wilayah.tampilkanRt();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";
        int pilihanRt;
        cin >> pilihanRt;
        cin.ignore();

        if (pilihanRt == 0)
            return;
        if (pilihanRt < 1 || pilihanRt > wilayah.rtList.size())
        {
            cout << "RT tidak valid!" << endl;
            system("pause");
            continue;
        }

        RT &rtTerpilih = wilayah.rtList[pilihanRt - 1];
        char editLagi;

        wilayah.rtList[pilihanRt - 1].tampilkanWarga();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih warga yang ingin diedit (nomor): ";
        int pilihanWarga;
        cin >> pilihanWarga;
        cin.ignore();

        if (pilihanWarga == 0)
            return;
        if (pilihanWarga < 1 || pilihanWarga > rtTerpilih.wargaList.size())
        {
            cout << "Warga tidak valid!" << endl;
            system("pause");
            continue;
        }

        Warga &wargaTerpilih = rtTerpilih.wargaList[pilihanWarga - 1];

        do
        {
            system("cls");
            cout << "RT " << rtTerpilih.nama << endl;
            cout << "====== EDIT DATA WARGA ======" << endl;

            cout << "Nama Warga: " << wargaTerpilih.nama << endl;
            cout << "Alamat: " << wargaTerpilih.alamat << endl;
            cout << "Kategori: " << wargaTerpilih.kategori << endl;

            cout << "Pilih data yang ingin diedit:" << endl;
            cout << "1. Nama" << endl;
            cout << "2. Alamat" << endl;
            cout << "3. Kategori" << endl;
            cout << "Pilihan menu (1-3): ";
            int pilihanMenu;
            cin >> pilihanMenu;
            cin.ignore();

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid. Masukkan angka saja!" << endl;
                system("pause");
                continue;
            }

            switch (pilihanMenu)
            {
            case 1:
                cout << "Masukkan nama baru: ";
                getline(cin, wargaTerpilih.nama);
                break;
            case 2:
                cout << "Masukkan alamat baru: ";
                getline(cin, wargaTerpilih.alamat);
                break;
            case 3:
                while (true)
                {

                    cout << "Masukkan kategori baru (Miskin, Lansia, atau Yatim): ";
                    getline(cin, wargaTerpilih.kategori);

                    string lowerKategori;
                    for (char c : wargaTerpilih.kategori)
                    {
                        lowerKategori += tolower(c);
                    }

                    if (lowerKategori == "miskin" || lowerKategori == "lansia" || lowerKategori == "yatim")
                    {
                        if (!wargaTerpilih.kategori.empty())
                        {
                            wargaTerpilih.kategori[0] = toupper(wargaTerpilih.kategori[0]);
                        }
                        break;
                    }

                    else
                    {
                        cout << "Kategori harus Miskin, Lansia, atau Yatim!" << endl;
                    }
                }
                break;
            default:
                cout << "Pilihan menu tidak valid!" << endl;
                break;
            }

            cout << "Edit lagi? (Y/N): ";
            cin >> editLagi;
            cin.ignore();
            editLagi = toupper(editLagi);
        } while (editLagi == 'Y');

        break;
    }
}

void hapusDataWarga(Wilayah &wilayah)
{
    while (true)
    {
        system("cls");
        cout << "====== HAPUS DATA WARGA ======" << endl;

        if (wilayah.rtList.empty())
        {
            cout << "Tidak ada RT yang tersedia. Silakan tambahkan RT terlebih dahulu." << endl;
            system("pause");
            return;
        }

        wilayah.tampilkanRt();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";

        int pilihanRt;
        if (!(cin >> pilihanRt))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input harus berupa angka!" << endl;
            system("pause");
            continue;
        }
        cin.ignore();

        if (pilihanRt == 0)
            return;
        if (pilihanRt < 1 || pilihanRt > wilayah.rtList.size())
        {
            cout << "RT tidak valid!" << endl;
            system("pause");
            continue;
        }

        RT &rtTerpilih = wilayah.rtList[pilihanRt - 1];

        if (rtTerpilih.wargaList.empty())
        {
            cout << "Tidak ada warga di RT " << rtTerpilih.nama << endl;
            system("pause");
            continue;
        }

        system("cls");
        cout << "====== DAFTAR WARGA DI RT " << rtTerpilih.nama << " ======" << endl;
        rtTerpilih.tampilkanWarga();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih warga yang ingin dihapus (nomor): ";

        int pilihanWarga;
        if (!(cin >> pilihanWarga))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input harus berupa angka!" << endl;
            system("pause");
            continue;
        }
        cin.ignore();

        if (pilihanWarga == 0)
            return;
        if (pilihanWarga < 1 || pilihanWarga > rtTerpilih.wargaList.size())
        {
            cout << "Warga tidak valid!" << endl;
            system("pause");
            continue;
        }

        Warga &wargaTerpilih = rtTerpilih.wargaList[pilihanWarga - 1];
        char konfirmasi;
        do
        {
            system("cls");
            cout << "====== KONFIRMASI PENGHAPUSAN ======" << endl;
            cout << "RT: " << rtTerpilih.nama << endl;
            cout << "Nama Warga: " << wargaTerpilih.nama << endl;
            cout << "Alamat: " << wargaTerpilih.alamat << endl;
            cout << "Kategori: " << wargaTerpilih.kategori << endl
                 << endl;

            cout << "Apakah Anda yakin ingin menghapus data ini? (Y/N): ";
            cin >> konfirmasi;
            cin.ignore();
            konfirmasi = toupper(konfirmasi);

            if (konfirmasi == 'N')
                return;
        } while (konfirmasi != 'Y' && konfirmasi != 'N');

        if (konfirmasi == 'Y')
        {
            rtTerpilih.wargaList.erase(rtTerpilih.wargaList.begin() + pilihanWarga - 1);
            cout << "Data warga berhasil dihapus." << endl;
            return;
        }
    }
}

void kelolaWarga(Wilayah &wilayah)
{
    bool running = true;

    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA WARGA ======" << endl;
        cout << "Wilayah: " << wilayah.nama << endl;

        cout << "1. Lihat Data Warga" << endl;
        cout << "2. Input Data Warga" << endl;
        cout << "3. Edit Data Warga" << endl;
        cout << "4. Hapus Data Warga" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilihan menu (1-5): ";

        int pilihan;
        cin >> pilihan;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan angka saja!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
            system("cls");
            cout << "====== DAFTAR WARGA ======" << endl;
            wilayah.tampilkanSemuaData();
            break;
        case 2:
            inputDataWarga(wilayah);
            break;
        case 3:
            editDataWarga(wilayah);
            break;
        case 4:
            hapusDataWarga(wilayah);
            break;
        case 5:
            running = false;
            break;

        default:
            cout << "Pilihan menu tidak valid!" << endl;
            break;
        }

        system("pause");
    }
}

int main()
{
    Wilayah wilayah("Kelurahan Caringin");
    wilayah.tambahRt(RT("RT 01"));
    wilayah.tambahRt(RT("RT 02"));
    wilayah.tambahRt(RT("RT 03"));

    wilayah.rtList[0].tambahWarga(Warga{"Asep", "Jl. Caringin No. 2", "Miskin"});
    wilayah.rtList[0].tambahWarga(Warga{"Budin", "Jl. Caringin No. 3", "Yatim"});
    wilayah.rtList[1].tambahWarga(Warga{"Chika", "Jl. Mekar No. 1", "Miskin"});
    wilayah.rtList[1].tambahWarga(Warga{"Doni", "Jl. Mekar No. 12", "Lansia"});
    wilayah.rtList[2].tambahWarga(Warga{"Elvis", "Jl. Baghdad No. 11", "Miskin"});
    wilayah.rtList[2].tambahWarga(Warga{"Fuki", "Jl. Baghdad No. 22", "Lansia"});

    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== APLIKASI PENDATAAN BANSOS ======" << endl;
        cout << "Wilayah: " << wilayah.nama << endl
             << endl;
        cout << "1. Kelola Data Warga" << endl;
        cout << "2. Daftarkan Bantuan untuk Warga" << endl;
        cout << "3. Lihat Antrian Penerima Bantuan" << endl;
        cout << "4. Lihat Riwayat Perubahan Data" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilihan menu (1-5): ";

        int pilihan;
        cin >> pilihan;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan angka saja!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
            kelolaWarga(wilayah);
            break;
        case 2:
            cout << "Fitur ini sedang dalam pengembangan" << endl;
            break;
        case 3:
            cout << "Fitur ini sedang dalam pengembangan" << endl;
            break;
        case 4:
            cout << "Fitur ini sedang dalam pengembangan" << endl;
            break;
        case 5:
            cout << "Ciao." << endl;
            running = false;
            break;

        default:
            cout << "Pilihan menu tidak valid!" << endl;
            break;
        }

        if (running && pilihan != 1)
        {
            system("pause");
        }
    }

    return 0;
}