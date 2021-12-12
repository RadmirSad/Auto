#include "pch.h"
#include <gtest/gtest.h>
#include "D:\Program\Auto\Lab2\RegExp.cpp"
#include "D:\Program\Auto\Lab2\Token.cpp"
#include <gvc.h>

class TestRegul : public ::testing::Test {
protected:
	std::vector<Regul> reg_expr;
	std::vector<std::vector<Token*>> test_vect;
	std::vector<std::string> test_strs;

	TestRegul() {
		reg_expr.push_back(Regul()); reg_expr.push_back(Regul()); 
		reg_expr.push_back(Regul()); reg_expr.push_back(Regul()); reg_expr.push_back(Regul());
		reg_expr[0].expr = "a|bb%%%";
		reg_expr[1].expr = "bgsasc?(as|f)";
		reg_expr[2].expr = "(abv|dgb...|dpo)|dop";
		reg_expr[3].expr = "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?";
		reg_expr[4].expr = "saf%?%asf...(gf%%%...cds|%...%fd)casv{5}";
	}

	void tokenize_strs() {
		for (int i = 0; i < reg_expr.size(); i++) {
			reg_expr[i].tokenize();
			if (reg_expr[i].tokens.empty())
				throw std::length_error("Vector is empty");
		}
		std::vector<Token*> buf;
		Token* dop = nullptr;			// a|bb%%%
		dop = new Token(L_RBR); buf.push_back(dop); dop = new Token("a"); buf.push_back(dop); dop = new Token(OR); buf.push_back(dop);
		dop = new Token("bb"); buf.push_back(dop); dop = new Token("%"); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);
		test_vect.push_back(buf); buf.clear();		// bgsasc?(as|f)
		dop = new Token(L_RBR); buf.push_back(dop); dop = new Token("bgsasc"); buf.push_back(dop); dop = new Token(OPT); buf.push_back(dop);
		dop = new Token(L_RBR); buf.push_back(dop);dop = new Token("as"); buf.push_back(dop); dop = new Token(OR); buf.push_back(dop); dop = new Token("f"); 
		buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);
		test_vect.push_back(buf); buf.clear();		// (abv|dgb...|dpo)|dop
		dop = new Token(L_RBR); buf.push_back(dop); dop = new Token(L_RBR); buf.push_back(dop); dop = new Token("abv"); buf.push_back(dop);
		dop = new Token(OR); buf.push_back(dop); dop = new Token("dgb"); buf.push_back(dop);dop = new Token(CL); buf.push_back(dop); 
		dop = new Token(OR); buf.push_back(dop);dop = new Token("dpo"); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);
		dop = new Token(OR); buf.push_back(dop); dop = new Token("dop"); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);
		test_vect.push_back(buf); buf.clear();		// fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?
		dop = new Token(L_RBR); buf.push_back(dop);
		dop = new Token("fasf"); buf.push_back(dop); dop = new Token(L_RBR); buf.push_back(dop);dop = new Token("fas"); buf.push_back(dop); dop = new Token(L_RBR); buf.push_back(dop);
		dop = new Token("v"); buf.push_back(dop); dop = new Token(OR); buf.push_back(dop);	dop = new Token("d"); buf.push_back(dop); dop = new Token(CL); buf.push_back(dop);
		dop = new Token(R_RBR); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);	dop = new Token(L_CBR); buf.push_back(dop); dop = new Token("8"); buf.push_back(dop);
		dop = new Token(R_CBR); buf.push_back(dop); dop = new Token("xz"); buf.push_back(dop); dop = new Token(OR); buf.push_back(dop);
		dop = new Token("sfaz"); buf.push_back(dop);	dop = new Token(OPT); buf.push_back(dop); dop = new Token("fv"); buf.push_back(dop);
		dop = new Token("..."); buf.push_back(dop); dop = new Token(CL); buf.push_back(dop);	dop = new Token("kgj15"); buf.push_back(dop); dop = new Token(L_RBR); buf.push_back(dop);
		dop = new Token("bgu"); buf.push_back(dop); dop = new Token(OR); buf.push_back(dop);	dop = new Token("gh"); buf.push_back(dop); dop = new Token(OPT); buf.push_back(dop);
		dop = new Token(R_RBR); buf.push_back(dop); dop = new Token(OPT); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);
		test_vect.push_back(buf); buf.clear();		// saf%?%asf...(gf%%%...cds|%...%fd)casv{5}
		dop = new Token(L_RBR); buf.push_back(dop);
		dop = new Token("saf"); buf.push_back(dop); dop = new Token("?"); buf.push_back(dop); dop = new Token("asf"); buf.push_back(dop); dop = new Token(CL); buf.push_back(dop);
		dop = new Token(L_RBR); buf.push_back(dop); dop = new Token("gf"); buf.push_back(dop);	dop = new Token("%"); buf.push_back(dop); dop = new Token(CL); buf.push_back(dop);
		dop = new Token("cds"); buf.push_back(dop); dop = new Token(OR); buf.push_back(dop);	dop = new Token("..."); buf.push_back(dop); dop = new Token("fd"); buf.push_back(dop);
		dop = new Token(R_RBR); buf.push_back(dop); dop = new Token("casv"); buf.push_back(dop);	dop = new Token(L_CBR); buf.push_back(dop); dop = new Token("5"); buf.push_back(dop);
		dop = new Token(R_CBR); buf.push_back(dop); dop = new Token(R_RBR); buf.push_back(dop);
		test_vect.push_back(buf); buf.clear();
	}

	void join_strs() {
		tokenize_strs();
		for (int i = 0; i < reg_expr.size(); i++) {
			reg_expr[i].join_strings();
			if (reg_expr[i].tokens.empty())
				throw std::length_error("Vector is empty");
		}

		test_vect[0][3]->change_str("bb%");
		delete test_vect[0][4];
		test_vect[0][4] = test_vect[0][5];
		test_vect[0].erase(test_vect[0].cend() - 1);

		test_vect[3][18]->change_str("fv..."); delete test_vect[3][19];
		test_vect[3].erase(test_vect[3].cbegin() + 19, test_vect[3].cbegin() + 20);

		test_vect[4][1]->change_str("saf?asf"); delete test_vect[4][2]; delete test_vect[4][3];
		test_vect[4].erase(test_vect[4].cbegin() + 2, test_vect[4].cbegin() + 4);
		test_vect[4][4]->change_str("gf%"); delete test_vect[4][5];
		test_vect[4].erase(test_vect[4].cbegin() + 5, test_vect[4].cbegin() + 6);
		test_vect[4][8]->change_str("...fd"); delete test_vect[4][9];
		test_vect[4].erase(test_vect[4].cbegin() + 9, test_vect[4].cbegin() + 10);
	}

	void add_symb() {
		join_strs();
		for (int i = 0; i < reg_expr.size(); i++) {
			reg_expr[i].add_strs_for_oper();
			if (reg_expr[i].tokens.empty())
				throw std::length_error("Vector is empty");
		}																	// a|bb%%%
		Token* dop = nullptr;												// bgsasc?(as|f)
		test_vect[1][1]->change_str("bgsas"); dop = new Token(CONCAT);
		test_vect[1].insert(test_vect[1].cbegin() + 2, dop); dop = new Token("c");
		test_vect[1].insert(test_vect[1].cbegin() + 3, dop); dop = new Token(CONCAT);
		test_vect[1].insert(test_vect[1].cbegin() + 5, dop);

		test_vect[2][4]->change_str("dg"); dop = new Token(CONCAT);			// (abv|dgb...|dpo)|dop
		test_vect[2].insert(test_vect[2].cbegin() + 5, dop); dop = new Token("b");
		test_vect[2].insert(test_vect[2].cbegin() + 6, dop); dop = new Token(CONCAT);

		test_vect[3].insert(test_vect[3].cbegin() + 2, dop); dop = new Token(CONCAT);	// fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?
		test_vect[3].insert(test_vect[3].cbegin() + 5, dop); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 16, dop);
		test_vect[3][19]->change_str("sfa"); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 20, dop); dop = new Token("z");
		test_vect[3].insert(test_vect[3].cbegin() + 21, dop); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 23, dop);
		test_vect[3][24]->change_str("fv.."); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 25, dop); dop = new Token(".");
		test_vect[3].insert(test_vect[3].cbegin() + 26, dop); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 28, dop); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 30, dop);
		test_vect[3][34]->change_str("g"); dop = new Token(CONCAT);
		test_vect[3].insert(test_vect[3].cbegin() + 35, dop); dop = new Token("h");
		test_vect[3].insert(test_vect[3].cbegin() + 36, dop);

		test_vect[4][1]->change_str("saf?as"); dop = new Token(CONCAT);					// saf%?%asf...(gf%%%...cds|%...%fd)casv{5}
		test_vect[4].insert(test_vect[4].cbegin() + 2, dop); dop = new Token("f");
		test_vect[4].insert(test_vect[4].cbegin() + 3, dop); dop = new Token(CONCAT);
		test_vect[4].insert(test_vect[4].cbegin() + 5, dop);
		test_vect[4][7]->change_str("gf"); dop = new Token(CONCAT);
		test_vect[4].insert(test_vect[4].cbegin() + 8, dop); dop = new Token("%");
		test_vect[4].insert(test_vect[4].cbegin() + 9, dop); dop = new Token(CONCAT);
		test_vect[4].insert(test_vect[4].cbegin() + 11, dop); dop = new Token(CONCAT);
		test_vect[4].insert(test_vect[4].cbegin() + 16, dop);
		test_vect[4][17]->change_str("cas"); dop = new Token(CONCAT);
		test_vect[4].insert(test_vect[4].cbegin() + 18, dop); dop = new Token("v");
		test_vect[4].insert(test_vect[4].cbegin() + 19, dop);
	}

	void build_trees(std::vector<std::string>& strs, TTree mass[5]) {
		add_symb();
		for (int i = 0; i < reg_expr.size(); i++) {								
			reg_expr[i].build_tree(mass[i]);									
			std::string adder;													
			int ind = 1;														
			reg_expr[i].width_byp(mass[i].get_node(), adder, ind);				
			strs.push_back(adder);
			if (reg_expr[i].tokens.empty())
				throw std::length_error("Vector is empty");
		}
		std::string buf;														// a|bb%
		buf += "\"|'1\"->\"a'2\";"; buf += "\"|'1\"->\"bb%'3\";";
		test_strs.push_back(buf);												//      2  1   3
		buf.clear();															// bgsas*c?*(as|f)
		buf += "\"*'1\"->\"*'2\";"; buf += "\"*'1\"->\"|'3\";";
		buf += "\"|'3\"->\"as'6\";"; buf += "\"|'3\"->\"f'7\";";
		buf += "\"*'2\"->\"bgsas'4\";"; buf += "\"*'2\"->\"?'5\";";
		buf += "\"?'5\"->\"c'10\";";
		test_strs.push_back(buf);												//     4  9    2    1
		buf.clear();															// (abv|dg*b...|dpo)|dop
		buf += "\"|'1\"->\"|'2\";"; buf += "\"|'1\"->\"dop'3\";";
		buf += "\"|'2\"->\"|'4\";"; buf += "\"|'2\"->\"dpo'5\";";
		buf += "\"|'4\"->\"abv'8\";"; buf += "\"|'4\"->\"*'9\";";
		buf += "\"*'9\"->\"dg'18\";"; buf += "\"*'9\"->\"...'19\";";		
		buf += "\"...'19\"->\"b'38\";";											//     4    18 37     9  2  1   48 24   12   6     3    14    7
		test_strs.push_back(buf);												// fasf*(fas*(v|d...)){8}*xz|sfa*z?*fv..*....*kgj15*(bgu|g*h?)?
		buf.clear();
		buf += "\"|'1\"->\"*'2\";"; buf += "\"|'1\"->\"*'3\";";
		buf += "\"*'3\"->\"*'6\";"; buf += "\"*'3\"->\"?'7\";";
		buf += "\"?'7\"->\"|'14\";";
		buf += "\"|'14\"->\"bgu'28\";"; buf += "\"|'14\"->\"*'29\";";
		buf += "\"*'29\"->\"g'58\";"; buf += "\"*'29\"->\"?'59\";";
		buf += "\"?'59\"->\"h'118\";";
		buf += "\"*'6\"->\"*'12\";"; buf += "\"*'6\"->\"kgj15'13\";";
		buf += "\"*'12\"->\"*'24\";"; buf += "\"*'12\"->\"...'25\";";
		buf += "\"...'25\"->\".'50\";";
		buf += "\"*'24\"->\"*'48\";"; buf += "\"*'24\"->\"fv..'49\";";
		buf += "\"*'48\"->\"sfa'96\";"; buf += "\"*'48\"->\"?'97\";";
		buf += "\"?'97\"->\"z'194\";";
		buf += "\"*'2\"->\"*'4\";"; buf += "\"*'2\"->\"xz'5\";";
		buf += "\"*'4\"->\"fasf'8\";"; buf += "\"*'4\"->\"<8>'9\";";
		buf += "\"<8>'9\"->\"*'18\";";
		buf += "\"*'18\"->\"fas'36\";"; buf += "\"*'18\"->\"|'37\";";
		buf += "\"|'37\"->\"v'74\";"; buf += "\"|'37\"->\"...'75\";";
		buf += "\"...'75\"->\"d'150\";";										// saf%?%asf...(gf%%%...cds|%...%fd)casv{5}
		test_strs.push_back(buf);												//       8    4   36   18  9      2   1  3
		buf.clear();															// saf?as*f...*(gf*%...*cds|...fd)*cas*v{5}
		buf += "\"*'1\"->\"*'2\";"; buf += "\"*'1\"->\"<5>'3\";";
		buf += "\"<5>'3\"->\"v'6\";";
		buf += "\"*'2\"->\"*'4\";"; buf += "\"*'2\"->\"cas'5\";";
		buf += "\"*'4\"->\"*'8\";"; buf += "\"*'4\"->\"|'9\";";
		buf += "\"|'9\"->\"*'18\";"; buf += "\"|'9\"->\"...fd'19\";";
		buf += "\"*'18\"->\"*'36\";"; buf += "\"*'18\"->\"cds'37\";";
		buf += "\"*'36\"->\"gf'72\";"; buf += "\"*'36\"->\"...'73\";";
		buf += "\"...'73\"->\"%'146\";";
		buf += "\"*'8\"->\"saf?as'16\";"; buf += "\"*'8\"->\"...'17\";";
		buf += "\"...'17\"->\"f'34\";";
		test_strs.push_back(buf);
		buf.clear();
	}

	void build_DFAs(std::vector<std::string>& strs) {
		TTree mass[5];
		build_trees(strs, mass);
		strs.clear();
		test_strs.clear();
		for (int i = 0; i < 3; i++) {
			reg_expr[i].tokens.clear();
			std::string str;
			TNode* this_tree = mass[i].get_node();
			reg_expr[i].make_reps(this_tree);
			Automata* start = nullptr, * end = nullptr;
			int id = 1;
			reg_expr[i].build_NFA(this_tree, start, end, id);
			end->change_receive(true);
			reg_expr[i].del_tree_with_tokens(this_tree);
			mass[i].change_node(nullptr);
			reg_expr[i].get_graph(start, str, 1);
			reg_expr[i].build_DFA(start);
			reg_expr[i].get_graph(start, str, 2);
			reg_expr[i].min_DFA(start);
			reg_expr[i].get_graph(start, str, 3);
			strs.push_back(str);
			reg_expr[i].del_Auto(start);
		}
		std::string buf; buf += "digraph ast { rankdir=LR;1 [shape = square];";														// a|bb%%%
		buf += "1->5 [label = \"a\"];";
		buf += "5 [shape = doublecircle];5->2 [label = \"a\"];";
		buf += "2->2 [label = \"a\"];2->2 [label = \"b\"];2->2 [label = \"%\"];";
		buf += "5->2 [label = \"b\"];5->2 [label = \"%\"];";
		buf += "1->3 [label = \"b\"];";
		buf += "3->2 [label = \"a\"];3->4 [label = \"b\"];";
		buf += "4->2 [label = \"a\"];4->2 [label = \"b\"];4->5 [label = \"%\"];";
		buf += "3->2 [label = \"%\"];";
		buf += "1->2 [label = \"%\"];}";
		test_strs.push_back(buf);												//      2  1   3
		buf.clear();															// bgsas*c?*(as|f)
		buf += "digraph ast { rankdir=LR;1 [shape = square];";
		buf += "1->3 [label = \"b\"];";
		buf += "3->2 [label = \"b\"];";
		buf += "2->2 [label = \"b\"];2->2 [label = \"g\"];2->2 [label = \"s\"];2->2 [label = \"a\"];2->2 [label = \"f\"];2->2 [label = \"c\"];";
		buf += "3->4 [label = \"g\"];";
		buf += "4->2 [label = \"b\"];4->2 [label = \"g\"];4->5 [label = \"s\"];";
		buf += "5->2 [label = \"b\"];5->2 [label = \"g\"];5->2 [label = \"s\"];5->6 [label = \"a\"];";
		buf += "6->2 [label = \"b\"];6->2 [label = \"g\"];6->7 [label = \"s\"];";
		buf += "7->2 [label = \"b\"];7->2 [label = \"g\"];7->2 [label = \"s\"];7->9 [label = \"a\"];";
		buf += "9->2 [label = \"b\"];9->2 [label = \"g\"];9->10 [label = \"s\"];";
		buf += "10 [shape = doublecircle];10->2 [label = \"b\"];10->2 [label = \"g\"];10->2 [label = \"s\"];10->2 [label = \"a\"];10->2 [label = \"f\"];10->2 [label = \"c\"];";
		buf += "9->2 [label = \"a\"];9->2 [label = \"f\"];9->2 [label = \"c\"];";
		buf += "7->10 [label = \"f\"];7->8 [label = \"c\"];";
		buf += "8->2 [label = \"b\"];8->2 [label = \"g\"];8->2 [label = \"s\"];8->9 [label = \"a\"];8->10 [label = \"f\"];8->2 [label = \"c\"];";
		buf += "6->2 [label = \"a\"];6->2 [label = \"f\"];6->2 [label = \"c\"];";
		buf += "5->2 [label = \"f\"];5->2 [label = \"c\"];";
		buf += "4->2 [label = \"a\"];4->2 [label = \"f\"];4->2 [label = \"c\"];";
		buf += "3->2 [label = \"s\"];3->2 [label = \"a\"];3->2 [label = \"f\"];3->2 [label = \"c\"];";
		buf += "1->2 [label = \"g\"];1->2 [label = \"s\"];1->2 [label = \"a\"];1->2 [label = \"f\"];1->2 [label = \"c\"];}";
		test_strs.push_back(buf);												//     4  9    2    1
		buf.clear();															// (abv|dg*b...|dpo)|dop
		buf += "digraph ast { rankdir=LR;1 [shape = square];";			// 1->7 2->4 3->3 4->2 5->5 6->1 7->6 8->9 9->8
		buf += "1->6 [label = \"a\"];";
		buf += "6->2 [label = \"a\"];";
		buf += "2->2 [label = \"a\"];2->2 [label = \"b\"];2->2 [label = \"v\"];2->2 [label = \"d\"];2->2 [label = \"g\"];2->2 [label = \"p\"];2->2 [label = \"o\"];";
		buf += "6->7 [label = \"b\"];";
		buf += "7->2 [label = \"a\"];7->2 [label = \"b\"];7->8 [label = \"v\"];";
		buf += "8 [shape = doublecircle];";
		buf += "8->2 [label = \"a\"];8->2 [label = \"b\"];8->2 [label = \"v\"];8->2 [label = \"d\"];8->2 [label = \"g\"];8->2 [label = \"p\"];8->2 [label = \"o\"];";
		buf += "7->2 [label = \"d\"];7->2 [label = \"g\"];7->2 [label = \"p\"];7->2 [label = \"o\"];";
		buf += "6->2 [label = \"v\"];6->2 [label = \"d\"];6->2 [label = \"g\"];6->2 [label = \"p\"];6->2 [label = \"o\"];";
		buf += "1->2 [label = \"b\"];1->2 [label = \"v\"];1->5 [label = \"d\"];";
		buf += "5->2 [label = \"a\"];5->2 [label = \"b\"];5->2 [label = \"v\"];5->2 [label = \"d\"];5->9 [label = \"g\"];";
		buf += "9 [shape = doublecircle];9->2 [label = \"a\"];9->9 [label = \"b\"];";
		buf += "9->2 [label = \"v\"];9->2 [label = \"d\"];9->2 [label = \"g\"];9->2 [label = \"p\"];9->2 [label = \"o\"];";
		buf += "5->3 [label = \"p\"];";
		buf += "3->2 [label = \"a\"];3->2 [label = \"b\"];3->2 [label = \"v\"];3->2 [label = \"d\"];3->2 [label = \"g\"];3->2 [label = \"p\"];3->8 [label = \"o\"];";
		buf += "5->4 [label = \"o\"];";
		buf += "4->2 [label = \"a\"];4->2 [label = \"b\"];4->2 [label = \"v\"];4->2 [label = \"d\"];4->2 [label = \"g\"];";
		buf += "4->8 [label = \"p\"];4->2 [label = \"o\"];";
		buf += "1->2 [label = \"g\"];1->2 [label = \"p\"];1->2 [label = \"o\"];}";
		test_strs.push_back(buf);
	}

	std::string check_combine(const std::vector<std::string>& strs, int& sz) {
		Automata* fir = new Automata(1), * sec = new Automata(2);
		for (int i = 0; i < strs.size(); i++) {
			fir->add_transit(sec, strs[i]);
			sec->add_prev(fir, strs[i]);
		}
		reg_expr[0].combine_condits(fir, sec);
		sz = fir->get_size_trans();
		return fir->get_transition(0).second;
	}

	void exclude_states(Automata* start, std::vector<Automata*> end) {
		reg_expr[0].exclude_other_states(start, end);
	}

	Automata* find_state(Automata* start, std::vector<Automata*> end) {
		return reg_expr[0].find_redundant_state(start, end);
	}

	void check_copy_little_auto(Automata* start, std::vector<Automata*> end, Automata*& new_start, std::vector<Automata*>& new_end) {
		reg_expr[0].get_little_copy(start, end, new_start, new_end);
	}

	void check_get_all_states(Automata* start, std::vector<Automata*>& states)
	{
		reg_expr[0].get_all_states(start, states);
	}

	void check_product(const Regul& fir_lang, const Regul& sec_lang, Automata*& new_st,
		std::vector<Automata*>& new_end, std::string& test, int ind) {
		fir_lang.cart_product(sec_lang, new_st, new_end);
		reg_expr[0].get_graph(new_st, test, ind);
	}

	void reverse(std::string& str) {
		reg_expr[0].inverse_str(str);
	}

	auto get_tokens(int i) {
		return reg_expr[i].tokens;
	}

};

