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
#include <random>   // for random engine
#include <chrono>   // for system time
#include <assert.h>
#include <string.h>

// #include <stdio.h>
// #include <stdlib.h>


using namespace std;

class Qwirkle{
public:
    Qwirkle():state(true),player1_(""),player2_("") {
        // random_bag(bag);
        // random_bag(bag);
        random_bag2(bag);
    }

    void random_bag(list<string>& bag) {
        char color[6] = {'R','O','Y','G','B','P'};
        char size[6] = {'1','2','3','4','5','6'};
        vector<bool> visit_color(6,false);
        bool allvisit_color = false;
        bool allvisit_size = false;
        int count_i = 0;
        string ltmp("  ");
        while (count_i < 6) {
            int i = rand()%6;
            if (visit_color[i] == false) {
                visit_color[i] = true;
                count_i++;
                int count_j = 0;
                vector<bool> visit_size(6,false);
                while (count_j < 6) {
                    int j = rand()%6;
                    if (visit_size[j] == false) {
                        visit_size[j] = true;
                        ltmp[0] = color[i];  ltmp[1] = size[j];
                        bag.push_back(ltmp);
                        count_j++;
                    }
                }
            }
        }
    }

    void random_bag2(list<string>& tbag){
        char color[6] = {'R','O','Y','G','B','P'},
             size[6] = {'1','2','3','4','5','6'};
        vector<string> vec_tmp;
        string ltmp("  ");
        for(int i=0; i<6; i++){
            for(int j=0; j<6; j++){
                ltmp[0] = color[i];  ltmp[1] = size[j];
                vec_tmp.emplace_back(ltmp);
                vec_tmp.emplace_back(ltmp);
            }
        }
        // random engine
        default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> distribution(0, 71);
        while(tbag.size()<72){
            int idx = distribution(generator);
            if(vec_tmp[idx].size()){
                tbag.push_back(vec_tmp[idx]);
                vec_tmp[idx] = "";
            }
        }
        // for_each(tbag.begin(), tbag.end(), [](auto& e){ cout<<e<<endl; });
    }

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
        print_list(ofs, bag);

        // save current palyer
        ofs<<cur_player_<<endl;

        ofs.close();
        return true;
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
    void parser_cmd(string& cmd, vector<string>& vec_cmd){
        char* target = strdup(cmd.c_str());
        char* token = strtok(target," ");
        while (token != nullptr) {
            vec_cmd.push_back(string(token));
            token = strtok(nullptr," ");
        }
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
        bool is_end = false;
        while ( !is_end) {
            show_play();
            string temp_cmd = "";
            vector<string> vec_cmd;
            getline(cin,temp_cmd);
            parser_cmd(temp_cmd,vec_cmd);
            while (isvalid_cmd(vec_cmd)) {
                cout<<"Please input valid cmd"<<endl;
                cout<<">";
                getline(cin,temp_cmd);
                parser_cmd(temp_cmd,vec_cmd);
            }

            if (vec_cmd[0] == "place") {
                bool is_qwirkle = false;
                int x = vec_cmd[3][0] - 'A';
                int y = vec_cmd[3][1] - '0';
                int score = get_current_score(x, y, vec_cmd[1], is_qwirkle);
                if (score == -1)    cout<<"This action is illeagal"<<endl;
                else {
                    if (cur_player_ == player1_) player1_score_ += score;
                    else player2_score_ += score;
                    board_[x][y] = vec_cmd[1];
                }
            }
            else if (vec_cmd[0] == "replace") {
                if (cur_player_ == player1_) {
                    auto iter = find(player1_tile_.begin(),player1_tile_.end(),vec_cmd[1]);
                    player1_tile_.erase(iter);
                    player1_tile_.push_back(bag.front());
                    bag.pop_front();
                }
            }
            else if (vec_cmd[0] == "save") {
                save_file(vec_cmd[1]);
            }

            if (cur_player_ == player1_) cur_player_ = player2_;
            else cur_player_ = player1_;
        }
    }

