#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int main() {
    ifstream input_data;
    ofstream output_data;
    double cp_list [30] = {0};
    double price[30] = {0}, weight[30] = {0}, most_array[30]={0};
    int most_value = 0, most_cp = 0, total_weight = 0, item_num = 0, capacity = 0, num = 0;
    string dataname , output_dataname;
    string input_floder_path = "HW1_data/dt0";
    string output_file_path = "ans_dt0";
    string input_txt = "/item.txt";
    string output_txt = ".txt";

    cout << "Please input which data you want to use." << endl;
    cin >> dataname;

    output_dataname = dataname + output_txt;
    dataname += input_txt;
    input_floder_path += dataname;
    output_file_path += output_dataname;
    input_data.open ( input_floder_path );
    output_data.open( output_file_path );

    input_data >> item_num >> capacity;
    while( !input_data.eof() ){ 
        input_data >> weight[num] >> price[num];
        cp_list[num] = price[num] / weight[num] ;
        num ++;
    }
    while (1){
        for ( int j = 0 ; j < num ; j ++ ){
            if ( cp_list[j] > cp_list[most_cp] ){
                most_cp = j;
            }
        }
        if ( total_weight + weight[most_cp] <= capacity ){
            total_weight += weight[most_cp];
            cp_list[most_cp] = 0;
            most_array[most_cp] = 1;
            most_value += price[most_cp];
        }
        else break;
    }
    output_data << "max profit:" << most_value << endl;
    output_data << "solution:";
    for ( int j = 0 ; j < num ; j ++ ){
        output_data << most_array[j];
    }
    output_data << endl;
    return 0;
}