TEST_F(TestRegul, TestTokens) {
	tokenize_strs();
	for (int i = 0; i < 5; i++) {
		auto buf = get_tokens(i);
		int sz = buf.size();
		ASSERT_EQ(buf.size(), test_vect[i].size());
		for (int j = 0; j < sz; j++) {
			auto fir = *(buf[j]), sec = * (test_vect[i][j]);
			ASSERT_EQ(fir, sec);
		}
	}
}

TEST_F(TestRegul, TestJoining) {
	join_strs();
	for (int i = 0; i < 5; i++) {
		auto buf = get_tokens(i);
		int sz = buf.size();
		ASSERT_EQ(buf.size(), test_vect[i].size());
		for (int j = 0; j < sz; j++) {
			auto fir = *(buf[j]), sec = *(test_vect[i][j]);
			ASSERT_EQ(fir, sec);
		}
	}
}

TEST_F(TestRegul, AddingSymbols) {
	add_symb();
	for (int i = 0; i < 5; i++) {
		auto buf = get_tokens(i);
		int sz = buf.size();
		ASSERT_EQ(buf.size(), test_vect[i].size());
		for (int j = 0; j < sz; j++) {
			auto fir = *(buf[j]), sec = *(test_vect[i][j]);
			ASSERT_EQ(fir, sec);
		}
	}
}

