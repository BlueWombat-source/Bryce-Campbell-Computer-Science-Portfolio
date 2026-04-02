package contactApp;

import java.util.Vector;
import java.util.Date;

public class AppointmentService {
	
	private Vector<Appointment> myAppointments = new Vector<Appointment>();
	
	private int numAppointments = 0;
	
	public Vector<Appointment> getAppointmentList() {
		return myAppointments;
	}
	
	public int getNumAppointments() {
		return numAppointments;
	}
	
	public void addAppointment(String id, Date date, String description) {
		Appointment newAppointment = new Appointment(id, date, description);
		
		// Checks if ID is already in use
		for (int i = 0; i < numAppointments; i++) {
			if (myAppointments.elementAt(i).getId() == id) {
				throw new IllegalArgumentException("Appointment ID already in use.");
			}
		}
		
		myAppointments.add(newAppointment);
		
		numAppointments++;
	}
	
	public void addAppointment(Appointment appointment) {
		myAppointments.add(appointment);
		
		numAppointments++;
	}
	
	public void removeAppointment(String id) {
		
		if (id == null || id.length() > 10) {
			throw new IllegalArgumentException("Invalid appointment ID.");
		}
		
		if (myAppointments.isEmpty()) {
			throw new IllegalArgumentException("There are no appointments.");
		}
		
		int index = -1;
		for (Appointment c: myAppointments) {
			if (c.getId() == id) {
				index = myAppointments.indexOf(c);
				}
		}
		
		if (index == -1) {
			System.out.println("Appointment not found.");
		}
		
		else {
			myAppointments.remove(index);
			numAppointments--;
			System.out.println("Appointment removed.");
		}
	}
}
