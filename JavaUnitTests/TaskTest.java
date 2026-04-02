package contactApp;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Assertions;

public class TaskTest {
	
	String testId = "123456";
	String testName = "Task name";
	String testDescription = "Task description";
	String longDescription = String.valueOf('A').repeat(51);
	
	@Test
	void testTaskNullArguments() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task(null, null, null);
		});
	}
	
	@Test
	void testTaskAndGetters() {
		Task task = new Task(testId, testName, testDescription);
		assertTrue(task.getTaskId().equals(testId));
		assertTrue(task.getName().equals(testName));
		assertTrue(task.getDescription().equals(testDescription));
	}
	
	@Test
	void testSetTaskId() {
		Task task = new Task(testId, testName, testDescription);
		task.setTaskId("123457");
		assertTrue(task.getTaskId().equals("123457"));
	}
	
	@Test
	void testSetTaskName() {
		Task task = new Task(testId, testName, testDescription);
		task.setName("New name");
		assertTrue(task.getName().equals("New name"));
	}
	
	@Test
	void testSetDescription() {
		Task task = new Task(testId, testName, testDescription);
		task.setDescription("New description");
		assertTrue(task.getDescription().equals("New description"));
	}
	
	@Test
	void testAttributeLength() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("12345678901", "112233445566778899001", 
					longDescription);
		});
	}
	
	@Test
	void TestAttributeNullValue() {
		Task task = new Task(testId, testName, testDescription);
		
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			task.setTaskId(null);
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			task.setName(null);
		});
		
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			task.setDescription(null);
		});
	}

}
