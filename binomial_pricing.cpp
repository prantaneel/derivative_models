#include <iostream>
#include <math.h>
using namespace std;

// RULE : Never use heap for creating objects, associate 

struct binomial_tree
{
    float S_0, u, d, p, vol, r, tick_period, v;
    int _levels;
    vector<vector<float>> _tree;

    binomial_tree(float price_t_0 ,float realized_vol, float risk_free_rate, int levels, float total_time){
        S_0 = price_t_0;
        vol = realized_vol;
        _levels = levels;
        r = risk_free_rate;
        tick_period = total_time / _levels;
        // calculate the u and d for the given rates as well as p
        u = exp(vol*sqrt(tick_period));
        d = 1 / u;
        v = risk_free_rate - pow(vol, 2) / 2;
        p = 0.5*(1 + v / vol); 
        _populate_asset_tree(); 
    } 
    void _populate_asset_tree(){
        // populate the tree upto the level time_index
        for(int i = 0; i < _levels; i++){
            //at level i
            float _val = pow(u, i)*S_0;
            _tree.push_back({});
            for(int j = 0; j <= i; j++){
                _tree[i].push_back(_val);
                _val = _val*(d / u);
            }
        }
    }
    vector<vector<float>> get_value_of_european_options_call(float strike){
        vector<vector<float>> values = _tree;
        // for the last level just use payout function
        for(int i = 0; i < _levels; i++){
            values[_levels - 1][i] = max((float)0, values[_levels-1][i] - strike);
        }
        for (int i = _levels - 2; i >= 0; i--){
            for(int j = 0; j <= i; j++){
                values[i][j] = exp(-r*tick_period)*(p*values[i+1][j] + (1-p)*values[i+1][j+1]);
            }
        }
        return values;
    }

};

struct trinomial_tree
{
    float S_0, u, d, p, vol, r, tick_period, v;
    int _levels;
    vector<vector<float>> _tree;
};


int main(){
    binomial_tree asset1(100, 0.2, 0.06, 60, 0.5);
    vector<vector<float>> eu_call_100 = asset1.get_value_of_european_options_call(100);
    for(auto x : eu_call_100){
        for(auto y: x){
            cout << y  << " ";
        }
        cout << endl;
    }
}