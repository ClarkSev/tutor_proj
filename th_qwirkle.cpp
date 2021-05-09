/*
 @file: the main function
*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <cassert>

#include <string.h>  // for strtok
// #include <stdio.h>
// #include <stdlib.h>


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
        parser_file(file_name);
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

    bool parser_file(const string& load_path){
        ifstream ifs(load_path, fstream::in);
        if( !ifs.good()){
            cout<<"Invalid filename"<<endl; return false;
        }
        unordered_set<char> color = {'R', 'O', 'Y', 'G', 'B', 'P'};

        string cur_str = "";
        vector<string> vec_str;
        int cur_int = -1;

        // get player1 name
        ifs>>cur_str;
        if(!valid_name(cur_str))  return false;
        player1_ = cur_str;
        
        // get player1 score
        ifs>>cur_int;
        if(cur_int == -1) return false;
        player1_score_ = cur_int;
        
        //get player1 hand
        ifs>>cur_str;
        split_string(cur_str, ",", vec_str);
        for(auto& vec_elem : vec_str){
            if(vec_elem.size() !=2 || !color.count(vec_elem[0]) || vec_elem[1]<'0' || vec_elem[1]>'6'){
                return false;
            }
            player1_tile_.push_back(vec_elem);
        }

        // get player2 name
        ifs>>cur_str;
        if( !valid_name(cur_str)) return false;
        player2_ = cur_str;

        // get player2 score
        cur_int = -1;
        ifs>>cur_int;
        if(cur_int == -1) return false;
        player2_score_ = cur_int;

        //get player2 hand
        ifs>>cur_str;
        vec_str.clear();
        split_string(cur_str, ",", vec_str);
        for(auto& vec_elem : vec_str){
            if(vec_elem.size() !=2 || !color.count(vec_elem[0]) || vec_elem[1]<'0' || vec_elem[1]>'6'){
                return false;
            }
            player2_tile_.push_back(vec_elem);
        }

        //get current board shape
        cur_int = -1;
        ifs>>cur_int;
        if(cur_int<=0 || cur_int>26) return false;
        row_ = cur_int;

        //fillter the common
        ifs>>cur_str;
        cur_str = "";

        cur_int = -1;
        ifs>>cur_int;
        if(cur_int<=0 || cur_int>26) return false;
        column_ = cur_int;

        // get the board state
        ifs>>cur_str;
        vec_str.clear();
        split_string(cur_str, ",", vec_str);
        for(auto& vec_elem : vec_str){
            if(vec_elem.size() != 5) return false;
            int row = vec_elem[0] - 'A', col = vec_elem[1] - '0';
            string ltile = vec_elem.substr(3);
            if(!position_in_board(row, col) || !color.count(ltile[0]) || ltile[1]<'0' || ltile[1]>'6') return false;
            board_[row][col] = ltile;
        }
        // get the tile bag
        ifs>>cur_str;
        vec_str.clear();
        split_string(cur_str, ",", vec_str);
        for(auto& vec_elem : vec_str){
            if(vec_elem.size() !=2 || !color.count(vec_elem[0]) || vec_elem[1]<'0' || vec_elem[1]>'6'){
                return false;
            }
            tile_bag_.push_back(vec_elem);
        }
        ifs.close();
        return true;
    }

    ///@note print list content into ostream
    void print_list(ostream& out, const list<string>& head){
        // auto& ptr = head;
        // while(ptr != nullptr){
        // }
        string list_str = "";
        for_each(head.begin(), head.end(), [&list_str](auto& s){ list_str += s+","; });
        out<<list_str<<endl;
    }

    ///@note return false if save failed.
    bool save_file(const string& save_path){
        ofstream ofs(save_path, fstream::out);
        if( !ofs.good() ){
            cout<<"Invalid filename"<<endl; return false;
        }
        // save player1
        ofs<<player1_<<endl<<player1_score_<<endl;
        print_list(ofs, player1_tile_);

        // save player2
        ofs<<player2_<<endl<<player2_score_<<endl;
        print_list(ofs, player2_tile_);

        // save current board shape
        ofs<<row_<<","<<column_<<endl;
        
        // save board state
        string lpos = "  ", board_state = "";
        for(int i=0; i<row_; i++){
            for(int j=0; j<column_; j++){
                if(board_[i][j] != "  "){
                    lpos[0] = static_cast<char>('A' + i);
                    lpos[1] = static_cast<char>('0' + j);
                    board_state += board_[i][j] + "@" + lpos + ",";
                }
            }
        }
        board_state.pop_back();
        ofs<<board_state<<endl;

        // save tile bag contents
        print_list(ofs, tile_bag_);

        // save current palyer
        ofs<<cur_player_<<endl;

        ofs.close();
        return true;
    }

    void parser_cmd(vector<string>& vec_cmd){
        string cmd;
        getline(cin, cmd);
        split_string(cmd, " ", vec_cmd);
    }

    void split_string(const string& cmd, const string& delim, vector<string>& vec_cmd){
        char* str = const_cast<char*>(cmd.data());
        char* token = strtok(str, delim.data());
        while(token != nullptr){
            vec_cmd.emplace_back(token);
            token = strtok(nullptr, delim.data());
        }
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
    void get_avail_position(const string& tile, vector<pair<int,int>>& avail_position){
        for(int i=0; i<row_; i++){
            for(int j=0; j<column_; j++){
                if(is_valid_position(i, j, tile)) {
                    avail_position.emplace_back(i,j);
                }
            }
        }
    }

    bool get_avail_position_score(const string& tile, const vector<pair<int,int>>& avail_position, unordered_map<string, int>& tab){
        bool is_qwirkle = false;
        for (auto &pos: avail_position)
        {
            int i = pos.first;
            int j = pos.second;
            int score = get_current_score(i, j, tile, is_qwirkle);
            if( score!= -1) {
                string lpos;  lpos.push_back(static_cast<char>('A' + i));  lpos.push_back(static_cast<char>('0' + j));
                tab[lpos] = score;
            }
        }
        return is_qwirkle;
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

        assert(tile.size() == 2);
        assert(row < row_);
        assert(col < column_);

        if(board_[row][col] != "  ") return -1;
        int dxy[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
        bool allBlank = true;
        for(int i=0; i<4; i++){
            int x = row + dxy[i][0], y = col + dxy[i][1];
            if(position_in_board(x,y)){
                // assert(board_[x][y].size() == 2);
                if(board_[x][y] != " "){
                    allBlank = false;
                }
            }
        }

        return !allBlank;
    }

    ///@note return -1 if there is an invaild position
    int get_current_score(const int row, const int col, const string& tile, bool& is_qwirkle){

        assert(tile.size() == 2);
        assert(row < row_);
        assert(col < column_);

        if(board_[row][col] != "  ") return -1;
        int dxy[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
        int ret = 0;
        bool flg = true;
        bool allBlank = true;
        for(int i=0; i<4; i++){
            int x = row + dxy[i][0], y = col + dxy[i][1];
            if(position_in_board(x,y)){

                // assert(board_[x][y].size() == 2);
                if(board_[x][y]==" "){
                    flg &= true;
                }
                else if(board_[x][y][0]==tile[0] || board_[x][y][1]==tile[1]){
                    allBlank = false;
                    int score = legal_direction(row, col, dxy[i], tile);
                    if(score == -1)  flg = false;
                    else {
                        ret += score;
                        if(score == 6) is_qwirkle = true;
                        flg &= true;
                    }
                }
                else{
                    flg = false;
                }
            }
        }
        if( !flg || allBlank) return -1;

        return ret;
    }

    ///@note text
    int legal_direction(const int x, const int y, const int* direction, const string& tile){
        int idx = tile[0] == board_[x][y][0] ? 0 : 1;
        int k=1, row = x + k * direction[0], col = y + k * direction[1];
        while(position_in_board(row, col) && board_[row][col] != "  "){
            if(board_[row][col][idx] == tile[idx]) k++;
            else return -1;
            row = x + k * direction[0], col = y + k * direction[1];
        }
        if(k >= 6) return -1;
        else return k+1;
    }

    ///@note text
    bool position_in_board(const int x, const int y){
        return x>=0 && x<row_ && y>=0 && y<column_;
    }

public:
    bool state;

private: 
    int row_, column_;
    int player1_score_, player2_score_;
    string cur_player_, player1_, player2_;
    list<string> player1_tile_, player2_tile_;
    list<string>* cur_tile_;

    list<string> tile_bag_;
    vector<vector<string>> board_;
};

int main(){
    Qwirkle q;
    // int selection;
    // while (q.state) {
    //     q.run();
    // }
    vector<string>vec;
    // q.parser_cmd(vec);
    // list<string> lst = {"124", "fjiag", "aege"};
    // for_each(lst.begin(), lst.end(), [&](auto& s){ cout<<s<<endl; });

    getchar();
    return 0;
}

