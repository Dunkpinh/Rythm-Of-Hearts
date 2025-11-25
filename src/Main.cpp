#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h> 
#include <windows.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

struct Record {
    string nickname;
    int skor;
    string tanggal;
};

string spade = u8"\u2660";
string heart = u8"\u2665";
string club = u8"\u2663";
string diamond = u8"\u2666";
// Method global
void delayPrintEndl(const string& text, int delayMs = 100) { // print dengan delay
        for (size_t i = 0; i < text.size(); i++) {
            char c = text[i];
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
        cout << endl;
}
void delayPrint(const string& text, int delayMs = 100) { // print dengan delay
        for (size_t i = 0; i < text.size(); i++) {
            char c = text[i];
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
}
void tunggu(int delayMs = 100){
    this_thread::sleep_for(chrono::milliseconds(delayMs));
}
void hideCursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void layarBersih() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string getTanggal(){
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&t);
    char buffer[11]; // "dd-mm-yyyy" = 10 + null terminator
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", localTime);
    return buffer;
}

// Merge Sort
void array_merge(vector<Record>& data, int left, int mid, int right) { // Merging
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Record> L;
    vector<Record> R;

    for (int i = 0; i < n1; i++) L.push_back(data[left + i]);
    for (int j = 0; j < n2; j++) R.push_back(data[mid + 1 + j]);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].skor >= R[j].skor) { // descending
            data[k++] = L[i++];
        } else {
            data[k++] = R[j++];
        }
    }

    while (i < n1) data[k++] = L[i++];
    while (j < n2) data[k++] = R[j++];
}

void merge_sort(vector<Record>& data, int left, int right) { // Rekursif
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(data, left, mid);
        merge_sort(data, mid + 1, right);
        array_merge(data, left, mid, right);
    }
}

void printVector(vector<Record>& data){
    int size = 3;
    if(data.size() < 3) size = data.size();

    for (size_t i = 0; i < size; i++)
    {
        cout << data[i].nickname << endl;
        cout << data[i].skor << endl;
        cout << data[i].tanggal << endl << endl;
    }
}

void readRecords(vector<Record>& records){
        fstream file("src/dataRecords.txt");
        Record r;
        string skorStr;
        while (getline(file, r.nickname) &&
            getline(file, skorStr) &&
            getline(file, r.tanggal)) {
            r.skor = stoi(skorStr);   
            records.push_back(r);
        }
        file.close();
}

void writeRecords(const Record& records) {
    fstream file("src/dataRecords.txt", ios::app); 
    if (!file) {
        cerr << "File tidak bisa dibuka!" << endl;
        return;
    }

    file << records.nickname << endl << records.skor << endl << records.tanggal << endl << endl;
}


class Game{
    private:
        int nyawa;
        int waktu;
        int skor;
        int jumlahKartu = 3;
        int jumlahRandom = 0;
        bool faseHati, faseWajik, faseKeriting, faseSekop;
        char inSpade = '1', inHeart = '2', inClub = '3', inDiamond = '4';
        string setKartu;

    public:
        Game(int nyawa, int waktu, int skor){ // konstruktor
            this->nyawa = nyawa;
            this->waktu = waktu;
            this->skor = skor;
        }
        int getNyawa(){
            return nyawa;
        }
        int getSkor(){
            return skor;
        }
        void statusFaseHati(string& text){
            int beruntun = 0;
            for(size_t i = 0; i < text.size(); i++){
                if(text.substr(i, 3) == spade){
                    i += spade.size() - 1;
                    beruntun++;
                }
                else{
                    beruntun = 0;
                }
                if(beruntun == 3){
                    cout << "Fase hati terbuka!" << endl;
                    faseHati = true;
                    jumlahKartu = 4;
                    return;
                }
            }
        }
        void kartuRandom(){
            srand(time(0));
            string tempSetKartu = "";
            int idxKartu;
            string kartu;
            if(jumlahKartu == 3){
                for (size_t i = 0; i < jumlahKartu; i++){
                    do{ 
                        idxKartu = rand() % 4 + 1; 
                    }while(idxKartu == 2);
                    switch(idxKartu){
                        case 1: kartu = spade; break; // ♠
                        // ♥
                        case 3: kartu = club; break; // ♣
                        case 4: kartu = diamond; break; // ♦
                    }
                    tempSetKartu += kartu;
                }
            }else{
                for (size_t i = 0; i < jumlahKartu; i++){
                    int idxKartu = rand() % 4 + 1; // angka random 1-4
                    string kartu;
                    switch(idxKartu){
                        case 1: kartu = spade; break; // ♠
                        case 2: kartu = heart; break; // ♥
                        case 3: kartu = club; break; // ♣
                        case 4: kartu = diamond; break; // ♦
                    }
                    tempSetKartu += kartu;
                }
            }
            jumlahRandom++;
            if(jumlahKartu == 3 && jumlahRandom == 15){
                setKartu = spade + spade + spade; 
                delayPrint(setKartu, 100);
                cout << endl;
                return;
            }  
            setKartu = tempSetKartu;
            delayPrint(setKartu, 100);
            cout << endl;
        }
        bool inputKartu(){
            string input;
            bool hit;
            for (size_t i = 0; i < jumlahKartu; i++){
                char c;
                c = _getch(); // baca input tanpa enter
                if (c == getInSpade()) {
                    cout << spade;
                    input += spade;
                } else if (c == getInHeart()) {
                    cout << heart;
                    input += heart;
                } else if (c == getInClub()) {
                    cout << club;
                    input += club;
                } else if (c == getInDiamond()) {
                    cout << diamond;
                    input += diamond;
                } else {
                    cout << c;
                }
            }
            if(setKartu == input){
                skor += 5;
                hit = true;
            }else{
                nyawa--;
                hit = false;
            }
            statusFaseHati(input);
            return hit;
        }
        void setInput(char inSpade, char inHeart, char inClub, char inDiamond){
            this->inSpade = inSpade;
            this->inHeart = inHeart;
            this->inClub = inClub;
            this->inDiamond = inDiamond;
        }
        char getInSpade(){
            return inSpade;
        }
        char getInHeart(){
            return inHeart;
        }
        char getInClub(){
            return inClub;
        }
        char getInDiamond(){
            return inDiamond;
        }

};