TEST_F(TestRegul, BuildingTrees) {
	std::vector<std::string> strs;
	TTree mass[5];
	build_trees(strs, mass);
	for (int i = 0; i < 5; i++)
		ASSERT_EQ(strs[i], test_strs[i]);
}

TEST_F(TestRegul, TestBuildDFA) {
	std::vector<std::string> strs;
	build_DFAs(strs);
	for (int i = 0; i < 3; i++)
		ASSERT_EQ(strs[i], test_strs[i]);
}

TEST_F(TestRegul, TestCompileAndMatch) {
	Regul a;
	a.compile("a|bb%%%");
	ASSERT_TRUE(a.match("a"));
	ASSERT_TRUE(a.match("bb%"));
	ASSERT_FALSE(a.match("ab"));
	ASSERT_FALSE(a.match("bb"));
	ASSERT_FALSE(a.match("b%"));
	ASSERT_FALSE(a.match("cd"));
	a.compile("bgsasc?(as|f)");
	ASSERT_TRUE(a.match("bgsascas"));
	ASSERT_TRUE(a.match("bgsasas"));
	ASSERT_TRUE(a.match("bgsascf"));
	ASSERT_TRUE(a.match("bgsasf"));
	ASSERT_FALSE(a.match("bgsasasf"));
	ASSERT_FALSE(a.match("abgsasas"));
	ASSERT_FALSE(a.match("b"));
	ASSERT_FALSE(a.match("as"));
	a.compile("(abv|dgb...|dpo)|dop");
	ASSERT_TRUE(a.match("dop"));
	ASSERT_TRUE(a.match("dpo"));
	ASSERT_TRUE(a.match("abv"));
	ASSERT_TRUE(a.match("dgb"));
	ASSERT_TRUE(a.match("dg"));
	ASSERT_TRUE(a.match("dgbbbbbbbbb"));
	ASSERT_FALSE(a.match("dopdpo"));
	ASSERT_FALSE(a.match("ab"));
	ASSERT_FALSE(a.match("dgbbbbbd"));
	ASSERT_FALSE(a.match("abc"));
	a.compile("fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?");
	ASSERT_TRUE(a.match("fasffasfasfasfasfasfasfasfasxz"));
	ASSERT_TRUE(a.match("fasffasvfasvfasfasvfasfasfasfasvxz"));
	ASSERT_TRUE(a.match("fasffasfasddddfasvfasdfasddddfasvfasfasxz"));
	ASSERT_TRUE(a.match("sfazfv..kgj15"));
	ASSERT_TRUE(a.match("sfafv........kgj15bgu"));
	ASSERT_TRUE(a.match("sfazfv...kgj15g"));
	ASSERT_FALSE(a.match("fasffasvfasfasvfasfasfasfasvxz"));
	ASSERT_FALSE(a.match("fasffasvfasvfasfasvdfasfasfasfasvxz"));
	ASSERT_FALSE(a.match("fasfasvfasvfasfasvfasfasfasfasvxz"));
	a.compile("saf%?%asf...(gf%%%...cds|%...%fd)casv{5}");
	ASSERT_TRUE(a.match("saf?asgfcdscasvvvvv"));
	ASSERT_TRUE(a.match("saf?asfgf%cdscasvvvvv"));
	ASSERT_TRUE(a.match("saf?asfffffgf%%%%%%cdscasvvvvv"));
	ASSERT_TRUE(a.match("saf?asf...fdcasvvvvv"));
	ASSERT_FALSE(a.match("safasfgfcdscasvvvvv"));
	ASSERT_FALSE(a.match("saf?asfgfcdscasvvvv"));
	ASSERT_FALSE(a.match("saf?asfffffgf%%%%%%fdcdscasvvvvv"));
	ASSERT_FALSE(a.match("saf?asfffffgf%%%%%%...cdscasvvvvv"));
}

