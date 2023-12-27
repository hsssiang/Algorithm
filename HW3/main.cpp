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
double MUTATION_RATE = 0.2;     // 突變率
float CROSSOVER_RATE = 1;          // 交叉率

// 初始化物品列表和背包容量
int price[30] = {0}, weight[30] = {0}, output_array[30]={0};
int item_num = 0, capacity = 0, num = 0, evaluation_max = 0;

// 生成隨機初始種群
vector< vector<bool> > init_pulation() {
    vector< vector<bool> > population( POPULATION_SIZE, vector<bool>( item_num ) );
    for ( int i = 0 ; i < POPULATION_SIZE ; i ++ ) {
        for ( int j = 0 ; j < item_num ; j ++ ) {
            population[i][j] = rand() % 2;
        }
    }
    return population;
}

bool repeat ( vector< vector<bool> >& population , vector<bool> check ) {
    int count = 0;
    for ( int i = 0 ; i < population.size() ; i ++ ) {
        int count_num = 0;
        for ( int j = 0; j < item_num ; j++ ){
            if ( population[i][j] == check[j] ) {
                count_num ++;
            }
        }
        if ( count_num == item_num ) {
            count ++;
        }
    }
    if ( count >= 2 ) {
        return true;
    }
    else {
        return false;
    }
}

// 計算個體的適應度
int objective_value ( const vector<bool>& input ) {
    int total_weight = 0, total_value = 0;
    for ( int i = 0 ; i < item_num ; i ++ ) {
        if ( input[i] == true ) {
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
vector<bool> selection ( const vector< vector<bool> >& population ) {
    vector<bool> best;
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
vector<bool> crossover( const vector<bool>& parent_a, const vector<bool>& parent_b, vector<bool> mask, bool piority ) {
    int point = 0;
    vector<bool> child(item_num);
    if (piority) {
        for ( int i = 0 ; i < item_num ; i ++ ) {
            if ( mask[i] ) {
                child[i] = parent_b[i];
            }
            else{
                child[i] = parent_a[i];
            }
        }
    }
    else if (!piority) {
        for ( int i = 0 ; i < item_num ; i ++ ) {
            if ( mask[i] ) {
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
void mutation(vector<bool>& input) {
    float rate = static_cast<float>(rand()) / RAND_MAX;
    if (rate < MUTATION_RATE) {
        int index = rand() % item_num;
        input[index] = !input[index];
    }
}

int main() {
    srand(time(NULL));
    int output_value =0;
    ifstream input_data;
    ofstream output_data;
    string dataname , output_dataname;
    string input_floder_path = "dataset/dt0";
    string output_file_path = "ans_dt0";
    string input_txt = "/item.txt";
    string output_txt = ".txt";
    string one = "1";
    string two = "2";
    string three = "3";

    cout << "Please input which data you want to use." << endl;
    cin >> dataname;
    cout << "Please input POPULATION_SIZE." << endl;
    cin >> POPULATION_SIZE;
    if ( dataname == one ) evaluation_max = 1000;
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
        vector< vector<bool> > population = init_pulation();
        for ( int i = 0 ; i < POPULATION_SIZE ; i ++ ) {
            if (repeat(population , population[i])){
                do {
                    for ( int j = 0 ; j < item_num ; j ++ ) {
                        population[i][j] = rand() % 2;
                    }
                } while (repeat(population , population[i]));
            }
        }

        // 進行每一代的演算法
        for (int i = 0; i < evaluation_max; i ++ ) {
            // 評估每個個體的適應度
            vector< int > objective_value_vector;
            for (int j = 0 ; j < item_num ; j ++) {
                vector<bool> new_population( item_num );
                int value = objective_value(population[j]);
                if (value == 0) {
                    while ( value > 0 )
                    {   
                        int change_num = rand() % 3 + 1;
                        for ( int s = 0 ; s < change_num ; s ++ ) {
                            int change = rand() % item_num;
                            new_population[change] = rand() % 2;
                        }
                        value = objective_value(new_population);
                    }
                    population[j] = new_population;
                }
                objective_value_vector.push_back(value);
                
            }

            // 選擇前幾個個體作為交叉的父母
            vector<vector<bool> > new_population;
            for (int j = 0; j < POPULATION_SIZE / 2; j ++   ) {
                
                vector<bool> parent_a = selection(population);
                vector<bool> parent_b = selection(population);

                // 進行交叉操作
                vector<bool> mask( item_num );
                vector<bool> child_a( item_num );
                vector<bool> child_b( item_num );
                for ( int z = 0 ; z < item_num ; z ++ ) {
                    mask[z] = rand() % 2;
                }
                float crossoverRate = static_cast<float>(rand()) / RAND_MAX;
                if ( crossoverRate < CROSSOVER_RATE ) {
                    child_a = crossover(parent_a, parent_b, mask, true);
                    child_b = crossover(parent_a, parent_b, mask, false);
                }
                mutation(child_a);
                mutation(child_b);
                if ( repeat( new_population, child_a ) || repeat( new_population, child_a ) ){
                    for ( int z = 0 ; z < item_num ; z ++ ) {
                        mask[z] = rand() % 2;
                    }
                    child_a = crossover(parent_a, parent_b, mask, true);
                    child_b = crossover(parent_a, parent_b, mask, false);
                }
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
