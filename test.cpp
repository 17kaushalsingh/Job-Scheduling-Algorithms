#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define all(x) (x).begin(), (x).end()
const int mod = (int)(1e9+7);

// Function Overriding (Runtime - Chapter 9)
class Base {
public:
    virtual void print() { cout << "Base" << endl; }
};
class Derived : public Base {
public:
    void print() override { cout << "Derived" << endl; }  // Overrides Base::print
};

class Multiply {
    int factor;

public:
    Multiply(int f) {
        this->factor = f;
    }

    int operator()(int x) {
        return x * factor;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Base* bptr = new Base();
    // bptr->print();

    vector<int> nums = {1, 2, 3, 4, 5};
    Multiply multBy3(10);
    
    transform(nums.begin(), nums.end(), nums.begin(), Multiply(12));

    for (int &x : nums) cout << x << " ";  // Outputs: 3 6 9 12 15
    

    return 0;
}