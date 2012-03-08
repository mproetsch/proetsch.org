//Matthew Proetsch
//COP 3330 - Section 12Spring_0001
//Allocation.java: A class that parses HW submissions from
//HW_List.in, processes TA requests from TA_Queries.in,
//and outputs HW selections to HW_Allocation.out

import java.util.*;
import java.io.*;

public class Allocation {

	public static void main(String[] args) {
		
		//Open necessary files for reading and writing
		
		try {
			Scanner hwIn = new Scanner( new File("HW_List.in"));
			PrintWriter out = new PrintWriter( new FileWriter("HW_Allocation.out"));

		
			//Initialize a HomeworkQueue
			HomeworkQueue hwQueue = new HomeworkQueue();
			
			
			while(hwIn.hasNext()) {
				
				//fill up our variables
				//good thing everything is 100% correctly formed!
				String lastName = hwIn.next();
				String firstName = hwIn.next();
				Name studName = new Name(firstName, lastName);
	
				int secNumber = hwIn.nextInt();
				int dateSubmitted = hwIn.nextInt();
				int numFiles = hwIn.nextInt();
	
				//Make a string array of the filenames so that we can
				//pass it as an argument to the Files() constructor
				String[] filesList = new String[numFiles];
	
				for(int i = 0; i < numFiles; i++) {
					filesList[i] = hwIn.next();
				}
	
				Files studFiles = new Files(filesList);
	
				//Finally call the Homework constructor and let it work its magic
				Homework studentHW = new Homework(Homework.generateNextUniqueId(),
						studName,
						secNumber,
						studFiles,
						dateSubmitted);
	
				//check to see if the Homework is successfully added
				boolean added = hwQueue.addHomework(studentHW);
	
				out.printf("Homework %s", studentHW.toString());
	
				//say whether it was added or not
				if(added) {
					out.printf(" added to the queue.\n");
				} else {
					out.printf(" already in queue. Not added.\n");
				}
			}
			
			//Print a "Finished" message and fire up our TA Query scanner
			out.printf("Finished processing homeworks!\n\n");
			
			Scanner TAQin = new Scanner( new File("TA_Queries.in"));
			
			while(TAQin.hasNext()) {
				int taUID = TAQin.nextInt();
				int taSec = TAQin.nextInt();
				int taHWs = TAQin.nextInt();
				
				int HWsAssigned = 0;
				
				for(int i = 0; i < taHWs; i++) {
					
					if(! hwQueue.sectionIsEmpty(taSec)) {
						//The section isn't empty and the TA needs more work, so get the next section HW
						Homework current = hwQueue.getHomework(taSec);
						out.printf("TA %d gets assigned homework %s\n", taUID, current.toString());
						HWsAssigned += 1;
						
					} else if(hwQueue.sectionIsEmpty(taSec) && HWsAssigned == 0) {
						//Section is empty and TA hasn't received any HW so print a special message
						out.printf("TA %d assigned no homeworks. Nothing in the queue for section %d\n\n",
								   taUID,
								   taSec);
						break;
						
					} else if(hwQueue.sectionIsEmpty(taSec) && (HWsAssigned < taHWs)) {
						//TA only has some of their requested assignments
						//print how many they have out of how many they requested
						out.printf("TA %d assigned %d/%d homework(s) from section %d.\n\n",
								   taUID,
								   HWsAssigned,
								   taHWs,
								   taSec);
						break;
					}
					
				}
				
				if(HWsAssigned == taHWs) {
					//TA received everything they ever wanted
					out.printf("TA %d assigned all %d requested homework(s) from section %d.\n\n",
							   taUID,
							   taHWs,
							   taSec);
				}
			
			}
			
			//we're done, close up all the files
			out.printf("Finished processing TA queries! Exiting.\n");
			
			hwIn.close();
			out.close();
			TAQin.close();
			}
		
		catch (IOException ex) {
				System.out.println("Unable to open handles to files");
		}
		
	}
}
