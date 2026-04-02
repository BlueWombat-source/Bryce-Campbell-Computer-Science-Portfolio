package contactApp;

public class Task {
	
	public Task(String taskId, String name, String description) {
		if (taskId == null || taskId.length() > 10) {
			throw new IllegalArgumentException("Invalid task ID");
		}
		else {
			this.taskId = taskId;
		}
		
		if (name == null || name.length() > 20) {
			throw new IllegalArgumentException("Invalid name");
		}
		else {
			this.name = name;
		}
		
		if (description == null || description.length() > 50) {
			throw new IllegalArgumentException("Invalid description");
		}
		else {
			this.description = description;
		}
	}

	// Create string variable to store task ID
	private String taskId;
	
	// Create string variable to store task name
	private String name;
	
	// Create string variable to store task description
	private String description;
	
	// Getters and Setters
	public String getTaskId() {
		return taskId;
	}
	
	public void setTaskId(String newTaskId) {
		if (newTaskId == null || newTaskId.length() > 10) {
			throw new IllegalArgumentException("Invalid task ID");
		}
		this.taskId = newTaskId;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String newName) {
		if (newName == null || newName.length() > 20) {
			throw new IllegalArgumentException("Invalid name");
		}
		this.name = newName;
	}
	
	public String getDescription() {
		return description;
	}
	
	public void setDescription(String newDescription) {
		if (newDescription == null || newDescription.length() > 50) {
			throw new IllegalArgumentException("Invalid description");
		}
		this.description = newDescription;
	}
	
	
	

}
