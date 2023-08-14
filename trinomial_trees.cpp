#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

struct trinomial_tree
{
    float S_0, u, d, p, m, q, vol, r, tick_period, v;
    int _levels;
    vector<vector<float>> _tree;

    trinomial_tree(float price_t_0 ,float realized_vol, float risk_free_rate, int levels, float total_time){
        S_0 = price_t_0;
        vol = realized_vol;
        _levels = levels;
        r = risk_free_rate;
        tick_period = total_time / _levels;
        // calculate the u and d for the given rates as well as p
        u = exp(vol*sqrt(2*tick_period));
        d = 1 / u;
        v = risk_free_rate - pow(vol, 2) / 2;
        float dx = vol*sqrt(3*tick_period);
        //risk-neutral probabilities
        p = 0.5*((vol*vol*tick_period + v*v*tick_period*tick_period) / (dx*dx) + (v*tick_period) / dx);
        m = 1 - ((vol*vol*tick_period + v*v*tick_period*tick_period) / (dx*dx));
        q = 1 - p - m;
        _populate_asset_tree();
    }
    //odd number of elements in each level
    void _populate_asset_tree(){
        float base_val = S_0;
        for(int i = 0; i < _levels; i++){
            _tree.push_back({});
            float t_base_val = base_val;
            for(int j = 0; j < 2*i + 1; j++){
                _tree[i].push_back(t_base_val);
                t_base_val /= u;
            }
            base_val *= u;
        }
    }
    vector<vector<float>> get_value_of_european_options_call(float strike){
        vector<vector<float>> values = _tree;
        // for the last level just use payout function
        for(int i = 0; i < 2*_levels + 1; i++){
            values[_levels - 1][i] = max((float)0, values[_levels-1][i] - strike);
        }
        for (int i = _levels - 2; i >= 0; i--){
            for(int j = 0; j < 2*i + 1; j++){
                values[i][j] = exp(-r*tick_period)*(p*values[i+1][j] + m*values[i+1][j+1] + q*values[i+1][j+2]);
            }
        }
        return values;
    }

};

int main(){
    trinomial_tree asset1(100, 0.2, 0.06, 5, 0.5);
    vector<vector<float>> eu_call_100 = asset1.get_value_of_european_options_call(100);
    for(auto x : eu_call_100){
        for(auto y: x){
            cout << y  << " ";
        }
        cout << endl;
    }
}
