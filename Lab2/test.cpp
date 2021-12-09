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
			std::vector<std::string> alphabet;
			reg_expr[i].get_graph(start, str, alphabet, 1);
			reg_expr[i].build_DFA(start, alphabet);
			reg_expr[i].get_graph(start, str, alphabet, 2);
			reg_expr[i].min_DFA(start, alphabet);
			reg_expr[i].get_graph(start, str, alphabet, 3);
			strs.push_back(str);
			reg_expr[i].del_Auto(start);
		}
		std::string buf; buf += "digraph ast { rankdir=LR;4 [shape = square];";														// a|bb%%%
		buf += "4->5 [label = \"a\"];";
		buf += "5 [shape = doublecircle];5->2 [label = \"a\"];";
		buf += "2->2 [label = \"a\"];2->2 [label = \"b\"];2->2 [label = \"%\"];";
		buf += "5->2 [label = \"b\"];5->2 [label = \"%\"];";
		buf += "4->1 [label = \"b\"];";
		buf += "1->2 [label = \"a\"];1->3 [label = \"b\"];";
		buf += "3->2 [label = \"a\"];3->2 [label = \"b\"];3->5 [label = \"%\"];";
		buf += "1->2 [label = \"%\"];4->2 [label = \"%\"];}";
		test_strs.push_back(buf);												//      2  1   3
		buf.clear();															// bgsas*c?*(as|f)
		buf += "digraph ast { rankdir=LR;4 [shape = square];";
		buf += "4->2 [label = \"b\"];";
		buf += "2->3 [label = \"b\"];";
		buf += "3->3 [label = \"b\"];3->3 [label = \"g\"];3->3 [label = \"s\"];3->3 [label = \"a\"];3->3 [label = \"f\"];3->3 [label = \"c\"];";
		buf += "2->5 [label = \"g\"];";
		buf += "5->3 [label = \"b\"];5->3 [label = \"g\"];5->1 [label = \"s\"];";
		buf += "1->3 [label = \"b\"];1->3 [label = \"g\"];1->3 [label = \"s\"];1->6 [label = \"a\"];";
		buf += "6->3 [label = \"b\"];6->3 [label = \"g\"];6->7 [label = \"s\"];";
		buf += "7->3 [label = \"b\"];7->3 [label = \"g\"];7->3 [label = \"s\"];7->9 [label = \"a\"];";
		buf += "9->3 [label = \"b\"];9->3 [label = \"g\"];9->10 [label = \"s\"];";
		buf += "10 [shape = doublecircle];10->3 [label = \"b\"];10->3 [label = \"g\"];10->3 [label = \"s\"];10->3 [label = \"a\"];10->3 [label = \"f\"];10->3 [label = \"c\"];";
		buf += "9->3 [label = \"a\"];9->3 [label = \"f\"];9->3 [label = \"c\"];";
		buf += "7->10 [label = \"f\"];7->8 [label = \"c\"];";
		buf += "8->3 [label = \"b\"];8->3 [label = \"g\"];8->3 [label = \"s\"];8->9 [label = \"a\"];8->10 [label = \"f\"];8->3 [label = \"c\"];";
		buf += "6->3 [label = \"a\"];6->3 [label = \"f\"];6->3 [label = \"c\"];";
		buf += "1->3 [label = \"f\"];1->3 [label = \"c\"];";
		buf += "5->3 [label = \"a\"];5->3 [label = \"f\"];5->3 [label = \"c\"];";
		buf += "2->3 [label = \"s\"];2->3 [label = \"a\"];2->3 [label = \"f\"];2->3 [label = \"c\"];";
		buf += "4->3 [label = \"g\"];4->3 [label = \"s\"];4->3 [label = \"a\"];4->3 [label = \"f\"];4->3 [label = \"c\"];}";
		test_strs.push_back(buf);												//     4  9    2    1
		buf.clear();															// (abv|dg*b...|dpo)|dop
		buf += "digraph ast { rankdir=LR;6 [shape = square];";
		buf += "6->7 [label = \"a\"];";
		buf += "7->4 [label = \"a\"];";
		buf += "4->4 [label = \"a\"];4->4 [label = \"b\"];4->4 [label = \"v\"];4->4 [label = \"d\"];4->4 [label = \"g\"];4->4 [label = \"p\"];4->4 [label = \"o\"];";
		buf += "7->1 [label = \"b\"];";
		buf += "1->4 [label = \"a\"];1->4 [label = \"b\"];1->9 [label = \"v\"];";
		buf += "9 [shape = doublecircle];";
		buf += "9->4 [label = \"a\"];9->4 [label = \"b\"];9->4 [label = \"v\"];9->4 [label = \"d\"];9->4 [label = \"g\"];9->4 [label = \"p\"];9->4 [label = \"o\"];";
		buf += "1->4 [label = \"d\"];1->4 [label = \"g\"];1->4 [label = \"p\"];1->4 [label = \"o\"];";
		buf += "7->4 [label = \"v\"];7->4 [label = \"d\"];7->4 [label = \"g\"];7->4 [label = \"p\"];7->4 [label = \"o\"];";
		buf += "6->4 [label = \"b\"];6->4 [label = \"v\"];6->5 [label = \"d\"];";
		buf += "5->4 [label = \"a\"];5->4 [label = \"b\"];5->4 [label = \"v\"];5->4 [label = \"d\"];5->8 [label = \"g\"];";
		buf += "8 [shape = doublecircle];8->4 [label = \"a\"];8->8 [label = \"b\"];";
		buf += "8->4 [label = \"v\"];8->4 [label = \"d\"];8->4 [label = \"g\"];8->4 [label = \"p\"];8->4 [label = \"o\"];";
		buf += "5->3 [label = \"p\"];";
		buf += "3->4 [label = \"a\"];3->4 [label = \"b\"];3->4 [label = \"v\"];3->4 [label = \"d\"];3->4 [label = \"g\"];3->4 [label = \"p\"];3->9 [label = \"o\"];";
		buf += "5->2 [label = \"o\"];";
		buf += "2->4 [label = \"a\"];2->4 [label = \"b\"];2->4 [label = \"v\"];2->4 [label = \"d\"];2->4 [label = \"g\"];";
		buf += "2->9 [label = \"p\"];2->4 [label = \"o\"];";
		buf += "6->4 [label = \"g\"];6->4 [label = \"p\"];6->4 [label = \"o\"];}";
		test_strs.push_back(buf);
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

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}