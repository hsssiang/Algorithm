#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;


// 定義常數
int POPULATION_SIZE = 6000;      // 種群大小
double MUTATION_RATE = 0.1;     // 突變率
float CROSSOVER_RATE = 1;          // 交叉率
string one = "1";
string zero = "0";

// 初始化物品列表和背包容量
int price[30] = {0}, weight[30] = {0}, output_array[30]={0};
int item_num = 0, capacity = 0, num = 0, evaluation_max = 0;

// 生成隨機初始種群
vector< vector<string> > init_pulation() {
    vector< vector<string> > population( POPULATION_SIZE, vector<string>( item_num , '0' ) );
    for ( int i = 0 ; i < POPULATION_SIZE ; i ++ ) {
        for ( int j = 0 ; j < item_num ; j ++ ) {
            if ( rand() % 2 == 1 ) {
                population[i][j] = one;
            }
        }
    }
    return population;
}

bool repeat ( vector< vector<string> >& population , vector<string> check ) {
    int count = 0;
    for ( int i = 0 ; i < population.size() ; i ++ ) {
        if ( population[i] == check ) {
            count ++;
        }
    }
    if ( count >= 4 ) {
        return true;
    }
    else {
        return false;
    }
}

// 計算個體的適應度
int objective_value ( const vector<string>& input ) {
    int total_weight = 0, total_value = 0;
    for ( int i = 0 ; i < item_num ; i ++ ) {
        if ( input[i] == one ) {
            total_weight += weight[i];
            total_value += price[i];
        }
    }
    if ( total_weight > capacity) {
        return 0;
    }
    return total_value;
}

// 使用錦標賽選擇法選擇父母進行交叉
vector<string> selection ( const vector< vector<string> >& population ) {
    vector<string> best;
    int index_a = rand() % POPULATION_SIZE;
    int index_b = rand() % POPULATION_SIZE;
    int fitness_a = objective_value(population[index_a]);
    int fitness_b = objective_value(population[index_b]);
    if (fitness_a > fitness_b) {
        best = population[index_a];
    }
    else {
        best = population[index_b];
    }
    return best;
}

// 進行交叉操作
vector<string> crossover( const vector<string>& parent_a, const vector<string>& parent_b, vector<string> mask, bool piority ) {
    int point = 0;
    vector<string> child(item_num);
    if (piority) {
        for ( int i = 0 ; i < item_num ; i ++ ) {
            if ( mask[i] == one ) {
                child[i] = parent_b[i];
            }
            else{
                child[i] = parent_a[i];
            }
        }
    }
    else if (!piority) {
        for ( int i = 0 ; i < item_num ; i ++ ) {
            if ( mask[i] == one ) {
                child[i] = parent_a[i];
            }
            else{
                child[i] = parent_b[i];
            }
        }
    }
    return child;
}

// 突變個體
void mutation(vector<string>& input) {
    float rate = static_cast<float>(rand()) / RAND_MAX;
    if (rate < MUTATION_RATE) {
        int index = rand() % item_num;
        if (input[index] == one) input[index] = zero;
        else input[index] = one;
    }
}

int main() {
    srand(time(NULL));
    int output_value =0, temp;
    ifstream input_data;
    ofstream output_data;
    string dataname , output_dataname;
    string input_floder_path = "dataset/dt0";
    string output_file_path = "ans_dt0";
    string input_txt = "/item.txt";
    string output_txt = ".txt";
    string two = "2";
    string three = "3";

    cout << "Please input which data you want to use." << endl;
    cin >> dataname;
    cout << "Please input POPULATION_SIZE." << endl;
    cin >> POPULATION_SIZE;
    if ( dataname == one ) evaluation_max = 500;
    else if ( dataname == two ) evaluation_max = 1000;
    else if ( dataname == three ) evaluation_max = 1000;
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
    for ( int k = 0 ; k < 30 ; k ++ ) {
        int max_value = 0;
        // 生成初始種群
        vector< vector<string> > population = init_pulation();
        for ( int i = 0 ; i < POPULATION_SIZE ; i ++ ) {
            if (repeat(population , population[i])){
                do {
                    for ( int j = 0 ; j < item_num ; j ++ ) {
                        temp = rand() % 2;
                        if ( temp == 1 ) {
                            population[i][j] = one;
                        }
                        else {
                            population[i][j] = zero;
                        }
                    }
                } while (repeat(population , population[i]));
            }
        }

        // 進行每一代的演算法
        for (int i = 0; i < evaluation_max; i ++ ) {
            // 評估每個個體的適應度
            vector< int > objective_value_vector;
            for (int j = 0 ; j < item_num ; j ++) {
                vector<string> new_population( item_num );
                int value = objective_value(population[j]);
                if (value == 0) {
                    while ( value > 0 )
                    {   
                        int change_num = rand() % 3 + 1;
                        for ( int s = 0 ; s < change_num ; s ++ ) {
                            int change = rand() % item_num;
                            temp = rand() % 2;
                            if ( temp == 1 ) {
                                new_population[change] = one;
                            }
                            else {
                                new_population[change] = zero;
                            }
                        }
                        value = objective_value(new_population);
                    }
                    population[j] = new_population;
                }
                objective_value_vector.push_back(value);
                
            }

            // 選擇前幾個個體作為交叉的父母
            vector<vector<string> > new_population;
            vector<vector<string> > selection_population;
            for (int j = 0; j < POPULATION_SIZE ; j ++ ){
                vector<string> parent = selection(population);
                while ( repeat( selection_population, parent ) ){
                    parent = selection(population);
                }
                selection_population.push_back(parent);
            }
            population = selection_population;
            for (int j = 0; j < POPULATION_SIZE / 2; j ++ ) {
                int p1 = rand() % POPULATION_SIZE;
                int p2 = rand() % POPULATION_SIZE;
                vector<string> parent_a = population[p1];
                vector<string> parent_b = population[p2];
                // 進行交叉操作
                vector<string> mask( item_num );
                vector<string> child_a( item_num );
                vector<string> child_b( item_num );
                for ( int z = 0 ; z < item_num ; z ++ ) {
                    temp = rand() % 2;
                    if ( temp == 1 ) {
                        mask[z] = one;
                    }
                    else {
                        mask[z] = zero;
                    }
                }
                float crossoverRate = static_cast<float>(rand()) / RAND_MAX;
                if ( crossoverRate < CROSSOVER_RATE ) {
                    child_a = crossover(parent_a, parent_b, mask, true);
                    child_b = crossover(parent_a, parent_b, mask, false);
                }
                mutation(child_a);
                mutation(child_b);
                new_population.push_back(child_a);
                new_population.push_back(child_b);
            }

            // 將新種群取代原有種群
            population = new_population;
            for ( int z = 0 ; z < item_num ; z ++ ) {
                if (objective_value_vector[z] > max_value) {
                    max_value = objective_value_vector[z];
                }
            }
            cout << i + 1 << ", Max value: " << max_value << endl;
        }
        cout << "Generation " << k + 1 << ", Best Fitness: " << max_value << endl;
        output_value += max_value;
    }
    cout << "Final Score " << output_value / 30 << endl;
    return 0;
}
