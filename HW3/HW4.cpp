
#include <ctime>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;
bool display = false;
char one = '1', zero = '0';
double MUTATION_RATE = 1;
int price[30] = {0}, weight[30] = {0};
int item_num = 0, capacity = 0, num = 0;
int POPULATION_SIZE = 6000, evaluation_max = 0, repeat_max = 2, CROSSOVER_RATE = 1;

int objective_value ( const string & input ) {
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

void mutation( string &input) {
    float rate = static_cast<float>(rand()) / RAND_MAX;
    if (rate < MUTATION_RATE) {
        int index = rand() % item_num;
        if (input[index] == one) input[index] = zero;
        else input[index] = one;
    }
}

bool repeat ( vector<string> & population , string check ) {
    int count = 0;
    for ( int i = 0 ; i < population.size() ; i ++ ) {
        if ( population[i] == check ) {
            count ++;
        }
    }
    if ( count >= repeat_max ) {
        return true;
    }
    else {
        return false;
    }
}

vector<string> init_pulation() {
    vector<string> population;
    string new_population( item_num , '0' );
    for ( int i = 0 ; i < POPULATION_SIZE ; i ++ ) {
        for ( int j = 0 ; j < item_num ; j ++ ) {
            if ( rand() % 2 == 1 ) {
                new_population[j] = one;
            }
        }
        population.push_back(new_population);
    }
    return population;
}

vector<string> selection ( vector<string> population ) {
    int best_index = 0;
    vector<string> selection_population;
    for ( int i = 0 ; i < POPULATION_SIZE / 2 ; i ++ ) {
        int max = 0;
        for ( int j = 0 ; j < population.size() ; j ++ ) {
            if ( objective_value(population[j]) >= max ) {
                max = objective_value(population[j]);
                best_index = j;
            }
        }
        selection_population.push_back(population[best_index]);
        population.erase(population.begin() + best_index);
    }
    return selection_population;
}

string crossover( const string & parent_a, const string & parent_b, string mask) {
    int point = 0;
    int piority = rand() % 2;
    string child(item_num , 0);
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

int main() {
    double START,END;
	START = clock();
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
    if ( dataname == "1" ) {
        POPULATION_SIZE = 10;
        evaluation_max = 150;
    }
    else if ( dataname == "2" ) {
        POPULATION_SIZE = 20;
        evaluation_max = 500;
    }
    else if ( dataname == "3" ) {
        POPULATION_SIZE = 300;
        evaluation_max = 400;
    }
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
        int max_index = 0;
        // 生成初始種群
        vector<string> population = init_pulation();
        for ( int i = 0 ; i < POPULATION_SIZE ; i ++ ) {
            while (repeat(population , population[i])){
                string temp_population( item_num , '0' );
                for ( int j = 0 ; j < item_num ; j ++ ) {
                    if ( rand() % 2 == 1 ) {
                        temp_population[j] = one;
                    }
                }
                population[i] = temp_population;
            }
        }

        // 進行每一代的演算法
        for (int i = 0; i < evaluation_max; i ++ ) {
            // 評估每個個體的適應度
            vector <int> objective_value_vector;
            for (int j = 0 ; j < item_num ; j ++) {
                int value = objective_value(population[j]);
                while (value <= 0) {
                    string temp_population = population[j];
                    int change = rand() % item_num;
                    if (temp_population[change] == one) {
                        temp_population[change] = zero;
                    } else {
                        temp_population[change] = one;
                    }
                    if (!repeat(population, temp_population)) {
                        population[j] = temp_population;
                        value = objective_value(population[j]);
                    }
                }
                objective_value_vector.push_back(value);
            }

            // 選擇前幾個個體作為交叉的父母
            vector<string> new_population;
            new_population = selection(population);
            for (int j = 0; j < POPULATION_SIZE / 2; j ++ ) {
                int p1 = rand() % POPULATION_SIZE/2;
                int p2 = rand() % POPULATION_SIZE/2;
                string parent_a = new_population[p1];
                string parent_b = new_population[p2];
                // 進行交叉操作
                string mask( item_num, 0 );
                string child( item_num, 0 );
                for ( int z = 0 ; z < item_num ; z ++ ) {
                    temp = rand() % 2;
                    if ( temp == 1 ) {
                        mask[z] = one;
                    }
                }
                float crossoverRate = static_cast<float>(rand()) / RAND_MAX;
                if ( crossoverRate < CROSSOVER_RATE ) {
                    child = crossover(parent_a, parent_b, mask);
                }
                mutation(child);
                while ( repeat( new_population, child ) ){
                    int p1 = rand() % POPULATION_SIZE/2;
                    int p2 = rand() % POPULATION_SIZE/2;
                    string parent_a = new_population[p1];
                    string parent_b = new_population[p2];
                    string mask( item_num, 0 );
                    for ( int z = 0 ; z < item_num ; z ++ ) {
                        temp = rand() % 2;
                        if ( temp == 1 ) {
                            mask[z] = one;
                        }
                    }
                    child = crossover(parent_a, parent_b, mask);
                    mutation(child);
                }
                new_population.push_back(child);
            }

            // 將新種群取代原有種群
            population = new_population;
            for ( int z = 0 ; z < item_num ; z ++ ) {
                if (objective_value_vector[z] > max_value) {
                    max_value = objective_value_vector[z];
                    max_index = z;
                }
            }
            if ( display ) cout << i + 1 << ", Max value: " << max_value << endl;
        }
        cout << "Generation " << k + 1 << ", Best Fitness: " << max_value << endl;
        output_data << "Generation " << k + 1 << endl;
        output_data << "max profit: " << max_value << endl;
        cout << "Population " << population[max_index] << endl;
        output_data << "solution: " << population[max_index] << endl<<endl;
        output_value += max_value;
    }
    cout << "Final Score " << output_value / 30 << endl;
    output_data << "Final Score: " << output_value / 30 << endl;
    END = clock();
	cout << endl << "程式執行所花費：" << (double)clock()/CLOCKS_PER_SEC << " S";
    return 0;
}