/*
TEST_F(TestRegul, TestOnlyMatch) {
	Regul a;
	ASSERT_TRUE(a.match("a", "a|bb%%%"));
	ASSERT_TRUE(a.match("bb%", "a|bb%%%"));
	ASSERT_FALSE(a.match("ab", "a|bb%%%"));
	ASSERT_FALSE(a.match("bb", "a|bb%%%"));
	ASSERT_FALSE(a.match("b%", "a|bb%%%"));
	ASSERT_FALSE(a.match("cd", "a|bb%%%"));
	ASSERT_TRUE(a.match("bgsascas", "bgsasc?(as|f)"));
	ASSERT_TRUE(a.match("bgsasas", "bgsasc?(as|f)"));
	ASSERT_TRUE(a.match("bgsascf", "bgsasc?(as|f)"));
	ASSERT_TRUE(a.match("bgsasf", "bgsasc?(as|f)"));
	ASSERT_FALSE(a.match("bgsasasf", "bgsasc?(as|f)"));
	ASSERT_FALSE(a.match("abgsasas", "bgsasc?(as|f)"));
	ASSERT_FALSE(a.match("b"));
	ASSERT_FALSE(a.match("as"));
	ASSERT_TRUE(a.match("dop", "(abv|dgb...|dpo)|dop"));
	ASSERT_TRUE(a.match("dpo", "(abv|dgb...|dpo)|dop"));
	ASSERT_TRUE(a.match("abv", "(abv|dgb...|dpo)|dop"));
	ASSERT_TRUE(a.match("dgb", "(abv|dgb...|dpo)|dop"));
	ASSERT_TRUE(a.match("dg", "(abv|dgb...|dpo)|dop"));
	ASSERT_TRUE(a.match("dgbbbbbbbbb", "(abv|dgb...|dpo)|dop"));
	ASSERT_FALSE(a.match("dopdpo", "(abv|dgb...|dpo)|dop"));
	ASSERT_FALSE(a.match("ab", "(abv|dgb...|dpo)|dop"));
	ASSERT_FALSE(a.match("dgbbbbbd", "(abv|dgb...|dpo)|dop"));
	ASSERT_FALSE(a.match("abc", "(abv|dgb...|dpo)|dop"));
	ASSERT_TRUE(a.match("fasffasfasfasfasfasfasfasfasxz", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_TRUE(a.match("fasffasvfasvfasfasvfasfasfasfasvxz", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_TRUE(a.match("fasffasfasddddfasvfasdfasddddfasvfasfasxz", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_TRUE(a.match("sfazfv..kgj15", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_TRUE(a.match("sfafv........kgj15bgu", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_TRUE(a.match("sfazfv...kgj15g", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_FALSE(a.match("fasffasvfasfasvfasfasfasfasvxz", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_FALSE(a.match("sfazfv...kgj15gu", "fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?"));
	ASSERT_TRUE(a.match("saf?asfffffgf%%%%%%cdscasvvvvv", "saf%?%asf...(gf%%%...cds|%...%fd)casv{5}"));
	ASSERT_TRUE(a.match("saf?asf...fdcasvvvvv", "saf%?%asf...(gf%%%...cds|%...%fd)casv{5}"));
	ASSERT_FALSE(a.match("safasfgfcdscasvvvvv", "saf%?%asf...(gf%%%...cds|%...%fd)casv{5}"));
	ASSERT_FALSE(a.match("saf?asfgfcdscasvvvv", "saf%?%asf...(gf%%%...cds|%...%fd)casv{5}"));
}
*/

