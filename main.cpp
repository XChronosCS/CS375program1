/*
Brute Force Algorithm should use the following format:
COMPUTEMAXPROFIT (I, W)
maxProfit = 0;
S = the empty set;
M = the empty set;
if the sum of the weights in I <= W
return the profit of the items in I, and I;
while more subsets to generate
if the sum of the weights in S <= W
if the profit of the items in S > maxProfit
update maxProfit;
copy S to M;
generate the next subset S;
return maxProfit and M;
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <cstring>

/*
Brute Force method of computing max profit
cards: Array of tuples containing a card and the price gertrude is selling it for.
  Using tuples to easily group up both the price and the card name together.
*/

//Using typedef to increase the readiblity of the code
typedef std::string string;
typedef std::vector<std::tuple<string, int, int>> card_list;
typedef std::tuple<int, card_list> money_list_tuple;
typedef std::pair<string, int> sell_price;


money_list_tuple ComputeMaxProfit(card_list cards, int max_money) {
	int max_profit = 0;
	card_list max_set; //set with current max profit
	int cards_sum = 0;
	int subsets_max = (int)pow(2, (double) cards.size());
	int subsets_done = 0;
	money_list_tuple ret_val;
	for (auto item : cards) {
		cards_sum += std::get<2>(item);
	}
	if (cards_sum <= max_money) {
		for (auto item : cards) {
			max_profit += std::get<1>(item);
		}
		max_profit -= cards_sum;
		ret_val = std::make_tuple(max_profit, cards);
		return ret_val;
	}
	while (subsets_done < (int)subsets_max) {
		cards_sum = 0;
		card_list subset; //possible set with max profit (Vector)
		for (unsigned int j = 0; j < cards.size(); j++) { //for all elements contained in the card list
			if ((subsets_done & (1 << j)) != 0)  subset.push_back(cards.at(j)); //if the bit is set, we consider the jth element to be from that set, and add it on
		}
		for (auto item : subset) {
			cards_sum += std::get<2>(item);
		}
		if (cards_sum <= max_money) {
			int money_earned = 0;
			for (auto item : subset) {
				money_earned += std::get<1>(item);
			}
			money_earned -= cards_sum;
			if (money_earned >= max_profit) {
				max_profit = money_earned;
					max_set = subset;
			}
		}
		subsets_done += 1;
	}
	ret_val = std::make_tuple(max_profit, max_set);
	return ret_val;
}

int main(int argc, char* argv[]) {
	string error_message = "One of these cards is not available on the price list.";
	//File contianing market prices
	char * mf_var; //variable showing where the market file is in the arguments
	char * pf_var; //variable showing where the price file is in the arguments
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-m") == 0)  mf_var = argv[i + 1]; 
		if (strcmp(argv[i], "-p") == 0)  pf_var = argv[i + 1]; 
	}
	std::ifstream market_file;
	market_file.open(mf_var, std::ifstream::in);
	int size;
	market_file >> size;
	sell_price ebay[size];
	string card_name; //temp variable for the card's name
	int card_price; //temp variable for the card's price
	for (int i = 0; i < size; i++) {
		market_file >> card_name;
		market_file >> card_price;
		ebay[i] = std::make_pair(card_name, card_price);
	}
	market_file.close();
	int ebay_size = sizeof(ebay) / sizeof(ebay[0]);
	//File containing prices
	std::ifstream price_file;
	price_file.open(pf_var, std::ifstream::in);
	std::ofstream output;
	output.open("output.txt", std::ofstream::out);
	while (price_file.peek() != EOF) {
		//create tuple to contain info about cards
		auto start_time = std::chrono::high_resolution_clock::now();
		card_list comparison;
		int money_to_spend;
		price_file >> size;
		price_file >> money_to_spend;
		for (int i = 0; i < size; i++) {
			price_file >> card_name;
			price_file >> card_price;
			auto matching_card = std::find_if(ebay, ebay + ebay_size, [&card_name](sell_price x) {return std::get<0>(x) == card_name; });
			if (matching_card == ebay + ebay_size) {
				output << error_message << std::endl;
				break;
			}
			auto compare_card = std::make_tuple(card_name, std::get<1>(*matching_card), card_price);
			comparison.push_back(compare_card);
		}
		money_list_tuple results = ComputeMaxProfit(comparison, money_to_spend);
		auto stop_time = std::chrono::high_resolution_clock::now();
		auto cpu_time = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
		output << "Size of Input for this problem: " << size << std::endl;
		output << "Maximum Profit: " << std::get<0>(results) <<  std::endl;
		output << "Number of cards needed for this profit: " << std::get<1>(results).size() << std::endl;
		output << "Time to calculate: " << cpu_time.count() << " microseconds." << std::endl;
	}
	output.close();
	price_file.close();
}
