Grazioso Salvare – MongoDB CRUD Python Module
Created by: Bryce Campbell

Project Overview
This project represents Phase One of the full-stack development effort for Grazioso Salvare, an international rescue-animal training organization. The purpose of this phase is to design and implement a MongoDB database that stores animal shelter data and provides CRUD (Create, Read, Update, Delete) functionality through a Python module.
The database supports Grazioso Salvare’s mission to identify dogs suitable for specialized search-and-rescue training. By leveraging structured shelter data, the application allows users to filter, categorize, and manage dog profiles based on age, breed, and other rescue-relevant attributes.
This repository contains the AnimalShelter CRUD Python module, which enables interaction between client-side code and the MongoDB database.

Purpose of the CRUD Python Module
The purpose of the CRUD Python module is to:
* Establish a connection to a MongoDB database.
* Provide programmatic access to the animals collection in the aac database.
* Implement Create, Read, Update, and Delete operations.
* Support future integration with a web-based dashboard.
* Provide maintainable, reusable, and open-source database interaction logic.
This module allows Grazioso Salvare to manage shelter data efficiently while preparing for future full-stack development phases.

Technologies Used
Python
Python was used to build the CRUD module due to its readability, strong community support, and compatibility with MongoDB.
PyMongo (MongoDB Python Driver)
This project uses PyMongo, the official MongoDB driver for Python.
Why PyMongo was chosen:
* Officially supported by MongoDB.
* Reliable and well-documented.
* Seamlessly integrates with Python applications.
* Provides full support for CRUD operations.
* Supports secure database connections and authentication.
The module imports:
from pymongo import MongoClient
from bson.objectid import ObjectId
MongoClient establishes the database connection.
ObjectId allows interaction with MongoDB document identifiers.

Database Configuration
The module connects to:
* Host: ‘localhost’
* Port: 27017
* Database: ‘aac’
* Collection: ‘animals’
* User: ‘aacuser’
The connection string used:
MongoClient('mongodb://127.0.0.1:27017/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+2.5.3')
This configuration ensures secure and direct communication with the MongoDB server

CRUD Functionality
The AnimalShelter class implements all CRUD operations.
1. Create Operation
def create(self, data)
Purpose:
Inserts a new document into the animals collection.
How it works:
* Accepts a dictionary (data) representing an animal record.
* Uses insert_one() to insert the document.
* Returns True if the insertion is acknowledged.
* Raises an exception if no data is provided.
Example Usage:
      new_animal = {
    "name": "Buddy",
    "breed": "Labrador Retriever",
    "age_upon_outcome_in_weeks": 60,
    "animal_type": "Dog"
}
 shelter.create(new_animal)

2. Read Operation
def read(self, criteria = None)
Purpose:
Retrieves documents from the collection.
How it works:
* If criteria are provided, returns matching documents.
* If no criteria are provided, returns all documents.
* Uses find() to query the collection.
* Excludes the id field from results.
Example Usage:
shelter.read({‘breed’: ‘German Shepherd’})

3. Update Operation
def update(self, search_criteria, new_data)
Purpose:
Updates one or multiple documents based on search criteria.
How it works:
* Uses count_documents() to determine how many records match.
* If more than one match ? uses update_many().
* If exactly one match ? uses update_one().
* Uses the $set operator to update only specified fields.
* Prints the number of modified documents.
Example Usage:
shelter.update({‘breed’ = ‘Southern’}, {‘color’ : ‘brown’})
4. Delete Operation
def delete(self, search_criteria, docs_to_delete= 'one')
Purpose:
Deletes one or multiple documents from the collection.
How it works:
* If docs_to_delete='one' ? uses delete_one().
* If docs_to_delete='all' ? uses delete_many().
* Prints the number of deleted documents.
Example Usage:
shelter.delete({‘animal_type’ : ‘Ostritch’}, 'one')

