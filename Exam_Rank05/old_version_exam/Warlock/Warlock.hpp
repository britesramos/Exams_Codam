#include <iostream>
#include <ostream>

class Warlock
{
	private:
		std::string name;
		std::string title;
	public:
		Warlock(std::string name, std::string title);
		~Warlock();
		const std::string getName() const;
		const std::string getTitle() const;
		void setTitle(const std::string title);
		void introduce() const;
};