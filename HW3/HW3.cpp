#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    ifstream input_data;
    ofstream output_data;
    size_t numRows = 6404200;
    size_t numCols = 30;
    int price[30] = {0}, weight[30] = {0}, output_array[30]={0};
    int item_num = 0, capacity = 0, num = 1;
    vector< vector<int> > dp_array;
    string dataname , output_dataname;
    string input_floder_path = "dataset/dt0";
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
    dp_array.push_back(vector<int>(numRows, 0));
    dp_array.push_back(vector<int>(numRows, 0));
    // 在向量中插入新的行，并初始化第一列
    for ( int i = 1 ; i <= ( item_num + 1 ) ; i ++ ){
        for ( int j = 1 ; j <= ( capacity + 1) ; j ++ ){
            if ( (j >= weight[i]) ){
                dp_array[i][j] = max( dp_array[i-1][j] , dp_array[i-1][j-weight[i]] + price[i] );
            }
            else{
                dp_array[i][j] = dp_array[i-1][j];
            }
        }
        dp_array.push_back(vector<int>(numRows, 0));
    }
    int j = capacity + 1;
    int i = item_num + 1;
    output_data << "max profit:" << dp_array[i-1][j] << endl;
    output_data << "solution:";
    while ( i != 0 ){
        if ( dp_array[i-1][j] != dp_array[i][j] ){
            output_array[i] = 1;
            j -= weight[i];
        }
        else{
            output_array[i] = 0;
        }
        i --;
    } 
    for (int i = 1; i < item_num + 1 ; i++ ){
        output_data << output_array[i];
    }
    output_data << endl;
    return 0;
}