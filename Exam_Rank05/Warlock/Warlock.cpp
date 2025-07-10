#include "Warlock.hpp"

Warlock::Warlock(std::string name, std::string title): name(name), title(title){
	std::cout << this->name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock(){
	std::cout << this->name << ": My job here is done!" << std::endl;
}

void Warlock::introduce() const{
	std::cout << this->name << ": I am " << this->name << ", " << this->title << "!" << std::endl;
}

const std::string Warlock::getName() const{
	return this->name;
}

const std::string Warlock::getTitle() const{
	return this->title;
}

void Warlock::setTitle(const std::string title){
	this->title = title;
}