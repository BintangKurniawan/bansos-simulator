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
        cout << "1. Input Data Warga" << endl;
        cout << "2. Lihat Data Warga" << endl;
        cout << "3. Daftarkan Bantuan untuk Warga" << endl;
        cout << "4. Lihat Antrian Penerima Bantuan" << endl;
        cout << "5. Lihat Riwayat Perubahan Data" << endl;
        cout << "6. Keluar" << endl;
        cout << "Pilihan menu (1-6): ";

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
            inputDataWarga(wilayah);
            break;
        case 2:
            system("cls");
            cout << "====== DAFTAR WARGA ======" << endl;
            wilayah.tampilkanSemuaData();
            break;
        case 3:
            cout << "Fitur ini sedang dalam pengembangan" << endl;
            break;
        case 4:
            cout << "Fitur ini sedang dalam pengembangan" << endl;
            break;
        case 5:
            cout << "Fitur ini sedang dalam pengembangan" << endl;
            break;
        case 6:
            cout << "Terima kasih. Program berakhir." << endl;
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