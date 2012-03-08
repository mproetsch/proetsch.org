import java.util.*;

/**
 * <p>A File class, representing the files submitted by a student as part of a homework submission.
 *  File submissions are <b>mutable</b> in the sense that after their creation, the files associated
 *  with a submission can be altered by adding files (however, files cannot be removed).</p>
 * 
 * <p>Multiple files of the same name can be kept in this list of files, and files are always kept
 *  in sorted order. An unbounded number of files may be added to this object.</p>
 *  
 *  @author Stephen Fulwider
 */
public class Files implements Comparable<Files> {
	
	// ArrayList of files. Implemented as an ArrayList so the list can grow. Notice this member is
	//	private, so the details of its implementation are hidden from the outside user.
	private ArrayList<String> files;
	
	/**
	 * Construct a new list of files. Initially constructs an empty list of files. You can add
	 *  files with the the addFile() method.
	 */
	public Files() {
		files=new ArrayList<String>();
	}
	
	/**
	 * Construct a new list of files. Initially the files contained are the files contained in the 
	 *  array passed in.
	 *  
	 *  @param initFiles The initial files to construct for this list of files.
	 */
	public Files(String[] initFiles) {
		files=new ArrayList<String>(initFiles.length);
		for (String f : initFiles) {
			files.add(f);
		}
	}
	
	/**
	 * Add a supplied file name to this list of files. The list of files is kept in sorted order
	 *  after this call.
	 *  
	 *  @param file The file to add to this list of files.
	 */
	public void addFile(String file) {
		files.add(file);
		Collections.sort(files);
	}
	
	/**
	 * Get the number of files contained in this files object.
	 * 
	 * @return The number of files in this Files object.
	 */
	public int getNumberOfFile() {
		return files.size();
	}
	
	/**
	 * compareTo method that defines the <i>natural ordering</i> of Files. Fewer files are ranked
	 *  before more files, and when the number of files are equal, the lists are compared
	 *  lexicographically, file by file until a difference is found.
	 *  
	 *  @param other The other Files object to compare this Files object to.
	 *  @return A negative value if this Files object should come first in sorted order, 0 if they
	 *   are the same Files object, or a positive if this Files object should come after other in
	 *   sorted order.
	 */
	public int compareTo(Files other) {
		int z=files.size()-other.files.size();
		for (int i=0; i<files.size() && z==0; ++i)
			z=files.get(i).compareTo(other.files.get(i));
		return z;
	}
	
	/**
	 * Return a String representation of these Files.
	 * 
	 * @return Returns the list of files, separated by spaces, enclosed in []'s.
	 */
	public String toString() {
		String res="[";
		for (int i=0; i<files.size(); ++i) {
			if (i>0) res+=" ";
			res+=files.get(i);
		}
		return res+"]";
	}

}
