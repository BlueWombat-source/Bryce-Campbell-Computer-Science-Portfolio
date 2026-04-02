package contactApp;

import static org.junit.jupiter.api.Assertions.*;
import java.util.Date;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

class AppointmentServiceTest {
	
	String testId = "123456";
	String testDescription = "Appointment description";
	String longDescription = String.valueOf('A').repeat(51);
	Date testDate = new Date(125, 8, 12);


	@Test
	void testAddAppointment() {
		AppointmentService testService = new AppointmentService();
		Appointment testAppt = new Appointment(testId, testDate, testDescription);
		testService.addAppointment(testAppt);
		
		// Check that appointment was added
		assertTrue(!testService.getAppointmentList().isEmpty());
		
		// Make sure appointment ID is correct
		assertTrue(testService.getAppointmentList().elementAt(0).getId().equals(testId));
		
		// Check that appointment count is greater than 0
		assertTrue(testService.getNumAppointments() > 0);
		
		// Test adding appointment using arguments instead of as an object
		testService.addAppointment("123457", testDate, testDescription);
		assertTrue(testService.getAppointmentList().elementAt(1).getId().equals("123457"));
		
		// Tests that appointments can not have duplicate IDs
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			testService.addAppointment(testId, testDate, longDescription);
		});
	}
		

	@Test
	void testRemoveAppointment() {
		AppointmentService testService = new AppointmentService();
		assertTrue(testService.getAppointmentList().isEmpty());
		assertTrue(testService.getNumAppointments() == 0);
		testService.addAppointment(testId, testDate, testDescription);
		
		assertTrue(!testService.getAppointmentList().isEmpty());
		assertTrue(testService.getNumAppointments() == 1);
		
		testService.removeAppointment(testId);
		assertTrue(testService.getAppointmentList().isEmpty());
		assertTrue(testService.getNumAppointments() == 0);
	}
	
}
