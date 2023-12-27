#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */

using namespace std;

vector<int> value;
vector<int> weight;

void show_vector(vector<int> vec, string title="")
{
    cout << title << ": ";
    for(auto it: vec){
        cout << it << ' ';
    }
    cout << endl;
}
void Population(vector<string>&, vector<int> &, vector<int>&, int);
void Evaluation(vector<string>&, vector<int> &, vector<int> &, int);
void Selection(vector<string>&, vector<int> &, vector<int> &);
void Crossover(vector<string>&, vector<int> &, vector<int> &, float);
void Mutation(vector<string>&, vector<int> &, vector<int> &, float);

int main(){ 
    cout << "演算法作業四: Use Genetic Algorithm To Solve 0/1 Knapsack Problem" << endl << endl;
    // 選擇測試資料
    string folder_name;
    while(1){
        cout << "請選擇資料夾名稱：";
        cin >> folder_name;
        if(folder_name == "dt01"  ||folder_name == "dt02" || folder_name == "dt03") break;
        cout << "查無此資料夾!" << endl;
    }

    // open file
    ifstream file_in("./" + folder_name + "/item.txt");
    cout << "已開啟檔案夾：" + folder_name << endl << endl;

    int data_amount;
    int max_weight;
    int n1, n2;

    // reading data
    file_in >> data_amount >> max_weight;
    for(int i=0; i<data_amount; i++){
        file_in >> n1 >> n2;
        value.push_back(n2);
        weight.push_back(n1);
    }
    // show_vector(value, "value");
    // show_vector(weight, "weight");

    // variables
    int population_size = 1000;
    float cross_rate = 1.0;
    float mutation_rate = 0.0;

    // start
    int eva_max;
    if(folder_name == "dt01") eva_max = 1000;
    else if(folder_name == "dt02") eva_max = 10000;
    else if(folder_name == "dt03") eva_max = 500000;
    vector<int> profit_vec;
    vector<string> solution_vec;

    for(int k=1;k<=30;k++){
        srand( time(NULL) );
        cout << "Round: " << k << endl;
        vector<string> population; // 存所有基因組合
        vector<int> ppl_val;  // 存每個組合的value
        vector<int> ppl_weight; // 存每個組合的weight
        Population(population, ppl_val, ppl_weight, population_size); // 先產生population_size個組合
        Evaluation(population, ppl_val, ppl_weight, max_weight);    
        for(int eva=0; eva<eva_max; eva++){
            Selection(population, ppl_val, ppl_weight);               // 選擇優良基因
            Crossover(population, ppl_val, ppl_weight, cross_rate);   // 交配產生新子代
            Mutation(population, ppl_val, ppl_weight, mutation_rate); // 產生突變基因
            Evaluation(population, ppl_val, ppl_weight, max_weight);  // 將不合理的組合刪掉
        }
        // output results
        cout << "Max profit: " << ppl_val[0] << endl;
        cout << "solution: " << population[0] << endl;
        cout << endl;
        // record
        profit_vec.push_back(ppl_val[0]);
        solution_vec.push_back(population[0]);
    }
    vector<int>::iterator max = max_element(profit_vec.begin(), profit_vec.end());
    string solution = solution_vec[max - profit_vec.begin()];
    cout << *max << endl; 
    // writting result to output file
    ofstream file_out("./" + folder_name + "/ans_" + folder_name + ".txt");
    file_out << "max profit:" << *max << endl;
    file_out << "solution:" << solution << endl;
    cout << ">> Output succesfully!"  << endl;
    return 0;
}
void Population(vector<string> &population, vector<int> &ppl_val, vector<int> &ppl_weight, int population_size)
{
    int r;
    int data_amount = weight.size();

    // 每圈做出一組基因
    while(population.size() < population_size){
        string select_str(data_amount, '0');// 目前這個人的基因
        int total_val = 0;
        int total_weight = 0;
        for(int i=0;i<data_amount;i++){ // "00000000" 遊歷
            r = rand()%100;
            if(r<40){
                select_str[i] = '1';
                total_val += value[i];
                total_weight += weight[i];
            }
        }
        population.push_back(select_str);
        ppl_val.push_back(total_val);
        ppl_weight.push_back(total_weight);
    }
}
void Evaluation(vector<string> &population, vector<int> &ppl_val, vector<int> & ppl_weight, int max_weight)
{
    // 超重
    for(int i=0; i<ppl_weight.size(); i++){
        if(ppl_weight[i] > max_weight){
            population.erase(population.begin()+i);
            ppl_val.erase(ppl_val.begin()+i);
            ppl_weight.erase(ppl_weight.begin()+i);
            i--;
        }
    }
    // 重複
    int size = population.size();
    for(int i=0;i<size-1;i++){
        for(int j=i+1;j<size;j++){
            if(population[i] == population[j])
            {
                population.erase(population.begin()+j);
                ppl_val.erase(ppl_val.begin()+j);
                ppl_weight.erase(ppl_weight.begin()+j);
                j--;
            }
            size = population.size();
        }
    }
    // cout << "Evaluation " << "population size: " << population.size() << endl;
    // for(int i=0;i<population.size();i++){
    //     cout << population[i] << ' ' << ppl_val[i] << ' ' << ppl_weight[i] << ' ';
    //     cout << endl;
    // }
    return ;
}
void Selection(vector<string> &population, vector<int> &ppl_val, vector<int> &ppl_weight)
{
    vector<string> new_population;
    vector<int> new_ppl_val;  
    vector<int> new_ppl_weight; 
    int size = population.size();
    int select_amount = 5;
    if(size == 1) return;

    vector<int> p(select_amount,0);
    for(int i=0; i<size; i++){
        for(int j=0; j<select_amount; j++) p[j] = rand()%size;
        int max=0, max_index = 0;
        for(int j=0;j<p.size();j++){
            if(ppl_val[p[j]] > max){
                max_index = p[j];
                max = ppl_val[p[j]];
            }
        }
        new_population.push_back(population[max_index]);
        new_ppl_val.push_back(ppl_val[max_index]);
        new_ppl_weight.push_back(ppl_weight[max_index]);
    }
    population = new_population;
    ppl_val = new_ppl_val;
    ppl_weight = new_ppl_weight;
    // cout << "Selection " << "population size: " << population.size() << endl;
    return;
}   
void Crossover(vector<string> &population, vector<int> &ppl_val, vector<int> &ppl_weight, float cr)
{
    int data_amount = value.size();
    int cur_ppl = population.size();
    for(int i=0; i<cur_ppl/2; i++){
        float rate = (float) rand () / (RAND_MAX + 1.0);
        // 如果小於cross rate才交配
        if(rate < cr)
        {
            // make mask
            string mask(data_amount,'0');
            for(int j=0;j<data_amount;j++){
                int rate2 = rand()%10;
                if(rate2 < 5) mask[j] = '1';
            }
            // swap
            string child0 = population[i];
            string child1 = population[i+1];
            // 記錄變化量的!!!
            int child0_val = 0, child1_val = 0;
            int child0_weight = 0, child1_weight = 0;

            for(int j=0;j<data_amount;j++){
                if(mask[j] == '1'){
                    child0[j] = population[i+1][j];
                    child1[j] = population[i][j];
                    if(population[i][j] == '0' && population[i+1][j] == '1'){
                        child0_val += value[j];
                        child1_val -= value[j];
                        child0_weight += weight[j];
                        child1_weight -= weight[j];
                    }
                    else if(population[i][j] == '1' && population[i+1][j] == '0'){
                        child0_val -= value[j];
                        child1_val += value[j];
                        child0_weight -= weight[j];
                        child1_weight += weight[j];
                    }
                    
                }
            }       
            population.push_back(child0);
            ppl_val.push_back(ppl_val[i] + child0_val);
            ppl_weight.push_back(ppl_weight[i] + child0_weight);

            population.push_back(child1);
            ppl_val.push_back(ppl_val[i+1] + child1_val);
            ppl_weight.push_back(ppl_weight[i+1] + child1_weight);
            // cout << "After: " << population[i] << " " << population[i+1] << endl;
        }
    }
    // cout << "Cross " << "population size: " << population.size() << endl;

}
void Mutation(vector<string> &population, vector<int> &ppl_val, vector<int> &ppl_weight, float mr)
{
    int size = population.size();
    if(mr == 0) return;
    for(int i=0; i<size; i++){
        float rate = (float) rand () / (RAND_MAX + 1.0);
        if(rate < mr)
        {
            int mutation_pos = rand()%(population[0].length());
            string new_ppl = population[i];
            if(population[i][mutation_pos] == '0'){

                new_ppl[mutation_pos] = '1';
                population.push_back(new_ppl);
                ppl_val.push_back(ppl_val[i] + value[mutation_pos]);
                ppl_weight.push_back(ppl_weight[i] + weight[mutation_pos]);
            }
            else{
                new_ppl[mutation_pos] = '0';
                population.push_back(new_ppl);
                ppl_val.push_back(ppl_val[i] - value[mutation_pos]);
                ppl_weight.push_back(ppl_weight[i] - weight[mutation_pos]);
            }
        }
    }
    // cout << "Mutation " << "population size: " << population.size() << endl;
    // cout << "After mutation: " << endl;
    // for(int i=0;i<population.size();i++){
    //     cout << population[i] << ' ' << ppl_val[i] << ' ' << ppl_weight[i] << ' ';
    //     cout << endl;
    // }

}
