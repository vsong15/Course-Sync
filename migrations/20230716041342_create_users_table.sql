-- User table
CREATE TABLE users (
   User_ID SERIAL PRIMARY KEY,
   Username VARCHAR(50) NOT NULL,
   Password VARCHAR(100) NOT NULL,
   Role_ID INT NOT NULL,
   First_Name VARCHAR(50) NOT NULL,
   Last_Name VARCHAR(50) NOT NULL,
   Email VARCHAR(100) NOT NULL,
   FOREIGN KEY (Role_ID) REFERENCES Roles(Role_ID)
);