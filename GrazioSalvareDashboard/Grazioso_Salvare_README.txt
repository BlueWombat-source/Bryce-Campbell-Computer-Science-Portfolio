Grazioso Salvare Search-and-Rescue Dashboard
CS-340 Client Dashboard Project
Created by: Bryce C

Project Overview
This project implements a full-stack dashboard application for Grazioso Salvare, an organization that identifies and trains dogs for specialized search-and-rescue operations. The dashboard enables users to interact with animal shelter data stored in MongoDB and dynamically filter and visualize rescue-eligible dogs.
The application connects to a MongoDB database containing Austin Animal Center data and presents an interactive web-based dashboard built using Dash.

Required Functionality
The dashboard satisfies all required specifications:
1. Interactive Filter Options
Users can filter dogs by rescue category:
* Water Rescue
* Mountain or Wilderness Rescue
* Disaster Rescue or Individual Tracking
* Reset (returns dashboard to original unfiltered state)
Each filter applies a MongoDB query based on:
* Preferred breeds
* Preferred sex
* Age range (in weeks)

2. Interactive Data Table
The dashboard includes a dynamic Data Table that:
* Updates based on selected filter
* Supports sorting
* Supports column filtering
* Supports pagination
* Allows row selection
The table dynamically responds to MongoDB queries and displays only relevant records.

3. Geolocation Map
A dash-leaflet map displays:
* The location of the selected animal
* Defaults to the first row when the dashboard loads
* Updates automatically when a different row is selected
* Displays breed and animal name in tooltip/popup

4. Secondary Visualization (Pie Chart)
A pie (donut) chart displays breed distribution:
* Dynamically updates with table filters
* Groups smaller categories into an  Other  slice
* Uses top 10 breeds for clarity
* Displays percentages inside slices

Tools and Technologies Used
MongoDB (Model Component)
MongoDB was used as the database layer because:
* It stores data in flexible JSON-like documents (BSON)
* It integrates naturally with Python using PyMongo
* It supports powerful filtering operators such as:
o $in
o $gte
o $lte
* It allows dynamic queries that respond to dashboard interaction
MongoDB was ideal because rescue categories required flexible multi-condition filtering (breed, age, sex).

Python
Python was used because:
* It integrates seamlessly with MongoDB
* It supports rapid development
* It is well-supported by Dash and Plotly
* It allows straightforward data manipulation using Pandas

Dash Framework (View + Controller)
Dash was used to build the interactive web dashboard because:
* It provides a reactive callback system
* It connects user inputs directly to database queries
* It supports dynamic UI updates
* It integrates Plotly for visualizations
* It requires no JavaScript for interactive web apps
Dash components used:
* dash_table.DataTable
* dcc.Dropdown
* dcc.Graph
* dash-leaflet for geolocation mapping
Dash enables a clean Model View Controller structure:
* Model ? MongoDB
* View ? Dash layout components
* Controller ? Dash callbacks

Plotly
Plotly was used to generate:
* Dynamic pie chart visualization
* Interactive and responsive chart rendering
Resources:
* https://plotly.com/python/pie-charts/

Dash Documentation Resources
* Dash Core Components:
https://dash.plotly.com/dash-core-components
* Dash DataTable:
https://dash.plotly.com/datatable
* Plotly Pie Charts:
https://plotly.com/python/pie-charts/

Steps Taken to Complete the Project
1. Designed and implemented a CRUD Python module for MongoDB interaction.
2. Connected the dashboard to the MongoDB database.
3. Created the interactive layout using Dash.
4. Implemented dropdown filtering tied to MongoDB queries.
5. Developed dynamic callbacks to:
o Update table
o Update pie chart
o Update geolocation map
6. Added breed grouping logic to clean up pie chart visualization.
7. Implemented default row selection for map display.
8. Added Grazioso Salvare branding and unique creator identifier.
9. Tested each filter to confirm correct database querying.
10. Validated dashboard responsiveness and error handling.

Challenges Encountered and How They Were Overcome
1. SchemaTypeValidationError in Dash Callbacks
Issue:
Dash required multiple outputs but the callback sometimes returned None.
Resolution:
Ensured that all execution paths returned a tuple matching declared Outputs.

2. Map Not Displaying on Initial Load
Issue:
derived_virtual_selected_rows returned an empty list, causing an IndexError.
Resolution:
Implemented safe logic:
if not selected_rows:
    row = 0
This ensured the map defaults to the first row.

3. Pie Chart Not Updating After Filtering
Issue:
The chart was using the global DataFrame instead of filtered table data.
Resolution:
Changed callback to use derived_virtual_data from the DataTable to ensure dynamic updates.

4. Pie Chart Visual Clutter
Issue:
Too many unique breeds created unreadable pie slices.
Resolution:
Grouped low count breeds into an  Other  category and converted chart to donut style.

5. MongoDB Query Structure Errors
Issue:
Multiple dictionaries were incorrectly passed into read().
Resolution:
Combined conditions into a single query dictionary per MongoDB syntax.

How to Reproduce This Project
Requirements
* Python 3.x
* MongoDB running locally
* Required Python packages:
o dash
o jupyter-dash
o dash-leaflet
o pandas
o plotly
o pymongo
o matplotlib
Install dependencies:
pip install dash jupyter-dash dash-leaflet pandas plotly pymongo matplotlib

Setup Steps
1. Ensure MongoDB is running locally.
2. Load Austin Animal Center dataset into MongoDB.
3. Confirm database name: aac
4. Confirm collection name: animals
5. Ensure CRUD_Python_Module.py is in the same directory.
6. Place Grazioso Salvare logo file in project directory.
7. Run the dashboard script: python dashboard.py



Proof of Functionality
https://youtu.be/OEsVb220sSs

