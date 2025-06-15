#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include <ranges>
#include <algorithm>
#include "json.hpp"
#include <queue>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <stack>
#include <regex>

using json = nlohmann::json;

using namespace std;

struct Bantuan
{
    string jenis;
    string tanggal;
    int nominal = 0;
};

struct Warga
{
    string nik;
    string nama;
    int umur;
    int penghasilan;
    string statusKeluarga;
    string alamat;
    string kategori;

    vector<Bantuan> daftarBantuan;
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
    string nikWarga;
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

    bool nikTerdaftar(const string &nik)
    {
        TreeNode *current = root;
        while (current != nullptr)
        {
            if (current->data.nik == nik)
                return true;
            if (nik < current->data.nik)
                current = current->left;
            else
                current = current->right;
        }
        return false;
    }

    TreeNode *insert(TreeNode *node, const Warga &warga)
    {
        if (node == nullptr)
        {
            return new TreeNode(warga);
        }

        if (warga.nik < node->data.nik)
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

        cout << "  " << counter++ << "." << "\n"
             << "     NIK: " << node->data.nik << "\n"
             << "     Nama: " << node->data.nama << "\n"
             << "     Umur: " << node->data.umur << "\n"
             << "     Penghasilan: " << node->data.penghasilan << "\n"
             << "     Status Keluarga: " << node->data.statusKeluarga << "\n"
             << "     Alamat: " << node->data.alamat << "\n"
             << "     Kategori: " << node->data.kategori
             << endl
             << endl;

        inOrderTraversal(node->right, counter);
    }

