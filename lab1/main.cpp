#include <iostream>
#include <random>

struct ADice{
public: 
    virtual unsigned roll() = 0;
};

struct Dice: virtual ADice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) {}

    virtual unsigned roll() {
        return dstr(reng);
    }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

struct ThreeDicePool: virtual ADice {
public:
    ThreeDicePool(ADice &_d1, ADice &_d2, ADice &_d3): d1(_d1), d2(_d2), d3(_d3) {}

    virtual unsigned roll() {
        return d1.roll() + d2.roll() + d3.roll();
    }

private:
    ADice &d1, &d2, &d3;
};

struct PenaltyDice: virtual ADice{
public:
    PenaltyDice(ADice &_d): d(_d) {}
    virtual unsigned roll() {
        return std::min(d.roll(), d.roll());
    }
private:
    ADice &d;
};

struct BonusDice: virtual ADice{
public:
    BonusDice(ADice &_d): d(_d) {}
    virtual unsigned roll() {
        return std::max(d.roll(), d.roll());
    }
private:
    ADice &d;
};

struct DoubleDice: virtual ADice, BonusDice, PenaltyDice{
public:
    DoubleDice(ADice &_d): ADice(), BonusDice(_d),  PenaltyDice(_d){}
    virtual unsigned roll() {
        return BonusDice::roll() + PenaltyDice::roll();
    }
};

/*
// к 3 заданию
struct DoubleDice: ADice {
    DoubleDice(ADice& _d) : bonus(_d), penalty(_d) {}
    
    virtual unsigned roll() override {
        return bonus.roll() + penalty.roll();
    }

private:
    BonusDice bonus;
    PenaltyDice penalty;
};
*/


double expected_value(ADice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double value_probability(unsigned value, ADice &d, unsigned number_of_rolls = 1){
    unsigned n = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) n += (d.roll() == value);
    return (double)n/number_of_rolls;
}

void print_hist_data(ADice& x){
    for(int i = 1; i <= 200; ++i) std::cout << value_probability(i, x, 100000) << ", ";
    std::cout << std::endl << std::endl;
}

int main(){
    Dice d1(6, 1), d2(6, 2), d3(6, 3);
    Dice d100(100, 4);
    ThreeDicePool three_d(d1, d2, d3);
    PenaltyDice pd(d100);
    BonusDice bd(d100);
    DoubleDice dd(d100);

    std::cout << expected_value(d1, 10000) << ", " << expected_value(three_d, 10000) << std::endl;
    //                  ~3.5                                ~10.5

    print_hist_data(d100);
    print_hist_data(pd);
    print_hist_data(bd);
    print_hist_data(three_d);
    print_hist_data(dd);

    return 0;
}