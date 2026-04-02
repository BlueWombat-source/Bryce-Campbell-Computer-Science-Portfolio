package contactApp;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Assertions;

public class TaskServiceTest {
	
	@Test
	void testAddTaskMethod() {
		TaskService taskService = new TaskService();
		String testId = taskService.generateUniqueId();
		Task task = new Task(testId, "Test name", "Test task description");
		
		// Add task to the list
		taskService.addTask(task);
		
		// Confirm task is in the list, count is not 0
		assertTrue(!taskService.getTaskList().isEmpty());
		assertTrue(taskService
				.getTaskList()
				.elementAt(0)
				.getTaskId()
				.equals(testId));
		assertTrue(taskService.getNumTasks() > 0);
	}
	
	@Test
	void testRemoveTaskMethod() {
		TaskService taskService = new TaskService();
		Task task = new Task("123456", "Test name", "Test task description");
		
		// Try to remove with null ID
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.removeTask("");
		});
		
		// Try to remove task with ID that is too long
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.removeTask("12345678901");
		});
		
		// Try to remove task from empty list
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.removeTask("1234567890");
		});
		
		taskService.addTask(task);
		
		// Attempt to remove task that is not there
		taskService.removeTask("123457");
		
		// Task list not empty, count not 0
		assertTrue(!taskService.getTaskList().isEmpty());
		assertTrue(taskService.getNumTasks() != 0);
		
		// Remove task
		taskService.removeTask("123456");
		
		// Task list is empty and count is 0, task removed successfully
		assertTrue(taskService.getTaskList().isEmpty());
		assertTrue(taskService.getNumTasks() == 0);
		
	}
	
	@Test
	void testUpdateTaskMethodErrors() {
		TaskService taskService = new TaskService();
		
		// Attempt to update task when list is empty
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask("123456", "New name", 1);
		});
		
		// Add task to list and ensure task was added
		Task task = new Task("123456", "Test name", "Test task description");
		taskService.addTask(task);
		assertTrue(!taskService.getTaskList().isEmpty());
		
		// ID too long
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask("12345678901", "New name", 1);
		});
		
		// ID value is null
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask(null, "New name", 1);
		});
		
		// Update value is null
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask("123456", null, 1);
		});
		
		// Selection is less that 0
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask("123456", "New name", -1);
		});
		
		// print "Task not found." to console
		taskService.updateTask("123457", "New name", 1);
		
		// print "Task not updated." to console
		taskService.updateTask("123456", "New name", 5);
	}
	
	@Test
	void testUpdateTaskMethod() {
		TaskService taskService = new TaskService();
		Task task = new Task("123456", "Test name", "Test description");
		taskService.addTask(task);
		assertTrue(!taskService.getTaskList().isEmpty());
		
		// Update task name
		taskService.updateTask("123456", "New name", 1);
		assertTrue(taskService.
				getTaskList()
				.elementAt(0)
				.getName()
				.equals("New name"));
		
		// Update task description
		taskService.updateTask("123456", "New description", 2);
		assertTrue(taskService.
				getTaskList()
				.elementAt(0)
				.getDescription()
				.equals("New description"));
		
		// Updated name is too long
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask("123456", "112233445566778899001", 1);
			});
		
		// Updated description is too long
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			taskService.updateTask("123456", 
					"111112222233333444445555566666777778888899999000001", 2);
			});
		
		// Check that list has been updated
		assertTrue(taskService.getNumTasks() == 1);
		assertTrue(taskService.getTaskList().elementAt(0)
				.getName().equals("New name"));
		assertTrue(taskService.getTaskList().elementAt(0)
				.getDescription().equals("New description"));
		
	}
}
