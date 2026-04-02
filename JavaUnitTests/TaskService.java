package contactApp;

import java.util.Vector;
import java.util.Random;

public class TaskService {
	
	// Create a vector to store tasks
	private Vector<Task> myTasks = new Vector<Task>();
	
	// Create an int variable to store the number of tasks
	private int numTasks;
	
	// Getter for number of tasks
	public int getNumTasks() {
		return numTasks;
	}
	
	// Getter for list of tasks
	public Vector<Task> getTaskList(){
		return myTasks;
	}
	
	// Function to add a new task to the list
	public void addTask(String taskId, String taskName, String taskDescription) {
		
		// Create a new task
		Task newTask = new Task(taskId, taskName, taskDescription);
		
		// Add the task to the list of tasks
		myTasks.add(newTask);
		
		// Increment the number of tasks
		numTasks++;
	}
	
	public void addTask(Task task) {
		
		myTasks.add(task);
		
		numTasks++;
	}
	
	public void removeTask(String taskId) {
		
		if (taskId == null || taskId.length() > 10) {
			throw new IllegalArgumentException("Invalid task ID");
		}
		
		if (myTasks.isEmpty()) {
			throw new IllegalArgumentException("There are no tasks saved.");
		}
		
		int index = -1;
		for (Task c: myTasks) {
			if (c.getTaskId() == taskId) {
				index = myTasks.indexOf(c);
			}
		}
		
		if (index == -1) {
			System.out.println("Task not found.");
			return;
		}
		
		else {
			myTasks.remove(index);
			numTasks--;
			System.out.println("Task removed.");
		}
		
	}
	
	public void removeTask(Task task) {
		myTasks.remove(task);
		numTasks--;
	}
	
	public void updateTask(String taskId, String update, int selection) {
		
		if (taskId == null || taskId.length() > 10) {
			throw new IllegalArgumentException("Invalid task ID.");
		}
		
		if (myTasks.isEmpty()) {
			throw new IllegalArgumentException("There are no tasks to update.");
		}
		
		if (selection < 0) {
			throw new IllegalArgumentException("Invalid selection.");
		}
		
		int index = -1;
		
		for (Task c: myTasks) {
			if(c.getTaskId() == taskId) {
				
				index = myTasks.indexOf(c);
			}
		}
		
		if (index == -1) {
			System.out.println("Task not found.");
			return;
		}
		
		Task updatedTask = myTasks.get(index);
		
		switch(selection) {
		
		     case 1:{
		    	 updatedTask.setName(update);
		    	 break;	
		     }
		     
		     case 2:{
		    	 updatedTask.setDescription(update);
		     }
		     
		     default:{
		    	 removeTask(myTasks.elementAt(index));
		    	 addTask(updatedTask);
		     }
		}
	}
	
	public String generateUniqueId() {
		Random rand = new Random();
		int newId = rand.nextInt(1000000000);
		String uniqueId = Integer.toString(newId);
		
		for (Task c: myTasks) {
			while(c.getTaskId() == uniqueId) {
				newId = rand.nextInt(1000000000);
				uniqueId = Integer.toString(newId);
			}
		}
		
		System.out.println("New Task ID created: " + uniqueId);
		return uniqueId;
	}
}