TEST_F(TestRegul, TestCombineStrs) {
	std::vector<std::string> strs;
	std::string checker;
	int sz = 0;
	strs.push_back("a"); strs.push_back("k"); strs.push_back("m");
	checker = check_combine(strs, sz);
	ASSERT_EQ(sz, 1);
	ASSERT_EQ(checker, "a|m|k");
	strs.clear();
	strs.push_back("d"); strs.push_back("c"); strs.push_back("m"); strs.push_back("bs...a"); strs.push_back("a"); strs.push_back("m|b");
	checker = check_combine(strs, sz);
	ASSERT_EQ(sz, 1);
	ASSERT_EQ(checker, "d|m|b|a|bs...a|m|c");
	strs.clear();
	strs.push_back("a"); strs.push_back("?"); strs.push_back("m"); strs.push_back("s..."); strs.push_back("{"); strs.push_back(")");
	checker = check_combine(strs, sz);
	ASSERT_EQ(sz, 1);
	ASSERT_EQ(checker, "a|%)%|%{%|s...|m|%?%");
	strs.clear();
	strs.push_back("da"); strs.push_back("bc"); strs.push_back("md"); strs.push_back("s%?%"); strs.push_back("{"); strs.push_back(")");
	checker = check_combine(strs, sz);
	ASSERT_EQ(sz, 1);
	ASSERT_EQ(checker, "da|%)%|%{%|s%?%|md|bc");
	strs.clear();
	strs.push_back("da"); strs.push_back("bc"); strs.push_back("md"); strs.push_back("s%?%"); strs.push_back("{"); strs.push_back(")");
	checker = check_combine(strs, sz);
	ASSERT_EQ(sz, 1);
	ASSERT_EQ(checker, "da|%)%|%{%|s%?%|md|bc");
}

