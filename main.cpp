#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include <ranges>
#include <algorithm>
#include "json.hpp"
#include <fstream>
#include <ctime>
#include <stack>

using json = nlohmann::json;

using namespace std;

struct Warga
{
    string nama;
    int umur;
    int penghasilan;
    string statusKeluarga;
    string alamat;
    string kategori;
};
struct TreeNode
{
    Warga data;
    TreeNode *left;
    TreeNode *right;

    TreeNode(const Warga &warga)
        : data(warga), left(nullptr), right(nullptr) {}
};

struct PerubahanData
{
    string waktu;
    string rt;
    string namaWarga;
    string fieldDiubah;
    string nilaiLama;
    string nilaiBaru;
    string aksi;
};

struct RT
{
    string nama;
    TreeNode *root = nullptr;

    RT(string nama) : nama(nama) {}

    ~RT()
    {
        clearTree(root);
    }

    void clearTree(TreeNode *node)
    {
        if (node == nullptr)
            return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    void tambahWarga(const Warga &warga)
    {
        root = insert(root, warga);
    }

    TreeNode *insert(TreeNode *node, const Warga &warga)
    {
        if (node == nullptr)
        {
            return new TreeNode(warga);
        }

        if (warga.nama < node->data.nama)
        {
            node->left = insert(node->left, warga);
        }
        else
        {
            node->right = insert(node->right, warga);
        }

        return node;
    }

    void tampilkanWarga() const
    {
        if (root == nullptr)
        {
            cout << "  Tidak ada warga terdaftar di " << nama << endl;
            return;
        }

        cout << "  Daftar Warga di " << nama << ":" << endl;
        int counter = 1;
        inOrderTraversal(root, counter);
    }

    void inOrderTraversal(TreeNode *node, int &counter) const
    {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left, counter);

        cout << "  " << counter++ << ". Nama: " << node->data.nama << "\n"
        << "     Umur: " << node->data.umur << "\n"
        << "     Penghasilan: " << node->data.penghasilan << "\n"
        << "     Status Keluarga: " << node->data.statusKeluarga << "\n"
        << "     Alamat: " << node->data.alamat << "\n"
        << "     Kategori: " << node->data.kategori
        << endl
        << endl;

        inOrderTraversal(node->right, counter);
    }

    TreeNode *cariWarga(TreeNode *node, const string &nama)
    {
        if (node == nullptr)
            return nullptr;

        if (node->data.nama == nama)
            return node;

        TreeNode *found = cariWarga(node->left, nama);
        if (found != nullptr)
            return found;

        return cariWarga(node->right, nama);
    }

    TreeNode *cariWargaByIndex(TreeNode *node, int target, int &current)
    {
        if (node == nullptr)
            return nullptr;

        TreeNode *left = cariWargaByIndex(node->left, target, current);
        if (left != nullptr)
            return left;

        if (current == target)
            return node;
        current++;

        return cariWargaByIndex(node->right, target, current);
    }

    void hapusWarga(const string &nama)
    {
        root = deleteNode(root, nama);
    }

