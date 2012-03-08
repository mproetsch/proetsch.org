//Matthew Proetsch
//COP 3330 - Section 12Spring_0001
//Name.java: Define a Name class that stores a first and last name
//Implements Comparable<Name>, and methods that allow it to be compared
//to other instances of the class
//This class' purpose is to belong to Homework, where it will represent
//the homework owner's name name

public class Name implements Comparable<Name>{
	
	private String firstName;
	private String lastName;
	
	//Store first and last name, the parameters,
	//in a new Name object
	public Name(String first, String last) {
		firstName = first;
		lastName = last;
	}
	
	//compare two instances lexicographically
	//Last name takes priority over first
	public int compareTo(Name other) {
		if(this.lastName.compareTo(other.lastName) != 0) {
			return this.lastName.compareTo(other.lastName);
		
		} else if (this.firstName.compareTo(other.firstName) != 0) {
			return this.firstName.compareTo(other.firstName);
			
		} else {
			return 0;
			
		}
			
	}
	
	//equals(): return 1 if equal, 0 if not equal
	//Need to compare letter-by-letter as per spec
	public boolean equals(Name other) {
		
		//first, check to see if the lengths differ
		if(! (this.firstName.length() == other.firstName.length() &&
			  this.lastName.length() == other.lastName.length()) ) {
			
			return false;
		}
		
		else {
			int lastNameLen = this.lastName.length();
			int firstNameLen = other.firstName.length();
			
			//first compare firstName letter by letter
			for(int x = 0; x < firstNameLen; x++) {
			
				if(this.firstName.charAt(x) != other.firstName.charAt(x)) {
					return false;
				}
			}
			
			//if we get this far we need to also compare lastName letter by letter
			for(int x = 0; x < lastNameLen; x++) {
				
				if(this.lastName.charAt(x) != other.lastName.charAt(x)) {
					return false;
				}
			}
			
			return true;
				
		}
	}
	
	//returns a string representation of the instance
	public String toString() {
		return firstName + " " + lastName;
	}

}
