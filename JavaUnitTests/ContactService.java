package contactApp;

import java.util.Vector;
import java.util.Random;

public class ContactService {
	
	// Create a vector to store contacts.
	private Vector<Contact> myContacts = new Vector<Contact>();
	
	// Create an int counter to store the number of contacts.
	private int numContacts = 0;
	
	// Getter for numContacts
	public int getNumContacts() {
		return numContacts;
	}
	
	// Getter for the list of contacts.
	public Vector<Contact> getContactLIst(){
		return myContacts;
	}
	
	// Function to add a contact to the list.
	public void addContact(String contactID, String fName, String lName, String phoneNumber, String contactAddress) {
		// Create a new contact
		Contact newContact = new Contact(contactID, fName, lName, phoneNumber, contactAddress);
		
		// Add the contact to the list of contacts.
		myContacts.add(newContact);
		
		// Increment the number of contacts.
		numContacts++;
	}
	
	public void addContact(Contact contact) {
		// Add the contact to the list of contacts.
		myContacts.add(contact);
		
		// Increment the number of contacts.
		numContacts++;
	}
	
	// Function to add a contact to the list.
	public void removeContact(String contactID) {
		
		if (contactID == null || contactID.length() > 10) {
			throw new IllegalArgumentException("Invalid contact ID");
		}
		
		if (myContacts.isEmpty()) {
			throw new IllegalArgumentException("There are no contacts saved.");
		}
		
		int index = -1;
		for (Contact c: myContacts) {
			if (c.getContactID() == contactID) {
				index = myContacts.indexOf(c);
			}
		}
		
		if (index == -1) {
			System.out.println("Not found.");
			return;
		}
		
		else {
			myContacts.remove(index);
			numContacts--;
			System.out.println("Removed.");
		}
	}
	
	public void removeContact(Contact contact) {
		myContacts.remove(contact);
		numContacts --;
	}
	
	// Update contact
	void updateContact(String ID, String update, int selection) {
		if (ID == null || ID.length() > 10 || update == null || selection < 0) {
			throw new IllegalArgumentException("Invalid Contact ID");
		}
		
		if (myContacts.isEmpty()) {
			throw new IllegalArgumentException("There are no contacts to update.");
		}
		
		int index = -1;
		
		for (Contact c: myContacts) {
			if (c.getContactID() == ID) {
				index = myContacts.indexOf(c);
			}
		}
		
		if (index == -1) {
			System.out.println("Not found");
			return;
		}
		
		Contact updatedContact = myContacts.get(index);
		
		switch(selection) {
			case 1:{
				updatedContact.setFirstName(update);
				break;
			}
			
			case 2:{
				updatedContact.setLastName(update);
				break;
			}
			
			case 3:{
				updatedContact.setPhoneNumber(update);
				break;
			}
			
			case 4:{
				updatedContact.setContactAddress(update);
				break;
			}
			
			default:{
				removeContact(myContacts.elementAt(index));
				addContact(updatedContact);
			}
		}
		
	}
	
	public String generateUniqueId() {
		Random rand = new Random();
		int newID = rand.nextInt(1000000000);
		String uniqueID = Integer.toString(newID);
		
		for (Contact c: myContacts) {
			while(c.getContactID() == uniqueID) {
				newID = rand.nextInt(1000000000);
				uniqueID = Integer.toString(newID);
			}
		}
		
		System.out.println("New Contact ID created: " + uniqueID);
		return uniqueID;
	}
}
