#pragma once
#include <iostream>
#include <vector>

class Automata {
private:
	bool is_receiving = false;
	int id = 0;
	std::vector<std::pair<Automata*, std::string>> transit;
	std::vector< std::pair<Automata*, std::string>> prev_state;
public:
	Automata() {};
	explicit Automata(int new_id): id(new_id) {};

	void add_transit(Automata* new_tr, const std::string& str) {
		if (new_tr) {
			for (int i = 0; i < transit.size(); i++)
				if ((transit[i].first == new_tr) && (str == transit[i].second))
					throw std::invalid_argument("This transition has already existed");
			transit.push_back({ new_tr, str });
		}
		else throw std::invalid_argument("New transition was 'nullptr'");
	}

	void add_transit(const std::pair<Automata*, std::string>& new_trans) {
		if (new_trans.first) {
			for (int i = 0; i < transit.size(); i++)
				if (transit[i] == new_trans)
					throw std::invalid_argument("This transition has already existed");
			transit.push_back(new_trans);
		}
		else throw std::invalid_argument("New transition was 'nullptr'");
	}

	void add_prev(Automata* new_prev, const std::string& str) {
		if (new_prev) {
			for (int i = 0; i < prev_state.size(); i++)
				if ((prev_state[i].first == new_prev) && (str == prev_state[i].second))
					throw std::invalid_argument("This transition has already existed");
			prev_state.push_back({ new_prev, str });
		}
		else throw std::invalid_argument("New previous state was 'nullptr'");
	}

	void add_prev(const std::pair<Automata*, std::string>& new_prev) {
		if (new_prev.first) {
			for (int i = 0; i < prev_state.size(); i++)
				if (prev_state[i] == new_prev)
					throw std::invalid_argument("This transition has already existed");
			prev_state.push_back(new_prev);
		}
		else throw std::invalid_argument("New transition was 'nullptr'");
	}

	bool delete_transit(Automata* old_transit) {
		auto it = transit.cbegin(), end = transit.cend();
		for(; (it != end) && ((*it).first != old_transit); it++);
		if (it == end) {
			throw std::invalid_argument("Pair with this ptr didn't found");
			return false;
		}
		transit.erase(it);
		return true;
	}

	bool delete_prev(Automata* old_prev) {
		auto it = prev_state.cbegin(), end = prev_state.cend();
		for (; (it != end) && ((*it).first != old_prev); it++);
		if (it == end) {
			throw std::invalid_argument("Pair with this ptr didn't found");
			return false;
		}
		prev_state.erase(it);
		return true;
	}

	bool delete_transit(Automata* old_transit, const std::string& str) {
		auto it = transit.cbegin(), end = transit.cend();
		for (; (it != end) && (((*it).first != old_transit) || ((*it).second != str)); it++);
		if (it == end) {
			throw std::invalid_argument("Pair with this ptr and this condition didn't found");
			return false;
		}
		transit.erase(it);
		return true;
	}

	bool delete_prev(Automata* old_prev, const std::string& str) {
		auto it = prev_state.cbegin(), end = prev_state.cend();
		for (; (it != end) && ((*it).first != old_prev) || ((*it).second != str); it++);
		if (it == end) {
			throw std::invalid_argument("Pair with this ptr and this condition didn't found");
			return false;
		}
		prev_state.erase(it);
		return true;
	}

	bool delete_transit(int index) {
		auto it = transit.cbegin();
		if (index >= transit.size()) {
			throw std::out_of_range("Index was bigger than size of vector of transitions");
			return false;
		}
		transit.erase(it + index);
		return true;
	}

	bool delete_prev(int index) {
		auto it = prev_state.cbegin();
		if (index >= transit.size()) {
			throw std::out_of_range("Index was bigger than size of vector of transitions");
			return false;
		}
		prev_state.erase(it + index);
		return true;
	}

	int get_size_trans() const {
		return transit.size();
	}

	int get_size_prev() const {
		return prev_state.size();
	}

	int get_id() const {
		return id;
	}

	bool get_receive() const { return is_receiving; }

	void change_receive(bool new_rec) { is_receiving = new_rec; }

	bool change_condit_trans(const std::string& new_str, Automata* state) {
		int sz = transit.size();
		for (int i = 0; i < sz; i++)
			if (state == transit[i].first) {
				transit[i].second = new_str;
				return true;
			}
		return false;
	}

	bool change_condit_prev(const std::string& new_str, Automata* state) {
		int sz = prev_state.size();
		for (int i = 0; i < sz; i++)
			if (state == prev_state[i].first) {
				prev_state[i].second = new_str;
				return true;
			}
		return false;
	}

	std::pair<Automata*, std::string> get_transition(int index) const {
		return transit.at(index);
	}

	Automata* get_next_state(const std::string& str) const {
		for (auto transition : transit)
			if (transition.second == str)
				return transition.first;
		std::cout << "Transition with this string didn't exist" << std::endl;
		return nullptr;
	}

	auto get_prev(int index) const {
		return prev_state.at(index);
	}
};