    void show_board() const{
        int row = board_.size();
        if (row == 0)   return;
        int col = board_[0].size();
        cout<<"    ";
        for(int i=0; i<col; i++){
            cout<<i<<"  ";
        }
        cout<<endl;
        cout<<"   ------------------"<<endl;
        for(int i=0; i<row; i++){
            cout<<static_cast<char>('A'+i)<<" |";
            for(int j=0; j<col; j++){
                cout<<board_[i][j]<<"|";
            }
            cout<<endl;
        }
    }

    void show_play() const {
        cout<<cur_player_<<", it's your turn"<<endl;
        cout<<"Score for "<<player1_<<": "<<player1_score_<<endl;
        cout<<"Score for "<<player2_<<": "<<player2_score_<<endl;
        show_board();
        cout<<endl;
        cout<<"Your hand is"<<endl;
        show_tiles();
        cout<<endl;
        cout<<">";
    }

    void show_tiles() const{
        string temp = "";
        for (auto tile:player1_tile_) {
            temp += tile + ",";
        }
        int n = temp.size();
        if (n == 0) return;
        cout<<temp.substr(0,n-1);
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

    bool isvalid_cmd(vector<string>& cmd) {
        int n = cmd.size();
        if (n == 0)    return false;

        bool isvalid = false;
        if (cmd[0] == "replace" && n >= 2) {
            if (isvalid_tile(cmd[1]))   isvalid = true;
        }
        else if (cmd[0] == "place" && n >= 4) {
            if (isvalid_tile(cmd[1]) && cmd[2] == "at" && isvalid_pos(cmd[3]))  isvalid = true;
        }
        else if (cmd[0] == "save" && n >= 2) {
            if (isvalid_file(cmd[1]))   isvalid = true;
        }

        return false;
    }

    bool isvalid_tile(string& s) {
        int n = s.size();
        if (n != 2) return false;

        if (isupper(s[0]) && isdigit(s[1])) {
            if (cur_player_ == player1_) {
                for (auto now:player1_tile_) {
                    if (now == s)   return true;
                }
            }
            else if (cur_player_ == player2_) {
                for (auto now:player2_tile_) {
                    if (now == s)   return true;
                }
            }
        }
        
        return false;
    }

    bool isvalid_pos(string& s) {
        int n = s.size();
        if (n != 2) return false;

        if ((s[0] >= 'A' && s[0] <= 'F') && (s[1] >= '0' && s[1] <= '5')) {
            int x = s[0] - 'A';
            int y = s[1] - '0';
            if (board_[x][y] == "  ")   return true;
        }

        return false;
    }

    bool isvalid_file(string& s) {
        int n = s.size();
        if (n <= 4) return false;

        if (s.substr(n-4) == ".txt")    return true;
        
        return false;
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
                if(board_[x][y] != "  "){
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

    // int get_score(const int x, const int y, const string& tile){
    //     int idx = tile[0] == board_[x][y][0] ? 0 : 1;
    //     int k=1, row = x + k * delta[0], col = y + k * delta[1];
    //     while(position_in_board(row, col) && board_[row][col] != "  "){
    //         if(board_[row][col][idx] == tile[idx]) k++;
    //         else return -1;
    //         if (board_[row][col] == tile) return -1;
    //         row = x + k * delta[0], col = y + k * delta[1];
    //     }
    //     if(k > 6) return -1;
    //     else return k;
    // }

    ///@note text
    int legal_direction(const int x, const int y, const int* direction, const string& tile){
        int idx = tile[0] == board_[x][y][0] ? 0 : 1;
        int k=1, row = x + k * direction[0], col = y + k * direction[1];
        while(position_in_board(row, col) && board_[row][col] != "  "){
            if(board_[row][col][idx] == tile[idx]) k++;
            else return -1;
            row = x + k * direction[0], col = y + k * direction[1];
        }
        if(k > 6) return -1;
        else return k;
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
    list<string> bag;
    list<string>* cur_tile_;

    vector<vector<string>> board_;
};

int main(){
    Qwirkle q;
    // while (q.state) {
    //     q.run();
    // }
    cout<<"success"<<endl;
    getchar();getchar();
    return 0;
}

