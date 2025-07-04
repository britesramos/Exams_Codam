//1) Include malloc library
//2) Include write library

//3) Write ft_putstr()
//4) Write ft_strlen()
//5) Infinte multiply function:
	//5.1) Store str lenghts.
	//5.2) Malloc a temp function with the sum of str1 and str2 lenght + 1 for the NULL terminator.
	//5.3) To while or for loops nested inside of each other. To iterate over each str.
		//5.3.1) Iner loop is going to store an "int mul" variable that is equal to the multiplication of a digit from each string + a carry + whatever is stored in this temp char*.
			//int = char - '0';
			//temp = (mul % 10) + '0'.
			//carry = mul / 10;
		//5.3.2) Outer loop keeps the carry = 0, adds any carry leftovers and iterates over the other string.
	//5.4)Skip leading zeros in temp.
	//5.5) Allocate new string to return the result.
	//5.6) Copy the new string from temp.
	//5.7) Return the new string and free temp.
	
//6) Main
	//6.1) If any of the string is zero
		//Write zero and return
	//6.2) Check negative signs
	//6.3) Call infinite multiply function
	//6.4) Write sign to the terminal.
	//6.5) Write infinite multiply function to the terminal.
	//6.6) Free memory.
	//6.7) Return 0;