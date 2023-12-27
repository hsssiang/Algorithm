#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int main() {
    ifstream input_data;
    ofstream output_data;
    int price[30] = {0}, weight[30] = {0}, all_possibile[30] = {0},most_value = 0, most_array[30] = {0};
    int item_num = 0, capacity = 0, num = 0;
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
        num ++;
    }

    for ( int i = 0 ; i < pow(2,num) - 1 ; i ++ ){
        all_possibile[num - 1] += 1;
        int count = num - 1, temp_weight = 0 , temp_value = 0;
        bool match = true;
        while (1){
            if ( all_possibile[count] == 2 ){
                all_possibile[count - 1] += 1;
                all_possibile[count] = 0;
            }
            if ( all_possibile[count - 1] < 2 ) break;
            else count --;
        }
        for ( int j = 0 ; j < num ; j ++ ){
            if ( all_possibile[j] ){
                if ( (temp_weight + weight[j]) <= capacity ){
                    temp_value += price[j];
                    temp_weight += weight[j];
                }
                else{
                    match = false;
                    break;
                }
            }
        }
        if ( match && temp_value > most_value ){
            for ( int j = 0 ; j < num ; j ++ ) most_array[j] = all_possibile[j];
            most_value = temp_value;
        }
    }
    output_data << "max profit:" << most_value << endl;
    output_data << "solution:";
    for ( int j = 0 ; j < num ; j ++ ) output_data << most_array[j];
    output_data << endl;
    return 0;
}