class Menu{
    private:
        Game rythmOfHearts = Game(3, 20, 0);
    public:
        string sambutan1 = spade + " Selamat Datang Di Game Rythm Of Hearts " + spade;
        string sambutan2 = heart + " Selamat Datang Di Game Rythm Of Hearts " + heart;
        string sambutan3 = club + " Selamat Datang Di Game Rythm Of Hearts " + club;
        string sambutan4 = diamond + " Selamat Datang Di Game Rythm Of Hearts " + diamond;
        Menu(){
            delayPrint(sambutan2, 50);
            tunggu(750);
            layarBersih();
            cout << sambutan3;
            tunggu(750);
            layarBersih();
            cout << sambutan4;
            tunggu(750);
            layarBersih();
            cout << sambutan1;
            tunggu(750);
            cout << endl;
            pilihan();
        }
        void pilihan(){
            delayPrint("(1) Peraturan", 25); 
            cout << endl;
            tunggu(250);
            delayPrint("(2) New game", 25);
            cout << endl;
            tunggu(250);
            delayPrint("(3) Record", 25);
            cout << endl;
            tunggu(250);
            delayPrint("(4) Setting", 25);
            cout << endl;
            tunggu(250);
            delayPrint("(5) Exit", 25);
            cout << endl;
            tunggu(250);
            delayPrint("Pilih: ", 15);
            char c;
            c = _getch();
            switch(c){
                case '1':tampilkanPeraturan(); break;
                case '2':layarBersih(); newGame(); break;
                case '3':layarBersih(); tampilkanRecord(); break;
                case '4': break;
                case '5':cout << endl; delayPrint("Terimakasih sudah bermain :) ", 25); break;
            }
            while(c != '4'){
                cout << "(1) Peraturan" << endl;
                cout << "(2) New game" << endl;
                cout << "(3) Record" << endl;
                cout << "(4) Setting" << endl;
                cout << "(5) Exit" << endl;
                delayPrint("Pilih: ", 15);
                c = _getch();
                switch(c){
                    case '1':tampilkanPeraturan(); break;
                    case '2':layarBersih(); newGame(); break;
                    case '3':layarBersih(); tampilkanRecord(); break;
                    case '4':; break;
                    case '5':cout << endl; delayPrint("Terimakasih sudah bermain :) ", 25); break;
                }
            }
            
        }
        void tampilkanRecord(){
            vector<Record> records;
            readRecords(records);
            merge_sort(records, 0, records.size()-1);
            printVector(records);
            cout << "Tekan tombol apa saja untuk keluar";
            char c;
            c = _getch();
            layarBersih();
        }
        void newGame(){
            Record records;
            string nickname;

            delayPrint("Masukkan nickname: ", 25);
            getline(cin, nickname);
            layarBersih();
            delayPrint(nickname + ", ", 25);
            tunggu(750);
            delayPrint("Bersiaplah! ", 25);
            tunggu(750);
            delayPrint(". . .", 250);
            tunggu(1500);
            layarBersih();

            while(rythmOfHearts.getNyawa() > 0){
                rythmOfHearts.kartuRandom();
                bool hit = rythmOfHearts.inputKartu();
                if(hit) cout << "  Berhasil!" << endl;
                else cout << "  Gagal!" << endl;
            }

            records.nickname = nickname;
            records.skor = rythmOfHearts.getSkor();
            records.tanggal = getTanggal();

            cout << endl;
            string strSkor = to_string(rythmOfHearts.getSkor()); 
            delayPrint("Skor = ", 50);
            tunggu(1500);
            delayPrint(strSkor, 25); 
            tunggu(500);
            cout << endl << endl; 

            delayPrint("Simpan Record? (Y/N)", 15);
            cout << endl;
            char c;
            c = _getch();
            switch (c){
                case 'y':writeRecords(records); delayPrint("Record tersimpan! ", 25); cout << endl;
                case 'n':delayPrint("E M U A C H", 250); cout << endl << endl;
            }
            cout << "Tekan tombol apa saja untuk keluar";
            c = _getch();
            layarBersih();
        }
        void tampilkanPeraturan(){
            char c;
            layarBersih();
            delayPrint("(1) Aturan Dasar", 25);
            cout << endl;
            tunggu();
            delayPrint("(2) Fase", 25);
            cout << endl;
            tunggu();
            delayPrint("(3) jkajks", 25);
            cout << endl;
            tunggu();
            delayPrint("(4) kalsfhak", 25);
            cout << endl;
            delayPrint("Pilih:", 15);
            cout << endl;
            c = _getch();
            switch(c){
                case '1':layarBersih(); aturanDasar(); layarBersih(); break;
                case '2':break;
                case '3':break;
                case '4':break;
            }
            while(c != '4'){
                cout << "(1) Aturan Dasar" << endl;
                cout << "(2) fase" << endl;
                cout << "(3) uhaiudhiu" << endl;
                cout << "(4) jlandj" << endl;
                delayPrint("Pilih: ", 15);
                cout << endl;
                c = _getch();
                switch(c){
                    case '1':layarBersih(); aturanDasar(); layarBersih(); break;
                    case '2':
                    case '3':
                    case '4':layarBersih();
                }
            }
        }
        void aturanDasar(){
            char c;
            delayPrint("Aturan Dasar", 25);
            c = _getch();
            cout << endl;
            delayPrint("    Akan muncul beberapa karakter secara random di layar", 25);
            c = _getch();
            cout << endl;
            delayPrint("    " + spade + " " + heart + " " + club + " " + diamond, 25);
            c = _getch();
            cout << endl;
            delayPrint("    Pemain harus menginput karakter yang muncul dengan benar dalam rentang waktu", 25);
            c = _getch();
            cout << endl;
            std::string teksPenjelasan = "Empat karakter diwakilkan oleh: " + std::string(1, rythmOfHearts.getInSpade()) + "(" + spade + "), " + std::string(1, rythmOfHearts.getInHeart()) + "(" + heart + "), " + std::string(1, rythmOfHearts.getInClub()) + "(" + club + "), " + std::string(1, rythmOfHearts.getInDiamond()) + "(" + diamond + ")";
            delayPrint("    " + teksPenjelasan, 25);
            c = _getch();
            cout << endl;
            delayPrint("    Misal", 25);
            c = _getch();
            cout << endl;
            delayPrint("        " + spade + heart + diamond, 25);
            c = _getch();
            cout << endl;
            delayPrint("    Maka pemain harus menginput: " + spade + heart + diamond, 25);
            c = _getch();
            cout << endl;
            delayPrint("    Kamu coba!", 25);
            c = _getch();
            cout << endl;
            cout << "        ";
            rythmOfHearts.kartuRandom();
            bool hit;
            do{
                cout << "        ";
                hit = rythmOfHearts.inputKartu();
                if(!hit) cout << endl << "    Coba lagi";
                cout << endl;
            }while(!hit);
            delayPrint("    Nice!", 25); 
            cout << endl;
            delayPrint("    Namun perlu diperhatikan, pemain harus memasukkan dalam rentang waktu", 25);
            cout << endl;
            c = _getch();
            delayPrint("    Maka pastikan jangan sampai lewat waktunya ya!", 25);
            cout << endl;
            c = _getch();
            delayPrint("    Setiap kali berhasil pemain akan mendapatkan 5 poin, namun jika gagal mengurangi 1 nyawa", 25);
            cout << endl;
            c = _getch();
            delayPrint("    Pemain akan memiliki 3 nyawa", 25);
            cout << endl;
            c = _getch();
            delayPrint("    Tugas pemain adalah mendapatkan poin sebanyak-banyaknya", 25);
            cout << endl;
            c = _getch();
            delayPrint("    Good luck!", 25);
            cout << endl;
            c = _getch();
        }

};

int main(int argc, char const *argv[])
{
    hideCursor();  
    layarBersih();
    SetConsoleOutputCP(CP_UTF8);
    Menu tampilanAwal;
}
