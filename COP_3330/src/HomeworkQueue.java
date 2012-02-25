import java.util.*;

/**
 * Objects of this class are used for ordering of Homework assignments for grading.
 * 
 * @author Stephen Fulwider
 */
public class HomeworkQueue {
	
	TreeMap<Integer,TreeSet<Homework>> homeworkQueue;
	
	/** Construct an initially empty HomeworkQueue */
	public HomeworkQueue() {
		homeworkQueue = new TreeMap<Integer,TreeSet<Homework>>();
	}
	
	/**
	 * Add a homework assignment to the queue. The section this homework assignment is part of
	 * 	is accessed and it is added to the queue for the specific section it is part of. If this
	 * 	homework is already part of this section, then the queue is not changed and false is
	 * 	returned (assuming Homework comparison is done properly).Otherwise, the homework is added
	 * 	and true is returned.
	 * 
	 * @param hw The homework to add to the section contained in this hw.
	 * @return Returns true if this homework is added to the queue, false otherwise.
	 */
	public boolean addHomework(Homework hw) {
		
		// Make sure this section already has at least one homework assignment. If not, start it.
		if (!homeworkQueue.containsKey(hw.getSection()))
			homeworkQueue.put(hw.getSection(), new TreeSet<Homework>());

		// Get the set of homeworks and try add this homework assignment to the set. Return whether
		//	or not the hw was added to the set.
		TreeSet<Homework> set=homeworkQueue.get(hw.getSection());
		return set.add(hw);
	}
	
	/**
	 * Get the next homework assignment for a given section. <i>You</i> must make sure that this 
	 * 	section has another homework assignment assigned to it, or a call to this method may cause 
	 * 	your program to crash. The homework assignment returned will be the next in the sorted order
	 * 	for this section, and that homework will be removed from the queue.
	 * 
	 * @param section The section to get the next homework assignment for.
	 * @return Returns the next homework assignment for the given section in sorted order.
	 */
	public Homework getHomework(int section) {
		TreeSet<Homework> set=homeworkQueue.get(section);
		return set.pollFirst();
	}
	
	/**
	 * Check if this section ID has no homework assignments associated with it. Will return false if
	 * 	this section never had any homework assignments associated with it or all homework
	 * 	assignments associated with this section have already been assigned to a TA.
	 * 
	 * @param section The section to check whether empty.
	 * @return Returns true if this section has no more homeworks to process, true otherwise.
	 */
	public boolean sectionIsEmpty(int section) {
		if (!homeworkQueue.containsKey(section))
			return true;
		return homeworkQueue.get(section).isEmpty();
	}
	
	/** 
	 * Return a String representation of the queue. This toString() method is only as good as the
	 * 	toString() method you write in the Homework class. This method may be useful for debugging.
	 */
	public String toString() {
		StringBuilder sb=new StringBuilder();
		for (int section : homeworkQueue.keySet()) {
			sb.append(String.format("Section %d: %s%n",section,homeworkQueue.get(section)));
		}
		return sb.toString();
	}
	
}
