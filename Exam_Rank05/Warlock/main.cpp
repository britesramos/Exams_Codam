#include "Warlock.hpp"

int main(){
	Warlock const richard("Richard", "Mistress of Magma");
	richard.introduce();
	std::cout << richard.getName() << " - " << richard.getTitle() << std::endl;

	Warlock *jack = new Warlock("Jack", "the long");
	jack->introduce();
	jack->setTitle("The Mighty");
	jack->introduce();

	delete jack;
	return 0;
}