TEST_F(TestRegul, TestingFindingStates) {
	int id = 1;
	Automata* start = new Automata(id++), * end = new Automata(id++), * dop = new Automata(id++), *sec = nullptr;
	end->change_receive(true);
	start->add_transit(end, "a"); end->add_prev(start, "a");
	start->add_transit(dop, "b"); dop->add_prev(start, "b");
	std::vector<Automata*> endings;
	endings.push_back(end);
	dop = find_state(start, endings);
	ASSERT_EQ(dop->get_id(), 3);
	start->delete_transit(dop, "b"); dop->delete_prev(start, "b");
	end->add_transit(dop, "c"); dop->add_prev(end, "c");
	dop = find_state(start, endings);
	ASSERT_EQ(dop->get_id(), 3);
	sec = new Automata(id++);
	sec->change_receive(true);
	start->add_transit(sec, "d"); sec->add_prev(start, "d");
	endings.push_back(sec);
	dop = find_state(start, endings);
	ASSERT_EQ(dop->get_id(), 3);
	end->delete_transit(dop, "c"); dop->delete_prev(end, "c");
	delete dop;
	dop = find_state(start, endings);
	ASSERT_EQ(dop, nullptr);
	endings.pop_back();
	dop = find_state(start, endings);
	ASSERT_EQ(dop->get_id(), 4);
	endings.pop_back();
	endings.push_back(dop);
	dop = find_state(start, endings);
	ASSERT_EQ(dop->get_id(), 2);
}

