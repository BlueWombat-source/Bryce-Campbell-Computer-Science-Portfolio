package contactApp;

public class Contact{
	private String contactID;
	private String firstName;
	private String lastName;
	private String phoneNumber;
	private String contactAddress;
	
	// constructor
	public Contact(String id, String fName, String lName, String pNumber, String conAddress) {
		if (id == null || id.length() > 10) {
			throw new IllegalArgumentException("Invalid contact ID");
		}
		
		if (fName == null || fName.length() > 10) {
			throw new IllegalArgumentException("Invalid first name");
		}
		
		if (lName == null || lName.length() > 10) {
			throw new IllegalArgumentException("Invalid last name");
		}
		
		if (pNumber == null || pNumber.length() > 10 || pNumber.length() < 10) {
			throw new IllegalArgumentException("Invalid phone number");
		}
		
		if (conAddress == null || conAddress.length() > 30) {
			throw new IllegalArgumentException("Invalid address");
		}
		
		this.contactID = id;
		this.firstName = fName;
		this.lastName = lName;
		this.phoneNumber = pNumber;
		this.contactAddress = conAddress;
	}
	
	// getters
	public String getContactID() {
		return this.contactID;
	}
	
	public String getName() {
		return this.firstName + " " + this.lastName;
	}
	
	public String getPhoneNumber() {
		return this.phoneNumber;
	}
	
	public String getContactAddress() {
		return this.contactAddress;
	}
	
	// setters
	public void setContactID(String newId) {
		if (newId == null || newId.length() > 10) {
			throw new IllegalArgumentException("Invalid contact ID");
		}
		
		contactID = newId;
	}
	
	public void setFirstName(String newFirstName) {
		if (newFirstName == null || newFirstName.length() > 10) {
			throw new IllegalArgumentException("Invalid first name");
		}
		
		firstName = newFirstName;
	}
	
	public void setLastName(String newLastName) {
		if (newLastName == null || newLastName.length() > 10) {
			throw new IllegalArgumentException("Invalid last name");
		}
		
		lastName = newLastName;
	}
	
	public void setPhoneNumber(String newPhoneNumber) {
		if (newPhoneNumber == null || newPhoneNumber.length() > 10 || newPhoneNumber.length() < 10) {
			throw new IllegalArgumentException("Invalid phone number");
		}

		phoneNumber = newPhoneNumber;
	}
	
	public void setContactAddress(String newContactAddress) {
		if (newContactAddress == null || newContactAddress.length() > 30) {
			throw new IllegalArgumentException("Invalid address");
		}
		
		contactAddress = newContactAddress;
	}
	
	
}