    TreeNode *deleteNode(TreeNode *node, const string &nama)
    {
        if (node == nullptr)
            return nullptr;

        if (nama < node->data.nama)
        {
            node->left = deleteNode(node->left, nama);
        }
        else if (nama > node->data.nama)
        {
            node->right = deleteNode(node->right, nama);
        }
        else
        {
            if (node->left == nullptr)
            {
                TreeNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                TreeNode *temp = node->left;
                delete node;
                return temp;
            }

            TreeNode *temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.nama);
        }
        return node;
    }

    TreeNode *minValueNode(TreeNode *node)
    {
        TreeNode *current = node;
        while (current && current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    int countWarga() const
    {
        return countNodes(root);
    }

    int countNodes(TreeNode *node) const
    {
        if (node == nullptr)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};
struct Wilayah
{
    string nama;
    vector<RT> rtList;
    stack<PerubahanData> riwayatPerubahan;

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
            cout << "  " << (i + 1) << ". " << rtList[i].nama << " ("
                << rtList[i].countWarga() << " warga)" << endl;
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

    void tambahRiwayat(const PerubahanData &perubahan)
    {
        riwayatPerubahan.push(perubahan);
    }

    void tampilkanRiwayat() const{
        if (riwayatPerubahan.empty())
        {
            cout << "Tidak ada riwayat perubahan data." << endl;
            return;
        }

        stack <PerubahanData> tempStack = riwayatPerubahan;
        vector<PerubahanData> riwayatTerbalik;

        while (!tempStack.empty()) {
            riwayatTerbalik.push_back(tempStack.top());
            tempStack.pop();
        }

        cout << "Riwayat Perubahan Data:" << endl;
        for (auto it = riwayatTerbalik.rbegin(); it != riwayatTerbalik.rend(); ++it) {
            const auto &perubahan = *it;
            cout << "  Waktu: " << perubahan.waktu << endl;
            cout << "  RT: " << perubahan.rt << endl;
            cout << "  Nama Warga: " << perubahan.namaWarga << endl;
            cout << "  Aksi: " << perubahan.aksi << endl;

            if (perubahan.aksi == "EDIT") {
                cout << "  Field yang diubah: " << perubahan.fieldDiubah << endl;
                cout << "  Nilai Lama: " << perubahan.nilaiLama << endl;
                cout << "  Nilai Baru: " << perubahan.nilaiBaru << endl;
            } else if (perubahan.aksi == "HAPUS") {
                cout << "  Data dihapus" << endl;
                cout << "  Nilai Lama: " << perubahan.nilaiLama << endl;
                cout << "  Nilai Baru: " << perubahan.nilaiBaru << endl;
            }

            cout << "  ------------------------" << endl;
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
            system("pause");
            return;
        }

        wilayah.tampilkanRt();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";

        int pilihanRt;
        while (!(cin >> pilihanRt) || pilihanRt < 0 || static_cast<size_t>(pilihanRt) > wilayah.rtList.size())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid! Masukkan nomor RT yang sesuai: ";
        }
        cin.ignore();

        if (pilihanRt == 0)
            return;

        RT &rtTerpilih = wilayah.rtList[pilihanRt - 1];
        char tambahLagi;

        do
        {
            system("cls");
            cout << "RT " << rtTerpilih.nama << endl;
            cout << "====== INPUT DATA WARGA BARU ======" << endl;

            Warga wargaBaru;

            do
            {
                cout << "Nama Warga: ";
                getline(cin, wargaBaru.nama);
                if (wargaBaru.nama.empty())
                {
                    cout << "Nama tidak boleh kosong!" << endl;
                }
                else if (any_of(wargaBaru.nama.begin(), wargaBaru.nama.end(), ::isdigit))
                {
                    cout << "Nama tidak boleh mengandung angka!" << endl;
                    wargaBaru.nama.clear();
                }
            } while (wargaBaru.nama.empty());

            do
            {
                cout << "Umur: ";
                while (!(cin >> wargaBaru.umur))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input harus angka! Masukkan umur: ";
                }
                cin.ignore();
                if (wargaBaru.umur < 0 || wargaBaru.umur > 120)
                {
                    cout << "Umur tidak valid (0-120 tahun)!" << endl;
                }
            } while (wargaBaru.umur < 0 || wargaBaru.umur > 120);

            do
            {
                cout << "Penghasilan: ";
                while (!(cin >> wargaBaru.penghasilan))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input harus angka! Masukkan penghasilan: ";
                }
                cin.ignore();
                if (wargaBaru.penghasilan < 0)
                {
                    cout << "Penghasilan tidak boleh negatif!" << endl;
                }
            } while (wargaBaru.penghasilan < 0);

            do
            {
                cout << "Status Keluarga (Yatim/Bukan): ";
                getline(cin, wargaBaru.statusKeluarga);
                if (wargaBaru.statusKeluarga.empty())
                {
                    cout << "Status Keluarga tidak boleh kosong!" << endl;
                }
                string statusLower = wargaBaru.statusKeluarga;
                transform(statusLower.begin(), statusLower.end(), statusLower.begin(), ::tolower);
                if (statusLower != "yatim" && statusLower != "bukan")
                {
                    cout << "Status harus 'Yatim' atau 'Bukan'!" << endl;
                    wargaBaru.statusKeluarga.clear();
                }
                else
                {
                    wargaBaru.statusKeluarga[0] = toupper(wargaBaru.statusKeluarga[0]);
                    for (size_t i = 1; i < wargaBaru.statusKeluarga.size(); i++)
                    {
                        wargaBaru.statusKeluarga[i] = tolower(wargaBaru.statusKeluarga[i]);
                    }
                }
            } while (wargaBaru.statusKeluarga.empty());

            do
            {
                cout << "Alamat: ";
                getline(cin, wargaBaru.alamat);
                if (wargaBaru.alamat.empty())
                {
                    cout << "Alamat tidak boleh kosong!" << endl;
                }
            } while (wargaBaru.alamat.empty());

            string statusLower = wargaBaru.statusKeluarga;
            transform(statusLower.begin(), statusLower.end(), statusLower.begin(), ::tolower);

            if (wargaBaru.umur > 60 ||
                (statusLower == "yatim" && wargaBaru.umur < 18) ||
                wargaBaru.penghasilan < 3370534)
            {
                wargaBaru.kategori = "Prioritas";
            }
            else
            {
                wargaBaru.kategori = "Reguler";
            }

            rtTerpilih.tambahWarga(wargaBaru);
            cout << "Warga " << wargaBaru.nama << " berhasil ditambahkan!" << endl;

            PerubahanData perubahan;
            time_t now = time(0);
            perubahan.waktu = ctime(&now);
            perubahan.rt = rtTerpilih.nama;
            perubahan.namaWarga = wargaBaru.nama;
            perubahan.aksi = "TAMBAH";
            perubahan.nilaiLama = "-";
            perubahan.nilaiBaru = wargaBaru.kategori;
            perubahan.fieldDiubah = "Semua Data";
            
            wilayah.tambahRiwayat(perubahan);

            do
            {
                cout << "Tambah warga lagi? (Y/N): ";
                cin >> tambahLagi;
                cin.ignore();
                tambahLagi = toupper(tambahLagi);
                if (tambahLagi != 'Y' && tambahLagi != 'N')
                {
                    cout << "Masukkan Y atau N!" << endl;
                }
            } while (tambahLagi != 'Y' && tambahLagi != 'N');

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
            system("pause");
            return;
        }

        wilayah.tampilkanRt();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";

        int pilihanRt;
        while (!(cin >> pilihanRt) || pilihanRt < 0 || pilihanRt > wilayah.rtList.size())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid! Masukkan nomor RT yang sesuai: ";
        }
        cin.ignore();

        if (pilihanRt == 0)
            return;

        RT &rtTerpilih = wilayah.rtList[pilihanRt - 1];

        if (rtTerpilih.countWarga() == 0)
        {
            cout << "Tidak ada warga yang terdaftar di RT ini." << endl;
            system("pause");
            continue;
        }

        rtTerpilih.tampilkanWarga();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih warga yang ingin diedit (nomor): ";

        int pilihanWarga;
        while (!(cin >> pilihanWarga) || pilihanWarga < 0 || pilihanWarga > rtTerpilih.countWarga())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid! Masukkan nomor warga yang sesuai: ";
        }
        cin.ignore();

        if (pilihanWarga == 0)
            return;

        int currentIndex = 0;
        TreeNode *nodeWarga = rtTerpilih.cariWargaByIndex(rtTerpilih.root, pilihanWarga - 1, currentIndex);
        if (nodeWarga == nullptr)
        {
            cout << "Warga tidak ditemukan!" << endl;
            system("pause");
            continue;
        }

        Warga &wargaTerpilih = nodeWarga->data;
        char editLagi;

        do
        {
            system("cls");
            cout << "RT " << rtTerpilih.nama << endl;
            cout << "====== EDIT DATA WARGA ======" << endl;
            cout << "Nama: " << wargaTerpilih.nama << endl;
            cout << "Umur: " << wargaTerpilih.umur << endl;
            cout << "Penghasilan: " << wargaTerpilih.penghasilan << endl;
            cout << "Status Keluarga: " << wargaTerpilih.statusKeluarga << endl;
            cout << "Alamat: " << wargaTerpilih.alamat << endl
                << endl;

            cout << "Pilih data yang ingin diedit:" << endl;
            cout << "1. Nama" << endl;
            cout << "2. Umur" << endl;
            cout << "3. Penghasilan" << endl;
            cout << "4. Status Keluarga" << endl;
            cout << "5. Alamat" << endl;
            cout << "0. Kembali" << endl;
            cout << "Pilihan menu (0-5): ";

            int pilihanMenu;
            while (!(cin >> pilihanMenu) || pilihanMenu < 0 || pilihanMenu > 5)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid! Masukkan angka 0-5: ";
            }
            cin.ignore();

            if (pilihanMenu == 0)
                break;

            switch (pilihanMenu)
            {
            case 1:
            {
                string namaLama = wargaTerpilih.nama;

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.namaWarga = wargaTerpilih.nama;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Nama";
                perubahan.nilaiLama = wargaTerpilih.nama; 
                perubahan.nilaiBaru = "-"; // Nilai baru akan diisi setelah input
                wilayah.tambahRiwayat(perubahan);   
                // Input nama baru

                do
                {
                    cout << "Nama saat ini: " << wargaTerpilih.nama << endl;
                    cout << "Masukkan nama baru: ";
                    getline(cin, wargaTerpilih.nama);

                    if (wargaTerpilih.nama.empty())
                    {
                        cout << "Nama tidak boleh kosong!" << endl;
                    }
                    else if (any_of(wargaTerpilih.nama.begin(), wargaTerpilih.nama.end(), ::isdigit))
                    {
                        cout << "Nama tidak boleh mengandung angka!" << endl;
                        wargaTerpilih.nama = namaLama;
                    }
                } while (wargaTerpilih.nama.empty());

                // Jika nama diubah, kita perlu reinsert node karena nama adalah key
                if (namaLama != wargaTerpilih.nama)
                {
                    Warga wargaTemp = wargaTerpilih;
                    rtTerpilih.hapusWarga(namaLama);
                    rtTerpilih.tambahWarga(wargaTemp);
                }
                break;
            }
            case 2:
            {

                int umurLama = wargaTerpilih.umur;

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.namaWarga = wargaTerpilih.nama;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Umur";
                perubahan.nilaiLama = to_string(wargaTerpilih.umur);
                perubahan.nilaiBaru = "-";

                wilayah.tambahRiwayat(perubahan);
                do
                {
                    cout << "Umur saat ini: " << wargaTerpilih.umur << endl;
                    cout << "Masukkan umur baru: ";
                    while (!(cin >> wargaTerpilih.umur))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input harus angka! Masukkan umur: ";
                    }
                    cin.ignore();

                    if (wargaTerpilih.umur < 0 || wargaTerpilih.umur > 120)
                    {
                        cout << "Umur tidak valid (0-120 tahun)!" << endl;
                    }
                } while (wargaTerpilih.umur < 0 || wargaTerpilih.umur > 120);
                break;
            }
            case 3:
            {
                do
                {
                    cout << "Penghasilan saat ini: " << wargaTerpilih.penghasilan << endl;
                    cout << "Masukkan penghasilan baru: ";
                    while (!(cin >> wargaTerpilih.penghasilan))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input harus angka! Masukkan penghasilan: ";
                    }
                    cin.ignore();

                    if (wargaTerpilih.penghasilan < 0)
                    {
                        cout << "Penghasilan tidak boleh negatif!" << endl;
                    }
                } while (wargaTerpilih.penghasilan < 0);
                break;
            }
            case 4:
            {
                do
                {
                    cout << "Status Keluarga saat ini: " << wargaTerpilih.statusKeluarga << endl;
                    cout << "Masukkan status keluarga baru (Yatim/Bukan): ";
                    getline(cin, wargaTerpilih.statusKeluarga);

                    if (wargaTerpilih.statusKeluarga.empty())
                    {
                        cout << "Status Keluarga tidak boleh kosong!" << endl;
                    }
                    else
                    {
                        string statusLower = wargaTerpilih.statusKeluarga;
                        transform(statusLower.begin(), statusLower.end(), statusLower.begin(), ::tolower);

                        if (statusLower != "yatim" && statusLower != "bukan")
                        {
                            cout << "Status harus 'Yatim' atau 'Bukan'!" << endl;
                            wargaTerpilih.statusKeluarga.clear();
                        }
                        else
                        {
                            wargaTerpilih.statusKeluarga[0] = toupper(wargaTerpilih.statusKeluarga[0]);
                            for (size_t i = 1; i < wargaTerpilih.statusKeluarga.size(); i++)
                            {
                                wargaTerpilih.statusKeluarga[i] = tolower(wargaTerpilih.statusKeluarga[i]);
                            }
                        }
                    }
                } while (wargaTerpilih.statusKeluarga.empty());
                break;
            }
            case 5:
            {
                do
                {
                    cout << "Alamat saat ini: " << wargaTerpilih.alamat << endl;
                    cout << "Masukkan alamat baru: ";
                    getline(cin, wargaTerpilih.alamat);

                    if (wargaTerpilih.alamat.empty())
                    {
                        cout << "Alamat tidak boleh kosong!" << endl;
                    }
                } while (wargaTerpilih.alamat.empty());
                break;
            }
            }

            // Update kategori setelah edit
            string statusLower = wargaTerpilih.statusKeluarga;
            transform(statusLower.begin(), statusLower.end(), statusLower.begin(), ::tolower);

            if (wargaTerpilih.umur > 60 ||
                (statusLower == "yatim" && wargaTerpilih.umur < 18) ||
                wargaTerpilih.penghasilan < 3370534)
            {
                wargaTerpilih.kategori = "Prioritas";
            }
            else
            {
                wargaTerpilih.kategori = "Reguler";
            }

            do
            {
                cout << "Edit data lain untuk warga ini? (Y/N): ";
                cin >> editLagi;
                cin.ignore();
                editLagi = toupper(editLagi);
                if (editLagi != 'Y' && editLagi != 'N')
                {
                    cout << "Masukkan Y atau N!" << endl;
                }
            } while (editLagi != 'Y' && editLagi != 'N');

        } while (editLagi == 'Y');

        cout << "Data warga berhasil diperbarui!" << endl;
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
        if (pilihanRt < 1 || static_cast<size_t>(pilihanRt) > wilayah.rtList.size())
        {
            cout << "RT tidak valid!" << endl;
            system("pause");
            continue;
        }

        RT &rtTerpilih = wilayah.rtList[pilihanRt - 1];

        if (rtTerpilih.countWarga() == 0)
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
        if (pilihanWarga < 1 || pilihanWarga > rtTerpilih.countWarga())
        {
            cout << "Warga tidak valid!" << endl;
            system("pause");
            continue;
        }

        int currentIndex = 0;
        TreeNode *nodeWarga = rtTerpilih.cariWargaByIndex(rtTerpilih.root, pilihanWarga - 1, currentIndex);
        if (nodeWarga == nullptr)
        {
            cout << "Warga tidak ditemukan!" << endl;
            system("pause");
            continue;
        }

        Warga &wargaTerpilih = nodeWarga->data;
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
            PerubahanData perubahan;
            time_t now = time(0);
            perubahan.waktu = ctime(&now);
            perubahan.rt = rtTerpilih.nama;
            perubahan.namaWarga = wargaTerpilih.nama;   
            perubahan.aksi = "HAPUS";
            perubahan.fieldDiubah = "Semua Data";
            perubahan.nilaiLama = "-"; 
            perubahan.nilaiBaru = "-"; 
            wilayah.tambahRiwayat(perubahan);

            rtTerpilih.hapusWarga(wargaTerpilih.nama);
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

    wilayah.rtList[0].tambahWarga(Warga{"Asep", 19, 2000000, "Bukan", "Jl. Caringin No. 2", "Prioritas"});
    wilayah.rtList[0].tambahWarga(Warga{"Budin", 17, 100000, "Yatim", "Jl. Caringin No. 3", "Prioritas"});
    wilayah.rtList[1].tambahWarga(Warga{"Cecep", 68, 10000000, "Yatim", "Jl. Amba No. 3", "Prioritas"});
    wilayah.rtList[1].tambahWarga(Warga{"Doni", 24, 100000, "Yatim", "Jl. Amba No. 2", "Reguler"});
    wilayah.rtList[2].tambahWarga(Warga{"Edi", 35, 200000, "Yatim", "Jl. Nigerian No. 1", "Prioritas"});
    wilayah.rtList[2].tambahWarga(Warga{"Feri", 18, 1400000, "Bukan", "Jl. Nigerian No. 2", "Prioritas"});

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