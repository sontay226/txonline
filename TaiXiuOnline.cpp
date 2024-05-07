#include<iostream> // Thư viện nhập xuất 
#include<fstream> // Thư viện đọc file
#include<random> // Thư viện random
#include<windows.h> // Thư viện xuất Tiếng Việt
#include<chrono>
#include<algorithm>
#include<cctype>
#include<cstdlib>
using namespace std;
    unsigned random = chrono::system_clock::now().time_since_epoch().count(); 
    mt19937 gen(random);
// Tài từ 11 điểm >= 18 điểm
// Xỉu từ 3 điểm >= 10 điểm
// Tỉ lệ ăn 1 : 1
// Logic game ( không bịp ) : dựa trên random
class ThuatToanXacSuat
{
private:
    string tai = "tai";
    string xiu = "xiu";
    int diemso;
public:
    ThuatToanXacSuat( string tai , string xiu , int diemso ) : tai(tai) , xiu(xiu) , diemso(diemso) {}
    ThuatToanXacSuat () {
        uniform_int_distribution<int> finalpoint(3,18);
        diemso = finalpoint(gen);
    }
friend class Display;
friend class RealPlayers;
friend class Stakes;
};
//Bot đặt tiền 
class FakePlayers
{
private:
    int tienTai;
    int tienXiu;
public:
    FakePlayers ( int tienTai , int tienXiu ) : tienTai(tienTai) , tienXiu(tienXiu) {}
    FakePlayers () {
        uniform_int_distribution<int> ttai(10000 , 100000000);
        uniform_int_distribution<int> txiu(10000 , 100000000);
        tienTai = ttai(gen);
        tienXiu = txiu(gen);
    }
friend class TienCanBang ;
friend class Display;
};
// Tính tiền cân bảng 
class TienCanBang 
{
private:
    int tiencanbang;
public:
    TienCanBang ( int tiencanbang ) : tiencanbang(tiencanbang) {}
    int TongTienDatCuoc ( FakePlayers tongtiendatcuoc ) {
        return tongtiendatcuoc.tienTai + tongtiendatcuoc.tienXiu;
    }
    TienCanBang ( FakePlayers tongtien ) {
       tiencanbang = TongTienDatCuoc(tongtien) / 2;
    }
friend class Display;
};
// Người chơi tham gia phiên cầu : Thắng hay thua , còn phần ăn tiền cược sẽ để ở class khác 
class RealPlayers 
{
private:
    string playerchoose;
    ThuatToanXacSuat ttxs;
public: 
    RealPlayers() {}
    RealPlayers(ThuatToanXacSuat ttxs) : ttxs(ttxs){} 
    RealPlayers ( string playerchoose ) : playerchoose(playerchoose) {}
    void InputChoose () {
        SetConsoleOutputCP(65001);
        cout << "Mời người chơi đặt cược : ";
        cin >> playerchoose;
        playerchoose.erase(remove_if(playerchoose.begin(), playerchoose.end(), ::isspace), playerchoose.end());
        transform(playerchoose.begin(), playerchoose.end(), playerchoose.begin(), ::tolower);
}
    bool WinOrLose(ThuatToanXacSuat ttxs) {
       if (playerchoose == "tai" && ttxs.diemso >= 11 && ttxs.diemso <= 18) {
        return true;
    } else if (playerchoose == "xiu" && ttxs.diemso >= 3 && ttxs.diemso <= 10) {
        return true;
    } else {
        return false;
    }
    }
friend class ThuatToanXacSuat;
friend class Display;
friend class Stakes;
};
//Đặt tiền và số tiền ăn 
class Stakes {
private:
    int startMoney;
    int loseStakes;
    int winStakes;
    int deposit;
    ThuatToanXacSuat ttxs;
public:
    Stakes() {}
    Stakes ( int loseStakes , int winStakes , int &startMoney , int deposit , ThuatToanXacSuat ttxs );
    friend class Display;
};

Stakes::Stakes ( int loseStakes , int winStakes , int &startMoney , int deposit , ThuatToanXacSuat ttxs ) : loseStakes(loseStakes) , winStakes(winStakes) , startMoney(startMoney) , deposit(deposit) , ttxs(ttxs) {
    // check file
    SetConsoleOutputCP(65001);
    ifstream fileInI4Player("I4Player.txt");
    if (fileInI4Player.is_open()) {
        fileInI4Player >> startMoney;
        fileInI4Player.close();
    } else {
        cout << "Không thể mở file I4Player.txt" << endl;
        exit(1);
    }
    // Hiển thị số tiền còn lại trước khi cược
    cout << "Số tiền mà bạn có : " << startMoney << endl;

    // Nhập số tiền cược mới
    cout << "Nhập vào số tiền đặt cược: ";
    cin >> deposit;
    if ( deposit > startMoney ) {
        cout << "Không đủ số dư , vui lòng nạp thêm tiền vào tài khoản , liên hệ : Phạm Sơn Tây ! " << endl;
        exit(1);
    }
    RealPlayers rp;
    Stakes stake;
    // Xử lý cược và cập nhật startMoney
    if ( rp.WinOrLose(ttxs) ) {
        startMoney += deposit * 2;
    } else {
        startMoney -= deposit;
    }

    // Ghi lại giá trị startMoney mới vào tập tin "I4Player.txt"
    ofstream fileOutI4Player("I4Player.txt");
    if (fileOutI4Player.is_open()) {
        fileOutI4Player << startMoney;
        fileOutI4Player.close();
    } else {
        cout << "Không thể mở file I4Player.txt" << endl;
        return;
    }
    // Hiển thị số tiền còn lại sau khi cược
}
// In ra dữ liệu 
class Display {

public:
    Display();
    Display ( ThuatToanXacSuat ttxs , RealPlayers rp , FakePlayers fp , TienCanBang tcb , int &startMoney );
};
Display::Display ( ThuatToanXacSuat ttxs ,RealPlayers rp , FakePlayers fp , TienCanBang tcb , int &startMoney) {
        // Tài hay Xỉu ?
        SetConsoleOutputCP(65001);
        if ( ttxs.diemso >= 3 && ttxs.diemso <= 10 ) {
            cout << "Phiên này cầu về Xỉu : " << ttxs.diemso << endl;
        }else {
            cout << "Phiên này cầu về Tài : " << ttxs.diemso << endl;
        }
        cout << "Bot đặt tiền cửa Tài số tiền : " << fp.tienTai << endl;
        cout << "Bot đặt tiền cửa Xỉu số tiền : " << fp.tienXiu << endl;    
        cout << "Trả Tiền Cân Cửa : " << tcb.tiencanbang << endl;
        // Trả về kết quả 
        if ( rp.WinOrLose(ttxs)) {
            cout << "Chúc mừng người chơi đã thắng cược ! Húp đấy hehe " << endl;
        }else {
            cout << "Đen thôi , đỏ là red =)) " << endl;
        }
        cout << "Số tiền còn lại sau khi cược : " << startMoney << endl;
    
}
int main () {  
    ThuatToanXacSuat ttxs;
    FakePlayers fp;
    TienCanBang tcb(fp);
    RealPlayers rp(ttxs);
    rp.InputChoose();
    int loseStakes; 
    int winStakes; 
    int startMoney; 
    int deposit;
    Stakes stakes(loseStakes , winStakes , startMoney , deposit , ttxs);
    Display (ttxs , rp , fp , tcb , startMoney );
}