    TreeNode *cariWarga(TreeNode *node, const string &nik)
    {
        if (node == nullptr)
            return nullptr;

        if (node->data.nik == nik)
            return node;

        TreeNode *found = cariWarga(node->left, nik);
        if (found != nullptr)
            return found;

        return cariWarga(node->right, nik);
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

    void hapusWarga(const string &nik)
    {
        root = deleteNode(root, nik);
    }

    TreeNode *deleteNode(TreeNode *node, const string &nik)
    {
        if (node == nullptr)
            return nullptr;

        if (nik < node->data.nik)
        {
            node->left = deleteNode(node->left, nik);
        }
        else if (nik > node->data.nik)
        {
            node->right = deleteNode(node->right, nik);
        }
        else
        {
            // Node dengan NIK yang cocok ditemukan
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

            // Node dengan dua anak, cari successor
            TreeNode *temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.nik);
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
struct SecTreeNode
{
    RT data;
    SecTreeNode *left;
    SecTreeNode *right;

    SecTreeNode(const RT &rt) : data(rt), left(nullptr), right(nullptr) {}
};

struct RW
{
    string nama;
    SecTreeNode *root = nullptr;
    stack<PerubahanData> riwayatPerubahan;

    RW(const string &nama) : nama(nama) {}

    void tambahRT(const RT &rt)
    {
        root = insert(root, rt);
    }

    SecTreeNode *insert(SecTreeNode *node, const RT &rt)
    {
        if (node == nullptr)
        {
            return new SecTreeNode(rt);
        }

        if (rt.nama < node->data.nama)
        {
            node->left = insert(node->left, rt);
        }
        else
        {
            node->right = insert(node->right, rt);
        }

        return node;
    }

    void tampilkanSemuaData() const
    {
        cout << "RW: " << nama << endl;
        tampilkanDataInOrder(root);
    }

    void tampilkanDataInOrder(SecTreeNode *node) const
    {
        if (node == nullptr)
            return;

        tampilkanDataInOrder(node->left);

        cout << node->data.nama << ":" << endl;
        node->data.tampilkanWarga();

        tampilkanDataInOrder(node->right);
    }

    void tambahRiwayat(const PerubahanData &perubahan)
    {
        riwayatPerubahan.push(perubahan);
    }

    void tampilkanRiwayat() const
    {
        if (riwayatPerubahan.empty())
        {
            cout << "Tidak ada riwayat perubahan data." << endl;
            return;
        }

        stack<PerubahanData> tempStack = riwayatPerubahan;
        vector<PerubahanData> riwayatTerbalik;

        while (!tempStack.empty())
        {
            riwayatTerbalik.push_back(tempStack.top());
            tempStack.pop();
        }

        cout << "Riwayat Perubahan Data:" << endl;
        for (auto it = riwayatTerbalik.rbegin(); it != riwayatTerbalik.rend(); ++it)
        {
            const auto &perubahan = *it;
            cout << "  Waktu: " << perubahan.waktu;
            cout << "  RT: " << perubahan.rt << endl;
            cout << "  NIK Warga: " << perubahan.nikWarga << endl; // Tampilkan NIK
            cout << "  Nama Warga: " << perubahan.namaWarga << endl;
            cout << "  Aksi: " << perubahan.aksi << endl;

            if (perubahan.aksi == "EDIT")
            {
                cout << "  Field yang diubah: " << perubahan.fieldDiubah << endl;
                cout << "  Data Lama: " << perubahan.nilaiLama << endl;
                cout << "  Data Baru: " << perubahan.nilaiBaru << endl;
            }
            else if (perubahan.aksi == "HAPUS")
            {
                cout << "  Data dihapus" << endl;
            }
            else if (perubahan.aksi == "TAMBAH")
            {
                cout << "  Data baru ditambahkan" << endl;
                cout << "  Kategori: " << perubahan.nilaiBaru << endl;
            }

            cout << "  ------------------------" << endl;
        }
    }

    int countRt() const
    {
        return countNodes(root);
    }

    int countNodes(SecTreeNode *node) const
    {
        if (node == nullptr)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    void tampilkanRT() const
    {
        if (root == nullptr)
        {
            cout << "  Tidak ada RT terdaftar di " << nama << endl;
            return;
        }

        cout << "  Daftar RT di " << nama << ":" << endl;
        int counter = 1;
        inOrderTraversal(root, counter);
    }

    void inOrderTraversal(SecTreeNode *node, int &counter) const
    {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left, counter);

        cout << "  " << counter++ << "." << "\n"
             << "    " << node->data.nama << "\n"

             << endl;

        inOrderTraversal(node->right, counter);
    }

    RT *getRTByIndex(int targetIndex)
    {
        int currentIndex = 0;
        return getRTByIndexHelper(root, targetIndex, currentIndex);
    }

    RT *getRTByIndexHelper(SecTreeNode *node, int targetIndex, int &currentIndex)
    {
        if (node == nullptr)
            return nullptr;

        RT *result = getRTByIndexHelper(node->left, targetIndex, currentIndex);
        if (result != nullptr)
            return result;

        if (currentIndex == targetIndex)
        {
            return &node->data;
        }
        currentIndex++;

        return getRTByIndexHelper(node->right, targetIndex, currentIndex);
    }

    RT *cariRT(const string &namaRT)
    {
        SecTreeNode *current = root;
        while (current != nullptr)
        {
            if (current->data.nama == namaRT)
                return &current->data;
            else if (namaRT < current->data.nama)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }
};
struct ThirdTreeNode
{
    RW data;
    ThirdTreeNode *left;
    ThirdTreeNode *right;

    ThirdTreeNode(const RW &rw) : data(rw), left(nullptr), right(nullptr) {}
};

struct Kelurahan
{
    string nama;
    ThirdTreeNode *root = nullptr;

    Kelurahan(const string &nama) : nama(nama) {}

    void tambahRW(const RW &rw)
    {
        root = insert(root, rw);
    }

    ThirdTreeNode *insert(ThirdTreeNode *node, const RW &rw)
    {
        if (node == nullptr)
        {
            return new ThirdTreeNode(rw);
        }

        if (rw.nama < node->data.nama)
        {
            node->left = insert(node->left, rw);
        }
        else
        {
            node->right = insert(node->right, rw);
        }

        return node;
    }

    void tampilkanRW() const
    {
        if (root == nullptr)
        {
            cout << "  Tidak ada RW terdaftar di Kelurahan " << nama << endl;
            return;
        }

        cout << "  Daftar RW di Kelurahan " << nama << ":" << endl;
        int counter = 1;
        inOrderTraversal(root, counter);
    }

    void inOrderTraversal(ThirdTreeNode *node, int &counter) const
    {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left, counter);

        cout << "  " << counter++ << "." << "\n"
             << " " << node->data.nama << "\n"

             << endl;

        inOrderTraversal(node->right, counter);
    }

    RW *cariRW(const string &namaRW)
    {
        ThirdTreeNode *current = root;
        while (current != nullptr)
        {
            if (current->data.nama == namaRW)
                return &current->data;
            else if (namaRW < current->data.nama)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }
};
struct QuadTreeNode
{
    Kelurahan data;
    QuadTreeNode *left;
    QuadTreeNode *right;

    QuadTreeNode(const Kelurahan &kelurahan) : data(kelurahan), left(nullptr), right(nullptr) {}
};

struct Kecamatan
{
    string nama;
    QuadTreeNode *root = nullptr;

    Kecamatan(const string &nama) : nama(nama) {}

    void tambahKelurahan(const Kelurahan &kelurahan)
    {
        root = insert(root, kelurahan);
    }

    QuadTreeNode *insert(QuadTreeNode *node, const Kelurahan &kelurahan)
    {
        if (node == nullptr)
        {
            return new QuadTreeNode(kelurahan);
        }

        if (kelurahan.nama < node->data.nama)
        {
            node->left = insert(node->left, kelurahan);
        }
        else
        {
            node->right = insert(node->right, kelurahan);
        }

        return node;
    }

    void tampilkanKelurahan() const
    {
        if (root == nullptr)
        {
            cout << "  Tidak ada Kelurahan terdaftar di Kecamatan " << nama << endl;
            return;
        }

        cout << "  Daftar Kelurahan di Kecamatan " << nama << ":" << endl;
        int counter = 1;
        inOrderTraversal(root, counter);
    }

    void inOrderTraversal(QuadTreeNode *node, int &counter) const
    {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left, counter);

        cout << "  " << counter++ << "." << "\n"
             << "      " << node->data.nama << "\n"

             << endl;

        inOrderTraversal(node->right, counter);
    }

    Kelurahan *cariKelurahan(const string &namaKelurahan)
    {
        QuadTreeNode *current = root;
        while (current != nullptr)
        {
            if (current->data.nama == namaKelurahan)
                return &current->data;
            else if (namaKelurahan < current->data.nama)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }
};

struct PentaTreeNode
{
    Kecamatan data;
    PentaTreeNode *left;
    PentaTreeNode *right;

    PentaTreeNode(const Kecamatan &kecamatan) : data(kecamatan), left(nullptr), right(nullptr) {}
};

struct Kota
{
    string nama;
    PentaTreeNode *root = nullptr;

    Kota(const string &nama) : nama(nama) {}

    void tambahKecamatan(const Kecamatan &kecamatan)
    {
        root = insert(root, kecamatan);
    }

    PentaTreeNode *insert(PentaTreeNode *node, const Kecamatan &kecamatan)
    {
        if (node == nullptr)
        {
            return new PentaTreeNode(kecamatan);
        }

        if (kecamatan.nama < node->data.nama)
        {
            node->left = insert(node->left, kecamatan);
        }
        else
        {
            node->right = insert(node->right, kecamatan);
        }

        return node;
    }

    void tampilkanKecamatan() const
    {
        if (root == nullptr)
        {
            cout << "  Tidak ada Kecamatan terdaftar di Kota " << nama << endl;
            return;
        }

        cout << "  Daftar Kecamatan di Kota " << nama << ":" << endl;
        int counter = 1;
        inOrderTraversal(root, counter);
    }

    void inOrderTraversal(PentaTreeNode *node, int &counter) const
    {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left, counter);

        cout << "  " << counter++ << "." << "\n"
             << "      " << node->data.nama << "\n"

             << endl;

        inOrderTraversal(node->right, counter);
    }

    Kecamatan *cariKecamatan(const string &namaKecamatan)
    {
        PentaTreeNode *current = root;
        while (current != nullptr)
        {
            if (current->data.nama == namaKecamatan)
                return &current->data;
            else if (namaKecamatan < current->data.nama)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }
};

bool isAllDigits(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

void inputDataWarga(RW &rw)
{
    while (true)
    {
        system("cls");

        cout << "====== INPUT DATA WARGA ======" << endl;

        rw.tampilkanRT();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";

        int pilihanRt;
        while (!(cin >> pilihanRt) || pilihanRt < 0 || static_cast<size_t>(pilihanRt) > rw.countRt())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid! Masukkan nomor RT yang sesuai: ";
        }
        cin.ignore();

        if (pilihanRt == 0)
            return;
        RT *rtPtr = rw.getRTByIndex(pilihanRt - 1);

        if (rtPtr == nullptr)
        {
            cout << "Tidak ada RT yang tersedia. Silakan tambahkan RT terlebih dahulu." << endl;
            system("pause");
            return;
        }
        RT &rtTerpilih = *rtPtr;

        char tambahLagi;

        do
        {
            system("cls");
            cout << "RT " << rtTerpilih.nama << endl;
            cout << "====== INPUT DATA WARGA BARU ======" << endl;

            Warga wargaBaru;

            do
            {
                cout << "NIK: ";
                getline(cin, wargaBaru.nik);

                if (rtTerpilih.nikTerdaftar(wargaBaru.nik))
                {
                    cout << "NIK sudah terdaftar! Masukkan NIK yang belum terdaftar." << endl;
                }

                if (wargaBaru.nik.length() != 16 || !isAllDigits(wargaBaru.nik))
                {
                    cout << "NIK tidak valid! Masukkan 16 digit angka." << endl;
                }

            } while ((wargaBaru.nik.length() != 16 || !isAllDigits(wargaBaru.nik)) || rtTerpilih.nikTerdaftar(wargaBaru.nik));

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
            perubahan.nikWarga = wargaBaru.nik;
            perubahan.namaWarga = wargaBaru.nama;
            perubahan.aksi = "TAMBAH";
            perubahan.nilaiLama = "-";
            perubahan.nilaiBaru = wargaBaru.kategori;
            perubahan.fieldDiubah = "Semua Data";

            rw.tambahRiwayat(perubahan);

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

void editDataWarga(RW &rw)
{
    while (true)
    {
        system("cls");

        cout << "====== EDIT DATA WARGA ======" << endl;

        rw.tampilkanRT();
        cout << "0. Kembali ke menu utama" << endl;
        cout << "Pilih RT (nomor): ";

        int pilihanRt;
        while (!(cin >> pilihanRt) || pilihanRt < 0 || pilihanRt > static_cast<int>(rw.countRt()))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid! Masukkan nomor RT yang sesuai: ";
        }
        cin.ignore();

        if (pilihanRt == 0)
            return;
        RT *rtPtr = rw.getRTByIndex(pilihanRt - 1);
        if (rtPtr == nullptr)
        {
            cout << "Tidak ada RT yang tersedia. Silakan tambahkan RT terlebih dahulu." << endl;
            system("pause");
            return;
        }
        RT &rtTerpilih = *rtPtr;

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
            cout << rtTerpilih.nama << endl;
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

                // Input nama baru
                string namaBaru;
                do
                {
                    cout << "Nama saat ini: " << wargaTerpilih.nama << endl;
                    cout << "Masukkan nama baru: ";
                    getline(cin, namaBaru);

                    if (namaBaru.empty())
                    {
                        cout << "Nama tidak boleh kosong!" << endl;
                    }
                    else if (any_of(namaBaru.begin(), namaBaru.end(), ::isdigit))
                    {
                        cout << "Nama tidak boleh mengandung angka!" << endl;
                        namaBaru.clear();
                    }
                } while (namaBaru.empty());

                // Jika nama diubah, kita perlu reinsert node karena nama adalah key
                if (namaLama != namaBaru)
                {
                    Warga wargaTemp = wargaTerpilih;
                    wargaTemp.nama = namaBaru;
                    rtTerpilih.hapusWarga(wargaTemp.nik);
                    rtTerpilih.tambahWarga(wargaTemp);
                    TreeNode *foundNode = rtTerpilih.cariWarga(rtTerpilih.root, wargaTemp.nik);
                    if (foundNode != nullptr)
                    {
                        wargaTerpilih = foundNode->data;
                    }
                }

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.nikWarga = wargaTerpilih.nik;
                perubahan.namaWarga = namaBaru;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Nama";
                perubahan.nilaiLama = namaLama;
                perubahan.nilaiBaru = namaBaru;
                rw.tambahRiwayat(perubahan);
                break;
            }
            case 2:
            {
                int umurLama = wargaTerpilih.umur;
                int umurBaru;
                do
                {
                    cout << "Umur saat ini: " << wargaTerpilih.umur << endl;
                    cout << "Masukkan umur baru: ";
                    while (!(cin >> umurBaru))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input harus angka! Masukkan umur: ";
                    }
                    cin.ignore();

                    if (umurBaru < 0 || umurBaru > 120)
                    {
                        cout << "Umur tidak valid (0-120 tahun)!" << endl;
                    }
                } while (umurBaru < 0 || umurBaru > 120);
                wargaTerpilih.umur = umurBaru;

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.nikWarga = wargaTerpilih.nik;
                perubahan.namaWarga = wargaTerpilih.nama;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Umur";
                perubahan.nilaiLama = to_string(umurLama);
                perubahan.nilaiBaru = to_string(umurBaru);
                rw.tambahRiwayat(perubahan);
                break;
            }
            case 3:
            {
                int penghasilanLama = wargaTerpilih.penghasilan;
                int penghasilanBaru;
                do
                {
                    cout << "Penghasilan saat ini: " << wargaTerpilih.penghasilan << endl;
                    cout << "Masukkan penghasilan baru: ";
                    while (!(cin >> penghasilanBaru))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input harus angka! Masukkan penghasilan: ";
                    }
                    cin.ignore();

                    if (penghasilanBaru < 0)
                    {
                        cout << "Penghasilan tidak boleh negatif!" << endl;
                    }
                } while (penghasilanBaru < 0);
                wargaTerpilih.penghasilan = penghasilanBaru;

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.nikWarga = wargaTerpilih.nik;
                perubahan.namaWarga = wargaTerpilih.nama;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Penghasilan";
                perubahan.nilaiLama = to_string(penghasilanLama);
                perubahan.nilaiBaru = to_string(penghasilanBaru);
                rw.tambahRiwayat(perubahan);
                break;
            }
            case 4:
            {
                string statusLama = wargaTerpilih.statusKeluarga;
                string statusBaru;
                do
                {
                    cout << "Status Keluarga saat ini (Yatim/Bukan): " << wargaTerpilih.statusKeluarga << endl;
                    cout << "Masukkan status keluarga baru (Yatim/Bukan): ";
                    getline(cin, statusBaru);

                    if (statusBaru.empty())
                    {
                        cout << "Status Keluarga tidak boleh kosong!" << endl;
                    }
                    else
                    {
                        string statusLower = statusBaru;
                        transform(statusLower.begin(), statusLower.end(), statusLower.begin(), ::tolower);

                        if (statusLower != "yatim" && statusLower != "bukan")
                        {
                            cout << "Status harus 'Yatim' atau 'Bukan'!" << endl;
                            statusBaru.clear();
                        }
                        else
                        {
                            statusBaru[0] = toupper(statusBaru[0]);
                            for (size_t i = 1; i < statusBaru.size(); i++)
                            {
                                statusBaru[i] = tolower(statusBaru[i]);
                            }
                        }
                    }
                } while (statusBaru.empty());
                wargaTerpilih.statusKeluarga = statusBaru;

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.nikWarga = wargaTerpilih.nik;
                perubahan.namaWarga = wargaTerpilih.nama;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Status Keluarga";
                perubahan.nilaiLama = statusLama;
                perubahan.nilaiBaru = statusBaru;
                rw.tambahRiwayat(perubahan);
                break;
            }
            case 5:
            {
                string alamatLama = wargaTerpilih.alamat;
                string alamatBaru;
                do
                {
                    cout << "Alamat saat ini: " << wargaTerpilih.alamat << endl;
                    cout << "Masukkan alamat baru: ";
                    getline(cin, alamatBaru);

                    if (alamatBaru.empty())
                    {
                        cout << "Alamat tidak boleh kosong!" << endl;
                    }
                } while (alamatBaru.empty());
                wargaTerpilih.alamat = alamatBaru;

                PerubahanData perubahan;
                time_t now = time(0);
                perubahan.waktu = ctime(&now);
                perubahan.rt = rtTerpilih.nama;
                perubahan.nikWarga = wargaTerpilih.nik;
                perubahan.namaWarga = wargaTerpilih.nama;
                perubahan.aksi = "EDIT";
                perubahan.fieldDiubah = "Alamat";
                perubahan.nilaiLama = alamatLama;
                perubahan.nilaiBaru = alamatBaru;
                rw.tambahRiwayat(perubahan);
                break;
            }
            }

            // Update kategori setelah edit
            string statusLower = wargaTerpilih.statusKeluarga;
            transform(statusLower.begin(), statusLower.end(), statusLower.begin(), ::tolower);

            if (wargaTerpilih.umur > 60 ||
                (statusLower == "yatim" && wargaTerpilih.umur < 18) ||
                wargaTerpilih.penghasilan < 500000)
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

void hapusDataWarga(RW &rw)
{
    while (true)
    {
        system("cls");

        cout << "====== HAPUS DATA WARGA ======" << endl;

        rw.tampilkanRT();
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
        RT *rtPtr = rw.getRTByIndex(pilihanRt - 1);
        if (rtPtr == nullptr)
        {
            cout << "Tidak ada RT yang tersedia. Silakan tambahkan RT terlebih dahulu." << endl;
            system("pause");
            return;
        }
        if (pilihanRt == 0)
            return;
        if (pilihanRt < 1 || static_cast<size_t>(pilihanRt) > rw.countRt())
        {
            cout << "RT tidak valid!" << endl;
            system("pause");
            continue;
        }

        RT &rtTerpilih = *rtPtr;

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
            cout << "NIK: " << wargaTerpilih.nik << endl;
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
            perubahan.nikWarga = wargaTerpilih.nik;
            perubahan.namaWarga = wargaTerpilih.nama;
            perubahan.aksi = "HAPUS";
            perubahan.fieldDiubah = "Semua Data";
            perubahan.nilaiLama = "-";
            perubahan.nilaiBaru = "-";
            rw.tambahRiwayat(perubahan);

            rtTerpilih.hapusWarga(wargaTerpilih.nik);
            cout << "Data warga berhasil dihapus." << endl;
            return;
        }
    }
}

void lihatRiwayatPerubahan(RW &rw)
{
    system("cls");
    cout << "====== RIWAYAT PERUBAHAN DATA ======" << endl;
    rw.tampilkanRiwayat();
}

void kelolaWarga(RW &rw)
{
    bool running = true;

    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA WARGA ======" << endl;
        cout << "Wilayah: " << rw.nama << endl;

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
            rw.tampilkanSemuaData();
            break;
        case 2:
            inputDataWarga(rw);
            break;
        case 3:
            editDataWarga(rw);
            break;
        case 4:
            hapusDataWarga(rw);
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

const int MAKS_ANTRIAN = 100;

struct Queue
{
    int front, rear;
    string isi[MAKS_ANTRIAN];
};

Queue antrianPrioritas, antrianReguler;

void createQueue(Queue &q)
{
    q.front = q.rear = -1;
}

bool isEmpty(Queue q)
{
    return q.rear == -1;
}

bool isFull(Queue q)
{
    return q.rear >= MAKS_ANTRIAN - 1;
}

void insertQueue(Queue &q, const string &data)
{
    if (isFull(q))
    {
        cout << "Antrian penuh!" << endl;
        return;
    }
    if (isEmpty(q))
    {
        q.front = q.rear = 0;
    }
    else
    {
        q.rear++;
    }
    q.isi[q.rear] = data;
}

void displayQueue(const Queue &q, const string &label)
{
    cout << "== " << label << " ==" << endl;
    if (isEmpty(q))
    {
        cout << "Antrian kosong" << endl;
        return;
    }
    for (int i = q.front; i <= q.rear; i++)
    {
        cout << i - q.front + 1 << ". " << q.isi[i] << endl;
    }
}



string toLower(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string formatWilayah(const string &input, const string &prefix)
{
    if (all_of(input.begin(), input.end(), ::isdigit))
    {
        stringstream ss;
        ss << prefix << " " << setw(2) << setfill('0') << stoi(input);
        return ss.str();
    }

    string lower = toLower(input);
    regex pattern(R"(rw\s*0?(\d+))");
    smatch match;
    if (prefix == "RW" && regex_match(lower, match, pattern))
    {
        int num = stoi(match[1]);
        stringstream ss;
        ss << prefix << " " << setw(2) << setfill('0') << num;
        return ss.str();
    }

    return input;
}

bool cekKembaliKeMenuUtama(const string &input)
{
    return toLower(input) == "m";
}

void menuDataWargaDanBantuan(Kota &kota)
{
    while (true)
    {
        system("cls");
        kota.tampilkanKecamatan();
        cout << "Masukkan nama Kecamatan (0: kembali): ";
        string inputKec;
        getline(cin, inputKec);
        if (inputKec == "0")
            return;
        if (inputKec.empty())
        {
            cout << "Nama Kecamatan tidak boleh kosong!\n";
            system("pause");
            continue;
        }

        Kecamatan *kec = nullptr;
        function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
        {
            if (!node)
                return;
            cariKec(node->left);
            if (toLower(node->data.nama) == toLower(inputKec))
                kec = &node->data;
            cariKec(node->right);
        };
        cariKec(kota.root);

        if (!kec)
        {
            cout << "Kecamatan \"" << inputKec << "\" tidak ditemukan!\n";
            system("pause");
            continue;
        }

        while (true)
        {
            system("cls");
            kec->tampilkanKelurahan();
            string inputKel;
            cout << "Masukkan nama Kelurahan (0: kembali, M: menu utama): ";
            getline(cin, inputKel);
            if (cekKembaliKeMenuUtama(inputKel))
                return;
            if (inputKel == "0")
                break;
            if (inputKel.empty())
            {
                cout << "Nama Kelurahan tidak boleh kosong!\n";
                system("pause");
                continue;
            }

            Kelurahan *kel = nullptr;
            function<void(QuadTreeNode *)> cariKel = [&](QuadTreeNode *node)
            {
                if (!node)
                    return;
                cariKel(node->left);
                if (toLower(node->data.nama) == toLower(inputKel))
                    kel = &node->data;
                cariKel(node->right);
            };
            cariKel(kec->root);

            if (!kel)
            {
                cout << "Kelurahan \"" << inputKel << "\" tidak ditemukan!\n";
                system("pause");
                continue;
            }

            while (true)
            {
                system("cls");
                kel->tampilkanRW();
                cout << "Masukkan nomor/nama RW (0: kembali, M: menu utama): ";
                string inputRW;
                getline(cin, inputRW);
                if (cekKembaliKeMenuUtama(inputRW))
                    return;
                if (inputRW == "0")
                    break;
                if (inputRW.empty())
                {
                    cout << "Input RW tidak boleh kosong!\n";
                    system("pause");
                    continue;
                }

                string targetRW = toLower(formatWilayah(inputRW, "RW"));
                RW *rw = nullptr;
                function<void(ThirdTreeNode *)> cariRW = [&](ThirdTreeNode *node)
                {
                    if (!node)
                        return;
                    cariRW(node->left);
                    string rwNama = toLower(formatWilayah(node->data.nama, "RW"));
                    if (rwNama == targetRW)
                        rw = &node->data;
                    cariRW(node->right);
                };
                cariRW(kel->root);

                if (!rw)
                {
                    cout << "RW \"" << inputRW << "\" tidak ditemukan!\n";
                    system("pause");
                    continue;
                }

                while (true)
                {
                    system("cls");
                    rw->tampilkanRT();
                    cout << "Masukkan nomor/nama RT (0: kembali, M: menu utama): ";
                    string inputRT;
                    getline(cin, inputRT);
                    if (cekKembaliKeMenuUtama(inputRT))
                        return;
                    if (inputRT == "0")
                        break;
                    if (inputRT.empty())
                    {
                        cout << "Input RT tidak boleh kosong!\n";
                        system("pause");
                        continue;
                    }

                    string targetRT = toLower(formatWilayah(inputRT, "RT"));
                    RT *rt = nullptr;
                    function<void(SecTreeNode *)> cariRT = [&](SecTreeNode *node)
                    {
                        if (!node)
                            return;
                        cariRT(node->left);
                        if (toLower(node->data.nama) == targetRT)
                            rt = &node->data;
                        cariRT(node->right);
                    };
                    cariRT(rw->root);

                    if (!rt)
                    {
                        cout << "RT \"" << inputRT << "\" tidak ditemukan!\n";
                        system("pause");
                        continue;
                    }

                    if (rt->countWarga() == 0)
                    {
                        cout << "RT ini belum memiliki warga.\n";
                        system("pause");
                        continue;
                    }

                    while (true)
                    {
                        system("cls");
                        cout << "Daftar Warga di " << rt->nama << ":\n";
                        rt->tampilkanWarga();
                        cout << "0. Kembali\nM. Menu utama\nPilih warga (nomor): ";
                        string input;
                        getline(cin, input);
                        if (cekKembaliKeMenuUtama(input))
                            return;
                        if (input == "0")
                            break;

                        int index = 0;
                        int pilihan = -1;
                        try
                        {
                            pilihan = stoi(input);
                        }
                        catch (...)
                        {
                        }
                        if (pilihan <= 0)
                        {
                            cout << "Input tidak valid!\n";
                            system("pause");
                            continue;
                        }

                        TreeNode *node = rt->cariWargaByIndex(rt->root, pilihan - 1, index);
                        if (!node)
                        {
                            cout << "Warga tidak ditemukan!\n";
                            system("pause");
                            continue;
                        }

                        while (true)
                        {
                            system("cls");
                            cout << "Detail Warga:\n"
                                 << "NIK  : " << node->data.nik << '\n'
                                 << "Nama : " << node->data.nama << "\n\n";

                            cout << "Daftar Bantuan:\n";
                            if (node->data.daftarBantuan.empty())
                                cout << "  - Belum ada bantuan.\n";
                            else
                            {
                                int no = 1;
                                for (const auto &b : node->data.daftarBantuan)
                                {
                                    cout << "  " << no++ << ". " << b.jenis;
                                    if (toLower(b.jenis) == "uang")
                                        cout << " (Rp " << b.nominal << ")";
                                    cout << " (Tanggal: " << b.tanggal << ")\n";
                                }
                            }

                            cout << "\n1. Tambah bantuan baru\n0. Kembali\nM. Menu utama\nPilih opsi: ";
                            string opsi;
                            getline(cin, opsi);
                            if (cekKembaliKeMenuUtama(opsi))
                                return;
                            if (opsi == "0")
                                break;
                            if (opsi != "1")
                            {
                                cout << "Opsi tidak valid!\n";
                                system("pause");
                                continue;
                            }

                            Bantuan bantuan;
                            while (true)
                            {
                                cout << "Jenis bantuan (Sembako/Uang/Pakaian): ";
                                getline(cin, bantuan.jenis);
                                string lower = toLower(bantuan.jenis);
                                if (lower == "sembako" || lower == "uang" || lower == "pakaian")
                                {
                                    bantuan.jenis = lower;
                                    if (lower == "uang")
                                    {
                                        while (true)
                                        {
                                            cout << "Masukkan nominal bantuan (dalam angka): Rp ";
                                            string inputNominal;
                                            getline(cin, inputNominal);
                                            try
                                            {
                                                bantuan.nominal = stoi(inputNominal);
                                                if (bantuan.nominal <= 0)
                                                    throw runtime_error("Nominal harus lebih dari 0.");
                                                break;
                                            }
                                            catch (...)
                                            {
                                                cout << "Nominal tidak valid!\n";
                                            }
                                        }
                                    }
                                    bantuan.jenis[0] = toupper(bantuan.jenis[0]);
                                    break;
                                }
                                cout << "Jenis bantuan tidak valid!\n";
                            }

                            while (true)
                            {
                                cout << "Tanggal bantuan (DD-MM-YYYY): ";
                                getline(cin, bantuan.tanggal);
                                regex tanggalRegex(R"(^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-(20\d{2})$)");
                                if (regex_match(bantuan.tanggal, tanggalRegex))
                                    break;
                                cout << "Format tanggal tidak valid!\n";
                            }

                            node->data.daftarBantuan.push_back(bantuan);
                            cout << "Bantuan berhasil ditambahkan!\n";
                            system("pause");
                        }
                    }
                }
            }
        }
    }
}

void statistikBantuanPerRW(Kota &kota)
{
    system("cls");
    cout << "====== STATISTIK BANTUAN ======" << endl;

    // --- 1. Pilih Kecamatan ---
    Kecamatan *kec = nullptr;
    while (!kec)
    {
        kota.tampilkanKecamatan();
        cout << "Masukkan nama Kecamatan (0 untuk batal): ";
        string inputKec;
        getline(cin, inputKec);
        if (inputKec == "0")
            return;

        function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
        {
            if (!node)
                return;
            cariKec(node->left);
            if (toLower(node->data.nama) == toLower(inputKec))
                kec = &node->data;
            cariKec(node->right);
        };
        cariKec(kota.root);
        if (!kec)
        {
            cout << "Kecamatan tidak ditemukan. Coba lagi.\n";
            system("pause");
            system("cls");
        }
    }

    // --- 2. Pilih Kelurahan ---
    Kelurahan *kel = nullptr;
    while (!kel)
    {
        system("cls");
        cout << "Daftar Kelurahan di Kecamatan " << kec->nama << ":\n";
        kec->tampilkanKelurahan();

        cout << "Masukkan nama Kelurahan (0 untuk batal): ";
        string inputKel;
        getline(cin, inputKel);
        if (inputKel == "0")
            return;

        function<void(QuadTreeNode *)> cariKel = [&](QuadTreeNode *node)
        {
            if (!node)
                return;
            cariKel(node->left);
            if (toLower(node->data.nama) == toLower(inputKel))
                kel = &node->data;
            cariKel(node->right);
        };
        cariKel(kec->root);
        if (!kel)
        {
            cout << "Kelurahan tidak ditemukan. Coba lagi.\n";
            system("pause");
            system("cls");
        }
    }

    // --- 3. Pilih RW ---
    RW *rw = nullptr;
    while (!rw)
    {
        system("cls");
        cout << "Daftar RW di Kelurahan " << kel->nama << ":\n";
        kel->tampilkanRW();

        cout << "Masukkan nama atau nomor RW (0 untuk batal): ";
        string inputRW;
        getline(cin, inputRW);
        if (inputRW == "0")
            return;

        string targetRW = toLower(formatWilayah(inputRW, "RW"));
        function<void(ThirdTreeNode *)> cariRW = [&](ThirdTreeNode *node)
        {
            if (!node)
                return;
            cariRW(node->left);
            string namaRW = toLower(formatWilayah(node->data.nama, "RW"));
            if (namaRW == targetRW)
                rw = &node->data;
            cariRW(node->right);
        };
        cariRW(kel->root);

        if (!rw)
        {
            cout << "RW tidak ditemukan. Coba lagi.\n";
            system("pause");
            system("cls");
        }
    }

    // --- 4. Tampilkan Statistik Tiap RT ---
    system("cls");
    cout << "Statistik Bantuan\n";
    cout << "Kecamatan : " << kec->nama << endl;
    cout << "Kelurahan : " << kel->nama << endl;
    cout << "RW        : " << rw->nama << endl;
    cout << "======================================" << endl;

    function<void(TreeNode *, map<string, int> &, int &)> hitungBantuan = [&](TreeNode *node, map<string, int> &counter, int &total)
    {
        if (!node)
            return;
        hitungBantuan(node->left, counter, total);
        for (const auto &b : node->data.daftarBantuan)
        {
            counter[b.jenis]++;
            total++;
        }
        hitungBantuan(node->right, counter, total);
    };

    function<void(SecTreeNode *)> tampilkanRTStatistik = [&](SecTreeNode *node)
    {
        if (!node)
            return;
        tampilkanRTStatistik(node->left);

        cout << "\nRT: " << node->data.nama << endl;
        map<string, int> counter;
        int total = 0;
        hitungBantuan(node->data.root, counter, total);

        if (total == 0)
        {
            cout << "  Belum ada bantuan tercatat." << endl;
        }
        else
        {
            cout << "  Total Bantuan: " << total << endl;
            for (const auto &entry : counter)
            {
                cout << "    - " << entry.first << ": " << entry.second << " kali" << endl;
            }
        }

        tampilkanRTStatistik(node->right);
    };

    tampilkanRTStatistik(rw->root);
}

void kelolaWilayahRT(RW &rw)
{
    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA RT ======" << endl;
        cout << "RW: " << rw.nama << endl;
        cout << "1. Tambah RT" << endl;
        cout << "2. Lihat Daftar RT" << endl;
        cout << "3. Kembali ke menu utama" << endl;
        cout << "Pilihan menu (1-3): ";

        int pilihan;
        string inputString;
        getline(cin, inputString);

        if (inputString.empty() || !(stringstream(inputString) >> pilihan) || pilihan < 1 || pilihan > 3)
        {
            cout << "Input tidak valid. Masukkan angka antara 1-3!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
        {
            system("cls");
            int nomorRt;
            bool validNomorRt = false;

            while (!validNomorRt)
            {
                cout << "Masukkan nomor RT yang akan ditambahkan (ketik 0 untuk kembali): ";

                string inputString;
                getline(cin, inputString);

                if (inputString == "0")
                {
                    cout << "Kembali ke menu utama." << endl;
                    return;
                }

                if (inputString.empty())
                {
                    cout << "Input tidak valid! Harap masukkan angka yang benar!" << endl;
                    system("pause");
                    system("cls");
                    continue;
                }

                stringstream ss(inputString);
                if (!(ss >> nomorRt))
                {
                    cout << "Input tidak valid! Harap masukkan angka yang benar!" << endl;
                    system("pause");
                    system("cls");
                    continue;
                }
                else
                {
                    if (nomorRt <= 0)
                    {
                        cout << "Nomor RT harus lebih besar dari 0!" << endl;
                        system("pause");
                        system("cls");
                    }
                    else
                    {
                        stringstream ssRt;
                        ssRt << setw(2) << setfill('0') << nomorRt;
                        string namaRt = "RT " + ssRt.str();

                        // === CEK APAKAH RT SUDAH ADA ===
                        bool rtExists = false;
                        int dummy = 0;
                        function<void(SecTreeNode *)> cekDuplikat = [&](SecTreeNode *node)
                        {
                            if (!node)
                                return;
                            cekDuplikat(node->left);
                            if (node->data.nama == namaRt)
                                rtExists = true;
                            cekDuplikat(node->right);
                        };
                        cekDuplikat(rw.root);

                        if (rtExists)
                        {
                            cout << "RT dengan nama '" << namaRt << "' sudah ada! Silakan masukkan nomor yang berbeda." << endl;
                            system("pause");
                            system("cls");
                        }
                        else
                        {
                            validNomorRt = true;
                            rw.tambahRT(RT(namaRt));
                            cout << namaRt << " berhasil ditambahkan!" << endl;
                            system("pause");
                        }
                    }
                }
            }
            break;
        }

        case 2:
            system("cls");
            rw.tampilkanRT();
            system("pause");
            break;

        case 3:
            running = false;
            break;

        default:
            cout << "Pilihan menu tidak valid!" << endl;
            break;
        }
    }
}

void kelolaWilayahRW(Kelurahan &kelurahan)
{
    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA RW ======" << endl;
        cout << "Kelurahan: " << kelurahan.nama << endl;
        cout << "1. Tambah RW" << endl;
        cout << "2. Lihat Daftar RW" << endl;
        cout << "3. Kembali ke menu utama" << endl;
        cout << "Pilihan menu (1-3): ";

        int pilihan;
        string inputString;
        getline(cin, inputString);

        if (inputString.empty() || !(stringstream(inputString) >> pilihan) || pilihan < 1 || pilihan > 3)
        {
            cout << "Input tidak valid. Masukkan angka antara 1-3!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
        {
            system("cls");
            int nomorRw;
            bool validNomorRw = false;

            while (!validNomorRw)
            {
                cout << "Masukkan nomor RW yang akan ditambahkan (ketik 0 untuk kembali): ";

                string inputString;
                getline(cin, inputString);

                if (inputString == "0")
                {
                    cout << "Kembali ke menu utama." << endl;
                    return;
                }

                if (inputString.empty())
                {
                    cout << "Input tidak valid! Harap masukkan angka yang benar!" << endl;
                    system("pause");
                    system("cls");
                    continue;
                }

                stringstream ss(inputString);
                if (!(ss >> nomorRw))
                {
                    cout << "Input tidak valid! Harap masukkan angka yang benar!" << endl;
                    system("pause");
                    system("cls");
                    continue;
                }
                else
                {
                    if (nomorRw <= 0)
                    {
                        cout << "Nomor RW harus lebih besar dari 0!" << endl;
                        system("pause");
                        system("cls");
                    }
                    else
                    {
                        stringstream ssRw;
                        ssRw << setw(2) << setfill('0') << nomorRw;
                        string namaRw = "RW " + ssRw.str();

                        // === CEK APAKAH RW SUDAH ADA ===
                        bool rwExists = false;
                        function<void(ThirdTreeNode *)> cekDuplikat = [&](ThirdTreeNode *node)
                        {
                            if (!node)
                                return;
                            cekDuplikat(node->left);
                            if (node->data.nama == namaRw)
                                rwExists = true;
                            cekDuplikat(node->right);
                        };
                        cekDuplikat(kelurahan.root);

                        if (rwExists)
                        {
                            cout << "RW dengan nama '" << namaRw << "' sudah ada! Silakan masukkan nomor yang berbeda." << endl;
                            system("pause");
                            system("cls");
                        }
                        else
                        {
                            validNomorRw = true;
                            kelurahan.tambahRW(RW(namaRw));
                            cout << namaRw << " berhasil ditambahkan!" << endl;
                            system("pause");
                        }
                    }
                }
            }
            break;
        }

        case 2:
        {
            system("cls");
            kelurahan.tampilkanRW();
            system("pause");
            break;
        }

        case 3:
        {
            running = false;
            break;
        }

        default:
        {
            cout << "Pilihan menu tidak valid!" << endl;
            system("pause");
            break;
        }
        }
    }
}

void kelolaWilayahKelurahan(Kecamatan &kecamatan)
{
    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA KELURAHAN ======" << endl;
        cout << "Kecamatan: " << kecamatan.nama << endl;
        cout << "1. Tambah Kelurahan" << endl;
        cout << "2. Lihat Daftar Kelurahan" << endl;
        cout << "3. Kembali ke menu utama" << endl;
        cout << "Pilihan menu (1-3): ";

        int pilihan;
        string inputString;
        getline(cin, inputString);

        if (inputString.empty() || !(stringstream(inputString) >> pilihan) || pilihan < 1 || pilihan > 3)
        {
            cout << "Input tidak valid. Masukkan angka antara 1-3!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
        {
            system("cls");
            bool valid = false;

            while (!valid)
            {
                cout << "Masukkan nama Kelurahan yang akan ditambahkan (ketik 0 untuk kembali): ";
                string namaKelurahan;
                getline(cin, namaKelurahan);

                if (namaKelurahan == "0")
                {
                    cout << "Kembali ke menu utama." << endl;
                    return;
                }

                if (namaKelurahan.empty())
                {
                    cout << "Input tidak boleh kosong!" << endl;
                    system("pause");
                    system("cls");
                    continue;
                }

                // Cek duplikat
                bool sudahAda = false;
                function<void(QuadTreeNode *)> cekDuplikat = [&](QuadTreeNode *node)
                {
                    if (!node)
                        return;
                    cekDuplikat(node->left);
                    if (node->data.nama == namaKelurahan)
                        sudahAda = true;
                    cekDuplikat(node->right);
                };
                cekDuplikat(kecamatan.root);

                if (sudahAda)
                {
                    cout << "Kelurahan '" << namaKelurahan << "' sudah ada!" << endl;
                    system("pause");
                    system("cls");
                }
                else
                {
                    valid = true;
                    kecamatan.tambahKelurahan(Kelurahan(namaKelurahan));
                    cout << "Kelurahan '" << namaKelurahan << "' berhasil ditambahkan!" << endl;
                    system("pause");
                }
            }
            break;
        }

        case 2:

            system("cls");
            kecamatan.tampilkanKelurahan();
            system("pause");
            break;

        case 3:

            running = false;
            break;

        default:

            cout << "Pilihan menu tidak valid!" << endl;
            system("pause");
            break;
        }
    }
}

void kelolaWilayahKecamatan(Kota &kota)
{
    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA KECAMATAN ======" << endl;
        cout << "Kota: " << kota.nama << endl;
        cout << "1. Tambah Kecamatan" << endl;
        cout << "2. Lihat Daftar Kecamatan" << endl;
        cout << "3. Kembali ke menu utama" << endl;
        cout << "Pilihan menu (1-3): ";

        int pilihan;
        string inputString;
        getline(cin, inputString);

        if (inputString.empty() || !(stringstream(inputString) >> pilihan) || pilihan < 1 || pilihan > 3)
        {
            cout << "Input tidak valid. Masukkan angka antara 1-3!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
        {
            system("cls");
            bool valid = false;

            while (!valid)
            {
                cout << "Masukkan nama kecamatan yang akan ditambahkan (ketik 0 untuk kembali): ";
                string namaKecamatan;
                getline(cin, namaKecamatan);

                if (namaKecamatan == "0")
                {
                    cout << "Kembali ke menu utama." << endl;
                    return;
                }

                if (namaKecamatan.empty())
                {
                    cout << "Input tidak boleh kosong!" << endl;
                    system("pause");
                    system("cls");
                    continue;
                }

                // Cek duplikat
                bool sudahAda = false;
                function<void(PentaTreeNode *)> cekDuplikat = [&](PentaTreeNode *node)
                {
                    if (!node)
                        return;
                    cekDuplikat(node->left);
                    if (node->data.nama == namaKecamatan)
                        sudahAda = true;
                    cekDuplikat(node->right);
                };
                cekDuplikat(kota.root);

                if (sudahAda)
                {
                    cout << "Kecamatan '" << namaKecamatan << "' sudah ada!" << endl;
                    system("pause");
                    system("cls");
                }
                else
                {
                    valid = true;
                    kota.tambahKecamatan(Kecamatan(namaKecamatan));
                    cout << "Kecamatan '" << namaKecamatan << "' berhasil ditambahkan!" << endl;
                    system("pause");
                }
            }
            break;
        }
        break;

        case 2:
            system("cls");
            kota.tampilkanKecamatan();
            system("pause");
            break;

        case 3:
            running = false;
            break;

        default:
            cout << "Pilihan menu tidak valid!" << endl;
            system("pause");
            break;
        }
    }
}

void pilihDanKelolaRT(Kota &kota)
{
    system("cls");
    kota.tampilkanKecamatan();

    cout << "Masukkan nama kecamatan: ";
    string namaKec;
    getline(cin, namaKec);

    // Pencarian case-insensitive untuk Kecamatan
    Kecamatan *kec = nullptr;
    function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
    {
        if (!node)
            return;
        cariKec(node->left);
        if (toLower(node->data.nama) == toLower(namaKec))
            kec = &node->data;
        cariKec(node->right);
    };
    cariKec(kota.root);

    if (!kec)
    {
        cout << "Kecamatan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kec->tampilkanKelurahan();
    cout << "Masukkan nama kelurahan: ";
    string namaKel;
    getline(cin, namaKel);

    // Pencarian case-insensitive untuk Kelurahan
    Kelurahan *kel = nullptr;
    function<void(QuadTreeNode *)> cariKel = [&](QuadTreeNode *node)
    {
        if (!node)
            return;
        cariKel(node->left);
        if (toLower(node->data.nama) == toLower(namaKel))
            kel = &node->data;
        cariKel(node->right);
    };
    cariKel(kec->root);

    if (!kel)
    {
        cout << "Kelurahan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kel->tampilkanRW();
    cout << "Masukkan nama RW: ";
    string namaRW;
    getline(cin, namaRW);

    // Pencarian case-insensitive untuk RW
    RW *rw = nullptr;
    function<void(ThirdTreeNode *)> cariRW = [&](ThirdTreeNode *node)
    {
        if (!node)
            return;
        cariRW(node->left);
        if (toLower(node->data.nama) == toLower(formatWilayah(namaRW, "RW")))
            rw = &node->data;
        cariRW(node->right);
    };
    cariRW(kel->root);

    if (!rw)
    {
        cout << "RW tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    kelolaWilayahRT(*rw);
}

void pilihDanKelolaRW(Kota &kota)
{
    system("cls");
    kota.tampilkanKecamatan();

    cout << "Masukkan nama kecamatan: ";
    string namaKec;
    getline(cin, namaKec);

    // Cari Kecamatan secara manual (case-insensitive)
    Kecamatan *kec = nullptr;
    function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
    {
        if (!node)
            return;
        cariKec(node->left);
        if (toLower(node->data.nama) == toLower(namaKec))
            kec = &node->data;
        cariKec(node->right);
    };
    cariKec(kota.root);

    if (!kec)
    {
        cout << "Kecamatan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kec->tampilkanKelurahan();

    cout << "Masukkan nama kelurahan: ";
    string namaKel;
    getline(cin, namaKel);

    // Cari Kelurahan secara manual (case-insensitive)
    Kelurahan *kel = nullptr;
    function<void(QuadTreeNode *)> cariKel = [&](QuadTreeNode *node)
    {
        if (!node)
            return;
        cariKel(node->left);
        if (toLower(node->data.nama) == toLower(namaKel))
            kel = &node->data;
        cariKel(node->right);
    };
    cariKel(kec->root);

    if (!kel)
    {
        cout << "Kelurahan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    kelolaWilayahRW(*kel);
}

void kelolaWilayah(Kota &kota)
{
    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== KELOLA DATA WILAYAH ======" << endl;
        cout << "1. Kelola Kecamatan" << endl;
        cout << "2. Kelola Kelurahan" << endl;
        cout << "3. Kelola RW" << endl;
        cout << "4. Kelola RT" << endl;
        cout << "5. Kembali ke menu utama" << endl;
        cout << "Pilihan menu (1-5): ";

        int pilihan;
        string inputString;
        getline(cin, inputString);

        if (inputString.empty() || !(stringstream(inputString) >> pilihan) || pilihan < 1 || pilihan > 5)
        {
            cout << "Input tidak valid. Masukkan angka antara 1-5!" << endl;
            system("pause");
            continue;
        }

        switch (pilihan)
        {
        case 1:
            kelolaWilayahKecamatan(kota);
            break;
        case 2:
        {

            system("cls");
            kota.tampilkanKecamatan();

            string input;
            cout << "Pilih Kecamatan: ";
            getline(cin, input);

            Kecamatan *kecamatan = nullptr;
            function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
            {
                if (!node)
                    return;
                cariKec(node->left);
                if (toLower(node->data.nama) == toLower(input))
                    kecamatan = &node->data;
                cariKec(node->right);
            };
            cariKec(kota.root);

            if (!kecamatan)
            {
                cout << "Kecamatan tidak ditemukan!" << endl;
                system("pause");
                continue;
            }

            kelolaWilayahKelurahan(*kecamatan);
            break;
        }
        case 3:
            pilihDanKelolaRW(kota);
            break;

        case 4:
            pilihDanKelolaRT(kota);
            break;
        case 5:
            running = false;
            break;
        default:
            cout << "Pilihan menu tidak valid!" << endl;
            system("pause");
            break;
        }
    }
}

void pilihKelolaWarga(Kota &kota)
{
    system("cls");
    kota.tampilkanKecamatan();

    cout << "Masukkan nama kecamatan: ";
    string namaKec;
    getline(cin, namaKec);

    Kecamatan *kec = nullptr;
    function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
    {
        if (!node)
            return;
        cariKec(node->left);
        if (toLower(node->data.nama) == toLower(namaKec))
            kec = &node->data;
        cariKec(node->right);
    };
    cariKec(kota.root);

    if (!kec)
    {
        cout << "Kecamatan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kec->tampilkanKelurahan();
    cout << "Masukkan nama kelurahan: ";
    string namaKel;
    getline(cin, namaKel);

    Kelurahan *kel = nullptr;
    function<void(QuadTreeNode *)> cariKel = [&](QuadTreeNode *node)
    {
        if (!node)
            return;
        cariKel(node->left);
        if (toLower(node->data.nama) == toLower(namaKel))
            kel = &node->data;
        cariKel(node->right);
    };
    cariKel(kec->root);

    if (!kel)
    {
        cout << "Kelurahan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kel->tampilkanRW();
    cout << "Masukkan nama RW: ";
    string namaRW;
    getline(cin, namaRW);

    RW *rw = nullptr;
    function<void(ThirdTreeNode *)> cariRW = [&](ThirdTreeNode *node)
    {
        if (!node)
            return;
        cariRW(node->left);
        if (toLower(node->data.nama) == toLower(formatWilayah(namaRW, "RW")))
            rw = &node->data;
        cariRW(node->right);
    };
    cariRW(kel->root);

    if (!rw)
    {
        cout << "RW tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    kelolaWarga(*rw);
}


void buatAntrianDariWarga(const RW &rw)
{
    createQueue(antrianPrioritas);
    createQueue(antrianReguler);

    // Fungsi untuk menelusuri seluruh RT dalam tree (SecTreeNode)
    function<void(SecTreeNode *)> traverseRTTree;
    traverseRTTree = [&](SecTreeNode *node)
    {
        if (!node)
            return;

        traverseRTTree(node->left);

        // Traversal pohon warga dalam RT (TreeNode*)
        function<void(TreeNode *)> traverseWarga;
        traverseWarga = [&](TreeNode *wargaNode)
        {
            if (!wargaNode)
                return;
            traverseWarga(wargaNode->left);

            string nama = wargaNode->data.nama;
            string kategori = wargaNode->data.kategori;

            if (kategori == "Prioritas")
                insertQueue(antrianPrioritas, nama);
            else
                insertQueue(antrianReguler, nama);

            traverseWarga(wargaNode->right);
        };

        // Akses root pohon warga dari RT ini
        traverseWarga(node->data.root);

        traverseRTTree(node->right);
    };

    // Mulai traversal dari root RW
    traverseRTTree(rw.root);

    cout << "Antrian berhasil dibuat dari data warga!" << endl;
}

void pilihKelolaAntrian(Kota &kota)
{
    system("cls");
    kota.tampilkanKecamatan();

    cout << "Masukkan nama kecamatan: ";
    string namaKec;
    getline(cin, namaKec);

    Kecamatan *kec = nullptr;
    function<void(PentaTreeNode *)> cariKec = [&](PentaTreeNode *node)
    {
        if (!node)
            return;
        cariKec(node->left);
        if (toLower(node->data.nama) == toLower(namaKec))
            kec = &node->data;
        cariKec(node->right);
    };
    cariKec(kota.root);

    if (!kec)
    {
        cout << "Kecamatan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kec->tampilkanKelurahan();
    cout << "Masukkan nama kelurahan: ";
    string namaKel;
    getline(cin, namaKel);

    Kelurahan *kel = nullptr;
    function<void(QuadTreeNode *)> cariKel = [&](QuadTreeNode *node)
    {
        if (!node)
            return;
        cariKel(node->left);
        if (toLower(node->data.nama) == toLower(namaKel))
            kel = &node->data;
        cariKel(node->right);
    };
    cariKel(kec->root);

    if (!kel)
    {
        cout << "Kelurahan tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    system("cls");
    kel->tampilkanRW();
    cout << "Masukkan nama RW: ";
    string namaRW;
    getline(cin, namaRW);

    RW *rw = nullptr;
    function<void(ThirdTreeNode *)> cariRW = [&](ThirdTreeNode *node)
    {
        if (!node)
            return;
        cariRW(node->left);
        if (toLower(node->data.nama) == toLower(formatWilayah(namaRW, "RW")))
            rw = &node->data;
        cariRW(node->right);
    };
    cariRW(kel->root);

    if (!rw)
    {
        cout << "RW tidak ditemukan!" << endl;
        system("pause");
        return;
    }

    // ====== Bagian yang kamu minta dipindah ======
    system("cls");
    cout << "====== ANTRIAN PENERIMA BANTUAN ======" << endl;
    buatAntrianDariWarga(*rw);
    displayQueue(antrianPrioritas, "Antrian Prioritas");
    cout << endl;
    displayQueue(antrianReguler, "Antrian Reguler");

}

int main()
{
    Kota kota("Bandung");
    Kecamatan kecamatan("Bubat");
    Kelurahan kelurahan("Caringin");
    RW rw("RW 01");

    rw.tambahRT(RT("RT 01"));
    rw.tambahRT(RT("RT 02"));
    rw.tambahRT(RT("RT 03"));

    RT *rt1 = rw.cariRT("RT 01");
    RT *rt2 = rw.cariRT("RT 02");
    RT *rt3 = rw.cariRT("RT 03");

    if (rt1)
    {
        rt1->tambahWarga(Warga{"3201012000010001", "Asep", 19, 20000, "Bukan", "Jl. Caringin No. 2", "Prioritas"});
        rt1->tambahWarga(Warga{"3201012000010002", "Budin", 17, 10000, "Yatim", "Jl. Caringin No. 3", "Prioritas"});
    }

    if (rt2)
    {
        rt2->tambahWarga(Warga{"3201012000010003", "Cecep", 68, 10000, "Yatim", "Jl. Amba No. 3", "Prioritas"});
        rt2->tambahWarga(Warga{"3201012000010004", "Doni", 24, 1000000, "Yatim", "Jl. Amba No. 2", "Reguler"});
    }

    if (rt3)
    {
        rt3->tambahWarga(Warga{"3201012000010005", "Edi", 35, 10000, "Yatim", "Jl. Nigerian No. 1", "Prioritas"});
        rt3->tambahWarga(Warga{"3201012000010006", "Feri", 18, 10000, "Bukan", "Jl. Nigerian No. 2", "Prioritas"});
    }

    kelurahan.tambahRW(rw);
    kecamatan.tambahKelurahan(kelurahan);
    kota.tambahKecamatan(kecamatan);

    bool running = true;
    while (running)
    {
        system("cls");
        cout << "====== APLIKASI PENDATAAN BANSOS ======" << endl;
        cout << "Wilayah: " << kota.nama << endl
             << endl;
        cout << "1. Kelola Data Warga" << endl;
        cout << "2. Daftarkan Bantuan untuk Warga" << endl;
        cout << "3. Lihat Antrian Penerima Bantuan" << endl;
        cout << "4. Lihat Riwayat Perubahan Data" << endl;
        cout << "5. Kelola Data Wilayah" << endl;
        cout << "6. Statistik Bantuan" << endl;
        cout << "7. Keluar" << endl;
        cout << "Pilihan menu (1-7): ";

        string inputString;
        getline(cin, inputString);

        if (inputString.empty())
        {
            cout << "Tidak ada input." << endl;
            system("pause");
            continue;
        }

        // Cek apakah input adalah angka valid
        bool isValid = true;
        for (char c : inputString)
        {
            if (!isdigit(c))
            {
                isValid = false;
                break;
            }
        }

        if (!isValid)
        {
            cout << "Input tidak valid. Masukkan angka saja!" << endl;
            system("pause");
            continue;
        }

        int pilihan = stoi(inputString);

        switch (pilihan)
        {
        case 1:
            pilihKelolaWarga(kota);
            break;
        case 2:
            menuDataWargaDanBantuan(kota);
            break;
        case 3:
            pilihKelolaAntrian(kota);
            break;
        case 4:
            lihatRiwayatPerubahan(rw);
            break;
        case 5:
            kelolaWilayah(kota);
            break;
        case 6:
            statistikBantuanPerRW(kota);
            break;
        case 7:
            cout << "Happy Nice Day!" << endl;
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