TEST_F(TestRegul, TestingExcludingStates) {
	int id = 1;
	Automata* start = new Automata(id++), * end = new Automata(id++), *dop = new Automata(id++), *sec = nullptr;
	end->change_receive(true);
	start->add_transit({ dop, "a" }); dop->add_prev({ start, "a" });
	dop->add_transit({ end, "b" }); end->add_prev({ dop, "b" });
	std::vector<Automata*> endings;
	endings.push_back(end);
	exclude_states(start, endings);
	ASSERT_EQ(start->get_size_trans(), 1);
	ASSERT_EQ(start->get_size_prev(), 0);
	ASSERT_EQ(end->get_size_trans(), 0);
	ASSERT_EQ(end->get_size_prev(), 1);
	ASSERT_EQ(start->get_transition(0).first, end);
	ASSERT_EQ(end->get_prev(0).first, start);
	ASSERT_EQ(start->get_transition(0).second, "ab");
	ASSERT_EQ(end->get_prev(0).second, "ab");
	dop = new Automata(id++);
	start->add_transit({ dop, "c" }); dop->add_prev({ start, "c" });
	dop->add_transit({ end, "da" }); end->add_prev({ dop, "da" });
	exclude_states(start, endings);
	ASSERT_EQ(start->get_size_trans(), 1);
	ASSERT_EQ(start->get_size_prev(), 0);
	ASSERT_EQ(end->get_size_trans(), 0);
	ASSERT_EQ(end->get_size_prev(), 1);
	ASSERT_EQ(start->get_transition(0).first, end);
	ASSERT_EQ(end->get_prev(0).first, start);
	ASSERT_EQ(start->get_transition(0).second, "ab|cda");
	ASSERT_EQ(end->get_prev(0).second, "ab|cda");
	start->change_condit_trans("a", end); end->change_condit_prev("a", start);
	dop = new Automata(id++);
	start->add_transit(dop, "b"); dop->add_prev(start, "b");
	for (int i = 1; i <= 5; i++) {
		sec = new Automata(id++);
		dop->add_transit(sec, "x" + std::to_string(i));
		sec->add_prev(dop, "x" + std::to_string(i));
		sec->add_transit(end, "y" + std::to_string(i));
		end->add_prev(sec, "y" + std::to_string(i));
	}
	dop->add_transit(end, "end"); end->add_prev(dop, "end");
	exclude_states(start, endings);
	ASSERT_EQ(start->get_size_trans(), 1);
	ASSERT_EQ(start->get_size_prev(), 0);
	ASSERT_EQ(end->get_size_trans(), 0);
	ASSERT_EQ(end->get_size_prev(), 1);
	ASSERT_EQ(start->get_transition(0).first, end);
	ASSERT_EQ(end->get_prev(0).first, start);
	ASSERT_EQ(start->get_transition(0).second, "a|bend|bx5y5|bx4y4|bx3y3|bx2y2|bx1y1");
	ASSERT_EQ(end->get_prev(0).second, "a|bend|bx5y5|bx4y4|bx3y3|bx2y2|bx1y1");
	start->change_condit_trans("a", end); end->change_condit_prev("a", start);
	dop = new Automata(id++);
	start->add_transit(dop, "b"); dop->add_prev(start, "b");
	dop->add_transit(dop, "c"); dop->add_prev(dop, "c");
	dop->add_transit(end, "d"); end->add_prev(dop, "d");
	sec = new Automata(id++);
	end->add_transit(sec, "s");
	sec->add_prev(end, "s");
	sec->add_transit(sec, "l"); sec->add_prev(sec, "l");
	sec->add_transit(dop, "dop"); dop->add_prev(sec, "dop");
	exclude_states(start, endings);
	ASSERT_EQ(start->get_size_trans(), 1);
	ASSERT_EQ(start->get_size_prev(), 0);
	ASSERT_EQ(end->get_size_trans(), 1);
	ASSERT_EQ(end->get_size_prev(), 2);
	ASSERT_EQ(start->get_transition(0).first, end);
	ASSERT_EQ(start->get_transition(0).second, "a|bc...d");
	ASSERT_EQ(end->get_prev(0).second, "a|bc...d");
	ASSERT_EQ(end->get_transition(0).second, "sl...dopc...d");
	ASSERT_EQ(end->get_prev(1).second, "sl...dopc...d");
}

TEST_F(TestRegul, TestingCopy) {
	int id = 1;
	Automata* start = new Automata(id++), *new_start = nullptr, * add_end = new Automata(id++);
	std::vector<Automata*> end, new_end;
	add_end->change_receive(true);
	start->add_transit(add_end, "a"); add_end->add_prev(start, "a");
	add_end->add_transit(start, "c"); start->add_prev(add_end, "c");
	add_end->add_transit(add_end, "b"); add_end->add_prev(add_end, "b");
	end.push_back(add_end);
	check_copy_little_auto(start, end, new_start, new_end);
	ASSERT_EQ(new_start->get_size_trans(), 1);
	ASSERT_EQ(new_start->get_size_prev(), 1);
	ASSERT_EQ(new_end.size(), 1);
	auto dop = new_end[0]->get_transition(0);
	ASSERT_EQ(dop.first, new_start);
	ASSERT_EQ(dop.second, "c");
	dop = new_end[0]->get_transition(1);
	ASSERT_EQ(dop.first, dop.first);
	ASSERT_EQ(dop.second, "b");
	start->change_receive(true);
	delete new_start; new_start = nullptr;
	delete new_end[0]; new_end.clear();
	end.push_back(start);
	check_copy_little_auto(start, end, new_start, new_end);
	ASSERT_EQ(new_end[1], new_start);
	ASSERT_EQ(new_start->get_size_trans(), 1);
	ASSERT_EQ(new_start->get_size_prev(), 1);
	ASSERT_EQ(new_end.size(), 2);
	dop = new_end[0]->get_transition(0);
	ASSERT_EQ(dop.first, new_start);
	ASSERT_EQ(dop.second, "c");
	dop = new_end[0]->get_transition(1);
	ASSERT_EQ(dop.first, dop.first);
	ASSERT_EQ(dop.second, "b");
	delete new_start; new_start = nullptr;
	delete new_end[0]; new_end.clear();
}

