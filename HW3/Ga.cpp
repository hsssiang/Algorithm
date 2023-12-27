#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

int evaluateAlgorithm(int POPULATION_SIZE, float CROSSOVER_RATE, float MUTATION_RATE) {

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
                if ( mask[i] == false ) {
                    child[i] = parent_a[i];
                }
                else{
                    child[i] = !parent_a[i];
                }
            }
        }
        else if (!piority) {
            for ( int i = 0 ; i < item_num ; i ++ ) {
                if ( mask[i] == true ) {
                    child[i] = parent_a[i];
                }
                else{
                    child[i] = !parent_a[i];
                }
            }
        }
        return child;
    }

    // 突變個體
    void mutation(vector<bool>& input , int item_num1) {
        float rate = static_cast<float>(rand()) / RAND_MAX;
        if (rate < MUTATION_RATE) {
            int index = rand() % item_num;
            input[index] = !input[index];
        }
    }
    int GA() {
        srand(time(NULL));
        int output_value =0;
        ifstream input_data;
        ofstream output_data;
        string dataname , output_dataname;
        string input_floder_path = "C:\\Algorithm\\Algorithm\\HW3\\dataset\\dt0";
        string output_file_path = "ans_dt0";
        string input_txt = "\\item.txt";
        string output_txt = ".txt";
        string one = "1";
        string two = "2";
        string three = "3";

        cout << "Please input which data you want to use." << endl;
        cin >> dataname;
        if ( dataname == one ) evaluation_max = 1000;
        else if ( dataname == two ) evaluation_max = 10000;
        else if ( dataname == three ) evaluation_max = 500000;
        output_dataname = dataname + output_txt;
        dataname += input_txt;
        input_floder_path += dataname;
        cout << input_floder_path << endl;
        output_file_path += output_dataname;
        input_data.open ( input_floder_path );
        output_data.open( output_file_path );

        input_data >> item_num >> capacity;
        while (input_data >> weight[num] >> price[num]) {
            num++;
        }
        item_num = ++ num;
        cout << item_num << endl;
        for ( int k = 0 ; k < 30 ; k ++ ) {
            int max_value = 0;
            // 生成初始種群
            vector< vector<bool> > population = init_pulation();

            // 進行每一代的演算法
            for (int i = 0; i < (evaluation_max - POPULATION_SIZE); i ++ ) {
                // 評估每個個體的適應度
                vector< int > objective_value_vector;
                for (int j = 0 ; j < item_num ; j ++) {
                    vector<bool> new_population( item_num );
                    int value = objective_value(population[j]);
                    if (value == 0) {
                        while ( value > 0 )
                        {
                            for ( int x = 0 ; x < item_num ; x ++ ) {
                                new_population[x] = rand() % 2;
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
                    mutation(child_a , num);
                    mutation(child_b , num);
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
                //cout << i + 1 << ", Max value: " << max_value << endl;
            }
            cout << "Generation " << k + 1 << ", Best Fitness: " << max_value << endl;
            output_value += max_value;
        }
        cout << "Final Score " << output_value / 30 << endl;
    return output_value / 30;
}

void gridSearch() {
    // 定義參數的範圍和步長
    vector<int> populationSizes = {50, 100, 200};
    vector<float> crossoverRates = {0.6, 0.7, 0.8};
    vector<float> mutationRates = {0.01, 0.02, 0.03};

    // 初始化最佳參數和最佳適應度
    int bestPopulationSize = 0, bestCrossoverRate = 0, bestMutationRate = 0;
    int bestFitness = 0;

    // 遍歷所有參數組合
    for (int populationSize : populationSizes) {
        for (float crossoverRate : crossoverRates) {
            for (float mutationRate : mutationRates) {
                // 評估當前參數組合的適應度
                int currentFitness = evaluateAlgorithm(populationSize, crossoverRate, mutationRate);

                // 更新最佳參數和最佳適應度
                if (currentFitness > bestFitness) {
                    bestFitness = currentFitness;
                    bestPopulationSize = populationSize;
                    bestCrossoverRate = crossoverRate;
                    bestMutationRate = mutationRate;
                }
            }
        }
    }

    // 輸出最佳參數和最佳適應度
    cout << "Best Population Size: " << bestPopulationSize << endl;
    cout << "Best Crossover Rate: " << bestCrossoverRate << endl;
    cout << "Best Mutation Rate: " << bestMutationRate << endl;
    cout << "Best Fitness: " << bestFitness << endl;
}

int main() {
    // 開始執行簡單的網格搜索
    gridSearch();

    return 0;
}

