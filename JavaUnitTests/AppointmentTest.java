package contactApp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;
import java.util.Date;

class AppointmentTest {
	
	// Create and initialize variables to be used as test values
	String testId = "123456";
	String testDescription = "Appointment description";
	String longDescription = String.valueOf('A').repeat(51);
	Date testDate = new Date(125, 8, 12);

	@Test
	void testGetId() {
		Appointment testAppt = new Appointment(testId,testDate, testDescription);
		assertTrue(testAppt.getId().equals(testId));
	}

	@Test
	void testGetDate() {
		Appointment testAppt = new Appointment(testId,testDate, testDescription);
		assertTrue(testAppt.getDate().equals(testDate));
	}

	@Test
	void testGetDescription() {
		Appointment testAppt = new Appointment(testId,testDate, testDescription);
		assertTrue(testAppt.getDescription().equals(testDescription));
	}

	@Test
	void testSetId() {
		Appointment testAppt = new Appointment(testId,testDate, testDescription);
		
		// Tests that ID was changed
		testAppt.setId("123457");
		assertTrue(testAppt.getId().equals("123457"));
		
		// Tests that null is not a valid argument
		assertThrows(IllegalArgumentException.class, () ->{
			testAppt.setId(null);
		});
		
		// Tests that a String longer than 10 chars is not a valid argument
		assertThrows(IllegalArgumentException.class, () ->{
			testAppt.setId("12345678901");
		});
	}

	@Test
	void testSetDate() {
		Appointment testAppt = new Appointment(testId,testDate, testDescription);
		
		testAppt.setDate(new Date(2025, 9, 12));
		assertTrue(testAppt.getDate().equals(new Date(2025, 9, 12)));
		
		assertThrows(IllegalArgumentException.class, () ->{
			testAppt.setDate(null);
		});
		
		assertThrows(IllegalArgumentException.class, () ->{
			testAppt.setDate(new Date(125, 7, 3));
		});
	}

	@Test
	void testSetDescription() {
		Appointment testAppt = new Appointment(testId,testDate, testDescription);
		
		testAppt.setDescription("New description");
		assertTrue(testAppt.getDescription().equals("New description"));
		
		assertThrows(IllegalArgumentException.class, () ->{
			testAppt.setDescription(null);
		});
		
		assertThrows(IllegalArgumentException.class, () ->{
			testAppt.setDescription(longDescription);
		});
	}

}