TEST_F(TestRegul, TestGetAllStates) {
	int id = 1;
	std::vector<Automata*> all_states;
	Automata* start = new Automata(id++), * adder = new Automata(id++), *buf = nullptr;
	start->add_transit(adder, "a"); adder->add_prev(start, "a");
	adder = new Automata(id++);
	start->add_transit(adder, "b"); adder->add_prev(start, "b");
	adder = new Automata(id++);
	start->add_transit(adder, "c"); adder->add_prev(start, "c");
	check_get_all_states(start, all_states);
	ASSERT_EQ(all_states.size(), 4);
	ASSERT_EQ(all_states[0]->get_id(), 1); ASSERT_EQ(all_states[1]->get_id(), 2);
	ASSERT_EQ(all_states[2]->get_id(), 3); ASSERT_EQ(all_states[3]->get_id(), 4);
	ASSERT_EQ(all_states[0], start); ASSERT_EQ(all_states[3], adder);
	ASSERT_EQ(all_states[0]->get_next_state("a"), all_states[1]);
	ASSERT_EQ(all_states[0]->get_next_state("b"), all_states[2]);
	ASSERT_EQ(all_states[0]->get_next_state("c"), all_states[3]);
	buf = new Automata(id++);
	start->add_transit(buf, "d"); buf->add_prev(start, "d");
	buf->add_transit(adder, "e"); adder->add_prev(buf, "e");
	all_states.clear();
	check_get_all_states(start, all_states);
	ASSERT_EQ(all_states.size(), 5);
	ASSERT_EQ(all_states[0]->get_id(), 1); ASSERT_EQ(all_states[1]->get_id(), 2);
	ASSERT_EQ(all_states[2]->get_id(), 3); ASSERT_EQ(all_states[3]->get_id(), 4);
	ASSERT_EQ(all_states[0], start); ASSERT_EQ(all_states[3], adder); ASSERT_EQ(all_states[4], buf);
	ASSERT_EQ(all_states[0]->get_next_state("a"), all_states[1]);
	ASSERT_EQ(all_states[0]->get_next_state("b"), all_states[2]);
	ASSERT_EQ(all_states[0]->get_next_state("c"), all_states[3]);
	ASSERT_EQ(all_states[0]->get_next_state("d"), all_states[4]);
}

TEST_F(TestRegul, TestProduct) {
	Regul a, b;
	a.compile("ab");
	b.compile("ba");
	Automata* st = nullptr;
	std::vector<Automata*> end;
	std::string str;
	check_product(a, b, st, end, str, 4);
	ASSERT_EQ(st->get_id(), 1);
	ASSERT_EQ(st->get_size_trans(), 2);
	ASSERT_EQ(end.size(), 0);
	a.compile("bg");
	b.compile("bg?");
	st = nullptr;
	end.clear();
	check_product(a, b, st, end, str, 5);
	ASSERT_EQ(st->get_id(), 1);
	ASSERT_EQ(st->get_size_trans(), 2);
	ASSERT_EQ(end.size(), 1);
	ASSERT_TRUE(st->get_next_state("b")->get_next_state("g")->get_receive());
	a.compile("abc");
	b.compile("abcd?|cdc");
	st = nullptr;
	end.clear();
	check_product(a, b, st, end, str, 6);
	ASSERT_EQ(st->get_id(), 1);
	ASSERT_EQ(st->get_size_trans(), 3);
	ASSERT_EQ(end.size(), 1);
	ASSERT_TRUE(st->get_next_state("a")->get_next_state("b")->get_next_state("c")->get_receive());
}

TEST_F(TestRegul, TestingLangCross) {
	Regul a, b, res;
	a.compile("bag");
	b.compile("c...baga?");
	a.lang_cross(b, res);
	ASSERT_EQ(res.get_str(), "");
	a.compile("bag");
	b.compile("b...baga?");
	a.lang_cross(b, res);
	ASSERT_EQ(res.get_str(), "(bag)");
	a.compile("bbbag");
	b.compile("(bgg)...b{3}aga?");
	a.lang_cross(b, res);
	ASSERT_EQ(res.get_str(), "(bbbag)");
}

TEST_F(TestRegul, TestingRestore) {
	Regul a;
	a.compile("abc");
	std::string restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(abc)");
	a.compile("bvdafa");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(bvdafa)");
	a.compile("ab|bc|ndg");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(ab|bc|ndg)");
	a.compile("ab...|bc|ndg");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(a)b...|(bc|ndg)");
	a.compile("ab|bc|nd{5}g");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(ab|bc|ndddddg)");
	a.compile("ab|bc|(ndg)...a");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(a)|(bc|ndg(ndg)...a|ab)");
	a.compile("bgu...|(ab){3}(a|b)");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(abababb|abababa)|(bg)u...");
	a.compile("(beb)?ra|(ab){3}(a|b)b...");
	restored_str = a.restore_regexpr();
	ASSERT_EQ(restored_str, "(abababb|abababa)b...|(ra|bebra)");
}

TEST_F(TestRegul, TestingSmallInverse) {
	std::string str("abc");
	reverse(str);
	ASSERT_EQ(str, "cba");
	str = "ab...c";
	reverse(str);
	ASSERT_EQ(str, "cb...a");
	str = "ab...cdd...(agb|baf)...bg(asf|s)";
	reverse(str);
	ASSERT_EQ(str, "(asf|s)gb(agb|baf)...d...dcb...a");
	str = "ab...cdd...(agb|baf)...bg(asf|s)gg%(%...";
	reverse(str);
	ASSERT_EQ(str, "%(%...gg(asf|s)gb(agb|baf)...d...dcb...a");
}

TEST_F(TestRegul, TestingAllReverse) {
	Regul a;
	a.compile("abc");
	a.inversion();
	ASSERT_EQ(a.get_str(), "(cba)");
	a.compile("abc...|ba...c");
	a.inversion();
	ASSERT_EQ(a.get_str(), "c...(ba)|(ca...b)");
	a.compile("abc...|ba...c|ab{3}a?");
	a.inversion();
	ASSERT_EQ(a.get_str(), "(ba)|(ca...b|abbba)|c...(cba)|(bbba)");

}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}