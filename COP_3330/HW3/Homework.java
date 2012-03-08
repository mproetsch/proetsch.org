//Matthew Proetsch
//COP 3330 - Section 12Spring_0001
//Homework.java
//A class representing a homework assignment
//Homework assignments consist of an id, the name 
//of the student submitting the assignment, the section
//that student is enrolled in, the files associated with 
//that assignment, and how many days late that assignment is.
//Methods include comparing, returning internal values, 
//and displaying string representations

public class Homework implements Comparable<Homework>{
	
	private int uid;
	private Name ownerName;
	private int ownerSection;
	private Files hwFiles;
	private int hwDateSubmitted;
	
	//start our uids at 0 since the generator function
	//increments before returning
	private static int nextUniqueId = 0;
	
	//represent the fifteenth of each month in a neat little int
	public static final int DUE_DATE = 15;
	
	//Create a new Homework with the given list of values
	public Homework(int id,
            Name name,
            int section,
            Files files,
            int dateSubmitted) {
		
		uid = id;
		ownerName = name;
		ownerSection = section;
		hwFiles = files;
		hwDateSubmitted = dateSubmitted;
	}
	
	//Create a new Homework with given values and an empty
	//list of Files
	public Homework(int id,
            Name name,
            int section,
            int dateSubmitted) {
		
		uid = id;
		ownerName = name;
		ownerSection = section;
		hwFiles = new Files();
		
	}
	
	//Construct a new Homework assignment with given values,
	//an empty list of files, and the next available UID
	public Homework(String first,
            String last,
            int section,
            int dateSubmitted) {
		
		uid = generateNextUniqueId();
		ownerName = new Name(first, last);
		ownerSection = section;
		hwFiles = new Files();
		hwDateSubmitted = dateSubmitted;
	}
	
	//in compareTo precedence should first be date submitted,
	//then Files.compareTo(),
	//then Name.compareTo(),
	//and if all these are equal or 0 we must be dealing with a copy
	public int compareTo(Homework other) {
		
		if(this.hwDateSubmitted != other.hwDateSubmitted) { //date submitted takes top priority
			return (this.hwDateSubmitted) - (other.hwDateSubmitted);
			
		} else if(this.hwFiles.compareTo(other.hwFiles) != 0) { //check the Files.compareTo()
			return this.hwFiles.compareTo(other.hwFiles);
			
		} else if(this.ownerName.compareTo(other.ownerName) != 0) { //finally see if names differ
			return this.ownerName.compareTo(other.ownerName);
			
		} else {
			return 0;
		}
		
	}
	
	//use nextUniqueId static variable, increment, then return it
	public static int generateNextUniqueId() {
		nextUniqueId += 1;
		return nextUniqueId;
	}
	
	//these functions hardly need explanation...
	//calculate days late (may be negative if early)
	public int getDaysLate() {
		return this.hwDateSubmitted - DUE_DATE;
	}
	
	//get the Files
	public Files getFiles() {
		return this.hwFiles;
	}
	
	//get the uid
	public int getId() {
		return this.uid;
	}
	
	//get the associated Name
	public Name getName() {
		return this.ownerName;
	}
	
	//get the section of the submitter
	public int getSection() {
		return this.ownerSection;
	}
	
	//return a String representation of the Homework
	public String toString() {
		String stringify = uid + ": " + hwFiles.getNumberOfFile() + " file(s) "
				+ hwFiles.toString() + " submitted by " + ownerName.toString() + " ";
		
		//handle daysLate on a case-by-case basis
		//since we also need to tell if its late, on time, or early
		if(this.getDaysLate() > 0) {
			//we need to use toString on the integer returned by getDaysLate()
			stringify = stringify.concat( Integer.toString(this.getDaysLate()) );
			stringify = stringify.concat(" day(s) late for section ");
			stringify = stringify.concat( Integer.toString(ownerSection) );
			
		} else if(this.getDaysLate() == 0) {
			stringify = stringify.concat("on time for section ");
			stringify = stringify.concat( Integer.toString(ownerSection) );
		
		} else {
			stringify = stringify.concat( Integer.toString( 0 - this.getDaysLate()) ); //get abs since it's negative
			stringify = stringify.concat(" day(s) early for section ");
			stringify = stringify.concat( Integer.toString(ownerSection) );
		}
		
		return stringify;
	}
}