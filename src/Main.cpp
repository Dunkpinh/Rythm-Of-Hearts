#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h> 
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

class game{
    private:
        int nyawa;
        int waktu;
        int skor;
        int jumlahKartu = 3;
        int jumlahRandom = 0;
        bool faseHati, faseWajik, faseKeriting, faseSekop;
        string setKartu;

    void delayPrint(const string& text, int delayMs = 100) { // print dengan delay
        for (size_t i = 0; i < text.size(); i++) {
            char c = text[i];
            cout << c;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
        cout << endl;
    }

    public:
        game(int nyawa, int waktu, int skor){ // konstruktor
            this->nyawa = nyawa;
            this->waktu = waktu;
            this->skor = skor;
        }
        int getNyawa(){
            return nyawa;
        }
        void statusFaseHati(string& text){
            int beruntun = 0;
            string spade = u8"\u2660";
            for(size_t i = 0; i < text.size(); i++){
                if(text.substr(i, 3) == spade){
                    i += spade.size() - 1;
                    beruntun++;
                }
                else{
                    beruntun = 0;
                }
                if(beruntun == 3){
                    cout << "Fase hati terbuka!";
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
            SetConsoleOutputCP(CP_UTF8);
            if(jumlahKartu == 3){
                for (size_t i = 0; i < jumlahKartu; i++){
                    do{ 
                        idxKartu = rand() % 4 + 1; 
                    }while(idxKartu == 2);
                    switch(idxKartu){
                        case 1: kartu = u8"\u2660"; break; // ♠
                        // ♥
                        case 3: kartu = u8"\u2663"; break; // ♣
                        case 4: kartu = u8"\u2666"; break; // ♦
                    }
                    tempSetKartu += kartu;
                }
            }else{
                for (size_t i = 0; i < jumlahKartu; i++){
                    int idxKartu = rand() % 4 + 1; // angka random 1-4
                    string kartu;
                    switch(idxKartu){
                        case 1: kartu = u8"\u2660"; break; // ♠
                        case 2: kartu = u8"\u2665"; break; // ♥
                        case 3: kartu = u8"\u2663"; break; // ♣
                        case 4: kartu = u8"\u2666"; break; // ♦
                    }
                    tempSetKartu += kartu;
                }
            }
            jumlahRandom++;
            if(jumlahKartu == 3 && jumlahRandom == 15){
                string spade = u8"\u2660";
                setKartu = spade + spade + spade; // langsung gabung 3 kali
                delayPrint(setKartu, 100);
                return;
            }  
            setKartu = tempSetKartu;
            delayPrint(setKartu, 100);
        }
        void inputKartu(){
            string input;
            for (size_t i = 0; i < jumlahKartu; i++){
                char c;
                c = _getch(); // baca input tanpa enter
                switch(c) {
                    case '1': cout << "\u2660"; input += u8"\u2660"; break; // ♠
                    case '2': cout << "\u2665"; input += u8"\u2665"; break; // ♥
                    case '3': cout << "\u2663"; input += u8"\u2663"; break; // ♣
                    case '4': cout << "\u2666"; input += u8"\u2666"; break; // ♦
                    default: cout << c; break;
                }
            }
            if(setKartu == string(input)){
                skor += 5;
                cout << endl << "Anda benar" << endl;
            }else{
                nyawa--;
                cout << endl << "Goblok lu" << endl;
            }
            statusFaseHati(input);
        }

};


int main(int argc, char const *argv[])
{
    game rythmOfHearts = game(3, 20, 0);
    while(rythmOfHearts.getNyawa() > 0){
    rythmOfHearts.kartuRandom();
    rythmOfHearts.inputKartu();
    }
}
