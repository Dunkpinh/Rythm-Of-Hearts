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
        int jumlahKartu = 3;

    void delayPrint(const string& text, int delayMs = 100) {
        for(char c : text) {
            cout << c;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
        cout << endl;
    }

    public:
        game(int nyawa, int waktu){
            this->nyawa = nyawa;
            this->waktu = waktu;
        }
        void ubahJumlahKartu(){
            jumlahKartu = 4;
        }
        void randomCard(){
            srand(time(0));
            string setKartu = "";
            SetConsoleOutputCP(CP_UTF8);
            for (size_t i = 0; i < jumlahKartu; i++){
                int idxKartu = rand() % 4 + 1;
                string kartu;
                switch(idxKartu){
                    case 1: kartu   = u8"\u2665"; break; // ♥
                    case 2: kartu   = u8"\u2660"; break; // ♠
                    case 3: kartu    = u8"\u2663"; break; // ♣
                    case 4: kartu  = u8"\u2666"; break; // ♦
                }
                setKartu += kartu;
            }
            delayPrint(setKartu, 100);
        }

};


int main(int argc, char const *argv[])
{
    game rythmOfHearts = game(3, 20);
    rythmOfHearts.randomCard();
     cout << "\u2665";
      cout << "\u2665";
       cout << "\u2665";
}
