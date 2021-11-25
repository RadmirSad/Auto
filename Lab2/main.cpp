#include "RegExp.h"

/*
* 12|sfsf(safa...)...fd|as{10}s
* asfb...ss?(fd|zs)?(bg){4}bp
* saf%?%asf...(gf%%%...cds|%...%fd)casv{5}
* fasf(fas(v|d...)){8}xz|sfaz?fv%...%...kgj15(bgu|gh?)?
*/

int main() {
	regul str;
	std::cout << "Put some regular expression: ";
	std::string dop;
	std::getline(std::cin, dop);
	str.compile(dop);
	return 0;
}