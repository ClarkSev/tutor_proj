/*
 @file: the main function
*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <fstream>

// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>

using namespace std;

class Qwirkle{
public:
    Qwirkle():state(true),player1_(""),player2_("") {}

    void show_menu() const{
        cout<<"Menu\n----\n1. New Game\n2. Load Game\n3. Credits (Show student information)\n4. Quit\n\n"<<endl;
    }

    void start_new_game(){
        cout<<"\nStarting a New Game"<<endl;
        while ( !valid_name(player1_)) {
            cout<<"\nEnter a name for player 1 (uppercase characters only)"<<endl;
            cout<<"> "; cin>>player1_;
        }

        while ( !valid_name(player2_)) {
            cout<<"\nEnter a name for player 2 (uppercase characters only)"<<endl;
            cout<<"> "; cin>>player2_;
        }

        cout<<"\nLet's Play!"<<endl;
        start_play();
    }

    void load_game(){
        string file_name;
        cout<<"Enter the filename from which load a game"<<endl;
        cout<<"> "; cin>>file_name;

        // justify whether the file exists
        bool is_exist = false;
        ifstream ifs;
        while( !is_exist ){
            ifs.open(file_name);
            if(!ifs.good()){
                cout<<"The file didn't exist!Please input right filename!";
                cout<<"> "; cin>>file_name;
            }else{
                is_exist = true;
            }
        }
        parser_file();
        start_play();
    }

    void credits() {

    }

    void exit() {
        state = false;
    }

    bool valid_name(string& s) {
        if (s.size() == 0) return false;
        for (auto unit:s) {
            if ( !isupper(unit))    return false;
        }
        return true;
    }

    void show_student_info(){

    }
    void parser_file(){

    }
    void parser_cmd(){

    }
    void start_play(){

    }
    void show_board(const int row, const int col, const vector<vector<string>>& tiles) const{
        cout<<"    ";
        for(int i=0; i<col; i++){
            cout<<i<<"  ";
        }
        cout<<endl;
        for(int i=0; i<row; i++){
            cout<<static_cast<char>('A'+i)<<" |";
            for(int j=0; j<col; j++){
                cout<<tiles[i][j]<<"|";
            }
            cout<<endl;
        }
    }

    void run() {
        int selection = -1;
        show_menu();
        cout<<">";
        cin>>selection;
        if (selection == 1) start_new_game();
        else if (selection == 2) load_game();
        else if (selection == 3) credits();
        else if (selection == 4) exit();
        else {
            cout<<"Invalid input"<<endl;
        }
    }

private:
    ///@param tile: eg. Y4  tab: eg. {B3, 10} ---> the first value means the available position,
    ///       the second value means the current player will get how many the scores are.
    ///@note  get the table of available position and score when the tile placed
    ///@return void
    void get_avail_position_score(const string& tile, unordered_map<string, int>& tab){
        for(int i=0; i<row_; i++){
            for(int j=0; j<column_; j++){
                int score = is_valid_position(i, j, tile);
                if( score!= -1) {
                    string lpos;  lpos.push_back('A' + row_);  lpos.push_back('0' + column_);
                    tab[lpos] = score;
                }
            }
        }
    }

    ///@note text
    bool is_empty_board(){
        for(int i=0; i<row_; i++){
            for(int j=0; j<column_; j++){
                if(board_[i][j] != "  ") return false;
            }
        }
        return true;
    }

    ///@note return -1 if there is an invaild position
    int is_valid_position(const int row, const int col, const string& tile){

        // assert(tile.size() == 2);
        // assert(row < row_);
        // assert(col < column_);

        if(board_[row][col] != "  ") return -1;
        int dxy[5][2] = {-1, 0, 0, 0, 1, 0, 0, -1, 0, 1};
        int ret = 0;
        bool flg = true;
        for(int i=0; i<5; i++){
            int x = row + dxy[i][0], y = col + dxy[i][1];
            if(x>=0 && x<row_ && y>=0 && y<column_){

                // assert(board_[x][y].size() == 2);

                if(board_[x][y][0]==tile[0] || board_[x][y][1]==tile[1]){
                    ret += get_score(row, col, dxy[i], tile);
                    flg &= true;
                }else if(board_[x][y]==" "){
                    flg &= true;
                }else{
                    flg = false;
                }
            }
        }
        if( !flg ) return -1;

        return ret;
    }

    ///@note text
    int get_score(const int x, const int y, const int* delta, const string& tile){
        int k=1, row = x + k * delta[0], col = y + k * delta[1];
        while(true){
            k++;
            row = x + k * delta[0], col = y + k * delta[1];

        }
    }

public:
    bool state;

private: 
    int row_, column_;
    int player1_score_, player2_score_;
    string cur_player_, player1_, player2_;
    list<string> player1_tile_, player2_tile_;
    list<string>* cur_tile_;

    vector<vector<string>> board_;
};

int main(){
    Qwirkle q;
    int selection;
    while (q.state) {
        q.run();
    }

    getchar();
    return 0;
}

