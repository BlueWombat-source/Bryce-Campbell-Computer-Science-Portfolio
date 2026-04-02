package contactApp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;


public class ContactTest {
	
	@Test
	void testContactNullArguments() {
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			new Contact(null, null, null, null, null);
		});
	}
	
	@Test
	void testContactAndGetters() {
		Contact contact = new Contact("123456", "Matt", "Haggard", "2565555555", "123 Anywhere St.");
		assertTrue(contact.getName().equals("Matt Haggard"));
		assertTrue(contact.getPhoneNumber().equals("2565555555"));
		assertTrue(contact.getContactAddress().equals("123 Anywhere St."));
		assertTrue(contact.getContactID().equals("123456"));
	}
	
	@Test
	void testSetFirstAndLastName() {
		Contact contact = new Contact("123456", "Matt", "Haggard", "2565555555", "123 Anywhere St.");
		contact.setFirstName("Matt");
		contact.setLastName("Haggard");
		assertTrue(contact.getName().equals("Matt Haggard"));
	}
	
	@Test
	void testSetPhoneNumberAndAddress() {
		Contact contact = new Contact("123456", "Matt", "Haggard", "2565555555", "123 Anywhere St.");
		contact.setPhoneNumber("2565555555");
		contact.setContactAddress("123 Anywhere St.");
		assertTrue(contact.getPhoneNumber().equals("2565555555"));
		assertTrue(contact.getContactAddress().equals("123 Anywhere St."));
	}
	
	@Test
	void testNullSetAttributes() {
		Contact contact = new Contact("123456", "Matt", "Haggard", "2565555555", "123 Anywhere St.");
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contact.setContactID(null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contact.setFirstName(null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contact.setLastName(null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contact.setContactAddress(null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contact.setPhoneNumber(null);
		});
	}
	
	@Test
	void testAllGetters() {
		Contact contact = new Contact("123456", "Matt", "Haggard", "2565555555", "123 Anywhere St.");
		assertTrue(contact.getName().equals("Matt Haggard"));
		assertTrue(contact.getContactID().equals("123456"));
		assertTrue(contact.getPhoneNumber().equals("2565555555"));
		assertTrue(contact.getContactAddress().equals("123 Anywhere St."));
	}

}
