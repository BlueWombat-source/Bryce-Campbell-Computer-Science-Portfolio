package contactApp;

import java.util.Date;

public class Appointment {
	
	String id;
	String description;
	Date appointmentDate;
	Date todaysDate;
	
	// Constructor, validates given arguments
	public Appointment(String id, Date date, String description) {
		todaysDate = new Date();
		if (id == null || id.length() > 10) {
			throw new IllegalArgumentException("Ivalid appointment ID.");
		}
		else {
			this.id = id;
		}
		
		if (description == null || description.length() > 50) {
			throw new IllegalArgumentException("Invalid appointment description.");
		}
		else {
			this.description = description;
		}
		
		if (date == null || date.before(todaysDate)) {
			throw new IllegalArgumentException("Invalid appointment date.");
		}
		else {
			this.appointmentDate = date;
		}
	}

	// Getters
	public String getId() {
		return id;
	}
	
	public Date getDate() {
		return appointmentDate;
	}
	
	public String getDescription() {
		return description;
	}
	
	public Date getTodaysDate() {
		return todaysDate;
	}
	
	// Setters
	public void setId(String id) {
		if (id == null || id.length() > 10) {
			throw new IllegalArgumentException("Ivalid appointment ID.");
		}
		else {
			this.id = id;
		}
	}
	
	public void setDate(Date date) {
		if (date == null || date.before(todaysDate)) {
			throw new IllegalArgumentException("Invalid appointment date.");
		}
		else {
			this.appointmentDate = date;
		}
	}
	
	public void setDescription(String description) {
		if (description == null || description.length() > 50) {
			throw new IllegalArgumentException("Invalid appointment description.");
		}
		else {
			this.description = description;
		}
	}
}
